/* pof2jed.c -- MAX 7000S / 7000AE POF file -> ATF1502AS / ATF1504AS / ATF1508AS JEDEC file.
 *
 * The POF is decoded (max_decode, max_to_netlist); every logic cell that can reach a pin is written into the ATF
 * macrocell of the same number (the two families put cell n on the same pin), setting by setting: product terms by
 * index, their special uses, the register type through the ATF XOR gate, the output and feedback selection, clock,
 * clear and preset, the parallel expander as the cascade link. The sources each block reads are placed on its UIM
 * lines by a deterministic augmenting-path matching; a global output enable goes to the ATF network with the same
 * inputs, or to the pin's own enable term when a logic cell drives it. Every other fuse takes the value the fitter
 * writes where nothing is used, and the file is written in the fitter's own layout. A file is written only when every
 * form it uses is in the table of forms checked on the reference designs (tab_p2j.c), and only after the ATF decoder
 * has read the new file back without a refusal. Anything else is refused by name.
 */
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "jedec.h"
#include "pof.h"
#include "atf.h"
#include "max.h"
#include "tables.h"
#include "p2j.h"

static const char *TGT[6][2] = { { "EPM7032S", "ATF1502AS" }, { "EPM7064S", "ATF1504AS" }, { "EPM7128S", "ATF1508AS" },
                                 { "EPM7032AE", "ATF1502AS" }, { "EPM7064AE", "ATF1504AS" }, { "EPM7128AE", "ATF1508AS" } };
static const char *PAIRS[][2] = { { "EPM7032S", "PLCC44" }, { "EPM7032S", "TQFP44" }, { "EPM7032AE", "PLCC44" }, { "EPM7032AE", "TQFP44" },
                                  { "EPM7064S", "PLCC44" }, { "EPM7064S", "TQFP44" }, { "EPM7064S", "PLCC84" }, { "EPM7064S", "TQFP100" },
                                  { "EPM7064AE", "PLCC44" }, { "EPM7064AE", "TQFP44" }, { "EPM7064AE", "TQFP100" },
                                  { "EPM7128S", "PLCC84" }, { "EPM7128S", "PQFP100" }, { "EPM7128S", "TQFP100" }, { "EPM7128S", "PQFP160" },
                                  { "EPM7128AE", "TQFP100" } };
static const char *QPT[][2] = { { "PLCC44", "44" }, { "TQFP44", "44" }, { "PLCC84", "84" }, { "TQFP100", "100" }, { "PQFP100", "100" }, { "PQFP160", "160" } };
/* dedicated input f of the MAX part = ATF pad CLK1 / OE1 / CLR / CLK2 (pad names "C" 1, "E" 1, "R", "C" 2) */
static const char *dedname(int f) {
    static const char pre[4] = { 'C', 'E', 'R', 'C' };
    static const int num[4] = { 1, 1, -1, 2 };
    return num[f] < 0 ? fmt("%c", pre[f]) : fmt("%c%d", pre[f], num[f]);
}
/* the fitter's values on an empty macrocell (the shared-fuse options xor_a_mux, dfast_mux, reset follow cas_mux,
 * o_mux, xor_invert) */
static const char *UNUSED_MC[][2] = { { "cas_mux", "GND" }, { "d_mux", "comb" }, { "fb_mux", "comb" }, { "gclk_mux", "GCLK2" },
                                      { "gclr_mux", "GND" }, { "low_power", "off" }, { "o_mux", "comb" }, { "oe_mux", "GND" },
                                      { "output_driver", "push_pull" }, { "pt1_mux", "sum" }, { "pt2_mux", "sum" }, { "pt3_mux", "sum" },
                                      { "pt4_func", "clk" }, { "pt4_mux", "sum" }, { "pt5_func", "as" }, { "pt5_mux", "sum" },
                                      { "pt_power", "off" }, { "slew_rate", "slow" }, { "storage", "latch" }, { "xor_b_mux", "VCC_pt12" },
                                      { "xor_invert", "on" } };
#define N_UNUSED ((int)(sizeof UNUSED_MC / sizeof UNUSED_MC[0]))
enum { O_CAS, O_D, O_FB, O_GCLK, O_GCLR, O_LP, O_O, O_OE, O_OD, O_PT1, O_PT2, O_PT3, O_PT4F, O_PT4, O_PT5F, O_PT5, O_PTP, O_SLEW, O_STOR, O_XB, O_XI };
#define PY_BOOL(x) ((x) ? "True" : "False")

static const char *lk(const char *t[][2], int n, const char *k) { for (int i = 0; i < n; i++) if (!strcmp(t[i][0], k)) return t[i][1]; return NULL; }

/* ---- the fuse array being written */
struct fz { int n; signed char *f; char *mine; };
static void put(struct fz *W, int i, int v, int design) {
    if (design) {
        if (W->mine[i] && W->f[i] != v) refuse("INTERNAL_FUSE_CONFLICT", "fuse %d", i);
        W->mine[i] = 1;
    }
    W->f[i] = (signed char)v;
}
static int fcode(const struct afield *f, const char *val) {
    for (int i = 0; i < f->n_vals; i++) if (!strcmp(f->vals[i].name, val)) return f->vals[i].code;
    fatal("field value %s missing", val); return 0;
}
static int has_val(const struct afield *f, const char *val) { for (int i = 0; i < f->n_vals; i++) if (!strcmp(f->vals[i].name, val)) return 1; return 0; }
static void opt(struct fz *W, const struct afield *f, const char *val, int design) {
    int c = fcode(f, val);
    for (int k = 0; k < f->n_fuses; k++) put(W, f->fuses[k], c >> k & 1, design);
}
/* macrocell by number (the tables list macrocells in name order) */
static const struct amc *mcn(const struct atfdev *A, int n) {
    for (int i = 0; i < A->n_mcs; i++) if (A->mcs[i].num == n) return &A->mcs[i];
    fatal("macrocell %d missing", n); return NULL;
}
static const struct afield *mcopt(const struct amc *m, const char *name) {
    for (int i = 0; i < m->n_opts; i++) if (!strcmp(m->opts[i].name, name)) return &m->opts[i].f;
    fatal("macrocell option %s missing", name); return NULL;
}
static const struct afield *cfgopt(const struct atfdev *A, const char *name) {
    for (int i = 0; i < A->n_cfg; i++) if (!strcmp(A->cfg[i].name, name)) return &A->cfg[i].f;
    fatal("config %s missing", name); return NULL;
}

/* ---- JEDEC text in the fitter's layout */
static int fuse_checksum(const signed char *f, int n) {
    unsigned s = 0;
    for (int i = 0; i < n; i += 8) {
        unsigned b = 0;
        for (int k = 0; k < 8 && i + k < n; k++) if (f[i + k]) b |= 1u << k;
        s += b;
    }
    return (int)(s & 0xFFFF);
}
static void bits(sbuf *b, const signed char *f, int a, int n) { for (int i = 0; i < n; i++) sb_addc(b, f[a + i] ? '1' : '0'); }
struct prow { int a, b, k, n; };
static int cmp_prow(const void *x, const void *y) { const struct prow *p = x, *q = y; return p->a < q->a ? -1 : p->a > q->a; }
static char *write_jed(const struct atfdev *A, const struct p2jdev *L, const signed char *f, const char *qp, const char *line2, int security, size_t *len) {
    sbuf b; sb_init(&b);
    sb_printf(&b, "\002JEDEC file for: %s\r\n%s\r\n*\r\nQF%d* QP%s * F0*%s\r\nNOTE\r\n%s", A->name, line2, A->qf, qp, security ? " G1*" : "", L->ruler);
    int np = 0; struct prow *rows = xcalloc((size_t)(A->n_mcs * 5 + 1), sizeof *rows);
    for (int m = 0; m < A->n_mcs; m++)
        for (int p = 0; p < A->mcs[m].n_pt; p++) {
            const struct ptr *t = &A->mcs[m].pt[p];
            int any = 0;
            for (int i = t->a; i < t->b; i++) if (f[i]) { any = 1; break; }
            if (any) { rows[np].a = t->a; rows[np].b = t->b; rows[np].k = atoi(t->name + 2); rows[np].n = A->mcs[m].num; np++; }
        }
    qsort(rows, (size_t)np, sizeof *rows, cmp_prow);
    for (int r = 0; r < np; r++) {
        sb_printf(&b, "L%d\r\n", rows[r].a);
        bits(&b, f, rows[r].a, 16); sb_add(&b, "\r\n");
        bits(&b, f, rows[r].a + 16, 40); sb_add(&b, "\r\n");
        bits(&b, f, rows[r].a + 56, rows[r].b - rows[r].a - 56);
        sb_printf(&b, "*  NOTE PT %d of MC %d *\r\n", rows[r].k, rows[r].n);
    }
    sb_add(&b, "\r\n");
    for (int s = 0; s < L->n_segs; s++) {
        const struct p2jseg *g = &L->segs[s];
        if (g->text) { sb_add(&b, g->text); continue; }
        sb_printf(&b, "L%d%s", g->addr, g->pre);
        int p = g->addr;
        for (int r = 0; r < g->n_runs; r++) { bits(&b, f, p, g->runs[r]); sb_add(&b, g->seps[r]); p += g->runs[r]; }
    }
    sb_printf(&b, "\r\nC%04X*\r\n\003" "0000\r\n", fuse_checksum(f, A->qf));
    *len = b.n;
    return sb_take(&b);
}

/* ---- deterministic augmenting-path matching (demands in order, candidates in order) */
struct kuhn { int nd; int *nc; int **c; int nr; int *owner; char *seen; };
static int k_try(struct kuhn *K, int d) {
    for (int i = 0; i < K->nc[d]; i++) {
        int r = K->c[d][i];
        if (K->seen[r]) continue;
        K->seen[r] = 1;
        if (K->owner[r] < 0 || k_try(K, K->owner[r])) { K->owner[r] = d; return 1; }
    }
    return 0;
}
static int k_run(struct kuhn *K) {
    K->owner = xmalloc(sizeof(int) * (size_t)(K->nr + 1)); K->seen = xmalloc((size_t)K->nr + 1);
    for (int r = 0; r < K->nr; r++) K->owner[r] = -1;
    for (int d = 0; d < K->nd; d++) {
        memset(K->seen, 0, (size_t)K->nr + 1);
        if (!k_try(K, d)) return d;
    }
    return -1;
}

/* ---- MAX side helpers */
static int raw(const struct decoded *X, const char *name, int inst) { return max_raw(X->d, X->bits, name, inst); }
static int clocked(const struct cell *c) { int cs = c->st[S_CLOCK_SELECT]; return cs == 0 || cs == 1 || cs == 2; }
static const int *donor_row(const struct decoded *X, const struct cell *c) {
    const struct maxdev *D = X->d->D;
    for (int i = 0; i < D->n_donor; i++) {
        const int *t = &D->donor[6 * i];
        if (t[0] == c->st[S_OR_INV] && t[1] == c->st[S_REG_OUT] && t[2] == clocked(c)) return t;
    }
    return NULL;
}
static int state_matters(const struct decoded *X, int n) {
    const struct cell *c = &X->cells[n];
    if (c->donor) { const int *t = donor_row(X, c); return t && (t[4] == 1 || t[5] == 1); }
    return c->reg || c->st[S_OR_INV] == 2 || c->st[S_OR_INV] == 3;
}
static int g_row(const struct cell *c, int p) { return c->rows[p].st != 2; }
static int free_slot(const struct decoded *X, int n) {   /* first free sum slot (0..3) or -1 */
    const struct cell *c = &X->cells[n]; const int *st = c->st;
    if ((st[S_INV_SEXP] && !X->live_sexp[n]) || (!st[S_INV_SEXP] && g_row(c, 0))) return 0;
    if ((st[S_INV_XOR] && !c->donor && (st[S_OR_INV] == 0 || st[S_OR_INV] == 2 || st[S_OR_INV] == 3)) || (!st[S_INV_XOR] && g_row(c, 1))) return 1;
    if (g_row(c, 2)) return 2;
    if ((st[S_CLK_CE] && !clocked(c)) || (!st[S_CLK_CE] && g_row(c, 3))) return 3;
    return -1;
}

/* a literal: source name (owned) or foldback number */
struct lit { int flb; char *src; int inv; };
struct lits { int gnd; int n; struct lit *v; };          /* gnd 1 = all fuses 0; n 0 = constant 1 */

struct ctx {
    const struct decoded *X; const struct atfdev *A; const struct p2jdev *L;
    int *pin_src_pin; char **pin_src_name; int n_ps;
};
static char *src_of(const struct ctx *C, const char *net) {
    if (!strncmp(net, "lc", 2)) return fmt("MC%d_FB", atoi(net + 2));
    int p = atoi(net + 3);
    for (int i = 0; i < C->n_ps; i++) if (C->pin_src_pin[i] == p) return xstrdup(C->pin_src_name[i]);
    fatal("pin %d has no source name", p); return NULL;
}
static struct lits mklits(const struct ctx *C, const struct row *r) {
    struct lits L = { 0, 0, NULL };
    if (r->st != 2) { L.gnd = 1; return L; }
    L.v = xcalloc((size_t)r->l.n + 1, sizeof *L.v);
    for (int i = 0; i < r->l.n; i++) {
        const char *s = r->l.v[i]; int inv = s[0] == '!'; if (inv) s++;
        if (!strncmp(s, "sexp", 4)) { L.v[L.n].flb = atoi(s + 4); L.v[L.n].src = NULL; }
        else { L.v[L.n].flb = 0; L.v[L.n].src = src_of(C, s); }
        L.v[L.n].inv = inv; L.n++;
    }
    return L;
}

struct pres { char *jed; size_t len; char *target; char *device; char *package; slist report; slist forms; };
static void form(slist *f, char *s) { if (sl_find(f, s) < 0) sl_push_own(f, s); }
static int cmp_int(const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; return x < y ? -1 : x > y; }

static int pof_security(const unsigned char *d, size_t n) {   /* packet 5, u16; 0 when absent */
    size_t k = 12;
    while (k + 6 <= n) {
        unsigned tag = d[k] | d[k + 1] << 8;
        unsigned long ln = (unsigned long)d[k + 2] | (unsigned long)d[k + 3] << 8 | (unsigned long)d[k + 4] << 16 | (unsigned long)d[k + 5] << 24;
        if (tag == 5 && ln >= 2 && k + 8 <= n) return d[k + 6] | d[k + 7] << 8;
        if (ln > n - k - 6) break;
        k += 6 + ln;
    }
    return 0;
}

static void convert(const unsigned char *data, size_t ndata, const char *part, int closed, struct pres *R) {
    memset(R, 0, sizeof *R);
    struct pof P; pof_parse(data, ndata, &P);
    /* part name: EPM7 + digits + optional S / AE / B */
    const char *dv = P.device ? P.device : "None";
    char mpart[64] = ""; int ok = 0;
    if (!strncmp(dv, "EPM7", 4)) {
        const char *q = dv + 4; while (*q >= '0' && *q <= '9') q++;
        if (q > dv + 4) {
            size_t L = (size_t)(q - dv);
            if (*q == 'S') L += 1; else if (q[0] == 'A' && q[1] == 'E') L += 2; else if (*q == 'B') L += 1;
            if (L < sizeof mpart) { memcpy(mpart, dv, L); mpart[L] = 0; ok = 1; }
        }
    }
    const char *dev = ok ? lk(TGT, 6, mpart) : NULL;
    if (!dev) refuse("NO_ATF_TARGET", "%s: no ATF1502/1504/1508 counterpart (EPM7032/7064/7128 S or AE only)", dv);
    struct maxctx *mc = max_device(P.device);
    const char *pkg = mc->pkg;
    size_t ml = strlen(mpart);
    int asv = ml >= 2 && !strcmp(mpart + ml - 2, "AE");
    if (part && *part) {
        char *p = xstrdup(part);
        for (char *q = p; *q; q++) if (*q >= 'a' && *q <= 'z') *q = (char)(*q - 32);
        size_t pl = strlen(p);
        int pv = pl >= 3 && !strcmp(p + pl - 3, "ASV");
        if (pv) p[pl - 1] = 0;
        if (strcmp(p, dev)) refuse("PART_PAIR_MISMATCH", "%s is not the AS or ASV part of the %s density (%s / %sV)", part, mpart, dev, dev);
        asv = pv;
    }
    int pair = 0;
    for (size_t i = 0; i < sizeof PAIRS / sizeof PAIRS[0]; i++) if (!strcmp(PAIRS[i][0], mpart) && !strcmp(PAIRS[i][1], pkg)) pair = 1;
    if (!pair) refuse("NO_ATF_TARGET_PACKAGE", "%s %s: no %s in this package with the same pins", mpart, pkg, dev);
    R->device = xstrdup(mpart); R->package = xstrdup(pkg); R->target = fmt("%s%s", dev, asv ? "V" : "");
    struct decoded Xs; max_decode(data, ndata, &Xs);
    const struct decoded *X = &Xs;
    max_to_netlist(X);
    const struct maxctx *d = X->d;
    const int nmc = d->D->n_mc;
    const struct atfdev *A = atf_find(dev);
    const struct p2jdev *L = NULL;
    for (int i = 0; i < n_p2j_devices; i++) if (!strcmp(p2j_devices[i].name, dev)) L = &p2j_devices[i];
    struct fz W = { A->qf, xmalloc((size_t)A->qf), xcalloc((size_t)A->qf, 1) };
    for (int i = 0; i < A->qf; i++) W.f[i] = -1;
    /* P1 power-up high */
    {
        slist s = { 0 }; int cnt = 0;
        for (int n = 1; n <= nmc; n++)
            if (X->live[n] && X->cells[n].pu == 1 && state_matters(X, n)) { if (cnt < 8) sl_push_own(&s, fmt("%d", n)); cnt++; }
        if (cnt) refuse("REGISTER_POWERS_UP_HIGH", "LC%s powers up high; ATF150x registers power up low", sl_join(&s, ","));
    }
    /* I/O cells in pin order */
    int nio = X->n_ios; int *ord = xmalloc(sizeof(int) * (size_t)(nio + 1));
    for (int i = 0; i < nio; i++) ord[i] = X->ios[i].pin * 4096 + i;
    qsort(ord, (size_t)nio, sizeof(int), cmp_int);
    for (int i = 0; i < nio; i++) ord[i] &= 4095;
    /* P2 ground pins */
    int *clamp_pin = xcalloc((size_t)nmc + 2, sizeof(int));
    for (int k = 0; k < nio; k++) {
        const struct io *io = &X->ios[ord[k]];
        if (io->has_clamp && io->clamp && io->lc == 0) {
            if (X->live[io->clamp_lc]) refuse("GND_CLAMP_MACROCELL_USED", "pin %d is a ground pin and LC%d carries logic", io->pin, io->clamp_lc);
            clamp_pin[io->clamp_lc] = io->pin;
        }
    }
    struct ctx C = { X, A, L, NULL, NULL, 0 };
    C.pin_src_pin = xmalloc(sizeof(int) * (size_t)(d->n_io + d->n_ib + 1)); C.pin_src_name = xmalloc(sizeof(char *) * (size_t)(d->n_io + d->n_ib + 1));
    for (int i = 0; i < d->n_io; i++) {
        if (d->io_val[i] == 0) continue;
        C.pin_src_pin[C.n_ps] = d->io_val[i];
        C.pin_src_name[C.n_ps++] = fmt("%s_PAD", atf_padname(&mcn(A, d->io_key[i] + 1)->pad));
    }
    for (int i = 0; i < d->n_ib; i++) { C.pin_src_pin[C.n_ps] = d->ib_val[i]; C.pin_src_name[C.n_ps++] = fmt("%s_PAD", dedname(d->ib_key[i])); }
    /* P3 / P3b enables: kind 0 none, 1 vcc, 2 goe, 3 pt5 */
    int *oek = xcalloc((size_t)nmc + 2, sizeof(int)); char **oenet = xcalloc((size_t)nmc + 2, sizeof(char *)); int *oeinv = xcalloc((size_t)nmc + 2, sizeof(int));
    int *reloc = xmalloc(sizeof(int) * (size_t)(nmc + 2)); for (int i = 0; i <= nmc + 1; i++) reloc[i] = -1;
    for (int k = 0; k < nio; k++) {
        const struct io *io = &X->ios[ord[k]];
        if (io->lc == 0) continue;
        int n = io->lc;
        if (!strcmp(io->oe, "vcc")) { oek[n] = 1; continue; }
        int inv = io->oe[0] == '!'; const char *net = io->oe + inv;
        if (!strncmp(net, "lc", 2)) {
            const struct cell *c = &X->cells[n];
            int busy = X->live[n] && c->rows[4].st == 2;
            if (busy && (c->st[S_PRE] || free_slot(X, n) < 0))
                refuse("PT_OE_CONFLICT", "pin %d: enable from LC%d needs PT5 of LC%d, which carries %s", io->pin, atoi(net + 2), n,
                       c->st[S_PRE] ? "a preset" : "a sum term and no slot is free");
            if (busy) reloc[n] = free_slot(X, n);
            oek[n] = 3;
        } else oek[n] = 2;
        oenet[n] = xstrdup(net); oeinv[n] = inv;
    }
    /* P4 donors */
    for (int n = 1; n <= nmc; n++) {
        const struct cell *c = &X->cells[n];
        if (!X->live[n] || !c->donor) continue;
        if (n % 16 == 0) refuse("DONOR_AT_BLOCK_EDGE", "LC%d lends its sum across a block edge", n);
        const int *t = donor_row(X, c);
        if ((t[3] == OPM_NORMAL || t[3] == OPM_INVERT) && !c->st[S_INV_XOR] && c->rows[1].st == 2)
            refuse("DONOR_FORM", "LC%d: own value 0 needs PT2, which carries a sum term", n);
        if (t[3] == OPM_INVERT && t[5] == 0 && X->live_sexp[n]) refuse("DONOR_SEXP_FORM", "LC%d: inverted own value and its shared expander is read", n);
    }
    /* defaults */
    for (int m = 0; m < A->n_mcs; m++) {
        const struct amc *M = &A->mcs[m];
        for (int p = 0; p < M->n_pt; p++) for (int i = M->pt[p].a; i < M->pt[p].b; i++) put(&W, i, 0, 0);
        for (int k = 0; k < N_UNUSED; k++) opt(&W, mcopt(M, UNUSED_MC[k][0]), UNUSED_MC[k][1], 0);
    }
    for (int s = 0; s < A->n_sws; s++) opt(&W, &A->sws[s].mux, "GND1", 0);
    for (int k = 0; k < 6; k++) { opt(&W, &A->glob[G_GOE1_MUX + k], "GND1", 0); put(&W, A->goe_inv0 + k, 1, 0); }
    opt(&W, &A->glob[G_GCLK1_MUX], fmt("%s_PAD", dedname(0)), 0); opt(&W, &A->glob[G_GCLK1_INV], "off", 0);
    opt(&W, &A->glob[G_GCLK2_MUX], fmt("%s_PAD", dedname(3)), 0); opt(&W, &A->glob[G_GCLK2_INV], "off", 0);
    opt(&W, &A->glob[G_GCLK3_MUX], L->clk3, 0); opt(&W, &A->glob[G_GCLK3_INV], "off", 0);
    opt(&W, &A->glob[G_GCLR_INV], "off", 0);
    for (int i = 0; i < L->n_pinfix; i++) put(&W, L->pinfix[2 * i], L->pinfix[2 * i + 1], 0);
    {
        const char *cv[][2] = { { "termination", "high_z" }, { "pd1_pin_func", "user" }, { "pd2_pin_func", "user" },
                                { "reset_hysteresis", asv ? "large" : "small" }, { "jtag_pin_func", X->jtag ? "jtag" : "user" },
                                { "arming_switch", "armed" }, { "read_protection", "off" } };
        for (int i = 0; i < 7; i++) opt(&W, cfgopt(A, cv[i][0]), cv[i][1], 0);
    }
    for (int i = 0; i < A->n_uncov; i++) if (A->uncov[2 * i + 1] >= 0) put(&W, A->uncov[2 * i], A->uncov[2 * i + 1], 0);
    /* global clocks / clear */
    {
        int used[3] = { 0, 0, 0 };
        for (int n = 1; n <= nmc; n++) if (X->live[n]) { int cs = X->cells[n].st[S_CLOCK_SELECT]; if (cs == 1 || cs == 2) used[cs] = 1; }
        sbuf f; sb_init(&f); sb_add(&f, "GCLK");
        for (int cs = 1; cs <= 2; cs++) {
            if (!used[cs]) continue;
            int g = -1; for (int i = 0; i < X->n_gclk; i++) if (X->gclk_net[i] == cs) g = i;
            opt(&W, &A->glob[cs == 1 ? G_GCLK1_MUX : G_GCLK2_MUX], fmt("%s_PAD", dedname(X->gclk_ib[g])), 1);
            opt(&W, &A->glob[cs == 1 ? G_GCLK1_INV : G_GCLK2_INV], X->gclk_inv[g] ? "on" : "off", 1);
            sb_printf(&f, " %d:%sIB%d", cs, X->gclk_inv[g] ? "~" : "", X->gclk_ib[g]);
        }
        form(&R->forms, sb_take(&f));
        int gclr = 0; for (int n = 1; n <= nmc; n++) if (X->live[n] && X->cells[n].st[S_CLEAR_SELECT] == 1) gclr = 1;
        if (gclr) { opt(&W, &A->glob[G_GCLR_INV], "on", 1); form(&R->forms, xstrdup("GCLR")); }
    }
    /* macrocells */
    struct lits *rows = xcalloc((size_t)(nmc + 2) * 5, sizeof *rows);
    char *has_rows = xcalloc((size_t)nmc + 2, 1);
    for (int n = 1; n <= nmc; n++) {
        if (!X->live[n]) continue;
        const struct cell *c = &X->cells[n]; const int *st = c->st; const struct amc *M = mcn(A, n);
        const char *o[N_UNUSED]; for (int k = 0; k < N_UNUSED; k++) o[k] = UNUSED_MC[k][1];
        o[O_PTP] = "on"; o[O_STOR] = "dff"; o[O_D] = "comb";
        struct lits Rr[5]; for (int p = 0; p < 5; p++) Rr[p] = mklits(&C, &c->rows[p]);
        struct lits GND = { 1, 0, NULL };
        int cs = st[S_CLOCK_SELECT], ck = clocked(c);
        if (st[S_INV_SEXP]) { o[O_PT1] = "flb"; if (!X->live_sexp[n]) Rr[0] = GND; }
        o[O_PT2] = st[S_INV_XOR] ? "xor" : "sum";
        o[O_PT3] = st[S_CLR] ? "ar" : "sum";
        if (cs == 0) { o[O_PT4] = "clk_ce"; o[O_PT4F] = "clk"; }
        else if (cs == 1 || cs == 2) { o[O_PT4] = st[S_CLK_CE] ? "clk_ce" : "sum"; o[O_PT4F] = "ce"; o[O_GCLK] = cs == 1 ? "GCLK1" : "GCLK2"; }
        else { o[O_PT4] = "sum"; o[O_PT4F] = "clk"; if (st[S_CLK_CE]) Rr[3] = GND; }
        o[O_PT5] = st[S_PRE] ? "as_oe" : "sum"; o[O_PT5F] = "as";
        o[O_GCLR] = st[S_CLEAR_SELECT] == 1 ? "GCLR" : "GND";
        if (c->donor) {
            const int *t = donor_row(X, c); int opm = t[3], om = t[4], rm = t[5];
            o[O_CAS] = "sum";
            if (opm == OPM_VCC) { if (st[S_INV_XOR]) Rr[1] = GND; o[O_PT2] = "sum"; }
            else { o[O_PT2] = "xor"; if (!st[S_INV_XOR]) Rr[1] = GND; }
            if (opm == OPM_INVERT && rm == 0) { o[O_PT1] = "sum"; if (st[S_INV_SEXP]) Rr[0] = GND; }
            o[O_XB] = rm == 1 ? "ff_qn" : "VCC_pt12";
            o[O_XI] = opm == OPM_INVERT ? "off" : "on";
            o[O_O] = o[O_FB] = om == 1 ? "sync" : "comb";
            form(&R->forms, fmt("DONOR %s %s %s", opm == OPM_VCC ? "vcc" : opm == OPM_NORMAL ? "normal" : "invert", om == 1 ? "reg" : "comb", rm == 1 ? "tff" : "dff"));
        } else {
            int oi = st[S_OR_INV];
            o[O_XB] = (oi == 2 || oi == 3) ? "ff_qn" : "VCC_pt12";
            o[O_XI] = (oi == 0 || oi == 2) ? "on" : "off";
            o[O_O] = o[O_FB] = c->reg ? "sync" : "comb";
            form(&R->forms, fmt("XOR %d %s %s", oi, PY_BOOL(st[S_INV_XOR]), o[O_O]));
        }
        o[O_LP] = raw(X, "MCELL_USE_TURBO", n - 1) ? "on" : "off";
        int ek = oek[n];
        if (ek == 0) o[O_OE] = "GND";
        else if (ek == 1) o[O_OE] = "VCC_pt5";
        else if (ek == 3) {
            if (reloc[n] >= 0) {
                int k = reloc[n];
                Rr[k] = Rr[4];
                o[k == 0 ? O_PT1 : k == 1 ? O_PT2 : k == 2 ? O_PT3 : O_PT4] = "sum";
                if (k == 3 && !ck) o[O_PT4F] = "clk";
                form(&R->forms, fmt("RELOC %d", k + 1));
            }
            struct lits e = { 0, 1, xcalloc(1, sizeof(struct lit)) };
            e.v[0].flb = 0; e.v[0].src = src_of(&C, oenet[n]); e.v[0].inv = oeinv[n];
            Rr[4] = e;
            o[O_PT5] = "as_oe"; o[O_PT5F] = "oe"; o[O_OE] = "VCC_pt5";
            form(&R->forms, fmt("PT5OE %s", PY_BOOL(oeinv[n])));
        }
        for (int p = 0; p < 5; p++) rows[n * 5 + p] = Rr[p];
        has_rows[n] = 1;
        form(&R->forms, fmt("CELL %s %s %s %s %s %s %s %s %s", o[O_PT1], o[O_PT2], o[O_PT3], o[O_PT4], o[O_PT4F], o[O_PT5], o[O_PT5F], o[O_GCLR],
                            ck ? "clocked" : "unclocked"));
        for (int k = 0; k < N_UNUSED; k++) {
            if (k == O_OD || k == O_SLEW) continue;
            if (k == O_OE && ek == 2) continue;
            opt(&W, mcopt(M, UNUSED_MC[k][0]), o[k], 1);
        }
    }
    for (int n = 1; n <= nmc; n++) {
        if (!clamp_pin[n]) continue;
        const struct amc *M = mcn(A, n);
        for (int k = 0; k < N_UNUSED; k++) {
            if (k == O_OD || k == O_SLEW) continue;
            opt(&W, mcopt(M, UNUSED_MC[k][0]), k == O_OE ? "VCC_pt5" : k == O_STOR ? "dff" : UNUSED_MC[k][1], 1);
        }
        form(&R->forms, xstrdup("GROUND_PIN"));
    }
    /* I/O cells with an output */
    char goe_set[6] = { 0 }; char *goe_src[6] = { 0 }; int goe_inv[6] = { 0 };
    for (int n = 1; n <= nmc; n++) {
        if (!oek[n]) continue;
        const struct amc *M = mcn(A, n); int io_i = n - 1;
        if (!X->live[n]) refuse("OUTPUT_CELL_NOT_LIVE", "LC%d", n);
        const struct io *io = max_io_at(X, max_io_pin(d, io_i));
        opt(&W, mcopt(M, "output_driver"), io->od ? "open_drain" : "push_pull", 1);
        opt(&W, mcopt(M, "slew_rate"), raw(X, "IO_USE_SLOW_SLEW", io_i) ? "slow" : "fast", 1);
        form(&R->forms, fmt("OD %s", PY_BOOL(io->od)));
        if (oek[n] == 2) {
            char *s_ = src_of(&C, oenet[n]);
            int poe = max_setting(d, X->bits, "IO_SELECT_OE_MAX7K", io_i) - 2;
            const struct maxdev *D = d->D;
            /* fan-in of global OE poe in position order */
            int nf = 0; char *fan[64];
            for (int pos = 0; pos < 64; pos++)
                for (int i = 0; i < D->n_poesrc; i++) {
                    const int *t = &D->poesrc[4 * i];
                    if (t[0] == poe && t[1] == pos && nf < 64) {
                        int kind = t[2], idx = t[3];
                        fan[nf++] = kind == SRC_MCELL ? fmt("MC%d_FB", idx + 1) : kind == SRC_IO ? fmt("%s_PAD", atf_padname(&mcn(A, idx + 1)->pad)) : fmt("%s_PAD", dedname(idx));
                    }
                }
            int found = -1, nfound = 0;
            for (int g = 0; g < 6; g++) {   /* GOE1..GOE6 = sorted names */
                int all = 1;
                for (int i = 0; i < nf; i++) if (!has_val(&A->glob[G_GOE1_MUX + g], fan[i])) all = 0;
                if (all) { if (found < 0) found = g; nfound++; }
            }
            if (nfound != 1) refuse("GLOBAL_OE_UNROUTABLE", "global OE %d of %s has %d ATF networks with its fan-in", poe, d->dev, nfound);
            int g = found;
            if (goe_set[g] && (strcmp(goe_src[g], s_) || goe_inv[g] != oeinv[n])) refuse("INTERNAL_GOE_CONFLICT", "GOE%d", g + 1);
            goe_set[g] = 1; goe_src[g] = s_; goe_inv[g] = oeinv[n];
            opt(&W, &A->glob[G_GOE1_MUX + g], s_, 1);
            put(&W, A->goe_inv0 + g, oeinv[n] ? 0 : 1, 1);
            opt(&W, mcopt(M, "oe_mux"), fmt("GOE%d", g + 1), 1);
            form(&R->forms, fmt("GOE %s", PY_BOOL(oeinv[n])));
        }
    }
    /* UIM: per block (sorted name), demands sorted by name, the block's switches in table order */
    slist blocks = { 0 };
    for (int b = 0; b < A->n_blks; b++) sl_push(&blocks, A->blks[b].name);
    sl_sort(&blocks);
    char ***uim_of = xcalloc((size_t)A->n_blks, sizeof(char **)); slist *needs = xcalloc((size_t)A->n_blks, sizeof(slist));
    for (int n = 1; n <= nmc; n++) {
        if (!has_rows[n]) continue;
        int bi = sl_find(&blocks, mcn(A, n)->block);
        for (int p = 0; p < 5; p++)
            for (int i = 0; i < rows[n * 5 + p].n; i++)
                if (!rows[n * 5 + p].v[i].flb && sl_find(&needs[bi], rows[n * 5 + p].v[i].src) < 0) sl_push(&needs[bi], rows[n * 5 + p].v[i].src);
    }
    int **uim_sw = xcalloc((size_t)A->n_blks, sizeof(int *));
    for (int bi = 0; bi < blocks.n; bi++) {
        slist *dem = &needs[bi];
        if (!dem->n) continue;
        sl_sort(dem);
        int nsw = 0; int *sw = xmalloc(sizeof(int) * (size_t)(A->n_sws + 1));
        for (int s = 0; s < A->n_sws; s++) if (!strcmp(A->sws[s].block, blocks.v[bi])) sw[nsw++] = s;
        struct kuhn K = { dem->n, xcalloc((size_t)dem->n, sizeof(int)), xcalloc((size_t)dem->n, sizeof(int *)), A->n_sws, NULL, NULL };
        for (int i = 0; i < dem->n; i++) {
            K.c[i] = xmalloc(sizeof(int) * (size_t)(nsw + 1));
            for (int j = 0; j < nsw; j++) if (has_val(&A->sws[sw[j]].mux, dem->v[i])) K.c[i][K.nc[i]++] = sw[j];
        }
        int bad = k_run(&K);
        if (bad >= 0) refuse("UIM_UNROUTABLE", "block %s: no free UIM line for %s", blocks.v[bi], dem->v[bad]);
        uim_sw[bi] = xmalloc(sizeof(int) * (size_t)dem->n);
        for (int r = 0; r < A->n_sws; r++) if (K.owner[r] >= 0) uim_sw[bi][K.owner[r]] = r;
        /* writes in demand order (the reference writes in the order of its result dict: demand order) */
        for (int i = 0; i < dem->n; i++) opt(&W, &A->sws[uim_sw[bi][i]].mux, dem->v[i], 1);
    }
    (void)uim_of;
    /* PT rows */
    for (int n = 1; n <= nmc; n++) {
        if (!has_rows[n]) continue;
        const struct amc *M = mcn(A, n);
        int bi = sl_find(&blocks, M->block);
        const struct ablk *B = NULL; for (int b = 0; b < A->n_blks; b++) if (!strcmp(A->blks[b].name, M->block)) B = &A->blks[b];
        for (int p = 0; p < 5; p++) {
            const struct lits *Lr = &rows[n * 5 + p];
            const struct ptr *t = NULL;
            char pn[16]; snprintf(pn, sizeof pn, "PT%d", p + 1);
            for (int i = 0; i < M->n_pt; i++) if (!strcmp(M->pt[i].name, pn)) t = &M->pt[i];
            if (Lr->gnd) { for (int i = t->a; i < t->b; i++) put(&W, i, 0, 1); continue; }
            char *col = xcalloc((size_t)(t->b - t->a) + 1, 1);
            for (int i = 0; i < Lr->n; i++) {
                int kind, num;
                if (Lr->v[i].flb) { kind = 3; num = Lr->v[i].flb; }
                else {
                    int di = sl_find(&needs[bi], Lr->v[i].src);
                    kind = Lr->v[i].inv ? 2 : 1; num = atoi(A->sws[uim_sw[bi][di]].name + 3);
                }
                int off = -1;
                for (int k = 0; k < B->w; k++) if (B->cols[2 * k] == kind && B->cols[2 * k + 1] == num) off = k;
                if (off < 0) fatal("column missing");
                col[off] = 1;
            }
            for (int i = t->a; i < t->b; i++) put(&W, i, col[i - t->a] ? 0 : 1, 1);
        }
    }
    /* user code */
    {
        const struct setting *us = max_find_setting(d, "JTAG_WRITE_USER_CODE");
        /* bit by bit from the setting's addresses: the AE code has 32 bits, more than an int shift allows */
        int nb = us->n_bits;
        int nmap = !strncmp(mpart, "EPM7128", 7) ? 15 : 16;
#define UBIT(k) (us->addr[k] >= 0 ? X->bits[us->addr[k]] : 0)
        for (int k = 0; k < L->n_user; k++) put(&W, L->user[k], k < nmap ? UBIT(k) : 1, 1);
        for (int k = nmap; k < nb; k++)
            if (!UBIT(k)) sl_push_own(&R->report, fmt("DROPPED USER_CODE_BIT_%d: 0 in the POF; the ATF part has no user fuse for it", k));
#undef UBIT
    }
    {
        const struct p2jfinp *fp = NULL;
        for (int i = 0; i < n_p2j_finp; i++) if (!strcmp(p2j_finp[i].part, mpart)) fp = &p2j_finp[i];
        int nd = 0;
        for (int i = 0; i < d->n_io; i++) {
            if (!d->io_val[i]) continue;
            int a = d->io_key[i] < fp->n ? fp->addr[d->io_key[i]] : -1;
            if (a < 0) fatal("no IO_USE_FINP_BUF bit for I/O cell %d", d->io_key[i]);
            if (X->bits[a] != 1) nd++;
        }
        if (nd) sl_push_own(&R->report, fmt("DROPPED IO_USE_FINP_BUF: not the default on %d IO cells; no logic effect on this part", nd));
    }
    /* an S-part unused cell (registered, no clock, clear constant true, output always enabled) is written as an output
     * driving 0, as the image reads; the report names each such pin, in pin order */
    if (!strcmp(d->fam, "max7000s"))
        for (int k = 0; k < nio; k++) {
            const struct io *io = &X->ios[ord[k]];
            if (!io->lc || strcmp(io->oe, "vcc") || io->od) continue;
            const struct cell *c = &X->cells[io->lc];
            if (c->reg && !c->donor && !clocked(c) && c->st[S_CLR] && c->rows[2].st == 2 && c->rows[2].l.n == 0)
                sl_push_own(&R->report, fmt("pin %d drives 0: level read from the image, not stated by the vendor", io->pin));
        }
    sl_push(&R->report, "PIN_KEEPER off (MAX 7000S/AE pins have no bus hold)");
    for (int i = 0; i < A->qf; i++) if (W.f[i] < 0) refuse("INTERNAL_FUSE_UNSET", "fuse %d", i);
    form(&R->forms, fmt("JTAG %s", PY_BOOL(X->jtag)));
    if (closed) {
        slist miss = { 0 };
        for (int i = 0; i < R->forms.n; i++) {
            int ok2 = 0;
            for (int j = 0; j < n_p2j_forms; j++) if (!strcmp(p2j_forms[j][0], R->target) && !strcmp(p2j_forms[j][1], R->forms.v[i])) ok2 = 1;
            if (!ok2) sl_push(&miss, R->forms.v[i]);
        }
        if (miss.n) { sl_sort(&miss); refuse("FORM_NOT_PROVEN", "%s: %s", R->target, sl_join(&miss, "; ")); }
    }
    R->jed = write_jed(A, L, W.f, lk(QPT, 6, pkg), fmt("Created on: " TOOL_VER " (from %s)", P.device), pof_security(data, ndata) != 0, &R->len);
}

int run_pof2jed(const char *in, const char *out, const char *part) {
    size_t n; unsigned char *data = read_file(in, &n);
    if (!data) fatal("cannot read %s", in);
    struct catcher c; int rc = 0;
    if (TRY(c)) {
        struct pres R; convert(data, n, part, 1, &R);
        struct catcher c2; char *dv = xstrdup(R.target); size_t dl = strlen(dv); if (dv[dl - 1] == 'V') dv[dl - 1] = 0;
        if (TRY(c2)) {
            struct anet N; atf_decode((const unsigned char *)R.jed, R.len, dv, R.package, &N);
            if (N.nu) refuse("UNDETERMINED", "%s", N.und[0].name);
            ENDTRY(c2);
        } else { ENDTRY(c2); refuse("JED_SELF_CHECK", "the ATF decoder refuses the new file: %s %s", refusal_name, refusal_detail ? refusal_detail : ""); }
        FILE *f = fopen(out, "wb");
        if (!f || fwrite(R.jed, 1, R.len, f) != R.len) { if (f) fclose(f); remove(out); fatal("cannot write %s", out); }
        fclose(f);
        size_t m; unsigned char *back = read_file(out, &m);
        if (!back || m != R.len || memcmp(back, R.jed, m)) { remove(out); refuse("JED_WRITE_CHECK", "%s does not read back as written", out); }
        put_text(stdout, fmt("target %s\n", R.target));
        for (int i = 0; i < R.report.n; i++) put_text(stdout, fmt("%s\n", R.report.v[i]));
        put_text(stdout, fmt("wrote %s\n", path_base(out)));
        ENDTRY(c);
    } else {
        ENDTRY(c);
        put_text(stdout, fmt("REFUSED %s: %s\n", refusal_name, refusal_detail ? refusal_detail : ""));
        rc = 3;
    }
    return rc;
}

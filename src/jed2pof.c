/* jed2pof.c -- ATF1502AS / ATF1504AS / ATF1508AS JEDEC file -> MAX 7000S / 7000AE POF file.
 *
 * The JEDEC file is decoded (atf_decode); every macrocell whose value or register can reach an enabled output pin is
 * written into the MAX logic cell of the same number (the two families put macrocell n on the same pin), setting by
 * setting; every other fuse takes the value the vendor's assembler writes where nothing is used. The PIA lines and the
 * global output enables are chosen by a deterministic augmenting-path matching over the routing tables. A file is
 * written only when every form it uses is in the table of forms checked on the reference designs (tab_forms.c), and
 * only after the MAX decoder has read the new image back without a refusal. Anything else is refused by name.
 */
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "rx.h"
#include "jedec.h"
#include "atf.h"
#include "max.h"
#include "tables.h"

extern const char *const dev_pairs[][2]; extern const int n_dev_pairs;
extern const char *const jed2pof_forms[][2]; extern const int n_jed2pof_forms;

#define CREATOR TOOL_VER
static const char *TGT[3][3] = { { "ATF1502AS", "EPM7032S", "EPM7032AE" }, { "ATF1504AS", "EPM7064S", "EPM7064AE" },
                                 { "ATF1508AS", "EPM7128S", "EPM7128AE" } };
static const char *SPEED[][2] = { { "EPM7032S", "-5" }, { "EPM7064S", "-5" }, { "EPM7128S", "-6" }, { "EPM7032AE", "-4" },
                                  { "EPM7064AE", "-4" }, { "EPM7128AE", "-5" } };
static const char *PKGCODE[][2] = { { "PLCC44", "LC44" }, { "TQFP44", "TC44" }, { "PLCC84", "LC84" }, { "TQFP100", "TC100" },
                                    { "PQFP100", "QC100" }, { "PQFP160", "QC160" } };
/* or-invert setting 3 (invert + T) and donor mode (2, 0, clocked) accepted by the MAX decoder's part tables */
static const char *OR3[] = { "EPM7032S", "EPM7128S", "EPM7128AE" };
static const char *DONOR_TFF[] = { "EPM7032S", "EPM7128S", "EPM7032AE", "EPM7128AE" };
/* first user fuse of each part (16 consecutive fuses, two groups of 8) */
static const int USER0[3] = { 16786, 34170, 74118 };

static int in_list(const char **l, int n, const char *s) { for (int i = 0; i < n; i++) if (!strcmp(l[i], s)) return 1; return 0; }
static const char *lookup2(const char *t[][2], int n, const char *k) { for (int i = 0; i < n; i++) if (!strcmp(t[i][0], k)) return t[i][1]; return NULL; }

/* ---- a set of names (linear: the netlists are small) */
static int has(const slist *s, const char *x) { return sl_find(s, x) >= 0; }
static void add(slist *s, const char *x) { if (!has(s, x)) sl_push(s, x); }

/* identifiers of an expression, as re.findall("[A-Za-z_][A-Za-z0-9_]*") finds them */
static void idents(const char *e, slist *out) {
    for (const char *p = e; *p;) {
        if ((*p >= 'A' && *p <= 'Z') || (*p >= 'a' && *p <= 'z') || *p == '_') {
            const char *q = p + 1;
            while ((*q >= 'A' && *q <= 'Z') || (*q >= 'a' && *q <= 'z') || (*q >= '0' && *q <= '9') || *q == '_') q++;
            sl_push_own(out, xstrndup(p, (size_t)(q - p)));
            p = q;
        } else p++;
    }
}

static const char *wire(const struct anet *N, const char *name) { return atf_wire(N, name); }

/* names reachable from the enabled output pads through wires and register functions */
static void observable(const struct anet *N, slist *seen) {
    slist todo = { 0 };
    for (int i = 0; i < N->no; i++) { sl_push(&todo, N->out[i].a); sl_push(&todo, N->out[i].en); }
    while (todo.n) {
        char *e = todo.v[--todo.n];
        slist ids = { 0 }; idents(e, &ids);
        for (int k = 0; k < ids.n; k++) {
            const char *nm = ids.v[k];
            if (has(seen, nm)) continue;
            sl_push(seen, nm);
            const char *w = wire(N, nm);
            if (w) sl_push(&todo, w);
            for (int r = 0; r < N->nr; r++) {
                char *q = fmt("%s_q", py_lower(N->r[r].mc));
                if (!strcmp(q, nm)) {
                    sl_push(&todo, N->r[r].clk); sl_push(&todo, N->r[r].ce); sl_push(&todo, N->r[r].ar);
                    sl_push(&todo, N->r[r].as); sl_push(&todo, N->r[r].d);
                }
            }
        }
    }
}

/* ---- MAX sources: kind rank as Python sorts the kind names (FINPBUF < IO < MCELL) */
enum { K_FINPBUF = 0, K_IO = 1, K_MCELL = 2 };
static const char *KNAME[] = { "FINPBUF", "IO", "MCELL" };
static int src_kind_tab(int k) { return k == K_MCELL ? SRC_MCELL : k == K_IO ? SRC_IO : SRC_FINPBUF; }
struct src { int kind, idx; };

struct jctx {
    const struct anet *N; const struct atfdev *A; const struct maxdev *D; const char *tgt;
    unsigned char *bits;
    int nmc_pad; char **mcpad;         /* pad name of macrocell n (index n), NULL = none */
};

static int mc_of_pad(const struct jctx *C, const char *pad) {
    for (int n = 1; n <= C->nmc_pad; n++) if (C->mcpad[n] && !strcmp(C->mcpad[n], pad)) return n;
    return 0;
}
/* the ATF dedicated input pads, by MAX dedicated input number: clock 1, output enable 1, clear, clock 2 */
static char *dname(int f) {
    static const char pre[4] = { 'C', 'E', 'R', 'C' };
    static const int num[4] = { 1, 1, -1, 2 };
    return num[f] < 0 ? fmt("%c", pre[f]) : fmt("%c%d", pre[f], num[f]);
}

static struct src maxsrc(const struct jctx *C, const char *e) {
    struct src s = { -1, -1 };
    if (startswith(e, "fb_MC") && e[5]) { s.kind = K_MCELL; s.idx = atoi(e + 5) - 1; return s; }
    if (startswith(e, "pad_") && e[4]) {
        const char *p = e + 4;
        int n = mc_of_pad(C, p);
        if (n) { s.kind = K_IO; s.idx = n - 1; return s; }
        for (int f = 0; f < 4; f++) if (!strcmp(p, dname(f))) { s.kind = K_FINPBUF; s.idx = f; return s; }
    }
    refuse("SOURCE_NO_MAX_COUNTERPART", "%s", e);
    return s;
}

/* ---- the bit image */
static const struct setting *st_of(const struct maxdev *D, const char *name) {
    for (int i = 0; i < D->n_settings; i++) if (!strcmp(D->settings[i].name, name) && D->settings[i].n_inst > 0) return &D->settings[i];
    return NULL;
}
/* writes bit by bit and stops at the first bit the part does not have (returns 0 then) */
static int set_raw(struct jctx *C, const char *name, int inst, int value) {
    const struct setting *s = st_of(C->D, name);
    if (!s || inst >= s->n_inst) fatal("setting %s instance %d missing", name, inst);
    for (int j = 0; j < s->n_bits; j++) {
        int a = s->addr[inst * s->n_bits + j];
        if (a < 0) return 0;
        C->bits[a] = (unsigned char)(value >> j & 1);
    }
    return 1;
}
static void set_setting(struct jctx *C, const char *name, int inst, int sv) {
    const struct setting *s = st_of(C->D, name);
    if (!s) fatal("setting %s missing", name);
    set_raw(C, name, inst, s->n_map >= 0 ? s->map[sv] : sv);
}
static void write_row(struct jctx *C, int mc, int pt, const ilist *cols, int gnd) {
    const struct maxdev *D = C->D;
    for (int c = 0; c < D->cols; c++) {
        int a = D->cols * D->n_pt * mc + D->cols * pt + c;
        C->bits[a] = gnd ? 0 : (il_find(cols, c) >= 0 ? 0 : 1);
    }
}

/* ---- fan-in of a PIA buffer / global OE: position of a source, -1 = absent */
static int buf_pos(const struct maxdev *D, int b, struct src s) {
    for (int i = 0; i < D->n_bufsrc; i++) {
        const int *r = &D->bufsrc[4 * i];
        if (r[0] == b && r[2] == src_kind_tab(s.kind) && r[3] == s.idx) return r[1];
    }
    return -1;
}
static int poe_pos(const struct maxdev *D, int k, struct src s) {
    for (int i = 0; i < D->n_poesrc; i++) {
        const int *r = &D->poesrc[4 * i];
        if (r[0] == k && r[2] == src_kind_tab(s.kind) && r[3] == s.idx) return r[1];
    }
    return -1;
}

/* ---- deterministic augmenting-path matching. A resource holds up to 2 exclusive keys. */
struct res { int a, b, nk; int k[2]; };
struct kuhn { int nd; int *nc; struct res **c; int *m; int nkeys; unsigned char *seen; };
static int k_try(struct kuhn *K, int d) {
    for (int i = 0; i < K->nc[d]; i++) {
        struct res *r = &K->c[d][i];
        int hit = 0;
        for (int q = 0; q < r->nk; q++) if (K->seen[r->k[q]]) hit = 1;
        if (hit) continue;
        for (int q = 0; q < r->nk; q++) K->seen[r->k[q]] = 1;
        int nb = 0, b = -1;
        for (int e = 0; e < K->nd; e++) {
            if (K->m[e] < 0 || e == d) continue;
            struct res *o = &K->c[e][K->m[e]];
            int shared = 0;
            for (int x = 0; x < o->nk; x++) for (int y = 0; y < r->nk; y++) if (o->k[x] == r->k[y]) shared = 1;
            if (shared) { nb++; b = e; }
        }
        if (nb > 1) continue;
        if (nb == 1) {
            int old = K->m[b]; K->m[b] = -1;
            if (!k_try(K, b)) { K->m[b] = old; continue; }
        }
        K->m[d] = i;
        return 1;
    }
    return 0;
}
static int k_run(struct kuhn *K) {   /* -> -1 all matched, else the first unmatched demand */
    K->m = xcalloc((size_t)K->nd + 1, sizeof(int));
    for (int d = 0; d < K->nd; d++) K->m[d] = -1;
    K->seen = xcalloc((size_t)K->nkeys + 1, 1);
    for (int d = 0; d < K->nd; d++) {
        memset(K->seen, 0, (size_t)K->nkeys + 1);
        if (!k_try(K, d)) return d;
    }
    return -1;
}

/* ---- product-term literals */
struct lit { int sexp; struct src s; int inv; int k; };
struct lits { int gnd; int n; struct lit *v; };   /* gnd 1 = constant 0; n = 0 and gnd 0 = constant 1 */
struct need { int lab; struct src s; };

static void need_add(struct need **v, int *n, int lab, struct src s) {
    for (int i = 0; i < *n; i++) if ((*v)[i].lab == lab && (*v)[i].s.kind == s.kind && (*v)[i].s.idx == s.idx) return;
    struct need *w = xcalloc((size_t)*n + 1, sizeof(struct need));   /* the tool's allocator: released at exit */
    if (*n) memcpy(w, *v, sizeof(struct need) * (size_t)*n);
    *v = w;
    (*v)[*n].lab = lab; (*v)[*n].s = s; (*n)++;
}

static struct lits mklits(struct jctx *C, int n, const char *expr, struct need **need, int *nneed) {
    struct lits L = { 0, 0, NULL };
    if (!strcmp(expr, "1'b0")) { L.gnd = 1; return L; }
    if (!strcmp(expr, "1'b1")) return L;
    char *e = py_strip_chars(expr, "()", 1, 1);
    L.v = xcalloc(strlen(e) + 1, sizeof(struct lit));
    for (char *p = e; p && *p;) {
        char *q = strstr(p, " & ");
        char *t = q ? xstrndup(p, (size_t)(q - p)) : xstrdup(p);
        p = q ? q + 3 : NULL;
        t = py_strip(t);
        int inv = t[0] == '~';
        char *name = t + strspn(t, "~");
        if (startswith(name, "uim_")) {
            const char *src = wire(C->N, name);
            if (!src) fatal("no wire %s", name);
            if (!strcmp(src, "1'b0")) {
                if (!inv) { L.gnd = 1; L.n = 0; return L; }
                continue;
            }
            struct src s = maxsrc(C, src);
            L.v[L.n].sexp = 0; L.v[L.n].s = s; L.v[L.n].inv = inv; L.n++;
            need_add(need, nneed, (n - 1) / 16, s);
        } else if (startswith(name, "flb_MC")) {
            int k = atoi(name + 6);
            const char *w = wire(C->N, name);
            if (w && !strcmp(w, "1'b1")) continue;
            if ((k - 1) / 16 != (n - 1) / 16) refuse("FOLDBACK_ACROSS_BLOCK", "MC%d reads the foldback of MC%d", n, k);
            L.v[L.n].sexp = 1; L.v[L.n].k = k; L.n++;
        } else refuse("PT_LITERAL_FORM", "%s", name);
    }
    return L;
}

static void form(slist *f, const char *s) { add(f, s); }

/* re.sub("^~?\\((.*)\\)$", "\\1", w): the source inside "~(...)" or "(...)"; anything else unchanged */
static char *goe_inner(const char *w) {
    const char *x = w[0] == '~' ? w + 1 : w;
    size_t l = strlen(x);
    if (l >= 2 && x[0] == '(' && x[l - 1] == ')') return xstrndup(x + 1, l - 2);
    return xstrdup(w);
}

static int cmp_need(const void *a, const void *b) {
    const struct need *x = a, *y = b;
    if (x->lab != y->lab) return x->lab - y->lab;
    if (x->s.kind != y->s.kind) return x->s.kind - y->s.kind;
    return x->s.idx - y->s.idx;
}
struct gdem { struct src s; int inv; };
static int cmp_gdem(const void *a, const void *b) {
    const struct gdem *x = a, *y = b;
    if (x->s.kind != y->s.kind) return x->s.kind - y->s.kind;
    if (x->s.idx != y->s.idx) return x->s.idx - y->s.idx;
    return x->inv - y->inv;
}

static unsigned short crc16_x25(const unsigned char *d, size_t n) {
    unsigned crc = 0xFFFF;
    for (size_t i = 0; i < n; i++) {
        crc ^= d[i];
        for (int k = 0; k < 8; k++) crc = (crc & 1) ? (crc >> 1) ^ 0x8408 : crc >> 1;
    }
    return (unsigned short)(crc ^ 0xFFFF);
}
static void put16(sbuf *b, unsigned v) { char c[2] = { (char)(v & 255), (char)(v >> 8 & 255) }; sb_addn(b, c, 2); }
static void put32(sbuf *b, unsigned long v) { put16(b, (unsigned)(v & 0xFFFF)); put16(b, (unsigned)(v >> 16 & 0xFFFF)); }
static unsigned char *write_pof(const char *devname, const unsigned char *bits, int nbits, int security, size_t *len) {
    sbuf b; sb_init(&b);
    sb_addn(&b, "POF\0", 4); put16(&b, 0); put16(&b, 1); put32(&b, 6);
    const char *txt[3] = { CREATOR, devname, "Untitled" };
    for (int i = 0; i < 3; i++) { put16(&b, (unsigned)(i + 1)); put32(&b, (unsigned long)strlen(txt[i]) + 1); sb_addn(&b, txt[i], strlen(txt[i]) + 1); }
    put16(&b, 5); put32(&b, 2); put16(&b, (unsigned)security);
    size_t nb = ((size_t)nbits + 7) / 8;
    put16(&b, 17); put32(&b, 12 + (unsigned long)nb); put16(&b, 0); put32(&b, 0); put32(&b, (unsigned long)nbits); put16(&b, 1);
    unsigned char *data = xcalloc(nb + 1, 1);
    for (int k = 0; k < nbits; k++) if (bits[k]) data[k >> 3] |= (unsigned char)(1 << (k & 7));
    sb_addn(&b, (const char *)data, nb);
    put16(&b, 8); put32(&b, 2);
    unsigned short crc = crc16_x25((const unsigned char *)b.s, b.n);
    put16(&b, crc);
    *len = b.n;
    return (unsigned char *)b.s;
}

struct jresult { char *target_name; slist report; slist forms; unsigned char *pof; size_t len; };

static void convert(const unsigned char *data, size_t ndata, const char *part, const char *package, const char *device, struct jresult *R) {
    memset(R, 0, sizeof *R);
    struct jedec J; jed_parse(data, ndata, &J);
    const char *qf = J.qf == 16808 ? "ATF1502AS" : J.qf == 34192 ? "ATF1504AS" : J.qf == 74136 ? "ATF1508AS" : NULL;
    int asv = 0; char *dev;
    if (part && *part) {
        char *p = py_upper(part);
        if (endswith(p, "ASV")) { asv = 1; p[strlen(p) - 1] = 0; }
        if (strcmp(p, "ATF1502AS") && strcmp(p, "ATF1504AS") && strcmp(p, "ATF1508AS")) refuse("DEVICE_NOT_MODELLED", "part %s", part);
        if (qf && strcmp(qf, p)) refuse("QF_NOT_DEVICE", "QF%d is not %s", J.qf, p);
        dev = p;
    } else {
        if (!qf) refuse("DEVICE_NOT_MODELLED", "QF%s", J.qf < 0 ? "None" : fmt("%d", J.qf));
        dev = xstrdup(qf);
    }
    char *pkg = package && *package ? py_upper(package) : NULL;
    if (!pkg) {
        static rx *rqp;
        if (!rqp) rqp = rx_compile("\\*\\s*QP(\\d+)", 0);
        char *t = latin1_to_u8(data, ndata);
        rxm mq; int hq = rx_search(rqp, t, 0, &mq);
        char *qp = hq ? rx_group(&mq, 1) : xstrdup("");
        static const char *T[][3] = { { "ATF1504AS", "84", "PLCC84" }, { "ATF1504AS", "100", "TQFP100" }, { "ATF1508AS", "84", "PLCC84" },
                                      { "ATF1508AS", "160", "PQFP160" } };
        for (int i = 0; i < 4; i++) if (!strcmp(T[i][0], dev) && !strcmp(T[i][1], qp)) pkg = xstrdup(T[i][2]);
        if (!pkg) refuse("PACKAGE_NOT_DETERMINED", "%s with QP%s: give --package", dev, hq ? qp : "?");
    }
    int di = !strcmp(dev, "ATF1502AS") ? 0 : !strcmp(dev, "ATF1504AS") ? 1 : 2;
    const char *S = TGT[di][1], *AE = TGT[di][2];
    const char *tgt = asv ? AE : S;
    if (device && *device) {
        static rx *rd;
        if (!rd) rd = rx_compile("(EPM7\\d+(?:S|AE))", 0);
        char *d = py_upper(device);
        rxm m; int ok = rx_match(rd, d, &m);
        char *g = ok ? rx_group(&m, 1) : NULL;
        if (!g || (strcmp(g, S) && strcmp(g, AE)))
            refuse("DEVICE_PAIR_MISMATCH", "%s is not the S or AE part of the %s density (%s / %s)", device, dev, S, AE);
        tgt = !strcmp(g, S) ? S : AE;
    }
    int sup = 0;
    for (int i = 0; i < n_dev_pairs; i++) if (!strcmp(dev_pairs[i][0], tgt) && !strcmp(dev_pairs[i][1], pkg)) sup = 1;
    if (!sup) refuse("NO_TARGET_PACKAGE", "%s has no %s package among the supported parts", tgt, pkg);
    /* decode */
    struct anet *N = xcalloc(1, sizeof *N);
    atf_decode(data, ndata, dev, pkg, N);
    if (N->nu) {
        slist u = { 0 };
        for (int i = 0; i < N->nu; i++) sl_push(&u, N->und[i].name);
        sl_sort(&u);
        refuse("PAD_ENABLE_UNDETERMINED", "pads %s: output enable not determined by the fuse map", sl_join(&u, ","));
    }
    struct jctx C; memset(&C, 0, sizeof C);
    C.N = N; C.A = N->D; C.tgt = tgt;
    for (int i = 0; i < n_max_devices; i++) {
        char *lo = py_lower(tgt);
        if (!strcmp(max_devices[i].name, lo)) C.D = &max_devices[i];
    }
    if (!C.D) fatal("no MAX tables for %s", tgt);
    const struct maxdev *D = C.D;
    C.nmc_pad = 0;
    for (int i = 0; i < C.A->n_mcs; i++) if (C.A->mcs[i].num > C.nmc_pad) C.nmc_pad = C.A->mcs[i].num;
    C.mcpad = xcalloc((size_t)C.nmc_pad + 1, sizeof(char *));
    for (int i = 0; i < C.A->n_mcs; i++) if (C.A->mcs[i].pad.num != -2) C.mcpad[C.A->mcs[i].num] = atf_padname(&C.A->mcs[i].pad);
    slist seen = { 0 }; observable(N, &seen);
    slist jtag_pads = { 0 };
    if (N->jtag_mode) for (int i = 0; i < 4; i++) sl_push_own(&jtag_pads, atf_padname(&C.A->spec[i]));
    /* blank image */
    C.bits = xcalloc((size_t)D->total + 1, 1);
    memset(C.bits, 1, (size_t)D->total);
    for (int mc = 0; mc < D->n_mc; mc++) {
        set_raw(&C, "MCELL_PTERM_PRE_FUNCTION", mc, 0); set_raw(&C, "MCELL_CLEAR_SELECT", mc, 0); set_raw(&C, "MCELL_OR_OUTPUT_INVERT", mc, 0);
        set_raw(&C, "MCELL_PEXP_ALLOCATION", mc, 0); set_raw(&C, "MCELL_REG_OUT_MAX7K", mc, 0); set_raw(&C, "MCELL_CLOCK_SELECT", mc, 3);
        set_raw(&C, "IO_SELECT_OE_MAX7K", mc, 4); set_raw(&C, "IO_USE_SLOW_SLEW", mc, 0);
    }
    static const int GA[3] = { 1, 0, 0 }, GB[3] = { 1, 1, 0 };
    for (int i = 0; i < D->n_gcc; i++) C.bits[D->gcc[i]] = (unsigned char)GA[i];
    /* full / foldback-only macrocells */
    ilist full = { 0 }, sexp_only = { 0 };
    static rx *r1, *r2, *r3;
    if (!r1) { r1 = rx_compile("mc(\\d+)_(?:q|xt|st)$", 0); r2 = rx_compile("(?:fb|casout)_MC(\\d+)$", 0); r3 = rx_compile("flb_MC(\\d+)$", 0); }
    for (int i = 0; i < seen.n; i++) {
        rxm m;
        if (rx_match(r1, seen.v[i], &m) || rx_match(r2, seen.v[i], &m)) il_push(&full, atoi(rx_group(&m, 1)));
    }
    for (int i = 0; i < N->no; i++) il_push(&full, mc_of_pad(&C, N->out[i].pad));
    il_sort(&full); il_uniq(&full);
    for (int i = 0; i < seen.n; i++) {
        rxm m;
        if (rx_match(r3, seen.v[i], &m)) {
            int k = atoi(rx_group(&m, 1));
            const char *w = wire(N, seen.v[i]);
            if (il_find(&full, k) < 0 && strcmp(w, "1'b1")) il_push(&sexp_only, k);
        }
    }
    il_sort(&sexp_only); il_uniq(&sexp_only);
    #define INFO(n) (&N->info[(n) - 1])
    #define OPT(I, k) atf_info_opt((I), (k))
    /* global clocks */
    slist used = { 0 };
    for (int i = 0; i < full.n; i++) {
        int n = full.v[i]; const struct ainfo *I = INFO(n);
        if (startswith(I->clk, "g_GCLK") && has(&seen, fmt("mc%d_q", n))) add(&used, wire(N, I->clk));
    }
    int gcc_b = 0;
    {
        int a = 1, b = 1;
        for (int i = 0; i < used.n; i++) {
            char *c1 = fmt("pad_%s", dname(0)), *c2 = fmt("pad_%s", dname(3)), *c1n = fmt("~pad_%s", dname(0));
            if (strcmp(used.v[i], c1) && strcmp(used.v[i], c2)) a = 0;
            if (strcmp(used.v[i], c1) && strcmp(used.v[i], c1n)) b = 0;
        }
        if (!a && !b) { sl_sort(&used); refuse("GLOBAL_CLOCK_FORM", "global clocks used %s (MAX: clock pins 1 and 2, or pin 1 and pin 1 inverted)", sl_join(&used, ",")); }
        gcc_b = !a;
    }
    for (int i = 0; i < D->n_gcc; i++) C.bits[D->gcc[i]] = (unsigned char)(gcc_b ? GB[i] : GA[i]);
    form(&R->forms, gcc_b ? "GCC B" : "GCC A");
    /* rows[n][p]: 0 not set, else lits */
    struct lits *rows = xcalloc((size_t)(D->n_mc + 1) * 5, sizeof(struct lits));
    char *rowset = xcalloc((size_t)(D->n_mc + 1) * 5, 1);
    struct need *need = NULL; int nneed = 0;
    int *oe_of = xcalloc((size_t)D->n_mc + 2, sizeof(int));      /* 0 none, 1 always, 2 global */
    char **oe_w = xcalloc((size_t)D->n_mc + 2, sizeof(char *));
    for (int i = 0; i < sexp_only.n; i++) {
        int n = sexp_only.v[i]; const struct ainfo *I = INFO(n);
        if (strcmp(OPT(I, "pt1_mux"), "flb")) refuse("FOLDBACK_FORM", "MC%d foldback read with pt1_mux %s", n, OPT(I, "pt1_mux"));
        rows[n * 5 + 0] = mklits(&C, n, wire(N, I->pt[0]), &need, &nneed); rowset[n * 5] = 1;
        set_raw(&C, "MCELL_PTERM_INV_SEXP_FUNCTION", n - 1, 1);
        form(&R->forms, "SEXP_ONLY");
    }
    for (int fi = 0; fi < full.n; fi++) {
        int n = full.v[fi]; const struct ainfo *I = INFO(n);
        char *mc = fmt("MC%d", n);
        int out_used = 0;
        if (I->pad && !has(&jtag_pads, I->pad)) for (int i = 0; i < N->no; i++) if (!strcmp(N->out[i].pad, I->pad)) out_used = 1;
        int fb_used = has(&seen, fmt("fb_%s", mc)), q_used = has(&seen, fmt("mc%d_q", n));
        const char *cw = wire(N, fmt("casout_%s", mc));
        int donor = !strcmp(OPT(I, "cas_mux"), "sum") && strcmp(cw ? cw : "1'b0", "1'b0") && has(&seen, fmt("casout_%s", mc));
        if (sl_find(&N->latches, mc) >= 0 && q_used) refuse("LATCH_NO_MAX_COUNTERPART", "%s: latch storage reaches a pin", mc);
        const char *om = OPT(I, "o_mux"), *fm = OPT(I, "fb_mux");
        if (out_used && fb_used && strcmp(om, fm)) refuse("OUTPUT_FEEDBACK_SPLIT", "%s drives its pin %s and feeds back %s", mc, om, fm);
        int xb = !strcmp(OPT(I, "xor_b_mux"), "ff_qn"), inv_on = !strcmp(OPT(I, "xor_invert"), "on");
        int pt1_flb = !strcmp(OPT(I, "pt1_mux"), "flb"), pt2_xor = !strcmp(OPT(I, "pt2_mux"), "xor");
        int cas = !strcmp(OPT(I, "cas_mux"), "sum");
        if (cas && !xb && !inv_on && pt1_flb) refuse("XOR_PT1_FORM", "%s: PT1 is the XOR operand", mc);
        const char *mode = out_used ? om : (fb_used ? fm : om);
        struct lits P[5]; int Pset[5];
        const char *ptx[5];
        for (int k = 0; k < 5; k++) { ptx[k] = wire(N, I->pt[k]); P[k] = mklits(&C, n, ptx[k], &need, &nneed); Pset[k] = 1; }
        int st[5] = { pt1_flb, pt2_xor, !strcmp(OPT(I, "pt3_mux"), "ar"), !strcmp(OPT(I, "pt4_mux"), "clk_ce"), !strcmp(OPT(I, "pt5_mux"), "as_oe") };
        const char *en = I->en;
        int en_logic = strcmp(en, "1'b0") && strcmp(en, "1'b1") && !startswith(en, "g_GOE");
        if (!strcmp(OPT(I, "pt5_mux"), "as_oe") && !strcmp(OPT(I, "pt5_func"), "oe")) {
            if (!P[4].gnd && out_used && en_logic) refuse("PT_OE_NO_MAX_COUNTERPART", "%s: output enable = product term", mc);
            P[4].gnd = 1; P[4].n = 0;
        }
        if (out_used && en_logic) refuse("PT_OE_NO_MAX_COUNTERPART", "%s: output enable = %s", mc, en);
        int clocked = 1;
        if (donor) {
            int own_obs = out_used || fb_used || q_used;
            int a_one = !pt2_xor || !strcmp(ptx[1], "1'b1");
            int k1 = !xb && inv_on && a_one && !q_used && (!strcmp(om, "comb") || !out_used) && (!strcmp(fm, "comb") || !fb_used);
            int k2 = xb && inv_on && a_one && strcmp(I->clk, "1'b1") && (!strcmp(om, "sync") || !out_used) && (!strcmp(fm, "sync") || !fb_used)
                     && in_list(DONOR_TFF, 4, tgt);
            if (own_obs && !(k1 || k2)) refuse("CASCADE_DONOR_FORM", "%s: cascade donor whose own value has no MAX donor mode", mc);
            int f2 = own_obs && k2 && !k1;
            set_raw(&C, "MCELL_PEXP_ALLOCATION", n - 1, 1);
            set_setting(&C, "MCELL_OR_OUTPUT_INVERT", n - 1, f2 ? 2 : 0); set_raw(&C, "MCELL_REG_OUT_MAX7K", n - 1, 0);
            clocked = f2;
            form(&R->forms, f2 ? "DONOR K2" : "DONOR K1");
        } else {
            if (cas) {
                int sums[5], ns = 0;
                for (int k = 0; k < 5; k++) if (!st[k]) sums[ns++] = k;
                if (pt2_xor) {
                    st[1] = 0;
                    for (int q = 0; q < ns; q++) { P[sums[q]].gnd = 1; P[sums[q]].n = 0; }
                } else {
                    if (!ns) refuse("CASCADE_FUSE_FORM", "%s: cascade fuse without a free sum slot for the constant 1", mc);
                    for (int q = 0; q < ns; q++) { P[sums[q]].gnd = 1; P[sums[q]].n = 0; }
                    P[sums[0]].gnd = 0; P[sums[0]].n = 0;
                }
                form(&R->forms, pt2_xor ? "CASFUSE True" : "CASFUSE False");
                pt2_xor = 0;
            }
            int orinv;
            if (!xb && inv_on) orinv = 0;
            else if (xb && inv_on) orinv = 2;
            else if (!xb && !inv_on) orinv = 1;
            else {
                if (!in_list(OR3, 3, tgt)) refuse("TFF_INVERT_UNWITNESSED_ON_TARGET", "%s: T flip-flop with inverted input on %s", mc, tgt);
                orinv = 3;
            }
            set_setting(&C, "MCELL_OR_OUTPUT_INVERT", n - 1, orinv);
            set_raw(&C, "MCELL_REG_OUT_MAX7K", n - 1, !strcmp(mode, "sync") ? 0 : 1);
            form(&R->forms, fmt("XOR %d %s %s", orinv, pt2_xor ? "True" : "False", mode));
        }
        if (!clocked) set_raw(&C, "MCELL_CLOCK_SELECT", n - 1, 3);
        else if (startswith(I->clk, "g_GCLK")) {
            if (q_used) {
                const char *w = wire(N, I->clk);
                int net = !strcmp(w, fmt("pad_%s", dname(0))) ? 1 : 2;
                set_setting(&C, "MCELL_CLOCK_SELECT", n - 1, net);
            } else set_raw(&C, "MCELL_CLOCK_SELECT", n - 1, 3);
        } else if (!strcmp(I->clk, "1'b1")) set_raw(&C, "MCELL_CLOCK_SELECT", n - 1, 3);
        else {
            set_setting(&C, "MCELL_CLOCK_SELECT", n - 1, 0);
            if (!st[3]) refuse("PT_CLOCK_FORM", "%s", mc);
        }
        form(&R->forms, fmt("CLK %s %s", startswith(I->clk, "g_") ? "global" : (!strcmp(I->clk, "1'b1") ? "none" : "pt"),
                            strcmp(I->ce, "1'b1") ? "True" : "False"));
        if (strstr(I->ar, "g_GCLR") && q_used) {
            if (!strcmp(wire(N, "g_GCLR"), "~pad_R")) { set_raw(&C, "MCELL_CLEAR_SELECT", n - 1, 1); form(&R->forms, "GCLR low"); }
            else {
                if (st[2] && !P[2].gnd) refuse("GLOBAL_CLEAR_POLARITY", "%s: active-high clear pin and a clear product term", mc);
                if (!st[2] && !P[2].gnd) {
                    static const int fr[4] = { 0, 1, 3, 4 };
                    int f = -1;
                    for (int q = 0; q < 4 && f < 0; q++) if (!st[fr[q]] && P[fr[q]].gnd) f = fr[q];
                    if (f < 0) refuse("GLOBAL_CLEAR_POLARITY", "%s: active-high clear pin, PT3 in the sum, no free sum slot", mc);
                    P[f] = P[2];
                }
                struct src R_ = maxsrc(&C, "pad_R");
                need_add(&need, &nneed, (n - 1) / 16, R_);
                P[2].gnd = 0; P[2].n = 1; P[2].v = xcalloc(1, sizeof(struct lit)); P[2].v[0].sexp = 0; P[2].v[0].s = R_; P[2].v[0].inv = 0;
                st[2] = 1;
                form(&R->forms, "GCLR high_pt3");
            }
        }
        static const char *SN[5] = { "MCELL_PTERM_INV_SEXP_FUNCTION", "MCELL_PTERM_INV_XOR_FUNCTION", "MCELL_PTERM_CLR_FUNCTION",
                                     "MCELL_PTERM_CLK_CE_FUNCTION", "MCELL_PTERM_PRE_FUNCTION" };
        for (int k = 0; k < 5; k++) set_raw(&C, SN[k], n - 1, st[k] ? 1 : 0);
        for (int k = 0; k < 5; k++) { rows[n * 5 + k] = P[k]; rowset[n * 5 + k] = (char)Pset[k]; }
        set_raw(&C, "MCELL_USE_TURBO", n - 1, !strcmp(OPT(I, "low_power"), "on") ? 1 : 0);
        if (out_used) {
            if (!strcmp(en, "1'b1")) oe_of[n] = 1;
            else { oe_of[n] = 2; oe_w[n] = xstrdup(wire(N, en)); }
        }
        static const char *SK[5] = { "sexp", "xor", "clr", "clkce", "pre" };
        slist ss = { 0 };
        for (int k = 0; k < 5; k++) if (st[k]) sl_push(&ss, SK[k]);
        sl_sort(&ss);
        form(&R->forms, ss.n ? fmt("STEER %s", sl_join(&ss, " ")) : "STEER");
    }
    /* IO cells of the outputs */
    for (int n = 1; n <= D->n_mc; n++) {
        if (!oe_of[n]) continue;
        const struct ainfo *I = INFO(n);
        int od = !strcmp(OPT(I, "output_driver"), "open_drain");
        set_raw(&C, "IO_USE_OPEN_DRAIN", n - 1, od ? 0 : 1);
        set_raw(&C, "IO_USE_SLOW_SLEW", n - 1, !strcmp(OPT(I, "slew_rate"), "slow") ? 1 : 0);
        form(&R->forms, od ? "OD True" : "OD False");
    }
    /* global OEs */
    struct gdem *gd = NULL; int ngd = 0;
    for (int n = 1; n <= D->n_mc; n++) {
        if (oe_of[n] != 2) continue;
        const char *w = oe_w[n];
        int inv = w[0] == '~';
        struct src s = maxsrc(&C, goe_inner(w));
        int dup = 0;
        for (int i = 0; i < ngd; i++) if (gd[i].s.kind == s.kind && gd[i].s.idx == s.idx && gd[i].inv == inv) dup = 1;
        if (dup) continue;
        struct gdem *g2 = xcalloc((size_t)ngd + 1, sizeof(struct gdem));
        if (ngd) memcpy(g2, gd, sizeof(struct gdem) * (size_t)ngd);
        gd = g2;
        gd[ngd].s = s; gd[ngd].inv = inv; ngd++;
    }
    if (ngd) qsort(gd, (size_t)ngd, sizeof *gd, cmp_gdem);
    struct kuhn K; memset(&K, 0, sizeof K);
    K.nd = ngd; K.nc = xcalloc((size_t)ngd + 1, sizeof(int)); K.c = xcalloc((size_t)ngd + 1, sizeof(struct res *)); K.nkeys = D->n_poe;
    for (int d = 0; d < ngd; d++) {
        K.c[d] = xcalloc((size_t)D->n_poe + 1, sizeof(struct res));
        for (int k = 0; k < D->n_poe; k++) if (poe_pos(D, k, gd[d].s) >= 0) { struct res r = { k, 0, 1, { k, 0 } }; K.c[d][K.nc[d]++] = r; }
    }
    int bad = k_run(&K);
    if (bad >= 0) refuse("GLOBAL_OE_UNROUTABLE", "%s%s%d: no free global OE", gd[bad].inv ? "~" : "", KNAME[gd[bad].s.kind], gd[bad].s.idx);
    for (int d = 0; d < ngd; d++) {
        int k = K.c[d][K.m[d]].a, pos = poe_pos(D, k, gd[d].s);
        for (int i = 0; i < D->poe_pbw; i++) C.bits[D->oe_base + k * D->oe_w + i] = i == pos ? 0 : 1;
        C.bits[D->oe_base + k * D->oe_w + D->poe_pbw] = (unsigned char)(gd[d].inv ? 1 : 0);
        form(&R->forms, gd[d].inv ? "GOE True" : "GOE False");
    }
    for (int n = 1; n <= D->n_mc; n++) {
        if (oe_of[n] == 1) set_setting(&C, "IO_SELECT_OE_MAX7K", n - 1, 1);
        else if (oe_of[n] == 2) {
            const char *w = oe_w[n]; int inv = w[0] == '~';
            struct src s = maxsrc(&C, goe_inner(w));
            for (int d = 0; d < ngd; d++) if (gd[d].s.kind == s.kind && gd[d].s.idx == s.idx && gd[d].inv == inv) set_setting(&C, "IO_SELECT_OE_MAX7K", n - 1, 2 + K.c[d][K.m[d]].a);
        }
    }
    /* PIA */
    if (nneed) qsort(need, (size_t)nneed, sizeof *need, cmp_need);
    struct kuhn P2; memset(&P2, 0, sizeof P2);
    P2.nd = nneed; P2.nc = xcalloc((size_t)nneed + 1, sizeof(int)); P2.c = xcalloc((size_t)nneed + 1, sizeof(struct res *));
    int nslot = D->n_lab * D->lines;
    P2.nkeys = 2 * (nslot > D->nbuf ? nslot : D->nbuf) + 2;
    for (int d = 0; d < nneed; d++) {
        P2.c[d] = xcalloc((size_t)D->lines * 2 + 1, sizeof(struct res));
        for (int line = 0; line < D->lines; line++) {
            int slot = need[d].lab * D->lines + line;
            int bufs[2], nb = 0;
            if (D->has2x2) {
                for (int i = 0; i < D->n_slotsel; i++) if (D->slotsel[3 * i] == slot) bufs[nb++] = D->slotsel[3 * i + 2];
                if (nb == 2 && bufs[0] > bufs[1]) { int t = bufs[0]; bufs[0] = bufs[1]; bufs[1] = t; }
            } else bufs[nb++] = slot;
            for (int q = 0; q < nb; q++)
                if (buf_pos(D, bufs[q], need[d].s) >= 0) { struct res r = { slot, bufs[q], 2, { 2 * slot, 2 * bufs[q] + 1 } }; P2.c[d][P2.nc[d]++] = r; }
        }
    }
    bad = k_run(&P2);
    if (bad >= 0) refuse("PIA_UNROUTABLE", "LAB %c: no free PIA line for %s%d", "ABCDEFGH"[need[bad].lab], KNAME[need[bad].s.kind], need[bad].s.idx);
    int *line_of = xcalloc((size_t)nneed + 1, sizeof(int));
    for (int d = 0; d < nneed; d++) {
        struct res *r = &P2.c[d][P2.m[d]];
        int slot = r->a, pb = r->b, pos = buf_pos(D, pb, need[d].s);
        for (int i = 0; i < D->pbw; i++) C.bits[D->pia_base + pb * D->pia_w + i] = i == pos ? 0 : 1;
        if (D->has2x2) {
            int v = -1;
            for (int i = 0; i < D->n_slotsel; i++) if (D->slotsel[3 * i] == slot && D->slotsel[3 * i + 2] == pb) v = D->slotsel[3 * i + 1];
            C.bits[D->pia_base + D->pbw + slot * D->pia_w] = (unsigned char)v;
        }
        line_of[d] = slot - need[d].lab * D->lines;
    }
    /* product-term rows */
    for (int n = 1; n <= D->n_mc; n++) for (int p = 0; p < 5; p++) {
        if (!rowset[n * 5 + p]) continue;
        struct lits *L = &rows[n * 5 + p];
        ilist cols = { 0 };
        if (!L->gnd) for (int i = 0; i < L->n; i++) {
            if (L->v[i].sexp) il_push(&cols, 2 * D->lines + (L->v[i].k - 1) % 16);
            else {
                int lab = (n - 1) / 16, li = -1;
                for (int d = 0; d < nneed; d++) if (need[d].lab == lab && need[d].s.kind == L->v[i].s.kind && need[d].s.idx == L->v[i].s.idx) li = line_of[d];
                il_push(&cols, 2 * li + (L->v[i].inv ? 1 : 0));
            }
        }
        write_row(&C, n - 1, p, &cols, L->gnd);
    }
    /* JTAG, power-up, ground clamp, user code, security */
    set_raw(&C, "JTAG_ENABLE_BST_SUPPORT", 0, N->jtag_mode ? 1 : 0);
    form(&R->forms, N->jtag_mode ? "JTAG True" : "JTAG False");
    int rb = 0;
    for (int i = 0; i < N->inputs.n; i++) for (int o = 0; o < N->no; o++) if (!strcmp(N->inputs.v[i], N->out[o].pad)) rb = 1;
    if (rb) form(&R->forms, "READBACK");
    if (st_of(D, "MCELL_REG_POWERS_UP_HIGH")) for (int n = 0; n < D->n_mc; n++) set_raw(&C, "MCELL_REG_POWERS_UP_HIGH", n, 1);
    if (st_of(D, "IO_USE_GND_CLAMP")) for (int n = 0; n < D->n_mc; n++) set_raw(&C, "IO_USE_GND_CLAMP", n, 1);
    const struct setting *uc = st_of(D, "JTAG_WRITE_USER_CODE");
    int nmap = startswith(tgt, "EPM7128") ? 15 : 16;
    unsigned long code = uc->n_bits >= 32 ? 0xFFFFFFFFul : ((1ul << uc->n_bits) - 1);
    for (int k = 0; k < 16; k++) {
        int f = USER0[di] + k;
        if (k < nmap) { if (!N->F[f]) code &= ~(1ul << k); }
        else if (!N->F[f]) sl_push_own(&R->report, fmt("DROPPED USER_FUSE_%d: ATF user fuse %d = 0 has no MAX user-code bit", k, f));
    }
    for (int j = 0; j < uc->n_bits; j++) C.bits[uc->addr[j]] = (unsigned char)(code >> j & 1);
    const char *cfg = NULL;
    for (int i = 0; i < N->notes.n; i++) if (startswith(N->notes.v[i], "config: ")) { cfg = N->notes.v[i] + 8; break; }
    if (!cfg) fatal("no config note");
    char *rp = NULL, *term = NULL, *hyst = NULL;
    {
        char *c = xstrdup(cfg);
        for (char *p = c; p && *p;) {
            char *q = strstr(p, ", "); if (q) *q = 0;
            char *eq = strchr(p, '=');
            if (eq) {
                *eq = 0;
                if (!strcmp(p, "read_protection")) rp = eq + 1;
                if (!strcmp(p, "termination")) term = eq + 1;
                if (!strcmp(p, "reset_hysteresis")) hyst = eq + 1;
            }
            p = q ? q + 2 : NULL;
        }
    }
    int sec = ((rp && !strcmp(rp, "on")) || N->security > 0) ? 1 : 0;
    if (term && !strcmp(term, "bus_keeper")) sl_push(&R->report, "DROPPED PIN_KEEPER: the ATF pin keeper is on; MAX 7000S/AE pins have none (tie unused inputs)");
    sl_push_own(&R->report, fmt("DROPPED POWER_RESET_HYSTERESIS: %s: no MAX setting", hyst ? hyst : "None"));
    /* closed set */
    slist miss = { 0 };
    for (int i = 0; i < R->forms.n; i++) {
        int ok = 0;
        for (int q = 0; q < n_jed2pof_forms; q++) if (!strcmp(jed2pof_forms[q][0], tgt) && !strcmp(jed2pof_forms[q][1], R->forms.v[i])) ok = 1;
        if (!ok) sl_push(&miss, R->forms.v[i]);
    }
    if (miss.n) { sl_sort(&miss); refuse("FORM_NOT_PROVEN", "%s: %s", tgt, sl_join(&miss, "; ")); }
    R->target_name = fmt("%s%s%s", tgt, lookup2(PKGCODE, 6, pkg), lookup2(SPEED, 6, tgt));
    R->pof = write_pof(R->target_name, C.bits, D->total, sec, &R->len);
}

/* the command: convert, read the image back with the MAX decoder, then write the file */
int run_jed2pof(const char *in, const char *out, const char *part, const char *pkg, const char *device) {
    size_t n; unsigned char *data = read_file(in, &n);
    if (!data) fatal("cannot read %s", in);
    struct catcher c; int rc = 0;
    if (TRY(c)) {
        struct jresult R; convert(data, n, part, pkg, device, &R);
        struct catcher c2;
        if (TRY(c2)) { struct decoded X; max_decode(R.pof, R.len, &X); ENDTRY(c2); }
        else { ENDTRY(c2); refuse("POF_SELF_CHECK", "the MAX decoder refuses the new image: %s %s", refusal_name, refusal_detail ? refusal_detail : ""); }
        FILE *f = fopen(out, "wb");
        if (!f || fwrite(R.pof, 1, R.len, f) != R.len) { if (f) fclose(f); remove(out); fatal("cannot write %s", out); }
        fclose(f);
        size_t m; unsigned char *back = read_file(out, &m);
        if (!back || m != R.len || memcmp(back, R.pof, m)) { remove(out); refuse("POF_WRITE_CHECK", "%s does not read back as written", out); }
        put_text(stdout, fmt("target %s\n", R.target_name));
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

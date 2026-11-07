/* diff.c -- the diff command: decode two fuse files of one part and list the decoded fields that differ.
 *
 * Every decoded field is one entry (key, field, value). Keys, in output order: "global"; the macrocells in number order
 * (LCn on MAX, MCn on ATF); the MAX I/O cells by pin (pin n); the ATF interconnect switches (UIMn).
 *   MAX global: gcc (global clear / clock / output-enable settings), jtag, gclk (global clock nets), sexp (shared
 *     expanders in use). LCn (decoded macrocells only): the eleven settings, reg, donor, powerup, fpin and the five
 *     product-term rows pt1..pt5 (GND, VCC, or the literal list). pin n: lc, oe, od, clamp, clamp_lc, and on parts with
 *     I/O attributes bus_hold, weak_pull_up, iost, delay.
 *   ATF global: security, jtag_mode, the device configuration fields, the global clock / clear / enable signals.
 *     MCn: every fuse field of the macrocell by its table name and the five product terms. UIMn: the selected source.
 * A key present in one file only is printed once as "<key> decoded: yes -> no" (or "no -> yes").
 */
#include "atf.h"
#include "max.h"
#include "pof.h"
#include "sha256.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

int input_class(const unsigned char *d, size_t n, const char **why);
void atf_part_pkg(const unsigned char *data, size_t n, const char *part, const char *pkg, char **dev_out, char **pkg_out);

struct ent { int cls, num; char *key, *field, *val; int seq; };
struct side { char *part; int n, cap; struct ent *e; char hex[65]; const char *name; };

static void add(struct side *s, int cls, int num, char *key, const char *field, char *val) {
    if (s->n == s->cap) {
        int c = s->cap ? s->cap * 2 : 256;
        struct ent *e = xcalloc((size_t)c, sizeof *e);
        if (s->n) memcpy(e, s->e, sizeof *e * (size_t)s->n);
        s->e = e; s->cap = c;
    }
    struct ent *x = &s->e[s->n]; x->cls = cls; x->num = num; x->key = key; x->field = xstrdup(field); x->val = val; x->seq = s->n; s->n++;
}

static const char *SETNAME[S_N] = { "inv_sexp", "inv_xor", "clr", "clk_ce", "pre", "clock_select", "clear_select", "or_inv", "pexp",
                                    "reg_out", "turbo" };

static char *row_text(const struct row *r) {
    if (r->st == 0) return xstrdup("not decoded");
    if (r->st == 1) return xstrdup("GND");
    if (!r->l.n) return xstrdup("VCC");
    return sl_join(&r->l, " & ");
}

static void max_fields(const struct decoded *X, struct side *s) {
    const struct maxdev *D = X->d->D;
    s->part = fmt("%s %s", py_upper(X->d->dev), X->d->pkg);
    char *g = xstrdup("global");
    add(s, 0, 0, g, "gcc", fmt("%d %d %d", X->gcc[0], X->gcc[1], X->gcc[2]));
    add(s, 0, 0, g, "jtag", fmt("%d", X->jtag));
    sbuf b; sb_init(&b);
    for (int i = 0; i < X->n_gclk; i++) sb_printf(&b, i ? " %d:%d,%d" : "%d:%d,%d", X->gclk_net[i], X->gclk_ib[i], X->gclk_inv[i]);
    add(s, 0, 0, g, "gclk", b.n ? sb_take(&b) : xstrdup("none"));
    for (int n = 1; n <= D->n_mc; n++) if (X->live_sexp[n]) sb_printf(&b, b.n ? " %d" : "%d", n);
    add(s, 0, 0, g, "sexp", b.n ? sb_take(&b) : xstrdup("none"));
    for (int n = 1; n <= D->n_mc; n++) {
        if (!X->live[n]) continue;
        const struct cell *c = &X->cells[n];
        char *k = fmt("LC%d", n);
        for (int q = 0; q < S_N; q++) add(s, 1, n, k, SETNAME[q], sv_str(c->st[q]));
        add(s, 1, n, k, "reg", fmt("%d", c->reg));
        add(s, 1, n, k, "donor", fmt("%d", c->donor));
        add(s, 1, n, k, "powerup", xstrdup(c->pu == 1 ? "high" : c->pu == 0 ? "low" : "unknown"));
        add(s, 1, n, k, "fpin", fmt("%d", c->fpin));
        for (int p = 0; p < 5; p++) add(s, 1, n, k, fmt("pt%d", p + 1), row_text(&c->rows[p]));
    }
    for (int q = 0; q < X->n_ios; q++) {
        const struct io *io = &X->ios[q];
        char *k = fmt("pin %s", pinname(io->pin));
        add(s, 2, io->pin, k, "lc", io->lc ? fmt("LC%d", io->lc) : xstrdup("none"));
        add(s, 2, io->pin, k, "oe", xstrdup(io->oe ? io->oe : "none"));
        add(s, 2, io->pin, k, "od", fmt("%d", io->od));
        add(s, 2, io->pin, k, "clamp", xstrdup(io->has_clamp ? (io->clamp ? "1" : "0") : "-"));
        add(s, 2, io->pin, k, "clamp_lc", fmt("%d", io->clamp_lc));
        const struct attr *a = max_attr_at(X, io->pin);
        if (a) {
            add(s, 2, io->pin, k, "bus_hold", fmt("%d", a->bus_hold));
            add(s, 2, io->pin, k, "weak_pull_up", fmt("%d", a->weak_pull_up));
            add(s, 2, io->pin, k, "iost", fmt("%d", a->iost));
            add(s, 2, io->pin, k, "delay", fmt("%d", a->delay));
        }
    }
    for (int i = 0; i < X->n_attrs; i++) {          /* attributes of pins that have no I/O cell entry */
        const struct attr *a = &X->attrs[i];
        if (max_io_at(X, a->pin)) continue;
        char *k = fmt("pin %s", pinname(a->pin));
        add(s, 2, a->pin, k, "bus_hold", fmt("%d", a->bus_hold));
        add(s, 2, a->pin, k, "weak_pull_up", fmt("%d", a->weak_pull_up));
        add(s, 2, a->pin, k, "iost", fmt("%d", a->iost));
        add(s, 2, a->pin, k, "delay", fmt("%d", a->delay));
    }
}

static int num_after(const char *s, const char *p) { return startswith(s, p) ? atoi(s + strlen(p)) : -1; }

static void atf_fields(const struct anet *N, struct side *s) {
    s->part = fmt("%s %s", N->dev, N->pkg);
    char *g = xstrdup("global");
    add(s, 0, 0, g, "security", xstrdup(N->security < 0 ? "none" : N->security ? "1" : "0"));
    add(s, 0, 0, g, "jtag_mode", fmt("%d", N->jtag_mode));
    for (int i = 0; i < N->notes.n; i++) {
        const char *t = N->notes.v[i];
        if (startswith(t, "config: ")) {
            char *c = xstrdup(t + 8);
            for (char *p = c, *e; p && *p; p = e) {
                e = strstr(p, ", "); if (e) { *e = 0; e += 2; }
                char *eqs = strchr(p, '=');
                if (eqs) { *eqs = 0; add(s, 0, 0, g, p, xstrdup(eqs + 1)); }
                else add(s, 0, 0, g, "config", xstrdup(p));
            }
        } else add(s, 0, 0, g, fmt("note%d", i + 1), xstrdup(t));
    }
    for (int i = 0; i < N->nw; i++) if (startswith(N->w[i].name, "g_")) add(s, 0, 0, g, N->w[i].name + 2, xstrdup(N->w[i].val));
    for (int i = 0; i < N->ni; i++) {
        const struct ainfo *I = &N->info[i];
        int n = num_after(I->mc, "MC");
        for (int q = 0; q < I->n_o; q++) add(s, 1, n, I->mc, I->o[q].name, xstrdup(I->o[q].val));
        for (int p = 0; p < 5; p++) {
            const char *w = atf_wire(N, fmt("mc%d_pt%d", n, p + 1));
            if (w) add(s, 1, n, I->mc, fmt("pt%d", p + 1), xstrdup(w));
        }
    }
    for (int i = 0; i < N->nw; i++) {
        int n = num_after(N->w[i].name, "uim_UIM");
        if (n >= 0) add(s, 3, n, xstrdup(N->w[i].name + 4), "source", xstrdup(N->w[i].val));
    }
}

/* decode one file into its field list; 0 done, 3 refused (the REFUSED line is printed) */
static int load(const char *path, const char *part, const char *pkg, struct side *s, int *kind) {
    size_t n; unsigned char *data = read_file(path, &n);
    if (!data) fatal("cannot read %s", path);
    sha256_hex(data, n, s->hex); s->name = path_base(path);
    struct catcher c;
    if (TRY(c)) {
        const char *why = NULL; int k = input_class(data, n, &why); *kind = k;
        if (k == 1) {
            struct pof P; pof_parse(data, n, &P);
            if (!P.device) fatal("the POF names no device");
            if (part && *part) {
                char *a = py_upper(P.device), *b = py_upper(part);
                if (!startswith(a, b)) refuse("PART_OVERRIDE_MISMATCH", "--part %s, POF device %s", part, P.device);
            }
            struct decoded X; max_decode(data, n, &X);
            max_fields(&X, s);
        } else if (k == 2) {
            char *dev, *pk; atf_part_pkg(data, n, part, pkg, &dev, &pk);
            struct anet N; atf_decode(data, n, dev, pk, &N);
            atf_fields(&N, s);
        } else refuse("INPUT_FORMAT_NOT_MODELLED", "%s", why);
        ENDTRY(c);
        return 0;
    }
    ENDTRY(c);
    put_text(stdout, fmt("REFUSED %s: %s: %s\n", refusal_name, s->name, refusal_detail));
    return 3;
}

static int ent_cmp(const struct ent *a, const struct ent *b) {
    if (a->cls != b->cls) return a->cls - b->cls;
    if (a->num != b->num) return a->num < b->num ? -1 : 1;
    return 0;
}
static int ent_sort(const void *x, const void *y) {
    const struct ent *a = x, *b = y;
    int c = ent_cmp(a, b);
    return c ? c : a->seq - b->seq;
}
static const struct ent *find(const struct side *s, const struct ent *k) {
    for (int i = 0; i < s->n; i++) if (!ent_cmp(&s->e[i], k) && !strcmp(s->e[i].field, k->field)) return &s->e[i];
    return NULL;
}
static int has_key(const struct side *s, int cls, int num) {
    for (int i = 0; i < s->n; i++) if (s->e[i].cls == cls && s->e[i].num == num) return 1;
    return 0;
}

int run_diff(const char *pa, const char *pb, const char *part, const char *pkg) {
    struct side A = { 0 }, B = { 0 };
    int ka = 0, kb = 0;
    if (load(pa, part, pkg, &A, &ka)) return 3;
    if (load(pb, part, pkg, &B, &kb)) return 3;
    if (ka != kb || strcmp(A.part, B.part)) {
        put_text(stdout, fmt("REFUSED PARTS_DIFFER: a %s, b %s\n", A.part, B.part));
        return 3;
    }
    qsort(A.e, (size_t)A.n, sizeof *A.e, ent_sort);
    qsort(B.e, (size_t)B.n, sizeof *B.e, ent_sort);
    for (int i = 0; i < A.n; i++) A.e[i].seq = i;
    for (int i = 0; i < B.n; i++) B.e[i].seq = i;
    sbuf o; sb_init(&o);
    sb_printf(&o, "a: %s sha256 %s\nb: %s sha256 %s\npart: %s\n", A.name, A.hex, B.name, B.hex, A.part);
    int nd = 0, i = 0, j = 0;
    /* walk both sorted lists key by key */
    while (i < A.n || j < B.n) {
        const struct ent *x = i < A.n ? &A.e[i] : NULL, *y = j < B.n ? &B.e[j] : NULL;
        int c = !x ? 1 : !y ? -1 : ent_cmp(x, y);
        if (c < 0 && !has_key(&B, x->cls, x->num)) {
            sb_printf(&o, "%s decoded: yes -> no\n", x->key); nd++;
            int cls = x->cls, num = x->num; while (i < A.n && A.e[i].cls == cls && A.e[i].num == num) i++;
            continue;
        }
        if (c > 0 && !has_key(&A, y->cls, y->num)) {
            sb_printf(&o, "%s decoded: no -> yes\n", y->key); nd++;
            int cls = y->cls, num = y->num; while (j < B.n && B.e[j].cls == cls && B.e[j].num == num) j++;
            continue;
        }
        /* the same key on both sides: its fields in A's order, then fields only B has */
        const struct ent *k0 = x && (c <= 0) ? x : y;
        int cls = k0->cls, num = k0->num;
        int i0 = i, j0 = j;
        while (i < A.n && A.e[i].cls == cls && A.e[i].num == num) {
            const struct ent *f = &A.e[i], *g = find(&B, f);
            if (!g) { sb_printf(&o, "%s %s: %s -> (none)\n", f->key, f->field, f->val); nd++; }
            else if (strcmp(f->val, g->val)) { sb_printf(&o, "%s %s: %s -> %s\n", f->key, f->field, f->val, g->val); nd++; }
            i++;
        }
        while (j < B.n && B.e[j].cls == cls && B.e[j].num == num) {
            const struct ent *g = &B.e[j];
            int seen = 0;
            for (int q = i0; q < i; q++) if (!strcmp(A.e[q].field, g->field)) seen = 1;
            if (!seen) { sb_printf(&o, "%s %s: (none) -> %s\n", g->key, g->field, g->val); nd++; }
            j++;
        }
        (void)j0;
    }
    sb_printf(&o, "differences: %d\n", nd);
    put_text(stdout, sb_take(&o));
    return 0;
}

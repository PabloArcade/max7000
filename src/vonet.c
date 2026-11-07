/* vonet.c -- reader of the atom-netlist dialect (max_io / max_mcell / max_sexp instances, "// Location:" comments,
 * defparam lines). Port values keep one trailing space when not empty, so that an escaped identifier ("\lc3_out ")
 * stays terminated when the value is written back.
 */
#include "max.h"
#include "rx.h"
#include <stdlib.h>
#include <string.h>

static void findall1(rx *r, const char *t, slist *out) {
    size_t pos = 0, len = strlen(t); rxm m;
    while (pos <= len && rx_search_n(r, t, len, pos, &m)) {
        sl_push_own(out, rx_group(&m, 1));
        pos = m.g[0][1] > m.g[0][0] ? m.g[0][1] : m.g[0][1] + 1;
    }
}

typedef struct { char *inst; int n, cap; struct par *v; } dpset;

void vo_parse(const char *text0, struct netlist *N) {
    static rx *rmod, *rin, *rout, *rio, *rwire, *rdef, *rinst, *rport, *rpin, *ratom;
    if (!rmod) {
        rmod = rx_compile("^module\\s+(\\w+)\\s*\\(", RX_M);
        rin = rx_compile("^\\s*input\\s+(\\w+)\\s*;", RX_M); rout = rx_compile("^\\s*output\\s+(\\w+)\\s*;", RX_M); rio = rx_compile("^\\s*inout\\s+(\\w+)\\s*;", RX_M);
        rwire = rx_compile("^\\s*wire\\s+(\\[\\d+:\\d+\\]\\s*)?(\\\\\\S+\\s|\\w+)\\s*;", RX_M);
        rdef = rx_compile("^defparam\\s+(\\\\\\S+\\s|\\w+)\\s*\\.(\\w+)\\s*=\\s*\\\"([^\\\"]*)\\\"\\s*;", RX_M);
        rinst = rx_compile("(?:^//\\s*Location:\\s*(\\S+)\\s*\\n)?^(max_\\w+)\\s+(\\\\\\S+\\s|\\w+)\\s*\\((.*?)\\);", RX_S | RX_M);
        rport = rx_compile("\\.(\\w+)\\s*\\(((?:[^()]|\\([^()]*\\))*)\\)", RX_S);
        rpin = rx_compile("PIN_[A-Z]\\d+$", 0);
        ratom = rx_compile("^max_\\w+\\s", RX_M);
    }
    char *text = replace_all(text0, "\r", "");
    memset(N, 0, sizeof *N);
    rxm m;
    if (!rx_search(rmod, text, 0, &m)) refuse("VO_SYNTAX", "no module");
    N->top = rx_group(&m, 1);
    findall1(rin, text, &N->inputs); findall1(rout, text, &N->outputs); findall1(rio, text, &N->inouts);
    size_t len = strlen(text), pos = 0;
    plist wr = { 0 }, ww = { 0 };
    while (pos <= len && rx_search_n(rwire, text, len, pos, &m)) {
        pl_push(&wr, py_strip(rx_group(&m, 1))); pl_push(&ww, py_strip(rx_group(&m, 2)));
        pos = m.g[0][1];
    }
    N->nw = wr.n; N->wr = (char **)wr.v; N->ww = (char **)ww.v;
    /* defparams by instance name, in first-seen order; a repeated parameter keeps its place and takes the new value */
    plist dps = { 0 };
    pos = 0;
    while (pos <= len && rx_search_n(rdef, text, len, pos, &m)) {
        char *in = py_strip(rx_group(&m, 1)), *k = rx_group(&m, 2), *v = rx_group(&m, 3);
        dpset *d = NULL;
        for (int i = 0; i < dps.n; i++) if (!strcmp(((dpset *)dps.v[i])->inst, in)) d = dps.v[i];
        if (!d) { d = xcalloc(1, sizeof *d); d->inst = in; pl_push(&dps, d); }
        int j; for (j = 0; j < d->n; j++) if (!strcmp(d->v[j].name, k)) break;
        if (j < d->n) d->v[j].val = v;
        else {
            if (d->n == d->cap) { int c = d->cap ? d->cap * 2 : 8; struct par *nv = xmalloc(sizeof *nv * (size_t)c); if (d->n) memcpy(nv, d->v, sizeof *nv * (size_t)d->n); d->v = nv; d->cap = c; }
            d->v[d->n].name = k; d->v[d->n].val = v; d->n++;
        }
        pos = m.g[0][1];
    }
    plist insts = { 0 };
    pos = 0;
    while (pos <= len && rx_search_n(rinst, text, len, pos, &m)) {
        struct inst *x = xcalloc(1, sizeof *x);
        x->loc = rx_gset(&m, 1) ? rx_group(&m, 1) : NULL;
        x->kind = rx_group(&m, 2); x->name = py_strip(rx_group(&m, 3));
        char *body = rx_group(&m, 4);
        rxm k;
        if (x->loc && rx_match(rpin, x->loc, &k)) x->loc = fmt("PIN_%d", (x->loc[4] - 64) * 1000 + atoi(x->loc + 5));
        if (strcmp(x->kind, "max_mcell") && strcmp(x->kind, "max_io") && strcmp(x->kind, "max_sexp")) refuse("VO_ATOM", "%s", x->kind);
        size_t bl = strlen(body), bp = 0; plist pn = { 0 }, pv = { 0 };
        while (bp <= bl && rx_search_n(rport, body, bl, bp, &k)) {
            char *nm = rx_group(&k, 1), *v = py_strip(rx_group(&k, 2));
            char *val = *v ? fmt("%s ", v) : xstrdup("");
            int j; for (j = 0; j < pn.n; j++) if (!strcmp(pn.v[j], nm)) break;
            if (j < pn.n) pv.v[j] = val; else { pl_push(&pn, nm); pl_push(&pv, val); }
            bp = k.g[0][1] > k.g[0][0] ? k.g[0][1] : k.g[0][1] + 1;
        }
        x->np = pn.n; x->p = xcalloc((size_t)pn.n + 1, sizeof(struct port));
        for (int j = 0; j < pn.n; j++) { x->p[j].name = pn.v[j]; x->p[j].val = pv.v[j]; }
        for (int i = 0; i < dps.n; i++) if (!strcmp(((dpset *)dps.v[i])->inst, x->name)) { x->npar = ((dpset *)dps.v[i])->n; x->par = ((dpset *)dps.v[i])->v; }
        pl_push(&insts, x);
        pos = m.g[0][1];
    }
    N->ni = insts.n; N->in = xcalloc((size_t)insts.n + 1, sizeof(struct inst));
    for (int i = 0; i < insts.n; i++) N->in[i] = *(struct inst *)insts.v[i];
    slist atoms = { 0 }; findall1(ratom, text, &atoms);
    if (atoms.n != N->ni) refuse("VO_SYNTAX", "%d atom lines, %d instances parsed", atoms.n, N->ni);
    for (int i = 0; i < N->ni; i++) {
        struct inst *x = &N->in[i];
        if (!strcmp(x->kind, "max_io")) {
            const char *bh = vo_par(x, "bus_hold"), *pu = vo_par(x, "weak_pull_up");
            if ((bh && strcmp(bh, "false") && strcmp(bh, "true")) || (pu && strcmp(pu, "false") && strcmp(pu, "true")))
                fatal("netlist reader: %s has a pad attribute other than true / false", x->name);
        }
        if (!strcmp(x->kind, "max_mcell")) {
            const char *p = vo_par(x, "power_up");
            if (p && strcmp(p, "low") && strcmp(p, "high")) fatal("netlist reader: %s power_up %s", x->name, p);
        }
    }
}

const char *vo_port(const struct inst *x, const char *name) { for (int i = 0; i < x->np; i++) if (!strcmp(x->p[i].name, name)) return x->p[i].val; return NULL; }
const char *vo_par(const struct inst *x, const char *name) { for (int i = 0; i < x->npar; i++) if (!strcmp(x->par[i].name, name)) return x->par[i].val; return NULL; }
static const char *par_or(const struct inst *x, const char *n, const char *d) { const char *v = vo_par(x, n); return v ? v : d; }
int vo_is_reg(const struct inst *x) {
    return !strcmp(x->kind, "max_mcell") && (!strcmp(par_or(x, "output_mode", "comb"), "reg") || !strcmp(par_or(x, "register_mode", "dff"), "tff"));
}
/* the items of a bus text "{a,b,...}" (or a single net), each stripped */
static void bus_items(const char *e, slist *out) {
    char *s = py_strip_chars(py_strip(e ? e : ""), "{}", 1, 1);
    char *p = s;
    for (;;) { char *c = strchr(p, ','); sl_push_own(out, py_strip(c ? xstrndup(p, (size_t)(c - p)) : p)); if (!c) break; p = c + 1; }
}
int vo_const_clear(const struct inst *x) {
    const char *a = vo_port(x, "aclr");
    if (!strcmp(py_strip(a ? a : ""), "vcc")) return 1;
    slist it = { 0 }; bus_items(vo_port(x, "paclr") ? vo_port(x, "paclr") : "", &it);
    for (int i = 0; i < it.n; i++) if (strcmp(it.v[i], "vcc")) return 0;
    return it.n > 0;
}
char *vo_regkey(const struct inst *x) {
    if (!x->loc || !startswith(x->loc, "LC")) refuse("VO_NO_LOCATION", "%s", x->name);
    return fmt("lc%d", atoi(x->loc + 2));
}

/* ------------------------------------------------------------------ RACING lint
 * Per register: CLOCK = support of clk + pclk; DATA = support of its next-state data; ENA = pena; ASYNC = aclr +
 * paclr + papre. A cone stops at input pins, bidirectional pads and register outputs. A register races when one
 * leaf of its clock cone also reaches its data, enable or asynchronous cone (reported as D, E, A). A register whose
 * clear is constantly true cannot race. */
typedef struct { const struct netlist *N; slist dn; int *di; char **dp; slist pk, pv; } lint;

static void nets(const char *e, slist *out) {
    static rx *rid;
    if (!rid) rid = rx_compile("\\\\\\S+|[A-Za-z_][\\w$]*", 0);
    if (!e) return;
    size_t len = strlen(e), pos = 0; rxm m;
    while (pos <= len && rx_search_n(rid, e, len, pos, &m)) {
        char *w = py_strip(py_strip_chars(rx_group(&m, 0), "\\", 1, 0));
        if (strcmp(w, "gnd") && strcmp(w, "vcc") && strcmp(w, "unknown") && sl_find(out, w) < 0) sl_push_own(out, w);
        pos = m.g[0][1];
    }
}
static void busnets(const char *e, slist *out) {
    char *s = py_strip(e ? e : "");
    if (!*s) return;
    slist it = { 0 };
    if (*s == '{') bus_items(s, &it); else sl_push(&it, s);
    for (int i = 0; i < it.n; i++) if (!strcmp(it.v[i], "gnd")) return;
    for (int i = 0; i < it.n; i++) nets(it.v[i], out);
}
static void add_all(slist *a, const slist *b) { for (int i = 0; i < b->n; i++) if (sl_find(a, b->v[i]) < 0) sl_push(a, b->v[i]); }

static void data_support(const struct inst *x, slist *nt, slist *leaves) {
    slist s = { 0 };
    for (int k = 0; k < 5; k++) busnets(vo_port(x, fmt("pterm%d", k)), &s);
    nets(vo_port(x, "pexpin"), &s);
    slist base = { 0 };
    if (!strcmp(par_or(x, "pexp_mode", ""), "on")) busnets(vo_port(x, "pterm5"), &base); else add_all(&base, &s);
    const char *op = par_or(x, "operation_mode", "normal"); int tff = !strcmp(par_or(x, "register_mode", "dff"), "tff");
    if (!strcmp(op, "vcc")) nets(vo_port(x, "fpin"), nt);
    else if (!strcmp(op, "xor")) { add_all(nt, &base); busnets(vo_port(x, "pxor"), nt); }
    else add_all(nt, &base);
    if (tff && vo_is_reg(x)) { char *q = fmt("q:%s", vo_regkey(x)); if (sl_find(leaves, q) < 0) sl_push_own(leaves, q); }
}
static void sum_support(const struct inst *x, slist *out) {
    if (strcmp(par_or(x, "pexp_mode", ""), "on")) return;
    nets(vo_port(x, "pexpin"), out);
    for (int k = 0; k < 5; k++) busnets(vo_port(x, fmt("pterm%d", k)), out);
}
static int drv_find(const lint *L, const char *n) { int r = -1; for (int i = 0; i < L->dn.n; i++) if (!strcmp(L->dn.v[i], n)) r = i; return r; }
static const char *pinof(const lint *L, const char *n) { const char *r = n; for (int i = 0; i < L->pk.n; i++) if (!strcmp(L->pk.v[i], n)) r = L->pv.v[i]; return r; }

static void cone(const lint *L, const slist *n0, const slist *l0, slist *leaves) {
    add_all(leaves, l0);
    slist work = { 0 }, seen = { 0 };
    add_all(&work, n0);
    while (work.n) {
        char *n = work.v[--work.n];
        if (sl_find(&seen, n) >= 0) continue;
        sl_push(&seen, n);
        int d = drv_find(L, n);
        if (d < 0) { const char *p = pinof(L, n); if (sl_find(leaves, p) < 0) sl_push(leaves, p); continue; }
        const struct inst *x = &L->N->in[L->di[d]]; const char *pn = L->dp[d];
        if (!strcmp(x->kind, "max_io")) {
            char *pad = py_strip(vo_port(x, "padio") ? vo_port(x, "padio") : "");
            const char *p = pinof(L, pad); if (sl_find(leaves, p) < 0) sl_push(leaves, p);
            if (!strcmp(par_or(x, "operation_mode", ""), "bidir")) { slist t = { 0 }; nets(vo_port(x, "datain"), &t); nets(vo_port(x, "oe"), &t); for (int i = 0; i < t.n; i++) sl_push(&work, t.v[i]); }
            continue;
        }
        slist t = { 0 }, lv = { 0 };
        if (!strcmp(x->kind, "max_sexp")) { busnets(vo_port(x, "datain"), &t); for (int i = 0; i < t.n; i++) sl_push(&work, t.v[i]); continue; }
        if (!strcmp(pn, "pexpout")) { sum_support(x, &t); for (int i = 0; i < t.n; i++) sl_push(&work, t.v[i]); continue; }
        if (!strcmp(par_or(x, "output_mode", "comb"), "reg")) { char *q = fmt("q:%s", vo_regkey(x)); if (sl_find(leaves, q) < 0) sl_push_own(leaves, q); continue; }
        data_support(x, &t, &lv);
        for (int i = 0; i < t.n; i++) sl_push(&work, t.v[i]);
        add_all(leaves, &lv);
    }
}

void race_analyse(const struct netlist *N, struct race *R) {
    lint L; memset(&L, 0, sizeof L); L.N = N;
    plist di = { 0 }, dp = { 0 };
    for (int i = 0; i < N->ni; i++) {
        const char *pns[2] = { "dataout", "pexpout" };
        for (int k = 0; k < 2; k++) {
            slist t = { 0 }; nets(vo_port(&N->in[i], pns[k]), &t);
            for (int j = 0; j < t.n; j++) { sl_push(&L.dn, t.v[j]); pl_push(&di, (void *)(size_t)i); pl_push(&dp, (void *)pns[k]); }
        }
    }
    L.di = xmalloc(sizeof(int) * (size_t)(di.n + 1)); L.dp = xmalloc(sizeof(char *) * (size_t)(dp.n + 1));
    for (int i = 0; i < di.n; i++) { L.di[i] = (int)(size_t)di.v[i]; L.dp[i] = dp.v[i]; }
    for (int i = 0; i < N->ni; i++) {
        const struct inst *x = &N->in[i];
        if (strcmp(x->kind, "max_io")) continue;
        char *pad = py_strip(vo_port(x, "padio") ? vo_port(x, "padio") : "");
        sl_push(&L.pk, pad);
        sl_push_own(&L.pv, (x->loc && startswith(x->loc, "PIN_")) ? fmt("pin%s", x->loc + 4) : xstrdup(pad));
    }
    memset(R, 0, sizeof *R);
    plist keys = { 0 }, lines = { 0 };
    for (int i = 0; i < N->ni; i++) {
        const struct inst *x = &N->in[i];
        if (!vo_is_reg(x)) continue;
        char *k = vo_regkey(x);
        slist c0 = { 0 }, none = { 0 }, ck = { 0 }, dn = { 0 }, dl = { 0 }, D = { 0 }, e0 = { 0 }, E = { 0 }, a0 = { 0 }, A = { 0 };
        nets(vo_port(x, "clk"), &c0); busnets(vo_port(x, "pclk"), &c0); cone(&L, &c0, &none, &ck);
        data_support(x, &dn, &dl); cone(&L, &dn, &dl, &D);
        busnets(vo_port(x, "pena"), &e0); cone(&L, &e0, &none, &E);
        nets(vo_port(x, "aclr"), &a0); busnets(vo_port(x, "paclr"), &a0); busnets(vo_port(x, "papre"), &a0); cone(&L, &a0, &none, &A);
        if (vo_const_clear(x)) continue;
        slist lf = { 0 };
        for (int j = 0; j < ck.n; j++) {
            const char *l = ck.v[j];
            if (sl_find(&D, l) >= 0 || sl_find(&E, l) >= 0 || sl_find(&A, l) >= 0) sl_push(&lf, l);
        }
        if (!lf.n) continue;
        sl_sort(&lf);                       /* by leaf name */
        slist sh = { 0 };
        for (int j = 0; j < lf.n; j++) {
            const char *l = lf.v[j];
            sl_push_own(&sh, fmt("%s:%s%s%s", l, sl_find(&D, l) >= 0 ? "D" : "", sl_find(&E, l) >= 0 ? "E" : "", sl_find(&A, l) >= 0 ? "A" : ""));
        }
        pl_push(&keys, k); pl_push(&lines, fmt("%s[%s]", k, sl_join(&sh, " ")));
    }
    R->n = keys.n; R->key = (char **)keys.v; R->line = (char **)lines.v;
}

/* sorted by register key: as text ("lc10" before "lc2"), or by the number after "lc"; insertion sort keeps it simple */
char *race_report(const struct race *R, int by_number) {
    int *o = xmalloc(sizeof(int) * (size_t)(R->n + 1));
    for (int i = 0; i < R->n; i++) o[i] = i;
    for (int i = 1; i < R->n; i++)
        for (int j = i; j > 0; j--) {
            const char *a = R->key[o[j - 1]], *b = R->key[o[j]];
            int gt = by_number ? atoi(a + 2) > atoi(b + 2) : strcmp(a, b) > 0;
            if (!gt) break;
            int t = o[j]; o[j] = o[j - 1]; o[j - 1] = t;
        }
    slist s = { 0 };
    for (int i = 0; i < R->n; i++) sl_push(&s, R->line[o[i]]);
    return sl_join(&s, by_number ? " " : ", ");
}

/* atfemit.c -- the ATF build input: a CUPL source for WinCUPL from the decoded netlist.
 *
 * Every read input pad and every driven pad that is a declared pin gets a PIN statement; every macrocell whose value
 * is used gets its node: feedback node 600 + MC, foldback node 300 + MC (the fitter's own numbering). Per macrocell:
 *   foldback (pt1 mux = flb):  !f<mc> = <term 1>;
 *   registered (its Q is used): name.d = <XT>; .ck / .ar / .ap / .ce as decoded; a T flip-flop (XOR operand = its own
 *                              complemented output, no cascade) as name.t; a latch as name.l / name.le
 *   combinational:             name = <XT>;      and name.oe for a pin whose enable is not constant 1.
 * Options as properties: Preassign = Keep; pin_keep OFF when termination = high_z; security on when the JEDEC G field
 * is 1; output_fast when every emitted output is fast; MC_power for low-power outputs; open_collector for open drain.
 * Driven pads that are not declared pins are left out (unconnected on the board) and listed in a comment. A read of a
 * bidirectional pin is written name.io. Refused (EMIT_*): a macrocell whose register output and combinational value
 * are both used, a registered macrocell with a constant clock, a read-back pad that is driven but not declared.
 */
#include "atf.h"
#include "rx.h"
#include <stdlib.h>
#include <string.h>

static const char *MNEM[][4] = {
    { "ATF1502AS", "PLCC44", "f1502plcc44", "f1502ispplcc44" }, { "ATF1502AS", "TQFP44", "f1502tqfp44", "f1502isptqfp44" },
    { "ATF1504AS", "PLCC44", "f1504plcc44", "f1504ispplcc44" }, { "ATF1504AS", "TQFP44", "f1504tqfp44", "f1504isptqfp44" },
    { "ATF1504AS", "PLCC84", "f1504plcc84", "f1504ispplcc84" }, { "ATF1504AS", "PLCC68", "f1504plcc68", "f1504ispplcc68" },
    { "ATF1504AS", "TQFP100", "f1504tqfp100", "f1504isptqfp100" }, { "ATF1504AS", "PQFP100", "f1504qfp100", "f1504ispqfp100" },
    { "ATF1508AS", "PLCC84", "f1508plcc84", "f1508ispplcc84" }, { "ATF1508AS", "TQFP100", "f1508tqfp100", "f1508isptqfp100" },
    { "ATF1508AS", "PQFP100", "f1508qfp100", "f1508ispqfp100" }, { "ATF1508AS", "PQFP160", "f1508pqfp160", "f1508isppqfp160" } };

typedef struct {
    const struct anet *N; const int *pins; char **names; int nn;
    slist out_pads; slist mcn_k, mcn_v;
} ectx;

static const char *name_of_pin(const ectx *E, int pin) { for (int i = 0; i < E->nn; i++) if (E->pins[i] == pin) return E->names[i]; return NULL; }
static int pin_of(const ectx *E, const char *pad) {
    int p = atf_pin(E->N, pad);
    if (!p) fatal("pad %s has no pin in %s", pad, E->N->pkg);
    return p;
}
static char *pn(const ectx *E, const char *pad) { int p = pin_of(E, pad); const char *n = name_of_pin(E, p); return n ? xstrdup(n) : fmt("p%d", p); }
static const char *mcname(const ectx *E, const char *mc) { int i = sl_find(&E->mcn_k, mc); if (i < 0) fatal("no macrocell %s", mc); return E->mcn_v.v[i]; }
static const char *wire(const ectx *E, const char *n) { const char *w = atf_wire(E->N, n); if (!w) fatal("no signal %s", n); return w; }

static int used_q(const ectx *E, const char *mc) {
    char *q = fmt("%s_q", py_lower(mc));
    const struct anet *N = E->N;
    for (int i = 0; i < N->nw; i++) {          /* the values of the signal map: the last definition of each name */
        if (atf_wire(N, N->w[i].name) != N->w[i].val) continue;
        if (strstr(N->w[i].val, q)) return 1;
    }
    for (int i = 0; i < N->no; i++) if (strstr(N->out[i].a, q)) return 1;
    for (int i = 0; i < N->nr; i++) if (strstr(N->r[i].d, q)) return 1;
    return 0;
}

static char *cupl(const ectx *E, const char *expr, int depth);
typedef struct { const ectx *E; int depth; } subctx;
static char *sub_tok(const rxm *m, void *vc) {
    static rx *rm2;
    if (!rm2) rm2 = rx_compile("(mc\\d+)_(q|xt|st|pt\\d)$", 0);
    subctx *c = vc; const ectx *E = c->E; const struct anet *N = E->N;
    char *t = rx_group(m, 0);
    if (startswith(t, "uim_") || startswith(t, "casout_") || startswith(t, "g_")) return fmt("(%s)", cupl(E, wire(E, t), c->depth + 1));
    if (startswith(t, "pad_")) {
        const char *pad = t + 4; const struct aout *o = atf_output(N, pad);
        if (o && (strcmp(o->en, "1'b1") || o->od)) {
            if (sl_find(&E->out_pads, pad) < 0) refuse("EMIT_BIDIR_UNDECLARED", "pad %s is read back and driven but not a declared pin", pad);
            return fmt("%s.io", pn(E, pad));
        }
        return pn(E, pad);
    }
    if (startswith(t, "fb_")) return xstrdup(mcname(E, t + 3));
    if (startswith(t, "flb_")) return strcmp(wire(E, t), "1'b1") ? fmt("f%s", py_lower(t + 4)) : xstrdup("'b'1");
    rxm k;
    if (rx_match(rm2, t, &k)) {
        char *mc = py_upper(rx_group(&k, 1)), *what = rx_group(&k, 2);
        if (!strcmp(what, "q")) return xstrdup(mcname(E, mc));
        if (!strcmp(what, "xt")) {
            if (used_q(E, mc)) refuse("EMIT_MIXED", "%s: XT and register output both used", mc);
            return xstrdup(mcname(E, mc));
        }
        return fmt("(%s)", cupl(E, wire(E, t), c->depth + 1));
    }
    return t;
}
static char *cupl(const ectx *E, const char *expr, int depth) {
    static rx *rtok, *rws;
    if (!rtok) { rtok = rx_compile("[A-Za-z_][A-Za-z0-9_]*", 0); rws = rx_compile("\\s+", 0); }
    if (depth > 40) refuse("EMIT_LOOP", "%s", u8_prefix(expr, 60));
    subctx c = { E, depth };
    char *e = rx_sub(rtok, expr, NULL, sub_tok, &c);
    e = replace_all(e, "1'b1", "'b'1"); e = replace_all(e, "1'b0", "'b'0"); e = replace_all(e, "~", "!");
    e = replace_all(e, "|", " # "); e = replace_all(e, "^", " $ "); e = replace_all(e, "&", " & ");
    return rx_sub(rws, e, " ", NULL, NULL);
}

static char *wrap70(const char *e) {   /* the fitter limits line length: break before # & $ operators */
    static rx *rsep;
    if (!rsep) rsep = rx_compile("(\\s[#&$]\\s)", 0);
    slist toks = { 0 };
    size_t len = strlen(e), pos = 0; rxm m;
    while (pos <= len && rx_search_n(rsep, e, len, pos, &m)) {
        sl_push_own(&toks, xstrndup(e + pos, m.g[0][0] - pos));
        sl_push_own(&toks, rx_group(&m, 1));
        pos = m.g[0][1];
    }
    sl_push_own(&toks, xstrdup(e + pos));
    slist out = { 0 }; sbuf line; sb_init(&line); int ll = 0;
    for (int i = 0; i < toks.n; i++) {
        int tl = u8_len(toks.v[i]);
        if (ll + tl > 70 && ll) {
            sl_push_own(&out, sb_take(&line));
            char *t = py_lstrip(toks.v[i]);
            sb_add(&line, "    "); sb_add(&line, t); ll = 4 + u8_len(t);
        } else { sb_add(&line, toks.v[i]); ll += tl; }
    }
    sl_push_own(&out, sb_take(&line));
    return sl_join(&out, "\n");
}

void atf_check_jtag_declared(const struct anet *N, const int *pins, int nn) {
    for (int i = 0; i < N->njd; i++) {
        int p = atf_pin(N, N->jd[i].name);
        if (!p) continue;
        for (int k = 0; k < nn; k++) if (pins[k] == p) refuse("JTAG_PAD_USED", "%s drives JTAG pad %s, a declared pin (JTAG mode)", N->jd[i].val, N->jd[i].name);
    }
}

typedef struct { int num; char *name; } node;
static int cmp_node(const void *a, const void *b) {
    const node *x = a, *y = b;
    if (x->num != y->num) return x->num - y->num;
    return strcmp(x->name, y->name);
}
static int cmp_mcname(const void *a, const void *b) {   /* "MC1" < "MC10" < "MC2": the text order */
    return strcmp((*(struct ainfo *const *)a)->mc, (*(struct ainfo *const *)b)->mc);
}

char *atf_emit(const struct anet *N, const int *pins, char **names, int nn, const char *design) {
    ectx E; memset(&E, 0, sizeof E); E.N = N; E.pins = pins; E.names = names; E.nn = nn;
    atf_check_jtag_declared(N, pins, nn);
    for (int i = 0; i < N->no; i++) if (name_of_pin(&E, pin_of(&E, N->out[i].pad))) sl_push(&E.out_pads, N->out[i].pad);
    /* dropped: driven pads that are not declared pins, by pin */
    int nd = 0; int *dpin = xmalloc(sizeof(int) * (size_t)(N->no + 1)); char **dpad = xmalloc(sizeof(char *) * (size_t)(N->no + 1));
    for (int i = 0; i < N->no; i++) if (sl_find(&E.out_pads, N->out[i].pad) < 0) { dpin[nd] = pin_of(&E, N->out[i].pad); dpad[nd] = N->out[i].pad; nd++; }
    for (int i = 1; i < nd; i++) for (int j = i; j > 0 && dpin[j - 1] > dpin[j]; j--) { int t = dpin[j]; dpin[j] = dpin[j - 1]; dpin[j - 1] = t; char *s = dpad[j]; dpad[j] = dpad[j - 1]; dpad[j - 1] = s; }
    for (int i = 0; i < N->ni; i++) {
        const struct ainfo *I = &N->info[i];
        sl_push(&E.mcn_k, I->mc);
        sl_push_own(&E.mcn_v, (I->pad && sl_find(&E.out_pads, I->pad) >= 0) ? pn(&E, I->pad) : fmt("n%s", py_lower(I->mc)));
    }
    int maxpin = 0;
    for (int i = 0; i < N->K->n; i++) if (N->K->pins[i].pin > maxpin) maxpin = N->K->pins[i].pin;
    char **pinname = xcalloc((size_t)maxpin + 2, sizeof(char *));
    for (int i = 0; i < N->inputs.n; i++) {
        int p = atf_pin(N, N->inputs.v[i]);
        if (!p) refuse("EMIT_PAD", "%s", N->inputs.v[i]);
        pinname[p] = pn(&E, N->inputs.v[i]);
    }
    node *nodes = xmalloc(sizeof(node) * (size_t)(2 * N->ni + 1)); int nnodes = 0;
    slist eqs = { 0 };
    static rx *rpt;
    if (!rpt) rpt = rx_compile("mc\\d+_pt", 0);
    for (int i = 0; i < N->ni; i++) {          /* the info list is in macrocell number order */
        const struct ainfo *I = &N->info[i];
        int k = atoi(I->mc + 2);
        int is_out = I->pad && sl_find(&E.out_pads, I->pad) >= 0;
        rx *rfb = rx_compile(fmt("\\bfb_%s\\b", I->mc), 0), *rflb = rx_compile(fmt("\\bflb_%s\\b", I->mc), 0);
        char *fbn = fmt("fb_%s", I->mc);
        int fb_used = 0, flb_used = 0; rxm m;
        for (int w = 0; w < N->nw; w++) {        /* over the signal map: the last definition of each name */
            if (atf_wire(N, N->w[w].name) != N->w[w].val) continue;
            if (startswith(N->w[w].name, "uim_") && (!strcmp(N->w[w].val, fbn) || rx_search(rfb, N->w[w].val, 0, &m))) fb_used = 1;
            if (I->pt1_flb && rx_match(rpt, N->w[w].name, &m) && rx_search(rflb, N->w[w].val, 0, &m)) flb_used = 1;
        }
        if (flb_used) {
            char *lmc = py_lower(I->mc);
            nodes[nnodes].num = 300 + k; nodes[nnodes].name = fmt("f%s", lmc); nnodes++;
            const char *pt1 = NULL;
            for (int q = 0; q < I->n_pt; q++) if (!strcmp(I->ptname[q], "PT1")) pt1 = I->pt[q];
            if (!pt1) fatal("%s has no PT1", I->mc);
            sl_push_own(&eqs, fmt("!f%s = %s;", lmc, cupl(&E, wire(&E, pt1), 0)));
        }
        if (!(is_out || fb_used)) continue;
        const char *name = mcname(&E, I->mc);
        if (is_out) pinname[pin_of(&E, I->pad)] = xstrdup(name);
        else { nodes[nnodes].num = 600 + k; nodes[nnodes].name = xstrdup(name); nnodes++; }
        int q = used_q(&E, I->mc);
        int x = (is_out && endswith(I->a, "_xt")) || (fb_used && endswith(I->fb, "_xt"));
        if (q && x) refuse("EMIT_MIXED", "%s: register output and XT both used", I->mc);
        if (q) {
            if (!strcmp(I->clk, "1'b1")) refuse("EMIT_CONST_CLOCK", "%s", I->mc);
            if (sl_find(&N->latches, I->mc) >= 0) {
                sl_push_own(&eqs, fmt("%s.l = %s;", name, cupl(&E, I->xt, 0)));
                sl_push_own(&eqs, fmt("%s.le = %s;", name, cupl(&E, I->clk, 0)));
            } else if (I->xb && strcmp(atf_info_opt(I, "cas_mux"), "sum")) {
                char *st = cupl(&E, fmt("%s_st", py_lower(I->mc)), 0);
                sl_push_own(&eqs, fmt("%s.t = %s;", name, I->inv_on ? st : fmt("!(%s)", st)));
                sl_push_own(&eqs, fmt("%s.ck = %s;", name, cupl(&E, I->clk, 0)));
            } else {
                sl_push_own(&eqs, fmt("%s.d = %s;", name, cupl(&E, I->xt, 0)));
                sl_push_own(&eqs, fmt("%s.ck = %s;", name, cupl(&E, I->clk, 0)));
            }
            if (strcmp(I->ar, "1'b0")) sl_push_own(&eqs, fmt("%s.ar = %s;", name, cupl(&E, I->ar, 0)));
            if (strcmp(I->as, "1'b0")) sl_push_own(&eqs, fmt("%s.ap = %s;", name, cupl(&E, I->as, 0)));
            if (strcmp(I->ce, "1'b1")) sl_push_own(&eqs, fmt("%s.ce = %s;", name, cupl(&E, I->ce, 0)));
        } else sl_push_own(&eqs, fmt("%s = %s;", name, cupl(&E, I->xt, 0)));
        if (is_out && strcmp(I->en, "1'b1")) sl_push_own(&eqs, fmt("%s.oe = %s;", name, cupl(&E, I->en, 0)));
    }
    slist props = { 0 };
    sl_push(&props, "PROPERTY ATMEL {Preassign = Keep};");
    /* the configuration note: "config: a=b, c=d, ..." */
    const char *cfgs = N->notes.v[0] + strlen("config: ");
    if (strstr(fmt(", %s, ", cfgs), ", termination=high_z, ")) sl_push(&props, "PROPERTY ATMEL {pin_keep OFF};");
    if (N->security == 1) sl_push(&props, "PROPERTY ATMEL {security on};");
    int nout = 0, nfast = 0;
    for (int i = 0; i < N->ni; i++) {
        const struct ainfo *I = &N->info[i];
        if (I->pad && sl_find(&E.out_pads, I->pad) >= 0) { nout++; if (!strcmp(atf_info_opt(I, "slew_rate"), "fast")) nfast++; }
    }
    if (nout && nfast == nout) sl_push(&props, "PROPERTY ATMEL {output_fast on};");
    else if (nfast) sl_push_own(&props, fmt("/* slew fast on %d of %d outputs: not reproducible with the fitter's per-pin syntax */", nfast, nout));
    const struct ainfo **srt = xmalloc(sizeof(void *) * (size_t)(N->ni + 1));
    for (int i = 0; i < N->ni; i++) srt[i] = &N->info[i];
    qsort(srt, (size_t)N->ni, sizeof(void *), cmp_mcname);
    for (int i = 0; i < N->ni; i++) {
        const struct ainfo *I = srt[i];
        if (strcmp(atf_info_opt(I, "low_power"), "on")) continue;
        if (I->pad && sl_find(&E.out_pads, I->pad) >= 0) sl_push_own(&props, fmt("PROPERTY ATMEL {MC_power = %s};", mcname(&E, I->mc)));
    }
    for (int i = 0; i < N->no; i++) if (N->out[i].od && sl_find(&E.out_pads, N->out[i].pad) >= 0) sl_push_own(&props, fmt("PROPERTY ATMEL {open_collector = %s};", pn(&E, N->out[i].pad)));
    const char *mn = NULL;
    for (size_t i = 0; i < sizeof MNEM / sizeof MNEM[0]; i++) if (!strcmp(MNEM[i][0], N->dev) && !strcmp(MNEM[i][1], N->pkg)) mn = MNEM[i][N->jtag_mode ? 3 : 2];
    if (!mn) fatal("no device name for %s %s", N->dev, N->pkg);
    slist L = { 0 };
    sl_push_own(&L, fmt("Name     %s;", design)); sl_push(&L, "PartNo   00;"); sl_push(&L, "Date     18/10/26;"); sl_push(&L, "Revision 01;");
    sl_push_own(&L, fmt("Designer %s;", CREDIT)); sl_push_own(&L, fmt("Company  %s;", CREDIT)); sl_push(&L, "Assembly None;"); sl_push(&L, "Location None;");
    sl_push_own(&L, fmt("Device   %s;", mn)); sl_push(&L, "");
    for (int i = 0; i < props.n; i++) sl_push(&L, props.v[i]);
    sl_push(&L, "");
    sl_push_own(&L, fmt("/* emitted by %s from a decoded %s %s fuse map */", TOOL_VER, N->dev, N->pkg));
    for (int i = 0; i < nd; i++) sl_push_own(&L, fmt("/* driven but undeclared pad, not emitted: %s (pin %d) */", dpad[i], dpin[i]));
    for (int p = 0; p <= maxpin; p++) if (pinname[p]) sl_push_own(&L, fmt("PIN %d = %s;", p, pinname[p]));
    qsort(nodes, (size_t)nnodes, sizeof(node), cmp_node);
    for (int i = 0; i < nnodes; i++) sl_push_own(&L, fmt("PINNODE %d = %s;", nodes[i].num, nodes[i].name));
    sl_push(&L, "");
    for (int i = 0; i < eqs.n; i++) sl_push_own(&L, wrap70(eqs.v[i]));
    return fmt("%s\n", sl_join(&L, "\n"));
}

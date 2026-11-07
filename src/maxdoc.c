/* maxdoc.c -- the documentation view of a MAX build input: per macrocell its product terms, sum, expander use, data,
 * register (type, clock, enable, clear, preset, power-up) and output, and per pin its value and output enable, as
 * readable Verilog equations. Written from the build input text itself: the build input is first rewritten in the
 * netlist dialect (pins as max_io, the emitter's constant nets substituted), then read back.
 * Notation: ~ not, & and, | or, ^ xor; lcN = macrocell N's output (a register: its state); sexpN = shared expander N
 * (NAND of its literals); pN = pin N.
 */
#include "max.h"
#include "rx.h"
#include <stdlib.h>
#include <string.h>

static const char *TAG = "DOCUMENTATION VIEW -- documentation, not the build input";

static char *rx_escape(const char *s) {   /* escape the characters that are special in a pattern */
    sbuf b; sb_init(&b);
    for (; *s; s++) { if (strchr("()[]{}?*+-|^$\\.&~# \t\n\r\v\f", *s)) sb_addc(&b, '\\'); sb_addc(&b, *s); }
    return sb_take(&b);
}
typedef struct { const char *v; } cst;
static char *const_sub_fn(const rxm *m, void *ctx) {
    const char *v = ((cst *)ctx)->v; int neg = m->g[1][1] > m->g[1][0];
    return xstrdup(((!strcmp(v, "1'b1")) == neg) ? "gnd " : "vcc ");
}
typedef struct { slist k, v; } smap;   /* insertion-ordered map; assignment keeps the first position */
static void sm_set(smap *m, const char *k, const char *v) {
    int i = sl_find(&m->k, k);
    if (i >= 0) m->v.v[i] = xstrdup(v); else { sl_push(&m->k, k); sl_push(&m->v, v); }
}
static const char *sm_get(const smap *m, const char *k) { int i = sl_find(&m->k, k); return i >= 0 ? m->v.v[i] : NULL; }

static char *subst_consts(const smap *cn, char *s) {
    for (int i = 0; i < cn->k.n; i++) {
        rx *r = rx_compile(fmt("(!?)%s\\s", rx_escape(cn->k.v[i])), 0);
        cst c = { cn->v.v[i] };
        s = rx_sub(r, s, NULL, const_sub_fn, &c);
    }
    return s;
}
static char *g01(const char *s) {
    static rx *r1, *r0;
    if (!r1) { r1 = rx_compile("1'b1", 0); r0 = rx_compile("1'b0", 0); }
    return rx_sub(r1, rx_sub(r0, s, "gnd", NULL, NULL), "vcc", NULL, NULL);
}

static char *em_to_vo(const char *t) {
    static rx *rc, *rp, *ro, *rh, *rk, *rm, *rd, *rod, *rtri;
    if (!rc) {
        rc = rx_compile("^assign (\\\\\\S+)\\s+= (1'b[01]);", RX_M); rp = rx_compile("^assign (\\\\pin\\d+)\\s+= (p\\d+);", RX_M);
        ro = rx_compile("^assign (p\\d+) = (.*);$", RX_M); rh = rx_compile("^module (\\w+) \\((.*?)\\);", RX_M | RX_S);
        rk = rx_compile("^(input|output|inout) (p\\d+);", RX_M);
        rm = rx_compile("^(max_mcell|max_sexp) (\\\\\\S+)\\s+\\((.*?)\\);\\n((?:defparam [^\\n]*\\n)*)", RX_M | RX_S);
        rd = rx_compile("\\d+", 0);
        rod = rx_compile("\\((.*) & !(.*)\\) \\? gnd : 1'bz$", 0); rtri = rx_compile("(.*?) \\? (.*) : 1'bz$", 0);
    }
    size_t len = strlen(t), pos; rxm m;
    smap cn = { { 0 }, { 0 } }, pin_rd = { { 0 }, { 0 } }, kind = { { 0 }, { 0 } };
    slist on = { 0 }, ov = { 0 };
    for (pos = 0; pos <= len && rx_search_n(rc, t, len, pos, &m); pos = m.g[0][1]) sm_set(&cn, rx_group(&m, 1), rx_group(&m, 2));
    for (pos = 0; pos <= len && rx_search_n(rp, t, len, pos, &m); pos = m.g[0][1]) sm_set(&pin_rd, rx_group(&m, 1), rx_group(&m, 2));
    for (pos = 0; pos <= len && rx_search_n(ro, t, len, pos, &m); pos = m.g[0][1]) { sl_push_own(&on, rx_group(&m, 1)); sl_push_own(&ov, rx_group(&m, 2)); }
    if (!rx_search(rh, t, 0, &m)) fatal("documentation view: no module header in the build input");
    char *top = rx_group(&m, 1), *plist_ = rx_group(&m, 2);
    slist ports = { 0 };
    { char *p = plist_; for (;;) { char *c = strchr(p, ','); char *x = py_strip(c ? xstrndup(p, (size_t)(c - p)) : p); if (*x) sl_push_own(&ports, x); if (!c) break; p = c + 1; } }
    for (pos = 0; pos <= len && rx_search_n(rk, t, len, pos, &m); pos = m.g[0][1]) sm_set(&kind, rx_group(&m, 2), rx_group(&m, 1));
    slist body = { 0 };
    for (pos = 0; pos <= len && rx_search_n(rm, t, len, pos, &m); pos = m.g[0][1]) {
        char *kindc = rx_group(&m, 1), *nm = rx_group(&m, 2), *pr = rx_group(&m, 3), *dp = rx_group(&m, 4);
        rxm k; rx_search(rd, nm, 0, &k);
        int n = atoi(rx_group(&k, 0));
        char *pr2 = g01(pr);
        if (!strcmp(kindc, "max_mcell") && !strstr(pr2, ".pena(")) {
            sbuf b; sb_init(&b); sb_add(&b, pr2); sb_add(&b, ", .pena({");
            for (int i = 0; i < 52; i++) sb_add(&b, i ? ",vcc" : "vcc");
            sb_add(&b, "})"); pr2 = sb_take(&b);
        }
        pr2 = subst_consts(&cn, pr2);
        sl_push_own(&body, fmt("// Location: %s%d\n%s %s (%s);\n%s", !strcmp(kindc, "max_mcell") ? "LC" : "SEXP", n, kindc, nm, pr2, dp));
    }
    slist io = { 0 };
    for (int i = 0; i < pin_rd.k.n; i++) {
        const char *nm = pin_rd.k.v[i], *p = pin_rd.v.v[i], *kd = sm_get(&kind, p);
        if (kd && !strcmp(kd, "input"))
            sl_push_own(&io, fmt("// Location: PIN_%s\nmax_io \\%s~I (.padio(%s), .dataout(%s ));\ndefparam \\%s~I .operation_mode = \"input\";", p + 1, p, p, nm, p));
    }
    for (int i = 0; i < on.n; i++) {
        const char *p = on.v[i];
        char *rhs = fmt("%s ", py_strip(g01(ov.v[i])));
        rhs = py_strip(subst_consts(&cn, rhs));
        const char *oe, *din, *odm; rxm k;
        if (rx_match(rod, rhs, &k)) { oe = py_strip(rx_group(&k, 1)); din = py_strip(rx_group(&k, 2)); odm = "true"; }
        else if (rx_match(rtri, rhs, &k)) { oe = py_strip(rx_group(&k, 1)); din = py_strip(rx_group(&k, 2)); odm = "false"; }
        else { oe = "vcc"; din = rhs; odm = "false"; }
        const char *kd = sm_get(&kind, p);
        const char *mode = (kd && !strcmp(kd, "inout")) ? "bidir" : "output";
        const char *dout = NULL;
        for (int j = 0; j < pin_rd.k.n; j++) if (!strcmp(pin_rd.v.v[j], p)) { dout = pin_rd.k.v[j]; break; }
        char *extra = (dout && !strcmp(mode, "bidir")) ? fmt(", .dataout(%s )", dout) : xstrdup("");
        sl_push_own(&io, fmt("// Location: PIN_%s\nmax_io \\%s~O (.datain(%s ), .oe(%s ), .padio(%s)%s);\ndefparam \\%s~O .operation_mode = \"%s\";\ndefparam \\%s~O .open_drain_output = \"%s\";",
                             p + 1, p, din, oe, p, extra, p, mode, p, odm));
    }
    slist wires = { 0 };
    { const char *p = t;
      while (*p) {
          const char *e = strchr(p, '\n'); size_t l = e ? (size_t)(e - p) : strlen(p);
          char *line = xstrndup(p, l);
          if (startswith(line, "wire ")) {
              int skip = 0;
              for (int i = 0; i < cn.k.n; i++) if (startswith(line, fmt("wire %s ", cn.k.v[i]))) skip = 1;
              if (!skip) sl_push_own(&wires, line);
          }
          if (!e) break;
          p = e + 1;
      } }
    slist decl = { 0 };
    for (int i = 0; i < ports.n; i++) {
        const char *kd = sm_get(&kind, ports.v[i]);
        if (!kd) fatal("documentation view: port %s has no direction", ports.v[i]);
        sl_push_own(&decl, fmt("%s %s;", kd, ports.v[i]));
    }
    return fmt("module %s (%s);\n%s\n%s\n%s\n%s\nendmodule\n", top, sl_join(&ports, ", "), sl_join(&decl, "\n"), sl_join(&wires, "\n"), sl_join(&io, "\n"), sl_join(&body, "\n"));
}

static char *dname(const char *net0) {
    static rx *rpin, *rlc, *rlcn, *rsx, *rpx;
    if (!rpin) {
        rpin = rx_compile("pin(\\d+)$", 0); rlc = rx_compile("lc(\\d+)_out$", 0); rlcn = rx_compile("lc(\\d+)", 0);
        rsx = rx_compile("sexp(\\d+)_out$", 0); rpx = rx_compile("lc(\\d+)_pexp$", 0);
    }
    char *net = py_strip(net0);
    int neg = net[0] == '!';
    char *n = py_strip(py_strip_chars(py_strip(py_strip_chars(net, "!", 1, 0)), "\\", 1, 0));
    rxm m;
    if (rx_match(rpin, n, &m)) n = fmt("p%s", n + 3);
    else if (rx_match(rlc, n, &m)) { rx_match(rlcn, n, &m); n = fmt("lc%s", rx_group(&m, 1)); }
    else if (rx_match(rsx, n, &m)) n = fmt("sexp%s", rx_group(&m, 1));
    else if (rx_match(rpx, n, &m)) { rx_match(rlcn, n, &m); n = fmt("lc%s_pexp", rx_group(&m, 1)); }
    else if (!strcmp(n, "vcc") || !strcmp(n, "gnd")) n = xstrdup(!strcmp(n, "vcc") ? "1'b1" : "1'b0");
    else refuse("DOC_NET", "%s", net);     /* a bare port name (pN) is not a net of the view */
    return neg ? fmt("~%s", n) : n;
}

/* a 52-entry product-term bus: kind 0 = constant 0 (a gnd entry), 1 = constant 1 (no literal), 2 = literals */
static char *product(const char *bus, int *kind) {
    char *s = py_strip_chars(py_strip(bus), "{}", 1, 1), *p = s;
    slist it = { 0 };
    for (;;) { char *c = strchr(p, ','); sl_push_own(&it, py_strip(c ? xstrndup(p, (size_t)(c - p)) : p)); if (!c) break; p = c + 1; }
    for (int i = 0; i < it.n; i++) if (!strcmp(it.v[i], "gnd")) { *kind = 0; return xstrdup("1'b0"); }
    slist lits = { 0 };
    for (int i = 0; i < it.n; i++) if (strcmp(it.v[i], "vcc")) sl_push_own(&lits, dname(it.v[i]));
    if (!lits.n) { *kind = 1; return xstrdup("1'b1"); }
    *kind = 2;
    return lits.n > 1 ? fmt("(%s)", sl_join(&lits, " & ")) : lits.v[0];
}

static const char *par_or(const struct inst *x, const char *n, const char *d) { const char *v = vo_par(x, n); return v ? v : d; }
static char *pc(const struct inst *x, const char *k, const char *d) { const char *v = vo_port(x, k); return py_strip(v && *v ? v : d); }

char *max_doc_view(const char *em) {
    static rx *rtop;
    if (!rtop) rtop = rx_compile("^module (\\w+)", RX_M);
    char *vo = em_to_vo(em);
    struct netlist N; vo_parse(vo, &N);
    rxm m; rx_search(rtop, em, 0, &m);
    char *top = rx_group(&m, 1);
    slist L = { 0 };
    sl_push_own(&L, fmt("// %s: %s.", top, TAG));
    sl_push_own(&L, fmt("// The build input is %s.v + %s.qsf (atom-level Verilog); equivalence to the build input not checked for this file.", top, top));
    sl_push_own(&L, fmt("// Written from the build input by %s; credit: %s", TOOL_VER, CREDIT));
    sl_push(&L, "// Notation: ~ not, & and, | or, ^ xor. lcN = macrocell N's output (a register: its state); sexpN = shared expander N");
    sl_push(&L, "// (NAND of its literals); pN = pin N. A register's lcN_d is the value it loads at an active clock edge while enabled;");
    sl_push(&L, "// clear wins over preset; the macrocell's output lcN is the register state (registered) or lcN_d (combinational).");
    slist allp = { 0 };
    for (int i = 0; i < N.inputs.n; i++) sl_push(&allp, N.inputs.v[i]);
    for (int i = 0; i < N.outputs.n; i++) sl_push(&allp, N.outputs.v[i]);
    for (int i = 0; i < N.inouts.n; i++) sl_push(&allp, N.inouts.v[i]);
    sl_push_own(&L, fmt("module %s_doc (%s);", top, sl_join(&allp, ", ")));
    for (int i = 0; i < N.inputs.n; i++) sl_push_own(&L, fmt("  input %s;", N.inputs.v[i]));
    for (int i = 0; i < N.outputs.n; i++) sl_push_own(&L, fmt("  output %s;", N.outputs.v[i]));
    for (int i = 0; i < N.inouts.n; i++) sl_push_own(&L, fmt("  inout %s;", N.inouts.v[i]));
    /* shared expanders, then macrocells, in location order (a stable sort of the instance list) */
    int nmx = 0;
    for (int i = 0; i < N.ni; i++) if (N.in[i].loc && (startswith(N.in[i].loc, "LC") || startswith(N.in[i].loc, "SEXP"))) { int v = atoi(N.in[i].loc + (N.in[i].loc[0] == 'L' ? 2 : 4)); if (v > nmx) nmx = v; }
    for (int pass = 0; pass < 2; pass++) {
        const char *kd = pass ? "max_mcell" : "max_sexp";
        for (int n = 0; n <= nmx; n++)
            for (int i = 0; i < N.ni; i++) {
                const struct inst *x = &N.in[i];
                if (strcmp(x->kind, kd)) continue;
                int xn = atoi(x->loc + (pass ? 2 : 4));
                if (xn != n) continue;
                if (!pass) {
                    int k; char *e = product(vo_port(x, "datain"), &k);
                    sl_push_own(&L, fmt("  wire %s = ~%s;   // shared expander, LAB %c", dname(vo_port(x, "dataout")), e, 65 + (xn - 1) / 16));
                    continue;
                }
                const char *om = par_or(x, "operation_mode", "normal"), *outm = par_or(x, "output_mode", "comb"), *rm = par_or(x, "register_mode", "dff"),
                           *pm = par_or(x, "pexp_mode", "off"), *pu = par_or(x, "power_up", "low");
                int reg = !strcmp(outm, "reg") || !strcmp(rm, "tff");
                slist terms = { 0 };
                for (int k = 0; k < 5; k++) { int kk; char *e = product(pc(x, fmt("pterm%d", k), "{gnd}"), &kk); if (kk) sl_push_own(&terms, e); }
                char *pin = pc(x, "pexpin", "gnd");
                if (strcmp(pin, "gnd") && *pin) sl_push_own(&terms, dname(pin));
                sl_push_own(&L, fmt("  // ---- LC%d (LAB %c): %d product term(s)%s ----", xn, 65 + (xn - 1) / 16, terms.n,
                                    !strcmp(pm, "on") ? fmt(", parallel-expander donor (its sum goes to LC%d)", xn + 1) : ""));
                sl_push_own(&L, fmt("  wire lc%d_sum = %s;", xn, terms.n ? sl_join(&terms, " | ") : "1'b0"));
                char *base = fmt("lc%d_sum", xn);
                if (!strcmp(pm, "on")) {
                    int k5; char *e5 = product(pc(x, "pterm5", "{gnd}"), &k5);
                    sl_push_own(&L, fmt("  wire lc%d_pexp = lc%d_sum;   // to the next macrocell's expander input", xn, xn));
                    sl_push_own(&L, fmt("  wire lc%d_own = %s;   // own data: product term 5", xn, k5 ? e5 : "1'b0"));
                    base = fmt("lc%d_own", xn);
                }
                char *q = (!reg || !strcmp(outm, "reg")) ? fmt("lc%d", xn) : fmt("lc%d_q", xn);
                char *dd = NULL; int tff = !strcmp(rm, "tff");
                if (!strcmp(om, "normal")) dd = tff ? fmt("%s ^ %s", base, q) : xstrdup(base);
                else if (!strcmp(om, "invert")) dd = tff ? fmt("%s ^ ~%s", base, q) : fmt("~%s", base);
                else if (!strcmp(om, "xor")) { int kx; char *ex = product(pc(x, "pxor", "{gnd}"), &kx); dd = fmt("%s ^ %s", base, kx ? ex : "1'b0"); }
                else if (!strcmp(om, "vcc")) dd = tff ? fmt("~%s", q) : (!strcmp(pc(x, "fpin", "vcc"), "vcc") ? xstrdup("1'b1") : dname(pc(x, "fpin", "vcc")));
                else refuse("DOC_MODE", "LC%d operation_mode %s", xn, om);
                sl_push_own(&L, fmt("  wire lc%d_d = %s;   // data (%s%s)", xn, dd, om, (tff && strcmp(om, "vcc")) ? fmt(", T flip-flop: toggles when %s = 1", base) : ""));
                if (reg) {
                    char *gk = pc(x, "clk", "gnd"); int kc, ke, ka, kp;
                    char *pclk = product(pc(x, "pclk", "{gnd}"), &kc), *en = product(pc(x, "pena", "{vcc}"), &ke);
                    slist cl = { 0 };
                    char *ac = pc(x, "aclr", "gnd");
                    if (strcmp(ac, "gnd") && *ac) sl_push_own(&cl, dname(ac));
                    char *pa = product(pc(x, "paclr", "{gnd}"), &ka);
                    if (ka) sl_push_own(&cl, pa);
                    char *pp = product(pc(x, "papre", "{gnd}"), &kp);
                    slist ck = { 0 };
                    if (strcmp(gk, "gnd") && *gk) sl_push_own(&ck, dname(gk));
                    if (kc) sl_push_own(&ck, pclk);
                    sl_push_own(&L, fmt("  wire lc%d_clk = %s;   // rising edge loads lc%d_d (%s clock)", xn, ck.n ? sl_join(&ck, " | ") : "1'b0", xn, (strcmp(gk, "gnd") && *gk) ? "global" : "product-term"));
                    sl_push_own(&L, fmt("  wire lc%d_ena = %s;", xn, ke ? en : "1'b0"));
                    sl_push_own(&L, fmt("  wire lc%d_clr = %s;", xn, cl.n ? sl_join(&cl, " | ") : "1'b0"));
                    sl_push_own(&L, fmt("  wire lc%d_pre = %s;", xn, kp ? pp : "1'b0"));
                    sl_push_own(&L, fmt("  // register lc%d: %s, power-up %s, output %s", xn, tff ? "T flip-flop" : "D flip-flop", pu,
                                        !strcmp(outm, "reg") ? "registered" : fmt("combinational (lc%d_d)", xn)));
                    if (strcmp(outm, "reg")) sl_push_own(&L, fmt("  wire lc%d = lc%d_d;   // combinational output of a registered cell (state lc%d_q)", xn, xn, xn));
                } else sl_push_own(&L, fmt("  wire lc%d = lc%d_d;   // combinational output", xn, xn));
            }
    }
    sl_push(&L, "  // ---- pins ----");
    for (int i = 0; i < N.ni; i++) {
        const struct inst *x = &N.in[i];
        if (strcmp(x->kind, "max_io") || !strcmp(par_or(x, "operation_mode", ""), "input")) continue;
        char *p = py_strip(vo_port(x, "padio") ? vo_port(x, "padio") : "");
        char *din = dname(vo_port(x, "datain") ? vo_port(x, "datain") : "");
        const char *oev = vo_port(x, "oe"); char *oe = py_strip(oev && *oev ? oev : "vcc");
        const char *od = vo_par(x, "open_drain_output");
        if (od && !strcmp(od, "true")) sl_push_own(&L, fmt("  assign %s = (%s & ~%s) ? 1'b0 : 1'bz;   // open drain", p, !strcmp(oe, "vcc") ? "1'b1" : dname(oe), din));
        else if (!strcmp(oe, "vcc")) sl_push_own(&L, fmt("  assign %s = %s;", p, din));
        else sl_push_own(&L, fmt("  assign %s = %s ? %s : 1'bz;   // output enable", p, dname(oe), din));
    }
    sl_push(&L, "endmodule");
    return fmt("%s\n", sl_join(&L, "\n"));
}

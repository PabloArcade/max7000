/* maxemit.c -- the MAX build input: the decoded design written as the vendor's atom primitives (max_mcell, max_sexp)
 * in Verilog, so the fitter keeps every macrocell, shared expander and parallel expander as decoded, plus the .qsf
 * with the device, the pins, the options and every macrocell frozen at its original location.
 *
 * Rules:
 *  - a raw unused macrocell (registered, no clock, not a donor) is written as constant 0 (its clear is constant true);
 *    an expander donor with no output as constant 1;
 *  - a ground-clamped pin is never declared (RESERVE_ALL_UNUSED_PINS "AS OUTPUT DRIVING GROUND" writes the clamp);
 *  - bonded I/O pins with the output off that nothing reads: the design reserved unused pins as tri-stated inputs;
 *    a raw unused pin is then declared and driven 0 (one option governs every unused pin);
 *  - a donor whose own output is constant is read as that constant;
 *  - a fast-input macrocell keeps its pin on .fpin and has no product-term ports;
 *  - MAX 7000B bus hold / weak pull-up go to the declared I/O pins; the undeclared pins choose the reserve mode.
 */
#include "max.h"
#include "rx.h"
#include <stdlib.h>
#include <string.h>

static char *c0(const char *s) {   /* gnd / vcc as Verilog constants */
    static rx *rg, *rv;
    if (!rg) { rg = rx_compile("\\bgnd\\b", 0); rv = rx_compile("\\bvcc\\b", 0); }
    return rx_sub(rv, rx_sub(rg, s, "1'b0", NULL, NULL), "1'b1", NULL, NULL);
}
static int locnum(const struct inst *x) { return atoi(x->loc + 2); }
static const char *pget(const struct inst *x, const char *n) { const char *v = vo_port(x, n); return v ? v : ""; }
static const char *par_or(const struct inst *x, const char *n, const char *d) { const char *v = vo_par(x, n); return v ? v : d; }
static void items(const char *v, slist *out) {
    char *s = py_strip_chars(v, "{}", 1, 1), *p = s;
    for (;;) { char *c = strchr(p, ','); sl_push_own(out, py_strip(c ? xstrndup(p, (size_t)(c - p)) : p)); if (!c) break; p = c + 1; }
}
static int all_eq(const slist *l, const char *v) { for (int i = 0; i < l->n; i++) if (strcmp(l->v[i], v)) return 0; return 1; }
static int has(const slist *l, const char *v) { return sl_find(l, v) >= 0; }

char *pinname(int c) { return c >= 1000 ? fmt("%c%d", 64 + c / 1000, c % 1000) : fmt("%d", c); }

void max_emit(const struct decoded *X, const char *top, const char *device, char **vout, char **qout) {
    const struct maxctx *d = X->d; const struct maxdev *D = d->D; int nmc = D->n_mc;
    struct netlist N; vo_parse(max_to_netlist(X), &N);
    ilist used = { 0 }, inputs = { 0 }, bidir = { 0 }, outs = { 0 };
    for (int i = 0; i < X->used.n; i++) if (startswith(X->used.v[i], "pin")) il_push(&used, atoi(X->used.v[i] + 3));
    il_uniq(&used);
    for (int i = 0; i < used.n; i++) il_push(max_io_at(X, used.v[i]) ? &bidir : &inputs, used.v[i]);
    for (int q = 0; q < X->n_ios; q++) { int p = X->ios[q].pin; if (il_find(&bidir, p) < 0 && !X->ios[q].clamp) il_push(&outs, p); }
    il_uniq(&outs);
    char *dead = xcalloc((size_t)nmc + 2, 1), *nb = xcalloc((size_t)nmc + 2, 1), *empty = xcalloc((size_t)nmc + 2, 1);
    for (int n = 1; n <= nmc; n++) {
        const struct cell *c = &X->cells[n]; int cs = c->st[S_CLOCK_SELECT];
        if (X->live[n] && c->reg && !(cs == 0 || cs == 1 || cs == 2) && !c->donor) dead[n] = 1;
    }
    for (int n = 1; n <= nmc; n++)
        if (dead[n] && X->cells[n].pu == 1 && !(X->cells[n].st[S_CLR] && X->cells[n].rows[2].st == 2 && X->cells[n].rows[2].l.n == 0))
            refuse("EMIT_UNCLOCKED_HIGH", "LC%d", n);
    int jt = X->jtag;
    int free_off = 0;
    for (int i = 0; i < d->n_io; i++) {
        int p = d->io_val[i];
        if (!max_io_at(X, p) && il_find(&used, p) < 0 && !(jt && (p == d->jtag_pins[0] || p == d->jtag_pins[1] || p == d->jtag_pins[2] || p == d->jtag_pins[3]))) free_off = 1;
    }
    for (int i = 0; i < N.ni; i++) {
        const struct inst *x = &N.in[i];
        if (!strcmp(x->kind, "max_mcell") && !strcmp(par_or(x, "pexp_mode", ""), "on") && !strcmp(par_or(x, "output_mode", ""), "comb") && !strcmp(par_or(x, "operation_mode", ""), "vcc"))
            nb[locnum(x)] = 1;
    }
    /* empty donors, in location order: a donor whose terms are all constant 0 and whose expander input is nothing */
    static rx *rbang;
    if (!rbang) rbang = rx_compile("[\\\\!]", 0);
    for (int n = 1; n <= nmc; n++)
        for (int i = 0; i < N.ni; i++) {
            const struct inst *x = &N.in[i];
            if (strcmp(x->kind, "max_mcell") || locnum(x) != n) continue;
            int zero = 1; rxm m;
            for (int k = 0; k < 5; k++) {
                const char *v = vo_port(x, fmt("pterm%d", k));
                if (!v) fatal("netlist: LC%d has no pterm%d", n, k);
                if (strstr(v, "vcc") || rx_search(rbang, v, 0, &m)) zero = 0;
            }
            char *pin = py_strip(pget(x, "pexpin"));
            if (!strcmp(par_or(x, "pexp_mode", ""), "on") && zero && (!strcmp(pin, "gnd") || !*pin || (n > 1 && empty[n - 1]))) empty[n] = 1;
        }
    ilist raw = { 0 };
    for (int i = 0; i < outs.n; i++) {
        const struct io *io = max_io_at(X, outs.v[i]);
        if (io->lc && (dead[io->lc] || nb[io->lc]) && !strcmp(io->oe, "vcc") && !io->od) il_push(&raw, outs.v[i]);
    }
    if (!free_off) { ilist o2 = { 0 }; for (int i = 0; i < outs.n; i++) if (il_find(&raw, outs.v[i]) < 0) il_push(&o2, outs.v[i]); outs = o2; }
    /* donors whose own output is constant */
    int *dconst = xmalloc(sizeof(int) * (size_t)(nmc + 2));
    for (int n = 0; n <= nmc + 1; n++) dconst[n] = -1;
    for (int i = 0; i < N.ni; i++) {
        const struct inst *x = &N.in[i];
        if (strcmp(x->kind, "max_mcell") || strcmp(par_or(x, "pexp_mode", ""), "on") || strcmp(par_or(x, "output_mode", ""), "comb")) continue;
        const char *opm = par_or(x, "operation_mode", "");
        if (strcmp(opm, "normal") && strcmp(opm, "invert")) continue;
        const char *p5 = vo_port(x, "pterm5"); if (!p5 || !*p5) p5 = "{gnd}";
        slist t5 = { 0 }; items(py_strip(p5), &t5);
        int c_ = has(&t5, "gnd") ? 0 : all_eq(&t5, "vcc") ? 1 : -1;
        if (c_ >= 0) dconst[locnum(x)] = !strcmp(opm, "normal") ? c_ : 1 - c_;
    }
    for (int i = 0; i < N.ni; i++) {
        const struct inst *x = &N.in[i];
        if (strcmp(x->kind, "max_mcell") || dconst[locnum(x)] >= 0) continue;
        for (int j = 0; j < x->np; j++) {
            if (!strcmp(x->p[j].name, "dataout") || !strcmp(x->p[j].name, "pexpout")) continue;
            for (int n = 1; n <= nmc; n++) {
                if (dconst[n] < 0) continue;
                rx *r = rx_compile(fmt("\\\\lc%d_out\\b", n), 0); rxm m;
                if (rx_search(r, x->p[j].val, 0, &m)) refuse("EMIT_DONOR_CONSTANT_READ", "%s reads a constant donor output", x->name);
            }
        }
    }
    struct race R; race_analyse(&N, &R);
    slist L = { 0 };
    sl_push_own(&L, fmt("// %s for %s, emitted by %s from a decoded fuse map; credit: %s", top, device, TOOL_VER, CREDIT));
    sl_push_own(&L, R.n ? fmt("// race hazard, clock and next state share a signal, behaviour at that instant depends on device timing: %s", race_report(&R, 1))
                        : xstrdup("// race hazard: none found"));
    ilist all = { 0 };
    for (int i = 0; i < inputs.n; i++) il_push(&all, inputs.v[i]);
    for (int i = 0; i < outs.n; i++) il_push(&all, outs.v[i]);
    for (int i = 0; i < bidir.n; i++) il_push(&all, bidir.v[i]);
    sbuf h; sb_init(&h);
    for (int i = 0; i < all.n; i++) sb_printf(&h, i ? ", p%d" : "p%d", all.v[i]);
    sl_push_own(&L, fmt("module %s (%s);", top, sb_take(&h)));
    for (int i = 0; i < inputs.n; i++) sl_push_own(&L, fmt("input p%d;", inputs.v[i]));
    for (int i = 0; i < outs.n; i++) sl_push_own(&L, fmt("output p%d;", outs.v[i]));
    for (int i = 0; i < bidir.n; i++) sl_push_own(&L, fmt("inout p%d;", bidir.v[i]));
    for (int i = 0; i < inputs.n; i++) sl_push_own(&L, fmt("wire \\pin%d ;", inputs.v[i]));
    for (int i = 0; i < bidir.n; i++) sl_push_own(&L, fmt("wire \\pin%d ;", bidir.v[i]));
    for (int n = 1; n <= nmc; n++) if (X->live[n]) sl_push_own(&L, fmt("wire \\lc%d_out ;", n));
    for (int n = 1; n <= nmc; n++) if (X->live_sexp[n]) sl_push_own(&L, fmt("wire \\sexp%d_out ;", n));
    for (int n = 1; n <= nmc; n++) if (X->live[n] && X->cells[n].donor) sl_push_own(&L, fmt("wire \\lc%d_pexp ;", n));
    for (int i = 0; i < inputs.n; i++) sl_push_own(&L, fmt("assign \\pin%d  = p%d;", inputs.v[i], inputs.v[i]));
    for (int i = 0; i < bidir.n; i++) sl_push_own(&L, fmt("assign \\pin%d  = p%d;", bidir.v[i], bidir.v[i]));
    for (int g = 0; g < 2; g++) {
        ilist *l = g ? &bidir : &outs;
        for (int i = 0; i < l->n; i++) {
            int p = l->v[i]; const struct io *io = max_io_at(X, p);
            if (!io->lc) fatal("pin %d: output with no macrocell", p);
            char *val = dconst[io->lc] < 0 ? fmt("\\lc%d_out ", io->lc) : fmt("1'b%d", dconst[io->lc]);
            char *oe = !strcmp(io->oe, "vcc") ? xstrdup("1'b1") : max_oe_text(io);
            if (io->od) sl_push_own(&L, fmt("assign p%d = (%s & !%s) ? 1'b0 : 1'bz;", p, oe, val));
            else if (!strcmp(io->oe, "vcc")) sl_push_own(&L, fmt("assign p%d = %s;", p, val));
            else sl_push_own(&L, fmt("assign p%d = %s ? %s : 1'bz;", p, oe, val));
        }
    }
    slist insts = { 0 };
    for (int i = 0; i < N.ni; i++) {
        const struct inst *x = &N.in[i];
        if (!strcmp(x->kind, "max_sexp")) {
            sl_push_own(&insts, fmt("max_sexp %s  (.datain(%s), .dataout(%s));", x->name, c0(pget(x, "datain")), pget(x, "dataout")));
            continue;
        }
        if (strcmp(x->kind, "max_mcell")) continue;
        int n = locnum(x);
        if (dead[n]) { sl_push_own(&L, fmt("assign \\lc%d_out  = 1'b0;  // raw unused macrocell, clear constant true", n)); continue; }
        if (nb[n]) sl_push_own(&L, fmt("assign \\lc%d_out  = 1'b1;  // expander donor, no output", n));
        if (empty[n] && nb[n]) continue;
        slist b = { 0 };
        char *fp = py_strip(pget(x, "fpin"));
        int fast = *fp && strcmp(fp, "vcc") && strcmp(fp, "gnd");
        for (int j = 0; j < x->np; j++) {
            const char *pn = x->p[j].name, *val = x->p[j].val;
            if (fast && (startswith(pn, "pterm") || startswith(pn, "pxor"))) continue;
            if (!strcmp(pn, "dataout") && nb[n]) continue;
            if (!strcmp(pn, "pexpin") && n > 1 && empty[n - 1]) continue;
            if (!strcmp(pn, "pexpout") && empty[n]) continue;
            char *v = py_strip(val);
            slist it = { 0 }; items(v, &it);
            if (!*v || (!strcmp(pn, "fpin") && (!strcmp(v, "vcc") || !strcmp(v, "gnd"))) ||
                ((!strcmp(pn, "clk") || !strcmp(pn, "aclr") || !strcmp(pn, "pexpin")) && !strcmp(v, "gnd"))) continue;
            /* a bus whose distinct items are exactly {gnd} / {vcc} */
            int only_gnd = 1, only_vcc = 1;
            for (int q = 0; q < it.n; q++) { if (strcmp(it.v[q], "gnd")) only_gnd = 0; if (strcmp(it.v[q], "vcc")) only_vcc = 0; }
            if ((startswith(pn, "pterm") || startswith(pn, "pxor") || startswith(pn, "pclk") || startswith(pn, "paclr") || startswith(pn, "papre")) && only_gnd) continue;
            if (!strcmp(pn, "pena") && only_vcc) continue;
            sl_push_own(&b, fmt(".%s(%s)", pn, c0(val)));
        }
        int anyp = 0;
        for (int q = 0; q < b.n; q++) if (startswith(b.v[q], ".pterm") || startswith(b.v[q], ".pexpin")) anyp = 1;
        if (!fast && !anyp) {
            slist b2 = { 0 }; sl_push_own(&b2, fmt(".pterm0(%s)", c0(pget(x, "pterm0"))));
            for (int q = 0; q < b.n; q++) sl_push(&b2, b.v[q]);
            b = b2;
        }
        sl_push_own(&insts, fmt("max_mcell %s  (%s);", x->name, sl_join(&b, ", ")));
        for (int q = 0; q < x->npar; q++)
            sl_push_own(&insts, fmt("defparam %s .%s = \"%s\";", x->name, x->par[q].name, (!strcmp(x->par[q].name, "pexp_mode") && empty[n]) ? "off" : x->par[q].val));
    }
    for (int i = 0; i < insts.n; i++) sl_push(&L, insts.v[i]);
    sl_push(&L, "endmodule");
    *vout = fmt("%s\n", sl_join(&L, "\n"));
    /* the .qsf */
    slist Q = { 0 };
    sl_push_own(&Q, fmt("set_global_assignment -name FAMILY \"%s\"", !strcmp(D->fam, "max7000ae") ? "MAX7000AE" : !strcmp(D->fam, "max7000b") ? "MAX7000B" : "MAX7000S"));
    sl_push_own(&Q, fmt("set_global_assignment -name DEVICE %s", device));
    sl_push_own(&Q, fmt("set_global_assignment -name TOP_LEVEL_ENTITY %s", top));
    sl_push_own(&Q, fmt("set_global_assignment -name VERILOG_FILE %s.v", top));
    sl_push_own(&Q, fmt("set_global_assignment -name MAX7000_ENABLE_JTAG_BST_SUPPORT %s", X->jtag ? "ON" : "OFF"));
    sl_push(&Q, "set_global_assignment -name AUTO_OPEN_DRAIN_PINS ON");
    sl_push_own(&Q, fmt("set_global_assignment -name RESERVE_ALL_UNUSED_PINS \"%s\"", free_off ? "AS INPUT TRI-STATED" : "AS OUTPUT DRIVING GROUND"));
    for (int i = 0; i < all.n; i++) sl_push_own(&Q, fmt("set_location_assignment PIN_%s -to p%d", pinname(all.v[i]), all.v[i]));
    if (X->n_attrs) {
        for (int i = 0; i < all.n; i++) {
            const struct attr *a = max_attr_at(X, all.v[i]);
            if (a && a->bus_hold) sl_push_own(&Q, fmt("set_instance_assignment -name ENABLE_BUS_HOLD_CIRCUITRY ON -to p%d", all.v[i]));
            if (a && a->weak_pull_up) sl_push_own(&Q, fmt("set_instance_assignment -name WEAK_PULL_UP_RESISTOR ON -to p%d", all.v[i]));
        }
        int kinds[4] = { 0, 0, 0, 0 }, nk = 0;     /* (bus hold, pull-up) pairs seen on the undeclared pins */
        for (int i = 0; i < d->n_io; i++) {
            int p = d->io_val[i];
            if (il_find(&all, p) >= 0) continue;
            const struct attr *a = max_attr_at(X, p);
            if (!a) continue;
            if (jt && (p == d->jtag_pins[0] || p == d->jtag_pins[1] || p == d->jtag_pins[2] || p == d->jtag_pins[3])) continue;
            int k = a->bus_hold * 2 + a->weak_pull_up;
            if (!kinds[k]) { kinds[k] = 1; nk++; }
        }
        if (nk && !(nk == 1 && kinds[0])) {
            if (nk > 1) {
                static const char *T[4] = { "(False, False)", "(False, True)", "(True, False)", "(True, True)" };
                sbuf s; sb_init(&s); int f = 1;
                for (int k = 0; k < 4; k++) if (kinds[k]) { sb_printf(&s, f ? "%s" : ", %s", T[k]); f = 0; }
                refuse("EMIT_RESERVE_ATTR_MIXED", "undeclared pins carry [%s]", sb_take(&s));
            }
            int k = kinds[1] ? 1 : kinds[2] ? 2 : 3;
            if (k == 3) refuse("EMIT_RESERVE_ATTR_MIXED", "undeclared pins with bus hold and pull-up");
            slist Q2 = { 0 };
            for (int i = 0; i < Q.n; i++) if (!strstr(Q.v[i], "RESERVE_ALL_UNUSED_PINS")) sl_push(&Q2, Q.v[i]);
            sl_push_own(&Q2, fmt("set_global_assignment -name RESERVE_ALL_UNUSED_PINS %s", k == 2 ? "AS_INPUT_TRI_STATED_WITH_BUS_HOLD" : "AS_INPUT_TRI_STATED_WITH_WEAK_PULL_UP"));
            Q = Q2;
        }
    }
    for (int n = 1; n <= nmc; n++) if (X->live[n] && !dead[n] && !(empty[n] && nb[n])) sl_push_own(&Q, fmt("set_location_assignment LC%d -to lc%d", n, n));
    *qout = fmt("# %s, credit: %s\n%s\n", TOOL_VER, CREDIT, sl_join(&Q, "\n"));
}

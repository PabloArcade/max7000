/* maxnet.c -- the decoded MAX design as an atom netlist (max_io / max_mcell / max_sexp with "// Location:" comments),
 * and the text dump of the decoded state used for debugging.
 *
 * A product-term bus is 52 entries wide: unused entries are vcc, a constant-0 term is all gnd. Net names: \pinN (pin N),
 * \lcN_out (macrocell N), \sexpN_out (shared expander N), \lcN_pexp (parallel expander from LC N to LC N+1).
 */
#include "max.h"
#include <stdlib.h>
#include <string.h>

static char *bus(const struct row *r) {   /* r == NULL: constant 0 */
    sbuf b; sb_init(&b); sb_addc(&b, '{');
    if (!r || r->st != 2) { for (int i = 0; i < 52; i++) sb_add(&b, i ? ",gnd" : "gnd"); }
    else {
        if (r->l.n > 52) refuse("PT_TOO_WIDE", "%d literals", r->l.n);
        int k = 0;
        for (int i = 0; i < 52 - r->l.n; i++, k++) sb_add(&b, k ? ",vcc" : "vcc");
        for (int i = 0; i < r->l.n; i++, k++) {
            const char *l = r->l.v[i];
            if (k) sb_addc(&b, ',');
            if (l[0] == '!') sb_printf(&b, "!\\%s ", l + 1); else sb_printf(&b, "\\%s ", l);
        }
    }
    sb_addc(&b, '}');
    return sb_take(&b);
}
static const struct row EMPTY_ROW = { 2, { 0 } };     /* a product term with no literal: constant 1 */

char *max_oe_text(const struct io *io) {
    if (!strcmp(io->oe, "vcc")) return xstrdup("vcc");
    const char *s = io->oe; int neg = 0;
    if (*s == '!') neg = 1;
    while (*s == '!') s++;
    return fmt("%s\\%s ", neg ? "!" : "", s);
}

static void io_par(slist *L, const char *inst, const char *mode, const struct io *io, const struct attr *a) {
    sl_push_own(L, fmt("defparam %s.bus_hold = \"%s\";", inst, a && a->bus_hold ? "true" : "false"));
    sl_push_own(L, fmt("defparam %s.open_drain_output = \"%s\";", inst, io && io->od ? "true" : "false"));
    sl_push_own(L, fmt("defparam %s.operation_mode = \"%s\";", inst, mode));
    sl_push_own(L, fmt("defparam %s.weak_pull_up = \"%s\";", inst, a && a->weak_pull_up ? "true" : "false"));
}

static const struct row *R(const struct cell *c, int p) { return c->rows[p].st ? &c->rows[p] : NULL; }   /* not decoded = constant 0 */

static void mcell(const struct decoded *X, int n, slist *L) {
    const struct maxctx *d = X->d; const struct maxdev *D = d->D; const struct cell *c = &X->cells[n]; const int *st = c->st;
    const struct row *sum[5] = { st[S_INV_SEXP] ? NULL : R(c, 0), st[S_INV_XOR] ? NULL : R(c, 1), st[S_CLR] ? NULL : R(c, 2),
                                 st[S_CLK_CE] ? NULL : R(c, 3), st[S_PRE] ? NULL : R(c, 4) };
    const struct row *pxor = st[S_INV_XOR] ? R(c, 1) : NULL, *paclr = st[S_CLR] ? R(c, 2) : NULL, *papre = st[S_PRE] ? R(c, 4) : NULL;
    int cs = st[S_CLOCK_SELECT];
    const struct row *pclk = NULL, *pena = &EMPTY_ROW, *own5 = NULL;
    char *clk = xstrdup("gnd");
    if (cs == 0) {
        if (!st[S_CLK_CE]) refuse("PT_CLOCK_WITHOUT_PT", "LC%d", n);
        pclk = R(c, 3);
    } else if (cs == 1 || cs == 2) {
        int g = -1;
        for (int i = 0; i < X->n_gclk; i++) if (X->gclk_net[i] == cs) g = i;
        if (g < 0) fatal("clock network %d not configured", cs);
        clk = fmt("%s\\pin%d ", X->gclk_inv[g] ? "!" : "", max_ib_pin(d, X->gclk_ib[g]));
        pena = st[S_CLK_CE] ? R(c, 3) : &EMPTY_ROW;
    }
    int oi = st[S_OR_INV], reg = c->reg;
    const char *opm, *rmode, *pexp;
    static const char *OPM[] = { "vcc", "normal", "invert", "xor" };
    if (c->donor) {
        int clocked = cs == 0 || cs == 1 || cs == 2, k = -1;
        for (int i = 0; i < D->n_donor; i++) {
            const int *t = &D->donor[6 * i];
            if (t[0] == oi && t[1] == st[S_REG_OUT] && t[2] == clocked) k = i;
        }
        if (k < 0) refuse("PEXP_DONOR_MODE_UNWITNESSED", "LC%d OR_OUTPUT_INVERT %s REG_OUT raw %s", n, sv_str(oi), sv_str(st[S_REG_OUT]));
        const int *t = &D->donor[6 * k];
        opm = OPM[t[3]]; reg = t[4] == 1; rmode = t[5] ? "tff" : "dff"; pexp = "on";
        own5 = (t[3] == OPM_NORMAL || t[3] == OPM_INVERT) ? pxor : NULL;   /* the donor's own data: the term steered as XOR operand */
        pxor = NULL;
    } else {
        pexp = "off";
        if (oi == 0) { opm = "normal"; rmode = "dff"; }
        else if (oi == 1) { opm = st[S_INV_XOR] ? "xor" : "invert"; rmode = "dff"; }
        else if (oi == 3) { opm = "invert"; rmode = "tff"; }
        else { opm = "normal"; rmode = "tff"; }
    }
    char *pexpin = (n > 1 && X->cells[n - 1].donor) ? fmt("\\lc%d_pexp ", n - 1) : xstrdup("gnd");
    char *inst = fmt("\\lc%d ", n);
    if (c->fpin) {
        opm = "vcc";                                   /* register data from the fast input pin */
        if (st[S_INV_XOR] && !c->donor) { sum[1] = R(c, 1); pxor = NULL; }
    }
    slist P = { 0 };
    sl_push_own(&P, fmt(".clk(%s)", clk));
    sl_push_own(&P, st[S_CLEAR_SELECT] == 1 ? fmt(".aclr(!\\pin%d )", max_ib_pin(d, 2)) : xstrdup(".aclr(gnd)"));
    sl_push_own(&P, fmt(".pexpin(%s)", pexpin));
    sl_push_own(&P, c->fpin ? fmt(".fpin(\\pin%d )", c->fpin) : xstrdup(".fpin(vcc)"));
    for (int k = 0; k < 5; k++) sl_push_own(&P, fmt(".pterm%d(%s)", k, bus(sum[k])));
    sl_push_own(&P, fmt(".pterm5(%s)", bus(c->donor ? own5 : NULL)));
    sl_push_own(&P, fmt(".pxor(%s)", bus(pxor)));
    sl_push_own(&P, fmt(".pclk(%s)", bus(pclk)));
    sl_push_own(&P, fmt(".pena(%s)", bus(pena)));
    sl_push_own(&P, fmt(".paclr(%s)", bus(paclr)));
    sl_push_own(&P, fmt(".papre(%s)", bus(papre)));
    sl_push_own(&P, fmt(".dataout(\\lc%d_out )", n));
    sl_push_own(&P, c->donor ? fmt(".pexpout(\\lc%d_pexp )", n) : xstrdup(".pexpout()"));
    sl_push_own(L, fmt("// Location: LC%d", n));
    sl_push_own(L, fmt("max_mcell %s(", inst));
    for (int i = 0; i < P.n; i++) sl_push_own(L, fmt("\t%s%s", P.v[i], i + 1 < P.n ? "," : ");"));
    sl_push_own(L, fmt("defparam %s.operation_mode = \"%s\";", inst, opm));
    sl_push_own(L, fmt("defparam %s.output_mode = \"%s\";", inst, reg ? "reg" : "comb"));
    sl_push_own(L, fmt("defparam %s.pexp_mode = \"%s\";", inst, pexp));
    sl_push_own(L, fmt("defparam %s.power_up = \"%s\";", inst, c->pu == 1 ? "high" : "low"));
    sl_push_own(L, fmt("defparam %s.register_mode = \"%s\";", inst, rmode));
}

static int pins_used(const struct decoded *X, ilist *ins, ilist *bid, ilist *outs) {
    ilist all = { 0 };
    for (int i = 0; i < X->used.n; i++) if (startswith(X->used.v[i], "pin")) il_push(&all, atoi(X->used.v[i] + 3));
    il_uniq(&all);
    for (int i = 0; i < all.n; i++) il_push(max_io_at(X, all.v[i]) ? bid : ins, all.v[i]);
    ilist io = { 0 };
    for (int q = 0; q < X->n_ios; q++) il_push(&io, X->ios[q].pin);
    il_uniq(&io);
    for (int i = 0; i < io.n; i++) if (il_find(bid, io.v[i]) < 0) il_push(outs, io.v[i]);
    return 0;
}

char *max_to_netlist(const struct decoded *X) {
    const struct maxctx *d = X->d; const struct maxdev *D = d->D;
    ilist ins = { 0 }, bid = { 0 }, outs = { 0 };
    pins_used(X, &ins, &bid, &outs);
    slist L = { 0 };
    if (X->n_attrs) {
        ilist ib = { 0 };
        for (int i = 0; i < d->n_ib; i++) il_push(&ib, d->ib_val[i]);
        il_sort(&ib);
        sbuf b; sb_init(&b);
        for (int i = 0; i < ib.n; i++) sb_printf(&b, i ? ",%d" : "%d", ib.v[i]);
        sl_push_own(&L, fmt("// max7k_native dedicated input pins: %s", sb_take(&b)));
    }
    sbuf h; sb_init(&h); int k = 0;
    ilist *grp[3] = { &ins, &outs, &bid };
    for (int g = 0; g < 3; g++) for (int i = 0; i < grp[g]->n; i++) sb_printf(&h, k++ ? ", p%d" : "p%d", grp[g]->v[i]);
    sl_push_own(&L, fmt("module dec (%s);", sb_take(&h)));
    for (int i = 0; i < ins.n; i++) sl_push_own(&L, fmt("input p%d;", ins.v[i]));
    for (int i = 0; i < outs.n; i++) sl_push_own(&L, fmt("output p%d;", outs.v[i]));
    for (int i = 0; i < bid.n; i++) sl_push_own(&L, fmt("inout p%d;", bid.v[i]));
    sl_push(&L, "wire gnd;"); sl_push(&L, "wire vcc;"); sl_push(&L, "assign gnd = 1'b0;"); sl_push(&L, "assign vcc = 1'b1;");
    for (int i = 0; i < ins.n; i++) sl_push_own(&L, fmt("wire \\pin%d ;", ins.v[i]));
    for (int i = 0; i < bid.n; i++) sl_push_own(&L, fmt("wire \\pin%d ;", bid.v[i]));
    for (int n = 1; n <= D->n_mc; n++) if (X->live[n]) sl_push_own(&L, fmt("wire \\lc%d_out ;", n));
    for (int n = 1; n <= D->n_mc; n++) if (X->live_sexp[n]) sl_push_own(&L, fmt("wire \\sexp%d_out ;", n));
    for (int n = 1; n <= D->n_mc; n++) if (X->live[n] && X->cells[n].donor) sl_push_own(&L, fmt("wire \\lc%d_pexp ;", n));
    for (int g = 0; g < 2; g++) {
        ilist *l = g ? &bid : &ins;
        for (int i = 0; i < l->n; i++) {
            int p = l->v[i]; const struct io *io = max_io_at(X, p);
            if (io && !io->lc) fatal("pin %d: an input read from a clamped output", p);
            sl_push_own(&L, fmt("// Location: PIN_%d", p));
            sl_push_own(&L, fmt("max_io \\pin%d~I  (", p));
            sl_push_own(&L, io ? fmt("\t.datain(\\lc%d_out ),", io->lc) : xstrdup("\t.datain(gnd),"));
            sl_push_own(&L, fmt("\t.oe(%s),", io ? max_oe_text(io) : "gnd"));
            sl_push_own(&L, fmt("\t.dataout(\\pin%d ),", p));
            sl_push_own(&L, fmt("\t.padio(p%d));", p));
            io_par(&L, fmt("\\pin%d~I ", p), g ? "bidir" : "input", io, max_attr_at(X, p));
        }
    }
    for (int i = 0; i < outs.n; i++) {
        int p = outs.v[i]; const struct io *io = max_io_at(X, p);
        sl_push_own(&L, fmt("// Location: PIN_%d", p));
        sl_push_own(&L, fmt("max_io \\pin%d~O  (", p));
        sl_push_own(&L, io->lc ? fmt("\t.datain(\\lc%d_out ),", io->lc) : xstrdup("\t.datain(gnd),"));
        sl_push_own(&L, fmt("\t.oe(%s),", max_oe_text(io)));
        sl_push(&L, "\t.dataout(),");
        sl_push_own(&L, fmt("\t.padio(p%d));", p));
        io_par(&L, fmt("\\pin%d~O ", p), "output", io, max_attr_at(X, p));
    }
    for (int n = 1; n <= D->n_mc; n++) {
        if (!X->live_sexp[n]) continue;
        sl_push_own(&L, fmt("// Location: SEXP%d", n));
        sl_push_own(&L, fmt("max_sexp \\sexp%d  (", n));
        sl_push_own(&L, fmt("\t.datain(%s),", bus(R(&X->cells[n], 0))));
        sl_push_own(&L, fmt("\t.dataout(\\sexp%d_out ));", n));
    }
    for (int n = 1; n <= D->n_mc; n++) if (X->live[n]) mcell(X, n, &L);
    sl_push(&L, "endmodule");
    return fmt("%s\n", sl_join(&L, "\n"));
}

static const char *pu_s(int pu) { return pu == 1 ? "high" : pu == 0 ? "low" : "None"; }

char *max_dump(const struct decoded *X) {
    const struct maxctx *d = X->d; const struct maxdev *D = d->D;
    sbuf b; sb_init(&b);
    sb_printf(&b, "kind MAX\npart %s %s\ngcc %d %d %d\njtag %d\ngclk", d->dev, d->pkg, X->gcc[0], X->gcc[1], X->gcc[2], X->jtag);
    for (int i = 0; i < X->n_gclk; i++) sb_printf(&b, " %d:%d,%d", X->gclk_net[i], X->gclk_ib[i], X->gclk_inv[i]);
    sb_add(&b, "\n");
    for (int n = 1; n <= D->n_mc; n++) {
        if (!X->live[n]) continue;
        const struct cell *c = &X->cells[n];
        sb_printf(&b, "cell %d st", n);
        for (int k = 0; k < S_N; k++) sb_printf(&b, " %s", sv_str(c->st[k]));
        sb_printf(&b, " reg %d donor %d pu %s fpin %d rows", c->reg, c->donor, pu_s(c->pu), c->fpin);
        for (int p = 0; p < 5; p++) {
            if (c->rows[p].st == 0) sb_add(&b, " N");
            else if (c->rows[p].st == 1) sb_add(&b, " G");
            else sb_printf(&b, " L%d", c->rows[p].l.n);
        }
        sb_add(&b, "\n");
    }
    for (int q = 0; q < X->n_ios; q++) {
        const struct io *io = &X->ios[q];
        sb_printf(&b, "io %d lc %d oe %s od %d clamp %s clamp_lc %d\n", io->pin, io->lc, io->oe, io->od, io->has_clamp ? (io->clamp ? "1" : "0") : "-", io->clamp_lc);
    }
    for (int i = 0; i < X->n_attrs; i++) {
        const struct attr *a = &X->attrs[i];
        sb_printf(&b, "attr %d %d %d %d %d\n", a->pin, a->bus_hold, a->weak_pull_up, a->iost, a->delay);
    }
    sb_add(&b, "sexp");
    for (int n = 1; n <= D->n_mc; n++) if (X->live_sexp[n]) sb_printf(&b, " %d", n);
    slist u = { 0 };
    for (int i = 0; i < X->used.n; i++) sl_push(&u, X->used.v[i]);
    sl_sort(&u);
    sb_printf(&b, "\nused %s\nnetlist:\n", sl_join(&u, " "));
    struct catcher c;
    if (TRY(c)) { sb_add(&b, max_to_netlist(X)); ENDTRY(c); }
    else { ENDTRY(c); sb_printf(&b, "REFUSED %s: %s\n", refusal_name, refusal_detail); }
    return sb_take(&b);
}

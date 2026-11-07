/* atfdec.c -- ATF150x fuse map -> decoded netlist (see atf.h).
 *
 * Fuses outside every field of the map must hold the constant every vendor-written file holds; any other value is
 * refused. Global output enable n: its invert fuse is at goe_inv0 + n - 1 (1 = the source, 0 = the inverted source).
 * Cascade: a macrocell whose cas mux is "sum" sends its sum (with its own cascade input) to the next macrocell of the
 * same block. Latch storage is a level-sensitive latch, transparent while the clock net is 1.
 */
#include "atf.h"
#include "jedec.h"
#include "rx.h"
#include <stdlib.h>
#include <string.h>

const struct atfdev *atf_find(const char *dev) {
    for (int i = 0; i < n_atf_devices; i++) if (!strcmp(atf_devices[i].name, dev)) return &atf_devices[i];
    return NULL;
}
char *atf_padname(const struct padname *p) { return p->num >= 0 ? fmt("%s%d", p->pre, p->num) : xstrdup(p->pre); }
int atf_pin(const struct anet *N, const char *pad) {
    for (int i = 0; i < N->K->n; i++) if (!strcmp(atf_padname(&N->K->pins[i].pad), pad)) return N->K->pins[i].pin;
    return 0;
}
static char *names_sorted(const struct afield *f) {
    slist s = { 0 };
    for (int i = 0; i < f->n_vals; i++) sl_push(&s, f->vals[i].name);
    sl_sort(&s);
    sbuf b; sb_init(&b); sb_addc(&b, '[');
    for (int i = 0; i < s.n; i++) { if (i) sb_add(&b, ", "); sb_add(&b, py_repr_str(s.v[i])); }
    sb_addc(&b, ']');
    return sb_take(&b);
}
char *atf_opt(const unsigned char *F, const struct afield *f, const char *where) {
    int v = 0;
    for (int n = 0; n < f->n_fuses; n++) v += F[f->fuses[n]] << n;
    for (int i = 0; i < f->n_vals; i++) if (f->vals[i].code == v) return xstrdup(f->vals[i].name);
    refuse("OPTION_VALUE_UNKNOWN", "%s: code %d not in %s", where, v, names_sorted(f));
    return NULL;
}
const char *atf_wire(const struct anet *N, const char *name) {
    const char *r = NULL;
    for (int i = 0; i < N->nw; i++) if (!strcmp(N->w[i].name, name)) r = N->w[i].val;
    return r;
}
const struct aout *atf_output(const struct anet *N, const char *pad) {
    for (int i = 0; i < N->no; i++) if (!strcmp(N->out[i].pad, pad)) return &N->out[i];
    return NULL;
}
const char *atf_info_opt(const struct ainfo *I, const char *name) {
    for (int i = 0; i < I->n_o; i++) if (!strcmp(I->o[i].name, name)) return I->o[i].val;
    fatal("macrocell %s has no field %s", I->mc, name);
    return NULL;
}

typedef struct { int n, cap; struct named *v; } nlist;
static void nl_push(nlist *l, const char *a, const char *b) {
    if (l->n == l->cap) { int c = l->cap ? l->cap * 2 : 64; struct named *v = xmalloc(sizeof *v * (size_t)c); if (l->n) memcpy(v, l->v, sizeof *v * (size_t)l->n); l->v = v; l->cap = c; }
    l->v[l->n].name = xstrdup(a); l->v[l->n].val = xstrdup(b); l->n++;
}
static int nl_find(const nlist *l, const char *a) { for (int i = 0; i < l->n; i++) if (!strcmp(l->v[i].name, a)) return i; return -1; }
static void nl_set(nlist *l, const char *a, const char *b) {   /* dict assignment: keeps the first position */
    int i = nl_find(l, a);
    if (i >= 0) l->v[i].val = xstrdup(b); else nl_push(l, a, b);
}

typedef struct { struct anet *N; slist jtag_pads; } actx;
static char *padsig(actx *C, const char *pad) {
    if (sl_find(&C->jtag_pads, pad) >= 0) refuse("JTAG_PAD_USED", "%s is a JTAG pin (jtag_pin_func = jtag)", pad);
    if (!atf_pin(C->N, pad)) refuse("PAD_NOT_BONDED", "%s has no pin in %s", pad, C->N->pkg);
    if (sl_find(&C->N->inputs, pad) < 0) sl_push(&C->N->inputs, pad);
    return fmt("pad_%s", pad);
}
static char *source(actx *C, const char *name, const char *where) {
    static rx *rfb, *rpad;
    if (!rfb) { rfb = rx_compile("(MC\\d+)_FB$", 0); rpad = rx_compile("(\\w+)_PAD$", 0); }
    rxm m;
    if (startswith(name, "GND")) return xstrdup("1'b0");
    if (rx_match(rfb, name, &m)) return fmt("fb_%s", rx_group(&m, 1));
    if (rx_match(rpad, name, &m)) return padsig(C, rx_group(&m, 1));
    refuse("SOURCE_NOT_MODELLED", "%s: %s", where, name);
    return NULL;
}
static const struct amc *find_mc(const struct atfdev *D, int num) { for (int i = 0; i < D->n_mcs; i++) if (D->mcs[i].num == num) return &D->mcs[i]; return NULL; }
static const struct ablk *find_blk(const struct atfdev *D, const char *b) { for (int i = 0; i < D->n_blks; i++) if (!strcmp(D->blks[i].name, b)) return &D->blks[i]; fatal("block %s missing", b); return NULL; }
static int cmp_mc(const void *a, const void *b) { return (*(const struct amc *const *)a)->num - (*(const struct amc *const *)b)->num; }
static const char *ptx(const struct ainfo *I, const char *nm) {   /* the wire name of a product term of this macrocell */
    for (int k = 0; k < I->n_pt; k++) if (!strcmp(I->ptname[k], nm)) return I->pt[k];
    fatal("macrocell %s has no product term %s", I->mc, nm);
    return NULL;
}
static const char *ptexpr(char **exprs, const struct ainfo *I, const char *nm) {
    for (int k = 0; k < I->n_pt; k++) if (!strcmp(I->ptname[k], nm)) return exprs[k];
    fatal("macrocell %s has no product term %s", I->mc, nm);
    return NULL;
}

void atf_decode(const unsigned char *data, size_t n, const char *dev, const char *pkg, struct anet *N) {
    struct jedec J; jed_parse(data, n, &J);
    memset(N, 0, sizeof *N);
    const struct atfdev *D = atf_find(dev);
    if (!D) refuse("DEVICE_NOT_MODELLED", "%s", dev);
    if (J.qf != D->qf) refuse("QF_NOT_DEVICE", "QF%d is not %s (QF%d)", J.qf, dev, D->qf);
    N->D = D; N->dev = xstrdup(dev); N->pkg = xstrdup(pkg);
    for (int i = 0; i < D->n_pkgs; i++) if (!strcmp(D->pkgs[i].name, pkg)) N->K = &D->pkgs[i];
    if (!N->K) refuse("PACKAGE_NOT_MODELLED", "%s %s", dev, pkg);
    unsigned char *F = J.fuses; N->F = F;
    for (int i = 0; i < D->n_uncov; i++) {
        int f = D->uncov[2 * i], v = D->uncov[2 * i + 1];
        if (v < 0) refuse("UNMODELLED_FUSE", "fuse %d is in no decoded field and has no recorded constant", f);
        if (F[f] != v) refuse("UNMODELLED_FUSE", "fuse %d = %d, recorded constant %d (meaning unknown)", f, F[f], v);
    }
    N->security = J.security;
    /* configuration */
    nlist cfg = { 0 };
    for (int i = 0; i < D->n_cfg; i++) nl_push(&cfg, D->cfg[i].name, atf_opt(F, &D->cfg[i].f, fmt("config.%s", D->cfg[i].name)));
    int k;
    if ((k = nl_find(&cfg, "arming_switch")) >= 0 && !strcmp(cfg.v[k].val, "safe")) refuse("ARMING_SAFE", "arming_switch = safe (all outputs disabled): not modelled");
    const char *pd[2] = { "pd1_pin_func", "pd2_pin_func" };
    for (int i = 0; i < 2; i++) if ((k = nl_find(&cfg, pd[i])) >= 0 && !strcmp(cfg.v[k].val, "pd")) refuse("POWER_DOWN_PIN", "%s = pd: not modelled", pd[i]);
    {
        slist kv = { 0 };
        for (int i = 0; i < cfg.n; i++) sl_push_own(&kv, fmt("%s=%s", cfg.v[i].name, cfg.v[i].val));
        /* sorted by (name, value): the names are distinct, so sorting the joined text gives the same order */
        slist nm = { 0 };
        for (int i = 0; i < cfg.n; i++) sl_push(&nm, cfg.v[i].name);
        sl_sort(&nm);
        sbuf b; sb_init(&b); sb_add(&b, "config: ");
        for (int i = 0; i < nm.n; i++) { if (i) sb_add(&b, ", "); sb_printf(&b, "%s=%s", nm.v[i], cfg.v[nl_find(&cfg, nm.v[i])].val); }
        sl_push_own(&N->notes, sb_take(&b));
    }
    actx C = { N, { 0 } };
    k = nl_find(&cfg, "jtag_pin_func");
    int jtag = k >= 0 && !strcmp(cfg.v[k].val, "jtag");
    if (jtag) for (int i = 0; i < 4; i++) { char *p = atf_padname(&D->spec[i]); if (sl_find(&C.jtag_pads, p) < 0) sl_push(&C.jtag_pads, p); }
    N->jtag_mode = jtag;
    /* globals */
    const char *gn[3] = { "GCLK1", "GCLK2", "GCLK3" };
    char *gpad[3], *ginv[3];
    for (int g = 0; g < 3; g++) {
        gpad[g] = atf_opt(F, &D->glob[G_GCLK1_MUX + g], fmt("%s.mux", gn[g]));
        ginv[g] = atf_opt(F, &D->glob[G_GCLK1_INV + g], fmt("%s.invert", gn[g]));
    }
    char *gclr_inv = atf_opt(F, &D->glob[G_GCLR_INV], "GCLR.invert");
    char *goe_src[7]; int goe_inv[7];
    for (int g = 1; g <= 6; g++) { goe_src[g] = atf_opt(F, &D->glob[G_GOE1_MUX + g - 1], fmt("GOE%d.mux", g)); goe_inv[g] = F[D->goe_inv0 + g - 1]; }
    slist used_g = { 0 };
#define GSIG(nm) (sl_find(&used_g, (nm)) < 0 ? sl_push(&used_g, (nm)) : (void)0, fmt("g_%s", (nm)))
    nlist W = { 0 };
    /* UIM switches */
    for (int i = 0; i < D->n_sws; i++) {
        char *where = fmt("switch %s", D->sws[i].name);
        char *src = atf_opt(F, &D->sws[i].mux, where);
        nl_push(&W, fmt("uim_%s", D->sws[i].name), source(&C, src, where));
    }
    /* macrocells, in number order */
    const struct amc **mcs = xmalloc(sizeof(void *) * (size_t)D->n_mcs);
    for (int i = 0; i < D->n_mcs; i++) mcs[i] = &D->mcs[i];
    qsort(mcs, (size_t)D->n_mcs, sizeof(void *), cmp_mc);
    char *casflag = xcalloc((size_t)D->n_mcs + 2, 1), *caszero = xcalloc((size_t)D->n_mcs + 2, 1);
    int maxnum = 0; for (int i = 0; i < D->n_mcs; i++) if (mcs[i]->num > maxnum) maxnum = mcs[i]->num;
    casflag = xcalloc((size_t)maxnum + 2, 1); caszero = xcalloc((size_t)maxnum + 2, 1);
    for (int i = 0; i < D->n_mcs; i++) {
        const struct amc *e = mcs[i]; const struct afield *cf = NULL;
        for (int q = 0; q < e->n_opts; q++) if (!strcmp(e->opts[q].name, "cas_mux")) cf = &e->opts[q].f;
        if (!cf) fatal("MC%d has no cas_mux field", e->num);
        casflag[e->num] = !strcmp(atf_opt(F, cf, fmt("MC%d.cas_mux", e->num)), "sum");
    }
    N->info = xcalloc((size_t)D->n_mcs + 1, sizeof(struct ainfo));
    nlist outs = { 0 }; plist outv = { 0 }; nlist jd = { 0 }, und = { 0 };
    plist regs = { 0 };
    for (int i = 0; i < D->n_mcs; i++) {
        const struct amc *e = mcs[i]; int num = e->num;
        struct ainfo *I = &N->info[N->ni++];
        I->mc = fmt("MC%d", num);
        char *mcl = fmt("mc%d", num);
        I->o = xcalloc((size_t)e->n_opts + 1, sizeof(struct named)); I->n_o = e->n_opts;
        for (int q = 0; q < e->n_opts; q++) { I->o[q].name = xstrdup(e->opts[q].name); I->o[q].val = atf_opt(F, &e->opts[q].f, fmt("%s.%s", I->mc, e->opts[q].name)); }
#define O(nm) atf_info_opt(I, nm)
        const struct ablk *B = find_blk(D, e->block);
        char *pts[5]; I->n_pt = e->n_pt;
        for (int p = 0; p < e->n_pt; p++) {
            int a = e->pt[p].a, b = e->pt[p].b, all1 = 1, any1 = 0;
            for (int f = a; f < b; f++) { if (F[f]) any1 = 1; else all1 = 0; }
            char *expr;
            if (all1) expr = xstrdup("1'b1");
            else if (!any1) expr = xstrdup("1'b0");
            else {
                slist lits = { 0 };
                for (int off = 0; off < b - a; off++) {
                    if (F[a + off]) continue;
                    if (off >= B->w || B->cols[2 * off] < 0) fatal("block %s has no column at offset %d", B->name, off);
                    int kd = B->cols[2 * off], nb = B->cols[2 * off + 1];
                    if (kd == 1) sl_push_own(&lits, fmt("uim_UIM%d", nb));
                    else if (kd == 2) sl_push_own(&lits, fmt("~uim_UIM%d", nb));
                    else if (kd == 3) sl_push_own(&lits, fmt("flb_MC%d", nb));
                    else refuse("PT_COLUMN_UNKNOWN", "%s %s column ?", I->mc, e->pt[p].name);
                }
                expr = fmt("(%s)", sl_join(&lits, " & "));
            }
            pts[p] = expr; I->ptname[p] = xstrdup(e->pt[p].name);
            char *wn = fmt("%s_%s", mcl, py_lower(e->pt[p].name));
            I->pt[p] = wn;
            nl_push(&W, wn, expr);
        }
        if (!strcmp(O("pt_power"), "off")) {
            int nonempty = 0;
            for (int p = 0; p < e->n_pt; p++) if (strcmp(pts[p], "1'b0")) nonempty = 1;
            if (nonempty) refuse("PT_POWER_OFF", "%s: pt_power off with a non-empty PT", I->mc);
        }
        int cas = !strcmp(O("cas_mux"), "sum"), xb = !strcmp(O("xor_b_mux"), "ff_qn"), inv_on = !strcmp(O("xor_invert"), "on");
        int pt1_flb = !strcmp(O("pt1_mux"), "flb"), pt2_xor = !strcmp(O("pt2_mux"), "xor");
        slist terms = { 0 };
        if (!pt1_flb) sl_push(&terms, ptx(I, "PT1"));
        if (!pt2_xor) sl_push(&terms, ptx(I, "PT2"));
        if (!strcmp(O("pt3_mux"), "sum")) sl_push(&terms, ptx(I, "PT3"));
        if (!strcmp(O("pt4_mux"), "sum")) sl_push(&terms, ptx(I, "PT4"));
        if (!strcmp(O("pt5_mux"), "sum")) sl_push(&terms, ptx(I, "PT5"));
        int src = num - 1;
        const struct amc *sm = find_mc(D, src);
        if (src >= 0 && src <= maxnum && sm && casflag[src] && !caszero[src]) {
            if (strcmp(sm->block, e->block)) refuse("CASCADE_BLOCK_EDGE_NOT_MODELLED", "MC%d -> %s", src, I->mc);
            sl_push_own(&terms, fmt("casout_MC%d", src));
        }
        char *st = terms.n ? fmt("(%s)", sl_join(&terms, " | ")) : xstrdup("1'b0");
        char *ffq = fmt("%s_q", mcl);
        const char *y1 = pt1_flb ? ptx(I, "PT1") : "1'b1", *y2 = pt2_xor ? ptx(I, "PT2") : "1'b1";
        const char *A, *Bx;
        /* the summed product terms, for the cascade checks */
        slist summed = { 0 };
        if (!pt1_flb) sl_push(&summed, "PT1");
        if (!pt2_xor) sl_push(&summed, "PT2");
        if (!strcmp(O("pt3_mux"), "sum")) sl_push(&summed, "PT3");
        if (!strcmp(O("pt4_mux"), "sum")) sl_push(&summed, "PT4");
        if (!strcmp(O("pt5_mux"), "sum")) sl_push(&summed, "PT5");
        if (cas) {
            int hascas = 0; for (int t = 0; t < terms.n; t++) if (startswith(terms.v[t], "casout_")) hascas = 1;
            int allz = 1; for (int t = 0; t < summed.n; t++) if (strcmp(ptexpr(pts, I, summed.v[t]), "1'b0")) allz = 0;
            caszero[num] = !hascas && allz;
            const struct amc *dm = find_mc(D, num + 1);
            if (!dm || strcmp(dm->block, e->block)) {
                int anyn = 0; for (int t = 0; t < summed.n; t++) if (strcmp(ptexpr(pts, I, summed.v[t]), "1'b0")) anyn = 1;
                if (strcmp(st, "1'b0") && anyn) refuse("CASCADE_BLOCK_EDGE_NOT_MODELLED", "%s: CASOUT leaves the block", I->mc);
            }
            nl_push(&W, fmt("casout_%s", I->mc), st);
            A = y2; Bx = xb ? fmt("~%s", ffq) : (!inv_on ? y1 : "1'b1");
        } else {
            A = st; Bx = xb ? fmt("~%s", ffq) : (!inv_on ? y2 : "1'b1");
        }
        char *xt = !inv_on ? fmt("(%s ^ %s)", A, Bx) : fmt("~(%s ^ %s)", A, Bx);
        int flb_in_xor = cas && !xb && !inv_on && pt1_flb;
        char *flb = (pt1_flb && !flb_in_xor) ? fmt("~%s", ptx(I, "PT1")) : xstrdup("1'b1");
        nl_push(&W, fmt("%s_st", mcl), st);
        nl_push(&W, fmt("%s_xt", mcl), xt);
        nl_push(&W, fmt("flb_%s", I->mc), flb);
        /* term 3: asynchronous reset */
        slist ar = { 0 };
        if (!strcmp(O("pt3_mux"), "ar")) sl_push(&ar, ptx(I, "PT3"));
        if (!strcmp(O("gclr_mux"), "GCLR")) sl_push_own(&ar, GSIG("GCLR"));
        char *arx = ar.n ? sl_join(&ar, " | ") : xstrdup("1'b0");
        /* term 4: clock / clock enable */
        char *clk, *ce;
        const char *p4m = O("pt4_mux"), *p4f = O("pt4_func");
        if (!strcmp(p4m, "clk_ce") && !strcmp(p4f, "ce")) { clk = GSIG(O("gclk_mux")); ce = xstrdup(ptx(I, "PT4")); }
        else if (!strcmp(p4m, "clk_ce") && !strcmp(p4f, "clk")) { clk = xstrdup(ptx(I, "PT4")); ce = xstrdup("1'b1"); }
        else if (!strcmp(p4m, "sum") && !strcmp(p4f, "ce")) { clk = GSIG(O("gclk_mux")); ce = xstrdup("1'b1"); }
        else { clk = xstrdup("1'b1"); ce = xstrdup("1'b1"); }
        /* term 5: preset / output enable */
        const char *p5m = O("pt5_mux"), *p5f = O("pt5_func"), *oem = O("oe_mux");
        char *k5 = fmt("(%s, %s, %s)", py_repr_str(p5m), py_repr_str(p5f), py_repr_str(oem));
        char *as = xstrdup("1'b0"), *en = NULL;
        char *pad = e->pad.num == -2 ? NULL : atf_padname(&e->pad);
        if (!strcmp(p5m, "sum")) {
            if (!strcmp(oem, "GND")) en = xstrdup("1'b0");
            else if (startswith(oem, "GOE")) en = GSIG(oem);
            else if (!strcmp(oem, "VCC_pt5") && !strcmp(p5f, "as")) en = xstrdup("1'b1");
            else {
                en = xstrdup("UNDETERMINED");
                if (!pad) fatal("%s has no pad", I->mc);
                nl_set(&und, pad, fmt("%s: %s", I->mc, k5));
            }
        } else if (!strcmp(p5f, "oe")) {
            if (!strcmp(oem, "VCC_pt5")) en = xstrdup(ptx(I, "PT5"));
            else if (!strcmp(oem, "GND") && !strcmp(ptexpr(pts, I, "PT5"), "1'b0")) en = xstrdup("1'b0");
            else if (startswith(oem, "GOE") && !strcmp(ptexpr(pts, I, "PT5"), "1'b0")) en = GSIG(oem);
            else refuse("PT5_GROUP_NOT_MODELLED", "%s: %s", I->mc, k5);
        } else {
            as = xstrdup(ptx(I, "PT5"));
            en = !strcmp(oem, "GND") ? xstrdup("1'b0") : !strcmp(oem, "VCC_pt5") ? xstrdup("1'b1") : GSIG(oem);
        }
        if (!strcmp(O("d_mux"), "fast")) refuse("FAST_INPUT_NOT_MODELLED", "%s: d_mux = fast", I->mc);
        char *xtn = fmt("%s_xt", mcl);
        char *aout = !strcmp(O("o_mux"), "sync") ? xstrdup(ffq) : xtn;
        char *fb = !strcmp(O("fb_mux"), "sync") ? xstrdup(ffq) : xtn;
        if (strcmp(O("storage"), "dff")) {
            if (strcmp(ce, "1'b1")) refuse("LATCH_CE_NOT_MODELLED", "%s: latch with clock enable %s", I->mc, ce);
            if (sl_find(&N->latches, I->mc) < 0) sl_push(&N->latches, I->mc);
        }
        nl_push(&W, fmt("fb_%s", I->mc), fb);
        struct areg *R = xcalloc(1, sizeof *R); R->mc = xstrdup(I->mc); R->clk = clk; R->ce = ce; R->ar = arx; R->as = as; R->d = xtn;
        pl_push(&regs, R);
        if (!strcmp(en, "UNDETERMINED")) {
            if (sl_find(&N->inputs, pad) >= 0) refuse("PT5_GROUP_NOT_MODELLED", "%s (pad %s is read back)", und.v[nl_find(&und, pad)].val, pad);
        } else if (strcmp(en, "1'b0")) {
            if (pad && sl_find(&C.jtag_pads, pad) >= 0) {
                nl_set(&jd, pad, I->mc);
                sl_push_own(&N->notes, fmt("JTAG mode: %s output buffer enabled on JTAG pad %s (not a pin, not modelled)", I->mc, pad));
            }
            if (!pad || !atf_pin(N, pad)) sl_push_own(&N->notes, fmt("output on unbonded pad %s (%s) dropped", pad ? pad : "None", I->mc));
            else {
                int q = nl_find(&outs, pad);
                struct aout *o = xcalloc(1, sizeof *o); o->pad = xstrdup(pad); o->a = aout; o->en = en; o->od = !strcmp(O("output_driver"), "open_drain");
                if (q >= 0) outv.v[q] = o; else { nl_push(&outs, pad, ""); pl_push(&outv, o); }
            }
        }
        I->xt = xt; I->clk = clk; I->ce = ce; I->ar = arx; I->as = as; I->en = en; I->a = aout; I->fb = fb; I->pad = pad;
        I->pt1_flb = pt1_flb; I->pt2_xor = pt2_xor; I->xb = xb; I->inv_on = inv_on;
#undef O
    }
    /* the global networks the macrocells use */
    sl_sort(&used_g);
    static rx *rfb2;
    if (!rfb2) rfb2 = rx_compile("MC\\d+_FB$", 0);
    for (int i = 0; i < used_g.n; i++) {
        const char *g = used_g.v[i];
        if (!strcmp(g, "GCLR")) nl_push(&W, "g_GCLR", fmt("%s%s", !strcmp(gclr_inv, "on") ? "~" : "", padsig(&C, "R")));
        else if (startswith(g, "GCLK")) {
            int q = atoi(g + 4) - 1;
            if (q < 0 || q > 2 || strlen(g) != 5) fatal("global %s not known", g);
            nl_push(&W, fmt("g_%s", g), fmt("%s%s", !strcmp(ginv[q], "on") ? "~" : "", padsig(&C, replace_all(gpad[q], "_PAD", ""))));
        } else {
            int q = startswith(g, "GOE") ? atoi(g + 3) : 0;
            if (q < 1 || q > 6 || strlen(g) != 4) fatal("global %s not known", g);
            rxm m;
            if (rx_match(rfb2, goe_src[q], &m)) refuse("GOE_PT_SOURCE_NOT_MODELLED", "%s = %s", g, goe_src[q]);
            nl_push(&W, fmt("g_%s", g), fmt("%s(%s)", goe_inv[q] == 0 ? "~" : "", source(&C, goe_src[q], g)));
        }
    }
    N->nw = W.n; N->w = W.v;
    N->nr = regs.n; N->r = xcalloc((size_t)regs.n + 1, sizeof(struct areg));
    for (int i = 0; i < regs.n; i++) N->r[i] = *(struct areg *)regs.v[i];
    N->no = outv.n; N->out = xcalloc((size_t)outv.n + 1, sizeof(struct aout));
    for (int i = 0; i < outv.n; i++) N->out[i] = *(struct aout *)outv.v[i];
    N->njd = jd.n; N->jd = jd.v; N->nu = und.n; N->und = und.v;
}

char *atf_dump(const struct anet *N) {
    sbuf b; sb_init(&b);
    sb_printf(&b, "kind ATF\ndevice %s %s\nsecurity %s\njtag_mode %d\n", N->dev, N->pkg, N->security < 0 ? "None" : N->security ? "1" : "0", N->jtag_mode);
    for (int i = 0; i < N->notes.n; i++) sb_printf(&b, "note %s\n", N->notes.v[i]);
    slist in = { 0 };
    for (int i = 0; i < N->inputs.n; i++) sl_push(&in, N->inputs.v[i]);
    sl_sort(&in);
    sb_printf(&b, "inputs %s\n", sl_join(&in, " "));
    for (int i = 0; i < N->no; i++) sb_printf(&b, "output %s | %s | %s | %d\n", N->out[i].pad, N->out[i].a, N->out[i].en, N->out[i].od);
    for (int i = 0; i < N->nw; i++) sb_printf(&b, "wire %s = %s\n", N->w[i].name, N->w[i].val);
    for (int i = 0; i < N->nr; i++) sb_printf(&b, "reg %s | %s | %s | %s | %s | %s\n", N->r[i].mc, N->r[i].clk, N->r[i].ce, N->r[i].ar, N->r[i].as, N->r[i].d);
    slist la = { 0 };
    for (int i = 0; i < N->latches.n; i++) sl_push(&la, N->latches.v[i]);
    sl_sort(&la);
    sb_printf(&b, "latches %s\njtag_driven", sl_join(&la, " "));
    for (int i = 0; i < N->njd; i++) sb_printf(&b, " %s=%s", N->jd[i].name, N->jd[i].val);
    sb_add(&b, "\n");
    for (int i = 0; i < N->nu; i++) sb_printf(&b, "undetermined %s = %s\n", N->und[i].name, N->und[i].val);
    for (int i = 0; i < N->ni; i++) {
        const struct ainfo *I = &N->info[i];
        sb_printf(&b, "mc %s en %s a %s fb %s pad %s\n", I->mc, I->en, I->a, I->fb, I->pad ? I->pad : "None");
    }
    return sb_take(&b);
}

/* maxdec.c -- MAX 7000S / AE / B fuse map -> decoded design.
 *
 * Product term (88 columns): column 2k / 2k+1 = PIA line k true / complement, 72 + k = shared expander of LC
 * lab*16 + k + 1. Fuse 0 = column connected. All columns connected = constant 0; none = constant 1.
 * PIA line: its buffer's select bits are one-cold (all ones = line unused, two zeros = invalid); the zero's position
 * picks the source: a macrocell output, an I/O pin, or a dedicated input.
 * I/O cell output enable: setting 0 = no output, 1 = always enabled, 2 + k = global output enable k (one-cold select of
 * its source, plus an invert bit). Open drain: raw 0.
 * Global clocks: the global clock configuration bits select, per network, a dedicated input and its polarity, from the
 * part's accepted patterns. Global clear: dedicated input 2, active low.
 * With boundary scan enabled (JTAG bit), the four test-port pins belong to the test port.
 */
#include "max.h"
#include "pof.h"
#include <stdlib.h>
#include <string.h>

static const char *SETNAMES[S_N] = { "MCELL_PTERM_INV_SEXP_FUNCTION", "MCELL_PTERM_INV_XOR_FUNCTION", "MCELL_PTERM_CLR_FUNCTION",
    "MCELL_PTERM_CLK_CE_FUNCTION", "MCELL_PTERM_PRE_FUNCTION", "MCELL_CLOCK_SELECT", "MCELL_CLEAR_SELECT",
    "MCELL_OR_OUTPUT_INVERT", "MCELL_PEXP_ALLOCATION", "MCELL_REG_OUT_MAX7K", "MCELL_USE_TURBO" };

static char *int_list(const int *v, int n, int tuple) {   /* Python repr of a list / tuple of ints */
    sbuf b; sb_init(&b); sb_addc(&b, tuple ? '(' : '[');
    for (int i = 0; i < n; i++) sb_printf(&b, i ? ", %d" : "%d", v[i]);
    if (tuple && n == 1) sb_addc(&b, ',');
    sb_addc(&b, tuple ? ')' : ']');
    return sb_take(&b);
}

static char *src_net(const struct maxctx *d, int kind, int i) {
    if (kind == SRC_MCELL) return fmt("lc%d_out", i + 1);
    if (kind == SRC_IO) {
        int p = max_io_pin(d, i);
        if (!p) refuse("PAD_NOT_BONDED", "IO cell %d has no pin in package %s", i, d->pkg);
        return fmt("pin%d", p);
    }
    if (kind == SRC_FINPBUF) return fmt("pin%d", max_ib_pin(d, i));
    refuse("SOURCE_KIND", "PIA_BUFFER");
    return NULL;
}

typedef struct { struct decoded *X; char **cache; char *cached; } dctx;

static char *pia(dctx *c, int lab, int line) {
    struct decoded *X = c->X; const struct maxdev *D = X->d->D;
    int s = lab * D->lines + line;
    if (c->cached[s]) return c->cache[s];
    int pb = s;
    if (D->has2x2) {
        int sel = X->bits[D->pia_base + D->pbw + s * D->pia_w];
        pb = -1;
        for (int i = 0; i < D->n_slotsel; i++) if (D->slotsel[3 * i] == s && D->slotsel[3 * i + 1] == sel) pb = D->slotsel[3 * i + 2];
        if (pb < 0) refuse("PIA_2X2_NO_BUFFER", "slot %d 2x2 select %d", s, sel);
    }
    int grp[64], zeros = 0, pos = -1;
    for (int j = 0; j < D->pbw; j++) { grp[j] = X->bits[D->pia_base + pb * D->pia_w + j]; if (!grp[j]) { zeros++; if (pos < 0) pos = j; } }
    char *r = NULL;
    if (zeros == 0) r = NULL;
    else if (zeros != 1) refuse("PIA_SELECT_INVALID", "PIA buffer %d code %s", pb, int_list(grp, D->pbw, 0));
    else {
        int k = -1;
        for (int i = 0; i < D->n_bufsrc; i++) if (D->bufsrc[4 * i] == pb && D->bufsrc[4 * i + 1] == pos) k = i;
        if (k < 0) refuse("PIA_NO_SOURCE", "PIA buffer %d position %d", pb, pos);
        r = src_net(X->d, D->bufsrc[4 * k + 2], D->bufsrc[4 * k + 3]);
    }
    c->cached[s] = 1; c->cache[s] = r;
    return r;
}

static void row(dctx *c, int mc, int pt, struct row *out) {
    struct decoded *X = c->X; const struct maxdev *D = X->d->D;
    int base = D->cols * D->n_pt * mc + D->cols * pt, any = 0;
    for (int col = 0; col < D->cols; col++) if (X->bits[base + col]) { any = 1; break; }
    memset(out, 0, sizeof *out);
    if (!any) { out->st = 1; return; }
    out->st = 2;
    for (int col = 0; col < D->cols; col++) {
        if (X->bits[base + col]) continue;
        if (col < 2 * D->lines) {
            char *net = pia(c, mc / 16, col / 2);
            if (!net) refuse("PIA_LINE_UNDRIVEN", "LC%d PT%d reads unused PIA line %d of LAB %d", mc + 1, pt + 1, col / 2, mc / 16);
            sl_push_own(&out->l, (col % 2) ? fmt("!%s", net) : xstrdup(net));
        } else sl_push_own(&out->l, fmt("sexp%d_out", (mc / 16) * 16 + col - 2 * D->lines + 1));
    }
}

/* work list: kind 0 = macrocell number, 1 = net name */
typedef struct { int n, cap; int *kind, *num; char **net; } wlist;
static void wpush(wlist *w, int kind, int num, const char *net) {
    if (w->n == w->cap) {
        int c = w->cap ? w->cap * 2 : 64;
        int *k = xmalloc(sizeof(int) * (size_t)c), *u = xmalloc(sizeof(int) * (size_t)c); char **s = xmalloc(sizeof(char *) * (size_t)c);
        if (w->n) { memcpy(k, w->kind, sizeof(int) * (size_t)w->n); memcpy(u, w->num, sizeof(int) * (size_t)w->n); memcpy(s, w->net, sizeof(char *) * (size_t)w->n); }
        w->kind = k; w->num = u; w->net = s; w->cap = c;
    }
    w->kind[w->n] = kind; w->num[w->n] = num; w->net[w->n] = net ? xstrdup(net) : NULL; w->n++;
}
static const char *nobang(const char *s) { while (*s == '!') s++; return s; }

static void need_row(dctx *c, wlist *w, int n, int p) {
    struct cell *ce = &c->X->cells[n];
    if (ce->rows[p].st == 0) row(c, n - 1, p, &ce->rows[p]);
    if (ce->rows[p].st == 2) for (int i = 0; i < ce->rows[p].l.n; i++) wpush(w, 1, 0, nobang(ce->rows[p].l.v[i]));
}

int max_used(const struct decoded *X, const char *net) { return sl_find(&X->used, net) >= 0; }
const struct io *max_io_at(const struct decoded *X, int pin) { for (int i = 0; i < X->n_ios; i++) if (X->ios[i].pin == pin) return &X->ios[i]; return NULL; }
const struct attr *max_attr_at(const struct decoded *X, int pin) { for (int i = 0; i < X->n_attrs; i++) if (X->attrs[i].pin == pin) return &X->attrs[i]; return NULL; }
static int in_list(int v, const int *l, int n) { for (int i = 0; i < n; i++) if (l[i] == v) return 1; return 0; }

void max_decode(const unsigned char *data, size_t n, struct decoded *X) {
    struct pof P; pof_parse(data, n, &P);
    if (!P.device) fatal("the POF names no device");
    memset(X, 0, sizeof *X);
    struct maxctx *d = max_device(P.device); X->d = d;
    const struct maxdev *D = d->D;
    if (!P.has_data) fatal("the POF has no fuse data packet");
    if (P.count != (unsigned long long)D->total) refuse("SHAPE", "%llu bits (part has %d)", P.count, D->total);
    X->nbits = D->total; X->bits = xmalloc((size_t)D->total);
    for (int k = 0; k < D->total; k++) X->bits[k] = (unsigned char)pof_bit(&P, (unsigned long long)k);
    unsigned char *bits = X->bits;
    /* the extra global bits of the S parts */
    int s7[7];
    for (int i = 0; i < d->n_s7; i++) s7[i] = bits[d->s7_addr[i]];
    int ok = 0;
    for (int i = 0, k = 0; i < D->n_s7set; i++) {
        int len = D->s7set[k];
        if (len == d->n_s7 && (len == 0 || !memcmp(&D->s7set[k + 1], s7, sizeof(int) * (size_t)len))) ok = 1;
        k += 1 + len;
    }
    if (!ok) refuse("S_GLOBAL_UNWITNESSED", "bits %d..%d = %s", d->s7_addr[0], d->s7_addr[d->n_s7 - 1], int_list(s7, d->n_s7, 1));
    for (int i = 0; i < 3; i++) X->gcc[i] = bits[D->gcc[i]];
    for (int i = 0; i < D->n_gcc_t; i++) {
        const int *g = &D->gcc_t[6 * i];
        if (g[0] == X->gcc[0] && g[1] == X->gcc[1] && g[2] == X->gcc[2] && X->n_gclk < 4) {
            X->gclk_net[X->n_gclk] = g[3]; X->gclk_ib[X->n_gclk] = g[4]; X->gclk_inv[X->n_gclk] = g[5]; X->n_gclk++;
        }
    }
    if (!X->n_gclk) refuse("GLOBAL_CLOCK_CONFIG_UNWITNESSED", "raw bits %s", int_list(X->gcc, 3, 1));
    int jtag = max_raw(d, bits, "JTAG_ENABLE_BST_SUPPORT", 0); X->jtag = jtag;
    if (jtag) {
        int outs[64], no = 0;
        for (int i = 0; i < d->n_io; i++) {
            int p = d->io_val[i];
            if (in_list(p, d->jtag_in, 3) && max_setting(d, bits, "IO_SELECT_OE_MAX7K", d->io_key[i]) != 0 && no < 64) outs[no++] = p;
        }
        if (no) refuse("JTAG_PIN_CONFLICT", "JTAG enabled and IO cells of JTAG input pins %s set as outputs", int_list(outs, no, 0));
    }
    X->cells = xcalloc((size_t)D->n_mc + 2, sizeof(struct cell));
    int has_puh = max_find_setting(d, "MCELL_REG_POWERS_UP_HIGH") != NULL, has_rms = max_find_setting(d, "MCELL_REG_MODE_SELECT") != NULL;
    for (int mc = 0; mc < D->n_mc; mc++) {
        struct cell *c = &X->cells[mc + 1];
        for (int k = 0; k < S_N; k++) c->st[k] = max_setting(d, bits, SETNAMES[k], mc);
        c->pu = 0;
        if (has_puh) {
            int raw = max_setting(d, bits, "MCELL_REG_POWERS_UP_HIGH", mc);
            c->pu = -1;
            for (int i = 0; i < D->n_puh; i++) if (D->puh[2 * i] == raw) c->pu = D->puh[2 * i + 1];
        }
        c->reg = c->st[S_REG_OUT] == 0; c->donor = c->st[S_PEXP] == 1;
        if (has_rms && max_raw(d, bits, "MCELL_REG_MODE_SELECT", mc) == 0 && c->st[S_REG_OUT] == 0) {
            int p = max_io_pin(d, mc);
            if (!p) refuse("FAST_INPUT_PAD_NOT_BONDED", "LC%d fast input from IO cell %d, no pin in %s", mc + 1, mc, d->pkg);
            c->fpin = p;
        }
    }
    if (max_find_setting(d, "IO_USE_BUS_HOLD")) {
        X->attrs = xcalloc((size_t)d->n_io + 1, sizeof(struct attr));
        for (int i = 0; i < d->n_io; i++) {
            int io = d->io_key[i], pin = d->io_val[i];
            struct attr *a = NULL;
            for (int k = 0; k < X->n_attrs; k++) if (X->attrs[k].pin == pin) a = &X->attrs[k];
            if (!a) a = &X->attrs[X->n_attrs++];
            a->pin = pin; a->bus_hold = max_raw(d, bits, "IO_USE_BUS_HOLD", io) == 0; a->weak_pull_up = max_raw(d, bits, "IO_USE_PULL_UP", io) == 0;
            a->iost = max_raw(d, bits, "IO_SELECT_IOST_MAX7K", io); a->delay = max_raw(d, bits, "IO_SET_DELAY_CHAIN_MAX7K", io);
            if (!in_list(a->iost, D->iost, D->n_iost))
                refuse("IOST_UNWITNESSED", "pin %d IO standard code %d (part table %s)", pin, a->iost, int_list(D->iost, D->n_iost, 0));
            if (!in_list(a->delay, D->delay, D->n_delay)) refuse("DELAY_CHAIN_UNWITNESSED", "pin %d delay chain raw %d", pin, a->delay);
        }
    }
    /* I/O cells in index order */
    ilist order = { 0 };
    for (int i = 0; i < d->n_io; i++) il_push(&order, d->io_key[i]);
    il_sort(&order);
    X->ios = xcalloc((size_t)d->n_io + 1, sizeof(struct io));
    int has_clamp = max_find_setting(d, "IO_USE_GND_CLAMP") != NULL;
    for (int oi = 0; oi < order.n; oi++) {
        int i = order.v[oi], pin = max_io_pin(d, i);
        int oe = max_setting(d, bits, "IO_SELECT_OE_MAX7K", i);
        if (oe == 0) {
            if (has_clamp && max_setting(d, bits, "IO_USE_GND_CLAMP", i) == D->gndclamp_on && D->gndclamp_on >= 0)
                if (!(jtag && in_list(pin, d->jtag_pins, 4))) refuse("GND_CLAMP_UNWITNESSED", "pin %d: GND clamp with the output off", pin);
            continue;
        }
        if (SV_IS_CODE(oe)) refuse("IO_SELECT_OE_UNWITNESSED", "IO%d %s", i, sv_str(oe));
        struct io io = { pin, i + 1, max_raw(d, bits, "IO_USE_OPEN_DRAIN", i) == 0, 0, 0, 0, NULL };
        if (has_clamp) {
            io.has_clamp = 1;
            io.clamp = D->gndclamp_on >= 0 && max_setting(d, bits, "IO_USE_GND_CLAMP", i) == D->gndclamp_on;
            if (io.clamp && oe == 1 && !io.od) { io.lc = 0; io.clamp_lc = i + 1; }
        }
        if (oe == 1) io.oe = xstrdup("vcc");
        else {
            int k = oe - 2;
            if (k >= D->n_poe) fatal("global output enable %d does not exist on %s", k, d->part);
            int grp[64], zeros = 0, pos = -1, b0 = D->oe_base + k * D->oe_w;
            for (int j = 0; j < D->poe_pbw; j++) { grp[j] = bits[b0 + j]; if (!grp[j]) { zeros++; if (pos < 0) pos = j; } }
            int inv = bits[b0 + D->poe_pbw];
            if (zeros != 1) refuse("GLOBAL_OE_SELECT_INVALID", "IO%d uses global OE %d, select %s invert %d", i, k, int_list(grp, D->poe_pbw, 0), inv);
            int s = -1;
            for (int q = 0; q < D->n_poesrc; q++) if (D->poesrc[4 * q] == k && D->poesrc[4 * q + 1] == pos) s = q;
            if (s < 0) refuse("GLOBAL_OE_NO_SOURCE", "global OE %d position %d", k, pos);
            char *net = src_net(d, D->poesrc[4 * s + 2], D->poesrc[4 * s + 3]);
            io.oe = inv == 1 ? fmt("!%s", net) : net;
        }
        if (jtag && in_list(pin, d->jtag_pins, 4)) continue;   /* the test port's own output, not a design pin */
        struct io *slot = NULL;
        for (int q = 0; q < X->n_ios; q++) if (X->ios[q].pin == pin) slot = &X->ios[q];
        if (!slot) slot = &X->ios[X->n_ios++];
        *slot = io;
    }
    /* liveness: decode only what an output can reach */
    dctx C = { X, xcalloc((size_t)D->n_lab * (size_t)D->lines + 1, sizeof(char *)), xcalloc((size_t)D->n_lab * (size_t)D->lines + 1, 1) };
    X->live = xcalloc((size_t)D->n_mc + 2, 1); X->live_sexp = xcalloc((size_t)D->n_mc + 2, 1);
    wlist w = { 0 };
    for (int q = 0; q < X->n_ios; q++) if (X->ios[q].lc) wpush(&w, 0, X->ios[q].lc, NULL);
    for (int q = 0; q < X->n_ios; q++) if (strcmp(X->ios[q].oe, "vcc")) wpush(&w, 1, 0, nobang(X->ios[q].oe));
    while (w.n) {
        w.n--;
        int kind = w.kind[w.n], num = w.num[w.n]; char *v = w.net[w.n];
        if (kind == 1) {
            if (max_used(X, v)) continue;
            sl_push(&X->used, v);
            if (startswith(v, "lc")) wpush(&w, 0, atoi(v + 2), NULL);
            else if (startswith(v, "sexp")) {
                int k = atoi(v + 4);
                if (!X->live_sexp[k]) {
                    X->live_sexp[k] = 1;
                    if (!X->cells[k].st[S_INV_SEXP]) refuse("SEXP_NOT_STEERED", "SEXP%d read but LC%d PT1 is not steered to it", k, k);
                    need_row(&C, &w, k, 0);
                }
            }
            continue;
        }
        int nn = num;
        if (X->live[nn]) continue;
        X->live[nn] = 1;
        struct cell *c = &X->cells[nn]; int *st = c->st;
        if (c->fpin) {
            if (!c->reg || st[S_OR_INV] != 0)
                refuse("FAST_INPUT_MODE_UNWITNESSED", "LC%d fast input with REG_OUT raw %s OR_OUTPUT_INVERT %s", nn, sv_str(st[S_REG_OUT]), sv_str(st[S_OR_INV]));
            wpush(&w, 1, 0, fmt("pin%d", c->fpin));
        }
        if (c->pu < 0) refuse("POWERUP_ENCODING_UNWITNESSED", "LC%d MCELL_REG_POWERS_UP_HIGH raw value not in the part's known table", nn);
        if (st[S_CLEAR_SELECT] != 0 && st[S_CLEAR_SELECT] != 1) refuse("GLOBAL_CLEAR_UNWITNESSED", "LC%d CLEAR_SELECT %s", nn, sv_str(st[S_CLEAR_SELECT]));
        if (st[S_CLEAR_SELECT] == 1) wpush(&w, 1, 0, fmt("pin%d", max_ib_pin(d, 2)));
        if (st[S_OR_INV] == 3 && D->or3 && !st[S_INV_XOR]) { /* invert + T flip-flop: accepted on this part */ }
        else if (st[S_OR_INV] != 0 && st[S_OR_INV] != 1 && st[S_OR_INV] != 2 && !c->donor)
            refuse("OR_OUTPUT_INVERT_UNWITNESSED", "LC%d setting %s", nn, sv_str(st[S_OR_INV]));
        int cs = st[S_CLOCK_SELECT];
        if (!(cs == 0 || cs == 1 || cs == 2 || cs == SV_CODE(3))) refuse("CLOCK_SELECT_UNWITNESSED", "LC%d %s", nn, sv_str(cs));
        for (int p = 0; p < 5; p++) {
            if (p == 0 && st[S_INV_SEXP]) continue;                       /* term 1 goes to the shared expander only */
            if (p == 3 && st[S_CLK_CE] && !(cs == 0 || cs == 1 || cs == 2)) continue;   /* no clock: term 4 has no effect */
            need_row(&C, &w, nn, p);
        }
        if (cs == 1 || cs == 2) {
            int g = -1;
            for (int i = 0; i < X->n_gclk; i++) if (X->gclk_net[i] == cs) g = i;
            if (g < 0) refuse("CLOCK_NETWORK_UNWITNESSED", "LC%d global clock network %d under config %s", nn, cs, int_list(X->gcc, 3, 1));
            wpush(&w, 1, 0, fmt("pin%d", max_ib_pin(d, X->gclk_ib[g])));
        }
        if (nn > 1 && X->cells[nn - 1].donor) {
            if ((nn - 2) / 16 != (nn - 1) / 16) refuse("PEXP_ACROSS_LAB", "LC%d donor at a LAB edge", nn - 1);
            wpush(&w, 0, nn - 1, NULL);
        }
    }
    /* ground clamp (AE): the clamped pin is driven 0; its macrocell may not carry logic */
    sbuf cl; sb_init(&cl); int ncl = 0;
    for (int q = 0; q < X->n_ios; q++) {
        struct io *io = &X->ios[q];
        if (!io->clamp) continue;
        if (max_used(X, fmt("pin%d", io->pin))) refuse("CLAMP_PIN_READ", "pin %d: clamp on and read by logic", io->pin);
        if (!io->lc) continue;
        struct cell *c = &X->cells[io->lc]; int logic = 0;
        for (int p = 0; p < 5; p++) if (c->rows[p].st == 2 && c->rows[p].l.n) logic = 1;
        int cs = c->st[S_CLOCK_SELECT];
        if (logic || cs == 0 || cs == 1 || cs == 2) { sb_printf(&cl, ncl ? ", (%d, %d)" : "(%d, %d)", io->pin, io->lc); ncl++; }
    }
    if (ncl) refuse("AE_GND_CLAMP_WITH_LOGIC_UNWITNESSED", "clamp on, macrocell carries logic (pin, LC): [%s]", sb_take(&cl));
    if (jtag) for (int i = 0; i < 4; i++) if (max_used(X, fmt("pin%d", d->jtag_pins[i]))) refuse("JTAG_PIN_USED", "pin %d read while JTAG is enabled", d->jtag_pins[i]);
}

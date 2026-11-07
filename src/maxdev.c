/* maxdev.c -- MAX 7000 device resolution from the POF device name, pad tables and named settings.
 *
 * A POF names its device as e.g. EPM7064SLC84-5: base part EPM7064, family suffix S / AE / B, then a package code
 * (LC84 = PLCC84). The package code is looked up whole, then as its last two letters plus the pin count.
 */
#include "max.h"
#include "rx.h"
#include <string.h>

char *sv_str(int v) { return SV_IS_CODE(v) ? fmt("code%d", -1000 - v) : fmt("%d", v); }

static const char *pkg_lookup(const char *code) {
    for (int i = 0; i < n_max_pkg_codes; i++) if (!strcmp(max_pkg_codes[i].a, code)) return max_pkg_codes[i].b;
    return NULL;
}

struct maxctx *max_device(const char *devname) {
    static rx *r1, *r2;
    if (!r1) { r1 = rx_compile("(EPM7\\d+)(S|AE|B)([A-Z]+\\d+)", 0); r2 = rx_compile("([A-Z]+?)(\\d+)$", 0); }
    rxm m, k;
    if (!rx_match(r1, devname, &m)) refuse("DEVICE_NOT_MODELLED", "%s", devname);
    char *base = rx_group(&m, 1), *suf = rx_group(&m, 2), *pc = rx_group(&m, 3);
    const char *pkg = pkg_lookup(pc);
    if (!pkg && rx_match(r2, pc, &k)) {
        char *let = rx_group(&k, 1); size_t L = strlen(let);
        pkg = pkg_lookup(fmt("%s%s", L > 2 ? let + L - 2 : let, rx_group(&k, 2)));
    }
    if (!pkg) refuse("PACKAGE_NOT_MODELLED", "%s", devname);
    struct maxctx *d = xcalloc(1, sizeof *d);
    d->part = fmt("%s%s", base, suf); d->dev = py_lower(d->part); d->fam = fmt("max7000%s", py_lower(suf)); d->pkg = xstrdup(pkg);
    for (int i = 0; i < n_max_devices; i++) if (!strcmp(max_devices[i].name, d->dev)) d->D = &max_devices[i];
    if (!d->D) fatal("device %s: no fuse map for this part (supported: the MAX 7000S / AE / B parts listed in the README)", d->part);
    for (int i = 0; i < n_max_padtables; i++)
        if (!strcmp(max_padtables[i].part, d->part) && !strcmp(max_padtables[i].pkg, d->pkg)) d->P = &max_padtables[i];
    if (!d->P) fatal("device %s: no pin table for package %s", d->part, d->pkg);
    d->io_key = xmalloc(sizeof(int) * (size_t)d->P->n); d->io_val = xmalloc(sizeof(int) * (size_t)d->P->n);
    for (int i = 0; i < d->P->n; i++) {
        int pin = d->P->pads[3 * i], kind = d->P->pads[3 * i + 1], idx = d->P->pads[3 * i + 2];
        int *K = kind == 1 ? d->io_key : kind == 0 ? d->ib_key : NULL, *V = kind == 1 ? d->io_val : d->ib_val, *N = kind == 1 ? &d->n_io : &d->n_ib;
        if (!K) continue;
        int j; for (j = 0; j < *N; j++) if (K[j] == idx) break;
        if (j < *N) V[j] = pin;            /* a later pad of the same cell replaces the pin, keeping its place */
        else { if (kind == 0 && *N >= 8) fatal("pin table: too many dedicated inputs"); K[*N] = idx; V[*N] = pin; (*N)++; }
    }
    for (int i = 0; i < 3; i++) d->jtag_in[i] = d->P->jtag[i];
    for (int i = 0; i < 4; i++) d->jtag_pins[i] = d->P->jtag[i];
    if (d->D->s7_first) { d->n_s7 = 7; for (int i = 0; i < 7; i++) d->s7_addr[i] = d->D->s7_first + i; }
    return d;
}

int max_io_pin(const struct maxctx *d, int io) {
    for (int i = 0; i < d->n_io; i++) if (d->io_key[i] == io) return d->io_val[i];
    return 0;
}
int max_ib_pin(const struct maxctx *d, int ib) {
    for (int i = 0; i < d->n_ib; i++) if (d->ib_key[i] == ib) return d->ib_val[i];
    fatal("dedicated input %d has no pin in package %s", ib, d->pkg);
    return 0;
}
const struct setting *max_find_setting(const struct maxctx *d, const char *name) {
    for (int i = 0; i < d->D->n_settings; i++)
        if (!strcmp(d->D->settings[i].name, name)) return d->D->settings[i].n_inst ? &d->D->settings[i] : NULL;
    return NULL;
}
int max_raw(const struct maxctx *d, const unsigned char *bits, const char *name, int inst) {
    const struct setting *s = max_find_setting(d, name);
    if (!s) fatal("setting %s does not exist on %s", name, d->part);
    if (inst < 0 || inst >= s->n_inst) fatal("setting %s has no instance %d on %s", name, inst, d->part);
    int v = 0;
    for (int j = 0; j < s->n_bits; j++) {
        int a = s->addr[inst * s->n_bits + j];
        if (a < 0) fatal("setting %s instance %d has no bit %d on %s", name, inst, j, d->part);
        v |= bits[a] << j;
    }
    return v;
}
int max_setting(const struct maxctx *d, const unsigned char *bits, const char *name, int inst) {
    const struct setting *s = max_find_setting(d, name);
    int c = max_raw(d, bits, name, inst);
    if (s->n_map >= 0) {
        for (int i = 0; i < s->n_map; i++) if (s->map[i] == c) return i;
        return SV_CODE(c);
    }
    return c;
}

/* jedec.c -- see jedec.h */
#include "jedec.h"
#include "rx.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

static int bws(int c) { return c == ' ' || (c >= 9 && c <= 13); }   /* whitespace of a byte string */
static int hexd(int c) { return (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') || (c >= 'A' && c <= 'F'); }

/* a decimal field value: the digits without leading zeros (as an integer prints) and its value, capped */
static char *dec_norm(const char *s, long long *v) {
    while (*s == '0' && s[1]) s++;
    size_t n = strlen(s);
    *v = n > 15 ? (long long)4e18 : atoll(s);
    return xstrdup(s);
}
static char *rep20(const char *f) { return py_repr_str(u8_prefix(f, 20)); }

void jed_parse(const unsigned char *d, unsigned long n, struct jedec *j) {
    static rx *rQ, *rF, *rL, *rWS, *rN01, *rC, *rG;
    if (!rQ) {
        rQ = rx_compile("Q([FPV])\\s*(\\d+)\\s*", 0); rF = rx_compile("F\\s*([01])\\s*", 0);
        rL = rx_compile("L\\s*(\\d+)\\s+(.*)", RX_S); rWS = rx_compile("\\s", 0); rN01 = rx_compile("[^01]", 0);
        rC = rx_compile("C\\s*([0-9A-Fa-f]{4})\\s*", 0); rG = rx_compile("G\\s*([01])\\s*", 0);
    }
    j->qf = -1; j->fuses = NULL; j->security = -1;
    const unsigned char *stx = memchr(d, 2, n);
    if (!stx) refuse("NO_STX", "no 0x02 byte");
    for (const unsigned char *p = d; p < stx; p++) if (!bws(*p)) refuse("TRAILING_GARBAGE", "non-whitespace before STX");
    const unsigned char *etx = memchr(stx, 3, n - (unsigned long)(stx - d));
    if (!etx) refuse("NO_ETX", "no 0x03 byte after STX");
    const unsigned char *tail = etx + 1; unsigned long tn = n - (unsigned long)(tail - d);
    int h4 = tn >= 4 && hexd(tail[0]) && hexd(tail[1]) && hexd(tail[2]) && hexd(tail[3]);
    int rest_ws = 1;
    if (h4) for (unsigned long i = 4; i < tn; i++) if (!bws(tail[i])) { rest_ws = 0; break; }
    if (!h4 || !rest_ws) {
        if (h4) refuse("TRAILING_GARBAGE", "bytes after the transmission checksum");
        refuse("BAD_XMIT_CHECKSUM_FORMAT", "%s", py_repr_bytes(tail, tn < 8 ? tn : 8));
    }
    char hx[5]; memcpy(hx, tail, 4); hx[4] = 0;
    unsigned field = (unsigned)strtoul(hx, NULL, 16), comp = 0;
    for (const unsigned char *p = stx; p <= etx; p++) comp += *p;
    comp &= 0xffff;
    if (field != 0 && field != comp) refuse("BAD_XMIT_CHECKSUM", "field %04X computed %04X", field, comp);
    char *text = latin1_to_u8(stx + 1, (size_t)(etx - stx - 1));
    unsigned char *covered = NULL; int dflt = -1, c_seen = 0; long cfield = -1;
    char *p = strchr(text, '*');
    if (!p) p = text + strlen(text); else p++;
    int more = *p || (p > text && p[-1] == '*');
    rxm m;
    while (more) {
        char *e = strchr(p, '*');
        char *raw = e ? xstrndup(p, (size_t)(e - p)) : xstrdup(p);
        if (e) p = e + 1; else more = 0;
        char *f = py_lstrip(raw);
        if (!*f) continue;
        char id = f[0];
        if (startswith(f, "QF") || startswith(f, "QP") || startswith(f, "QV")) {
            if (!rx_fullmatch(rQ, f, &m)) refuse("QF_BAD", "%s", rep20(f));
            char *ty = rx_group(&m, 1); long long v; dec_norm(rx_group(&m, 2), &v);
            if (*ty == 'F') {
                if (j->qf >= 0) refuse("QF_DUPLICATE", "%s", rep20(f));
                if (v > 50000000) fatal("JEDEC fuse count %lld is beyond this tool's limit", v);
                j->qf = (int)v; j->fuses = xcalloc((size_t)v + 1, 1); covered = xcalloc((size_t)v + 1, 1);
            }
        } else if (id == 'F') {
            if (!rx_fullmatch(rF, f, &m)) refuse("F_BAD", "%s", rep20(f));
            if (dflt >= 0) refuse("F_DUPLICATE", "%s", rep20(f));
            dflt = f[m.g[1][0]] - '0';
        } else if (id == 'L') {
            if (j->qf < 0) refuse("L_BEFORE_QF", "%s", rep20(f));
            if (!rx_fullmatch(rL, f, &m)) refuse("L_BAD", "%s", rep20(f));
            long long addr; char *as = dec_norm(rx_group(&m, 1), &addr);
            char *bits = rx_sub(rWS, rx_group(&m, 2), "", NULL, NULL);
            if (!*bits) refuse("L_BAD", "empty fuse list at L%s", as);
            rxm m2;
            if (rx_search(rN01, bits, 0, &m2)) refuse("L_BAD_FUSE_CHAR", "L%s", as);
            long long nb = (long long)strlen(bits);
            if (addr + nb > j->qf) refuse("L_BEYOND_QF", "L%s +%lld > QF%d", as, nb, j->qf);
            for (long long i = 0; i < nb; i++) if (covered[addr + i]) refuse("L_OVERLAP", "L%s +%lld", as, nb);
            for (long long i = 0; i < nb; i++) { j->fuses[addr + i] = bits[i] == '1'; covered[addr + i] = 1; }
        } else if (id == 'C') {
            if (!rx_fullmatch(rC, f, &m)) refuse("C_BAD", "%s", rep20(f));
            if (c_seen) refuse("C_DUPLICATE", "%s", rep20(f));
            c_seen = 1; cfield = strtol(rx_group(&m, 1), NULL, 16);
        } else if (id == 'G') {
            if (!rx_fullmatch(rG, f, &m)) refuse("G_BAD", "%s", rep20(f));
            j->security = f[m.g[1][0]] - '0';
        } else if (id == 'N' || strchr("XVPJDEU", id)) {
            /* notes and the fields this reader keeps without reading */
        } else refuse("FIELD_UNKNOWN", "%s", rep20(f));
    }
    if (j->qf < 0) refuse("QF_MISSING", "");
    if (dflt < 0) {
        for (int i = 0; i < j->qf; i++) if (!covered[i]) refuse("FUSE_UNDEFINED", "no F field; first uncovered fuse %d", i);
    } else for (int i = 0; i < j->qf; i++) if (!covered[i]) j->fuses[i] = (unsigned char)dflt;
    unsigned s = 0;
    for (int i = 0; i < j->qf; i += 8) { unsigned b = 0; for (int k = 0; k < 8 && i + k < j->qf; k++) if (j->fuses[i + k]) b |= 1u << k; s += b; }
    s &= 0xffff;
    if (cfield >= 0 && (unsigned)cfield != s) refuse("C_MISMATCH", "field %04lX computed %04X", cfield, s);
}

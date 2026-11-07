/* pof.c -- see pof.h */
#include "pof.h"
#include "util.h"
#include <string.h>

static unsigned u16(const unsigned char *p) { return (unsigned)p[0] | (unsigned)p[1] << 8; }
static unsigned long long u32(const unsigned char *p) { return (unsigned long long)p[0] | (unsigned long long)p[1] << 8 | (unsigned long long)p[2] << 16 | (unsigned long long)p[3] << 24; }

static unsigned crc16_x25(const unsigned char *d, size_t n) {
    unsigned crc = 0xffff;
    for (size_t i = 0; i < n; i++) {
        crc ^= d[i];
        for (int k = 0; k < 8; k++) crc = (crc & 1) ? (crc >> 1) ^ 0x8408 : crc >> 1;
    }
    return crc ^ 0xffff;
}

static int known_tag(unsigned t) { return (t >= 1 && t <= 10) || t == 12 || t == 14 || t == 16 || t == 17; }

static char *body_text(const unsigned char *b, size_t n) {
    const unsigned char *z = memchr(b, 0, n);
    return latin1_to_u8(b, z ? (size_t)(z - b) : n);
}

void pof_parse(const unsigned char *d, size_t n, struct pof *p) {
    memset(p, 0, sizeof *p);
    if (n < 12) refuse("SHORT_HEADER", "%zu bytes", n);
    if (memcmp(d, "POF\0", 4) != 0) refuse("BAD_MAGIC", "%s", py_repr_bytes(d, 4));
    unsigned z = u16(d + 4), one = u16(d + 6); unsigned long long cnt = u32(d + 8);
    if (z != 0 || one != 1) refuse("HEADER_FIELD", "u16 %u, u16 %u (measured 0, 1)", z, one);
    size_t k = 12; unsigned long long npk = 0;
    for (;;) {
        if (k + 6 > n) refuse(k == n ? "NO_TERMINATOR" : "PACKET_TRUNCATED", "at offset %zu", k);
        unsigned tag = u16(d + k); unsigned long long ln = u32(d + k + 2);
        if ((unsigned long long)k + 6 + ln > n) refuse("PACKET_TRUNCATED", "tag %u length %llu at offset %zu, file %zu", tag, ln, k, n);
        if (!known_tag(tag)) refuse("TAG_UNKNOWN", "tag %u at offset %zu", tag, k);
        const unsigned char *b = d + k + 6;
        npk++;
        if (tag == 1 || tag == 3) { /* creator, comment: kept by the format, unused here */ }
        else if (tag == 2) p->device = body_text(b, (size_t)ln);
        else if (tag == 17) {
            if (p->has_data) refuse("PACKET_DUPLICATE_TAG17", "second data packet at offset %zu", k);
            if (ln < 12) refuse("LAD32_SHAPE", "length %llu < 12", ln);
            unsigned a = u16(b); unsigned long long start = u32(b + 2), count = u32(b + 6); unsigned bb = u16(b + 10);
            unsigned long long want = 12 + (count + 7) / 8;
            if (a != 0 || bb != 1 || ln != want)
                refuse("LAD32_SHAPE", "u16 %u, start %llu, count %llu, u16 %u, length %llu (want %llu)", a, start, count, bb, ln, want);
            p->has_data = 1; p->start = start; p->count = count; p->data = b + 12;
        } else if (tag == 8) {
            if (npk != cnt || k + 6 + ln != n)
                refuse("TERMINATOR_NOT_LAST", "packet %llu of %llu, ends %llu of %zu", npk, cnt, (unsigned long long)k + 6 + ln, n);
            if (ln < 2) fatal("POF terminator packet shorter than its CRC field");
            unsigned field = u16(b), comp = crc16_x25(d, k + 6);
            if (field != 0 && field != comp) refuse("BAD_CRC", "field %04X computed %04X", field, comp);
            return;
        }
        k += 6 + (size_t)ln;
    }
}

int pof_bit(const struct pof *p, unsigned long long k) { return (p->data[k >> 3] >> (k & 7)) & 1; }

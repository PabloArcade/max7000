/* pof.h -- Altera POF (Programmer Object File) reader: file structure only.
 *
 * Header (12 bytes, little-endian): "POF\0", u16 0, u16 1, u32 packet count. Then packets back to back:
 * u16 tag, u32 length, body. Tags: 1 creator, 2 device name, 3 comment, 4 reserved, 5 security bit, 6 / 7 / 12
 * address-and-data variants, 8 terminator (u16 CRC-16/X25 over every byte before the CRC field; 0 = not checked),
 * 9 symbol table, 10 test vectors, 14 element count, 16 programming checksum, 17 logical address and data (32-bit):
 * u16 0, u32 start, u32 bit count, u16 1, then ceil(count / 8) bytes, bit k = (byte k/8 >> k%8) & 1.
 * The terminator must be the last packet and end the file. Only one data packet (tag 17) is accepted.
 */
#ifndef POF_H
#define POF_H
#include <stddef.h>
struct pof {
    char *device;                 /* device name (Latin-1 text as UTF-8), NULL when absent */
    int has_data;
    unsigned long long start, count;
    const unsigned char *data;    /* bit data of the tag-17 packet */
};
void pof_parse(const unsigned char *d, size_t n, struct pof *p);   /* refuses by name */
int pof_bit(const struct pof *p, unsigned long long k);
#endif

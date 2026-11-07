/* jedec.h -- JEDEC (JESD3) fuse file reader: file structure only.
 *
 * STX (0x02) ... ETX (0x03) followed by a 4-hex-digit transmission checksum (0000 = not checked; otherwise the 16-bit
 * sum of every byte from STX through ETX). Only whitespace before STX and after the checksum. The text between STX and
 * the first '*' is the design specification; then '*'-separated fields, identified by their first character:
 * QF / QP / QV counts, F default fuse state, L<address> <bits> fuse list, C<4 hex> fuse checksum (16-bit sum of the
 * fuses packed eight at a time, lowest fuse in the least significant bit), G security fuse, N note, and X V P J D E U
 * kept unread. Overlapping L fields are refused, as is a fuse left undefined with no F field.
 */
#ifndef JEDEC_H
#define JEDEC_H
struct jedec {
    int qf;              /* -1 = no QF field */
    unsigned char *fuses;
    int security;        /* -1 = no G field */
};
void jed_parse(const unsigned char *d, unsigned long n, struct jedec *j);   /* refuses by name */
#endif

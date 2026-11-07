/* sha256.h -- SHA-256 (FIPS 180-4) of a byte buffer, as lowercase hex */
#ifndef SHA256_H
#define SHA256_H
#include <stddef.h>
void sha256_hex(const unsigned char *p, size_t n, char out[65]);
#endif

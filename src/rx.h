/* rx.h -- a small backtracking regular-expression matcher.
 *
 * The output formats of this tool (Verilog atom netlists, CUPL equations, JEDEC fields) are produced and re-read with
 * regular expressions, and the exact match rules decide the exact text. This matcher implements the rules of the
 * common backtracking dialect: leftmost match, alternatives tried in order, greedy quantifiers longest first, lazy ones
 * shortest first.
 *
 * Supported: literals and escaped punctuation; . ; [...] and [^...] with ranges and \s \S \w \W \d \D inside;
 * \s \S \w \W \d \D \b \B; groups (...) and (?:...); | ; * + ? {n} {m,} {m,n} and their lazy forms; ^ and $.
 * Flags: RX_M (^ $ at line breaks), RX_S (. matches a line break), RX_I (ASCII / Latin-1 case folding),
 * RX_A (byte mode: one byte per character, ASCII classes). Text is UTF-8; one character = one code point.
 * $ without RX_M matches at the end or before a final line break.
 */
#ifndef RX_H
#define RX_H
#include <stddef.h>

enum { RX_M = 1, RX_S = 2, RX_I = 4, RX_A = 8 };
#define RX_MAXG 10

typedef struct rx rx;
typedef struct { int ok; size_t g[RX_MAXG][2]; int ng; const char *s; } rxm;  /* g[k] = byte span of group k; -1 unset */

rx *rx_compile(const char *pat, int flags);
int rx_match(rx *r, const char *s, rxm *m);                 /* at the start */
int rx_fullmatch(rx *r, const char *s, rxm *m);
int rx_search(rx *r, const char *s, size_t from, rxm *m);   /* first match at or after byte offset from */
int rx_match_n(rx *r, const char *s, size_t len, size_t pos, int full, rxm *m);  /* explicit length (bytes) */
int rx_search_n(rx *r, const char *s, size_t len, size_t from, rxm *m);
char *rx_group(const rxm *m, int k);                        /* copy of group k ("" if unset) */
int rx_gset(const rxm *m, int k);
/* substitute every non-overlapping match: fn(match, ctx) returns the replacement (NULL = literal repl) */
char *rx_sub(rx *r, const char *s, const char *repl, char *(*fn)(const rxm *, void *), void *ctx);
#endif

/* util.c -- see util.h */
#ifndef _WIN32
#define _POSIX_C_SOURCE 200809L
#endif
#include "util.h"

#include <errno.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#ifdef _WIN32
#include <direct.h>
#endif

/* ------------------------------------------------------------------ memory */
struct blk { struct blk *next; };
static struct blk *blocks;

void *xmalloc(size_t n) {
    struct blk *b = malloc(sizeof(struct blk) + (n ? n : 1));
    if (!b) { fprintf(stderr, "error: out of memory\n"); exit(1); }
    b->next = blocks; blocks = b;
    return b + 1;
}
void *xcalloc(size_t n, size_t m) { void *p = xmalloc(n * m); memset(p, 0, n * m); return p; }
char *xstrndup(const char *s, size_t n) { char *p = xmalloc(n + 1); memcpy(p, s, n); p[n] = 0; return p; }
char *xstrdup(const char *s) { return xstrndup(s, strlen(s)); }
void mem_release_all(void) {
    while (blocks) { struct blk *n = blocks->next; free(blocks); blocks = n; }
}

/* ------------------------------------------------------------------ strings */
void sb_init(sbuf *b) { b->s = NULL; b->n = b->cap = 0; }
static void sb_grow(sbuf *b, size_t need) {
    if (b->n + need + 1 <= b->cap) return;
    size_t c = b->cap ? b->cap * 2 : 64;
    while (c < b->n + need + 1) c *= 2;
    char *p = xmalloc(c);
    if (b->n) memcpy(p, b->s, b->n);
    b->s = p; b->cap = c;
}
void sb_addn(sbuf *b, const char *s, size_t n) { sb_grow(b, n); memcpy(b->s + b->n, s, n); b->n += n; b->s[b->n] = 0; }
void sb_add(sbuf *b, const char *s) { sb_addn(b, s, strlen(s)); }
void sb_addc(sbuf *b, char c) { sb_addn(b, &c, 1); }
void sb_printf(sbuf *b, const char *f, ...) {
    va_list ap; va_start(ap, f);
    va_list aq; va_copy(aq, ap);
    int k = vsnprintf(NULL, 0, f, ap); va_end(ap);
    sb_grow(b, (size_t)k);
    vsnprintf(b->s + b->n, (size_t)k + 1, f, aq); va_end(aq);
    b->n += (size_t)k;
}
char *sb_take(sbuf *b) { char *s = b->s ? b->s : xstrdup(""); sb_init(b); return s; }
char *fmt(const char *f, ...) {
    va_list ap; va_start(ap, f);
    va_list aq; va_copy(aq, ap);
    int k = vsnprintf(NULL, 0, f, ap); va_end(ap);
    char *p = xmalloc((size_t)k + 1);
    vsnprintf(p, (size_t)k + 1, f, aq); va_end(aq);
    return p;
}

/* ------------------------------------------------------------------ lists */
void pl_push(plist *l, void *p) {
    if (l->n == l->cap) { int c = l->cap ? l->cap * 2 : 8; void **v = xmalloc(sizeof(void *) * (size_t)c); if (l->n) memcpy(v, l->v, sizeof(void *) * (size_t)l->n); l->v = v; l->cap = c; }
    l->v[l->n++] = p;
}
void sl_push_own(slist *l, char *s) { pl_push((plist *)l, s); }
void sl_push(slist *l, const char *s) { sl_push_own(l, xstrdup(s)); }
int sl_find(const slist *l, const char *s) { for (int i = 0; i < l->n; i++) if (!strcmp(l->v[i], s)) return i; return -1; }
static int cmp_str(const void *a, const void *b) { return strcmp(*(char *const *)a, *(char *const *)b); }
void sl_sort(slist *l) { if (l->n > 1) qsort(l->v, (size_t)l->n, sizeof(char *), cmp_str); }
char *sl_join(const slist *l, const char *sep) {
    sbuf b; sb_init(&b);
    for (int i = 0; i < l->n; i++) { if (i) sb_add(&b, sep); sb_add(&b, l->v[i]); }
    return sb_take(&b);
}
void il_push(ilist *l, int x) {
    if (l->n == l->cap) { int c = l->cap ? l->cap * 2 : 8; int *v = xmalloc(sizeof(int) * (size_t)c); if (l->n) memcpy(v, l->v, sizeof(int) * (size_t)l->n); l->v = v; l->cap = c; }
    l->v[l->n++] = x;
}
int il_find(const ilist *l, int x) { for (int i = 0; i < l->n; i++) if (l->v[i] == x) return i; return -1; }
static int cmp_int(const void *a, const void *b) { int x = *(const int *)a, y = *(const int *)b; return (x > y) - (x < y); }
void il_sort(ilist *l) { if (l->n > 1) qsort(l->v, (size_t)l->n, sizeof(int), cmp_int); }
void il_uniq(ilist *l) {
    il_sort(l); int k = 0;
    for (int i = 0; i < l->n; i++) if (!k || l->v[k - 1] != l->v[i]) l->v[k++] = l->v[i];
    l->n = k;
}

/* ------------------------------------------------------------------ refusals */
struct catcher *catch_top;
char refusal_name[64];
char *refusal_detail;
void refuse(const char *name, const char *f, ...) {
    snprintf(refusal_name, sizeof refusal_name, "%s", name);
    va_list ap; va_start(ap, f);
    va_list aq; va_copy(aq, ap);
    int k = vsnprintf(NULL, 0, f, ap); va_end(ap);
    refusal_detail = xmalloc((size_t)k + 1);
    vsnprintf(refusal_detail, (size_t)k + 1, f, aq); va_end(aq);
    if (!catch_top) { fprintf(stderr, "error: refusal %s outside a catcher\n", name); exit(1); }
    longjmp(catch_top->jb, 1);
}
void fatal(const char *f, ...) {
    va_list ap; va_start(ap, f);
    fprintf(stderr, "error: ");
    vfprintf(stderr, f, ap); va_end(ap);
    fprintf(stderr, "\n");
    exit(1);
}

/* ------------------------------------------------------------------ UTF-8 and Python str rules */
int u8_next(const char *s, int *len) {
    const unsigned char *p = (const unsigned char *)s;
    if (p[0] < 0x80) { *len = 1; return p[0]; }
    int n = p[0] >= 0xf0 ? 4 : p[0] >= 0xe0 ? 3 : p[0] >= 0xc0 ? 2 : 0;
    if (!n) { *len = 1; return p[0]; }
    int cp = p[0] & (0x3f >> (n - 1));
    for (int i = 1; i < n; i++) {
        if ((p[i] & 0xc0) != 0x80) { *len = 1; return p[0]; }
        cp = (cp << 6) | (p[i] & 0x3f);
    }
    *len = n; return cp;
}
int u8_len(const char *s) { int n = 0, l; while (*s) { u8_next(s, &l); s += l; n++; } return n; }
char *u8_prefix(const char *s, int ncp) {
    const char *p = s; int l;
    while (*p && ncp > 0) { u8_next(p, &l); p += l; ncp--; }
    return xstrndup(s, (size_t)(p - s));
}
void u8_put(sbuf *b, int cp) {
    char c[4];
    if (cp == 0) { sb_addn(b, "\xc0\x80", 2); return; }   /* U+0000 in two bytes, so that a C string never ends inside text */
    if (cp < 0x80) { c[0] = (char)cp; sb_addn(b, c, 1); }
    else if (cp < 0x800) { c[0] = (char)(0xc0 | (cp >> 6)); c[1] = (char)(0x80 | (cp & 0x3f)); sb_addn(b, c, 2); }
    else if (cp < 0x10000) { c[0] = (char)(0xe0 | (cp >> 12)); c[1] = (char)(0x80 | ((cp >> 6) & 0x3f)); c[2] = (char)(0x80 | (cp & 0x3f)); sb_addn(b, c, 3); }
    else { c[0] = (char)(0xf0 | (cp >> 18)); c[1] = (char)(0x80 | ((cp >> 12) & 0x3f)); c[2] = (char)(0x80 | ((cp >> 6) & 0x3f)); c[3] = (char)(0x80 | (cp & 0x3f)); sb_addn(b, c, 4); }
}
char *latin1_to_u8(const unsigned char *p, size_t n) {
    sbuf b; sb_init(&b);
    for (size_t i = 0; i < n; i++) u8_put(&b, p[i]);
    return sb_take(&b);
}
int py_isspace(int cp) {
    if (cp < 0x100) return (cp >= 9 && cp <= 13) || (cp >= 0x1c && cp <= 0x20) || cp == 0x85 || cp == 0xa0;
    return cp == 0x1680 || (cp >= 0x2000 && cp <= 0x200a) || cp == 0x2028 || cp == 0x2029 || cp == 0x202f || cp == 0x205f || cp == 0x3000;
}
int py_isdigit(int cp) { return cp >= '0' && cp <= '9'; }
int py_isword(int cp) {
    if (cp < 0x80) return (cp >= '0' && cp <= '9') || (cp >= 'A' && cp <= 'Z') || (cp >= 'a' && cp <= 'z') || cp == '_';
    if (cp < 0x100) return cp == 0xaa || cp == 0xb2 || cp == 0xb3 || cp == 0xb5 || cp == 0xb9 || cp == 0xba || (cp >= 0xbc && cp <= 0xbe) ||
                           (cp >= 0xc0 && cp <= 0xd6) || (cp >= 0xd8 && cp <= 0xf6) || cp >= 0xf8;
    /* above Latin-1: letters and digits of most scripts; spaces and the punctuation blocks are not word characters */
    return !py_isspace(cp) && !(cp >= 0x2000 && cp <= 0x206f) && !(cp >= 0x3000 && cp <= 0x303f);
}
char *py_upper(const char *s) {
    sbuf b; sb_init(&b); int l;
    while (*s) {
        int c = u8_next(s, &l); s += l;
        if (c >= 'a' && c <= 'z') c -= 32;
        else if (c == 0xb5) c = 0x39c;
        else if (c == 0xdf) { sb_add(&b, "SS"); continue; }
        else if ((c >= 0xe0 && c <= 0xf6) || (c >= 0xf8 && c <= 0xfe)) c -= 32;
        else if (c == 0xff) c = 0x178;
        u8_put(&b, c);
    }
    return sb_take(&b);
}
char *py_lower(const char *s) {
    sbuf b; sb_init(&b); int l;
    while (*s) {
        int c = u8_next(s, &l); s += l;
        if ((c >= 'A' && c <= 'Z') || (c >= 0xc0 && c <= 0xd6) || (c >= 0xd8 && c <= 0xde)) c += 32;
        u8_put(&b, c);
    }
    return sb_take(&b);
}
static int in_chars(int cp, const char *chars) {
    int l;
    if (!chars) return py_isspace(cp);
    while (*chars) { if (u8_next(chars, &l) == cp) return 1; chars += l; }
    return 0;
}
char *py_strip_chars(const char *s, const char *chars, int left, int right) {
    const char *a = s, *e = s + strlen(s); int l;
    if (left) while (*a) { int c = u8_next(a, &l); if (!in_chars(c, chars)) break; a += l; }
    if (right) {
        /* walk forward remembering the end of the last character that is not stripped */
        const char *p = a, *last = a;
        while (p < e) { int c = u8_next(p, &l); p += l; if (!in_chars(c, chars)) last = p; }
        e = last;
    }
    return xstrndup(a, (size_t)(e - a));
}
char *py_strip(const char *s) { return py_strip_chars(s, NULL, 1, 1); }
char *py_lstrip(const char *s) { return py_strip_chars(s, NULL, 1, 0); }
char *py_rstrip(const char *s) { return py_strip_chars(s, NULL, 0, 1); }
int startswith(const char *s, const char *p) { return strncmp(s, p, strlen(p)) == 0; }
int endswith(const char *s, const char *p) { size_t a = strlen(s), b = strlen(p); return a >= b && !strcmp(s + a - b, p); }
static int py_printable(int cp) {
    if (cp < 0x20 || cp == 0x7f) return 0;
    if (cp >= 0x80 && cp <= 0xa0) return 0;
    if (cp == 0xad) return 0;
    return 1;
}
char *py_repr_str(const char *s) {
    int sq = strchr(s, '\'') != NULL, dq = strchr(s, '"') != NULL;
    char q = (sq && !dq) ? '"' : '\'';
    sbuf b; sb_init(&b); sb_addc(&b, q); int l;
    while (*s) {
        int c = u8_next(s, &l); s += l;
        if (c == q || c == '\\') { sb_addc(&b, '\\'); sb_addc(&b, (char)c); }
        else if (c == '\t') sb_add(&b, "\\t");
        else if (c == '\n') sb_add(&b, "\\n");
        else if (c == '\r') sb_add(&b, "\\r");
        else if (!py_printable(c)) sb_printf(&b, c < 0x100 ? "\\x%02x" : c < 0x10000 ? "\\u%04x" : "\\U%08x", c);
        else u8_put(&b, c);
    }
    sb_addc(&b, q);
    return sb_take(&b);
}
char *py_repr_bytes(const unsigned char *p, size_t n) {
    int sq = memchr(p, '\'', n) != NULL, dq = memchr(p, '"', n) != NULL;
    char q = (sq && !dq) ? '"' : '\'';
    sbuf b; sb_init(&b); sb_addc(&b, 'b'); sb_addc(&b, q);
    for (size_t i = 0; i < n; i++) {
        int c = p[i];
        if (c == q || c == '\\') { sb_addc(&b, '\\'); sb_addc(&b, (char)c); }
        else if (c == '\t') sb_add(&b, "\\t");
        else if (c == '\n') sb_add(&b, "\\n");
        else if (c == '\r') sb_add(&b, "\\r");
        else if (c < 0x20 || c >= 0x7f) sb_printf(&b, "\\x%02x", c);
        else sb_addc(&b, (char)c);
    }
    sb_addc(&b, q);
    return sb_take(&b);
}
char *replace_all(const char *s, const char *a, const char *b) {
    sbuf o; sb_init(&o); size_t la = strlen(a);
    if (!la) return xstrdup(s);
    for (;;) {
        const char *p = strstr(s, a);
        if (!p) { sb_add(&o, s); break; }
        sb_addn(&o, s, (size_t)(p - s)); sb_add(&o, b); s = p + la;
    }
    return sb_take(&o);
}

/* ------------------------------------------------------------------ files */
unsigned char *read_file(const char *path, size_t *n) {
    FILE *f = fopen(path, "rb");
    if (!f) return NULL;
    sbuf b; sb_init(&b); char buf[65536]; size_t k;
    while ((k = fread(buf, 1, sizeof buf, f)) > 0) sb_addn(&b, buf, k);
    fclose(f);
    *n = b.n;
    return (unsigned char *)sb_take(&b);
}
int put_text(FILE *f, const char *text) {
    /* the two-byte form of U+0000 (see u8_put) is written as the 0 byte it stands for */
    int ok = 1;
    for (const char *p = text;;) {
        const char *z = strstr(p, "\xc0\x80");
        size_t n = z ? (size_t)(z - p) : strlen(p);
        if (n && fwrite(p, 1, n, f) != n) ok = 0;
        if (!z) break;
        if (fputc(0, f) == EOF) ok = 0;
        p = z + 2;
    }
    return ok ? 0 : -1;
}
int write_text_file(const char *path, const char *text) {
    FILE *f = fopen(path, "w");
    if (!f) return -1;
    int ok = put_text(f, text) == 0;
    return (fclose(f) == 0 && ok) ? 0 : -1;
}
static int is_sep(char c) {
#ifdef _WIN32
    return c == '/' || c == '\\';
#else
    return c == '/';
#endif
}
int mkdir_p(const char *path) {
    char *p = xstrdup(path); size_t n = strlen(p);
    for (size_t i = 1; i <= n; i++) {
        if (i == n || is_sep(p[i])) {
            char c = p[i]; p[i] = 0;
            if (*p && !(i == 2 && p[1] == ':')) {
#ifdef _WIN32
                int r = _mkdir(p);
#else
                int r = mkdir(p, 0777);
#endif
                if (r != 0 && errno != EEXIST) return -1;
            }
            p[i] = c;
        }
    }
    struct stat st;
    return (stat(path, &st) == 0 && S_ISDIR(st.st_mode)) ? 0 : -1;
}
char *path_join(const char *a, const char *b) {
    size_t n = strlen(a);
    if (!n || is_sep(a[n - 1])) return fmt("%s%s", a, b);
#ifdef _WIN32
    return fmt("%s\\%s", a, b);
#else
    return fmt("%s/%s", a, b);
#endif
}
const char *path_base(const char *p) {
    const char *b = p;
    for (const char *q = p; *q; q++) if (is_sep(*q)) b = q + 1;
    return b;
}

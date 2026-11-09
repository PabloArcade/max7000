/* util.h -- memory, growable strings and lists, refusals, and the text rules the output formats depend on.
 *
 * Text inside the program is UTF-8. Fuse files are read as bytes; their text parts (JEDEC fields, the POF device name)
 * are taken one byte per character (Latin-1) and stored as UTF-8, so every character keeps its identity when it is
 * printed into a report. U+0000 is stored as the two bytes 0xC0 0x80 and written back as one 0 byte. Character classes (space, word character, upper / lower case) follow the Unicode rules for
 * code points 0..255; above 255 only ASCII-compatible behaviour is defined.
 */
#ifndef UTIL_H
#define UTIL_H

#include <setjmp.h>
#include <stddef.h>
#include <stdio.h>

#define TOOL_NAME "max7000"
#define TOOL_VERSION "v3.28"
#define TOOL_VER TOOL_NAME " " TOOL_VERSION          /* written into every emitted file */
#define PROG_NAME "max7000dump"                      /* the program name the usage screen prints */
#define BANNER "max7000 decoder tool by Darksoft " TOOL_VERSION ". Use at your own risk."
#define CREDIT "DARKSOFT"

/* ---- memory: every block is kept on one list and released at exit */
void *xmalloc(size_t n);
void *xcalloc(size_t n, size_t m);
char *xstrdup(const char *s);
char *xstrndup(const char *s, size_t n);
void mem_release_all(void);

/* ---- growable string */
typedef struct { char *s; size_t n, cap; } sbuf;
void sb_init(sbuf *b);
void sb_addn(sbuf *b, const char *s, size_t n);
void sb_add(sbuf *b, const char *s);
void sb_addc(sbuf *b, char c);
void sb_printf(sbuf *b, const char *fmt, ...);
char *sb_take(sbuf *b);                  /* the text; the buffer is reset */
char *fmt(const char *f, ...);           /* formatted copy */

/* ---- list of strings / pointers */
typedef struct { void **v; int n, cap; } plist;
void pl_push(plist *l, void *p);
#define PL_GET(l, i) ((l)->v[i])
typedef struct { char **v; int n, cap; } slist;
void sl_push(slist *l, const char *s);   /* stores a copy */
void sl_push_own(slist *l, char *s);     /* stores the pointer */
int sl_find(const slist *l, const char *s);
void sl_sort(slist *l);                  /* byte order, as Python sorts str */
char *sl_join(const slist *l, const char *sep);

/* ---- integer list / set */
typedef struct { int *v; int n, cap; } ilist;
void il_push(ilist *l, int x);
int il_find(const ilist *l, int x);
void il_sort(ilist *l);
void il_uniq(ilist *l);

/* ---- refusals: a named stop, the tool's documented outcome for anything not decoded.
 * refuse() records the name and detail and jumps to the active catcher. fatal() is the outcome where the reference
 * stops without a report (an internal limit, a missing table): exit code 1, no REPORT. */
struct catcher { jmp_buf jb; struct catcher *prev; };
extern struct catcher *catch_top;
extern char refusal_name[64];
extern char *refusal_detail;
void refuse(const char *name, const char *f, ...);
void fatal(const char *f, ...);
/* On 64-bit Windows the mingw-w64 setjmp records a frame for structured-exception unwinding, and its longjmp then
 * unwinds through that frame -- which ends the process when it fails. With a null frame longjmp just restores the
 * saved registers, as on every other platform. */
#if defined(_WIN64) && defined(__MINGW64__)
#define TOOL_SETJMP(b) _setjmp((b), NULL)
#else
#define TOOL_SETJMP(b) setjmp(b)
#endif
#define TRY(c) ((c).prev = catch_top, catch_top = &(c), TOOL_SETJMP((c).jb) == 0)
#define ENDTRY(c) (catch_top = (c).prev)

/* ---- UTF-8 text with Python's str semantics */
int u8_next(const char *s, int *len);    /* code point at s (invalid byte: that byte), its byte length */
int u8_len(const char *s);               /* number of code points */
char *u8_prefix(const char *s, int ncp); /* first ncp code points (Python s[:n]) */
void u8_put(sbuf *b, int cp);
char *latin1_to_u8(const unsigned char *p, size_t n);
int py_isspace(int cp);
int py_isword(int cp);                   /* regex \w on str */
int py_isdigit(int cp);                  /* regex \d on str */
char *py_upper(const char *s);
char *py_lower(const char *s);
char *py_strip(const char *s);           /* str.strip(): Python whitespace */
char *py_lstrip(const char *s);
char *py_rstrip(const char *s);
char *py_strip_chars(const char *s, const char *chars, int left, int right);
int startswith(const char *s, const char *p);
int endswith(const char *s, const char *p);
char *py_repr_str(const char *s);        /* repr() of a str */
char *py_repr_bytes(const unsigned char *p, size_t n);   /* repr() of bytes */
char *replace_all(const char *s, const char *a, const char *b);

/* ---- files */
unsigned char *read_file(const char *path, size_t *n);
int write_text_file(const char *path, const char *text);   /* text mode, as the reference writes */
int put_text(FILE *f, const char *text);                    /* the same rule on an open stream */
int mkdir_p(const char *path);
char *path_join(const char *a, const char *b);
const char *path_base(const char *p);

#endif

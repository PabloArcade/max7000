/* rx.c -- see rx.h */
#include "rx.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>

enum { N_CHAR, N_ANY, N_CLASS, N_BOL, N_EOL, N_WB, N_NWB, N_GROUP, N_REP };
enum { C_SPACE = 1, C_NSPACE = 2, C_WORD = 4, C_NWORD = 8, C_DIGIT = 16, C_NDIGIT = 32 };

typedef struct seq seq;
typedef struct node {
    int t, cp;
    int neg, cflags, nr; int *lo, *hi;          /* class */
    int g; seq **alts; int nalt;                /* group */
    int min, max, lazy; struct node *child; seq *cseq;   /* repeat */
} node;
struct seq { node **v; int n, cap; };
struct rx { seq *top; int ng, flags; int first; /* literal first character or -1 */ int bol; };

/* ------------------------------------------------------------------ parser */
typedef struct { const char *p; int flags, ng; const char *pat; } ps;

static void seq_push(seq *s, node *n) {
    if (s->n == s->cap) { int c = s->cap ? s->cap * 2 : 8; node **v = xmalloc(sizeof(node *) * (size_t)c); if (s->n) memcpy(v, s->v, sizeof(node *) * (size_t)s->n); s->v = v; s->cap = c; }
    s->v[s->n++] = n;
}
static node *nnew(int t) { node *n = xcalloc(1, sizeof(node)); n->t = t; n->g = -1; return n; }
static seq *parse_alts(ps *P, node *grp);

static int class_flag(char c) {
    switch (c) {
    case 's': return C_SPACE; case 'S': return C_NSPACE; case 'w': return C_WORD; case 'W': return C_NWORD;
    case 'd': return C_DIGIT; case 'D': return C_NDIGIT; default: return 0;
    }
}
static int esc_char(char c) { return c == 'n' ? '\n' : c == 't' ? '\t' : c == 'r' ? '\r' : c == 'f' ? '\f' : c == 'v' ? '\v' : (unsigned char)c; }

static void cls_add(node *n, int lo, int hi) {
    int *a = xmalloc(sizeof(int) * (size_t)(n->nr + 1)), *b = xmalloc(sizeof(int) * (size_t)(n->nr + 1));
    if (n->nr) { memcpy(a, n->lo, sizeof(int) * (size_t)n->nr); memcpy(b, n->hi, sizeof(int) * (size_t)n->nr); }
    a[n->nr] = lo; b[n->nr] = hi; n->lo = a; n->hi = b; n->nr++;
}
static int pat_char(ps *P) { int l; int c = u8_next(P->p, &l); P->p += l; return c; }

static node *parse_class(ps *P) {
    node *n = nnew(N_CLASS);
    if (*P->p == '^') { n->neg = 1; P->p++; }
    int first = 1;
    while (*P->p && (*P->p != ']' || first)) {
        first = 0;
        int lo;
        if (*P->p == '\\') {
            P->p++;
            int f = class_flag(*P->p);
            if (f) { n->cflags |= f; P->p++; continue; }
            lo = esc_char(*P->p); P->p++;
        } else lo = pat_char(P);
        int hi = lo;
        if (P->p[0] == '-' && P->p[1] && P->p[1] != ']') {
            P->p++;
            if (*P->p == '\\') { P->p++; hi = esc_char(*P->p); P->p++; } else hi = pat_char(P);
        }
        cls_add(n, lo, hi);
    }
    if (*P->p != ']') fatal("pattern: unterminated class in %s", P->pat);
    P->p++;
    return n;
}

static node *parse_atom(ps *P) {
    char c = *P->p;
    if (c == '(') {
        P->p++;
        node *n = nnew(N_GROUP);
        if (P->p[0] == '?' && P->p[1] == ':') P->p += 2;
        else { n->g = ++P->ng; if (n->g >= RX_MAXG) fatal("pattern: too many groups in %s", P->pat); }
        parse_alts(P, n);
        if (*P->p != ')') fatal("pattern: missing ) in %s", P->pat);
        P->p++;
        return n;
    }
    if (c == '[') { P->p++; return parse_class(P); }
    if (c == '.') { P->p++; return nnew(N_ANY); }
    if (c == '^') { P->p++; return nnew(N_BOL); }
    if (c == '$') { P->p++; return nnew(N_EOL); }
    if (c == '\\') {
        P->p++;
        char e = *P->p++;
        if (e == 'b') return nnew(N_WB);
        if (e == 'B') return nnew(N_NWB);
        int f = class_flag(e);
        if (f) { node *n = nnew(N_CLASS); n->cflags = f; return n; }
        node *n = nnew(N_CHAR); n->cp = esc_char(e); return n;
    }
    node *n = nnew(N_CHAR); n->cp = pat_char(P); return n;
}

static int parse_int(ps *P, int *v) {
    if (*P->p < '0' || *P->p > '9') return 0;
    int x = 0; while (*P->p >= '0' && *P->p <= '9') x = x * 10 + (*P->p++ - '0');
    *v = x; return 1;
}

static seq *parse_seq(ps *P) {
    seq *s = xcalloc(1, sizeof(seq));
    while (*P->p && *P->p != '|' && *P->p != ')') {
        node *a = parse_atom(P);
        int mn = -1, mx = -1;
        if (*P->p == '*') { mn = 0; mx = -2; P->p++; }
        else if (*P->p == '+') { mn = 1; mx = -2; P->p++; }
        else if (*P->p == '?') { mn = 0; mx = 1; P->p++; }
        else if (*P->p == '{') {
            const char *save = P->p; P->p++;
            int a1, a2;
            if (parse_int(P, &a1)) {
                if (*P->p == '}') { mn = mx = a1; P->p++; }
                else if (*P->p == ',') {
                    P->p++;
                    if (*P->p == '}') { mn = a1; mx = -2; P->p++; }
                    else if (parse_int(P, &a2) && *P->p == '}') { mn = a1; mx = a2; P->p++; }
                    else P->p = save;
                } else P->p = save;
            } else P->p = save;
            if (P->p == save) { /* a literal brace */ node *n = nnew(N_CHAR); n->cp = '{'; P->p++; seq_push(s, a); a = n; }
        }
        if (mn >= 0) {
            node *r = nnew(N_REP); r->min = mn; r->max = mx == -2 ? 0x7fffffff : mx; r->child = a;
            if (*P->p == '?') { r->lazy = 1; P->p++; }
            r->cseq = xcalloc(1, sizeof(seq)); seq_push(r->cseq, a);
            a = r;
        }
        seq_push(s, a);
    }
    return s;
}

static seq *parse_alts(ps *P, node *grp) {
    seq **alts = NULL; int n = 0;
    for (;;) {
        seq *s = parse_seq(P);
        seq **a = xmalloc(sizeof(seq *) * (size_t)(n + 1));
        if (n) memcpy(a, alts, sizeof(seq *) * (size_t)n);
        a[n++] = s; alts = a;
        if (*P->p != '|') break;
        P->p++;
    }
    grp->alts = alts; grp->nalt = n;
    return alts[0];
}

rx *rx_compile(const char *pat, int flags) {
    ps P = { pat, flags, 0, pat };
    node *g = nnew(N_GROUP); g->g = 0;
    parse_alts(&P, g);
    if (*P.p) fatal("pattern: unexpected %c in %s", *P.p, pat);
    rx *r = xcalloc(1, sizeof(rx));
    r->top = xcalloc(1, sizeof(seq)); seq_push(r->top, g);
    r->ng = P.ng + 1; r->flags = flags; r->first = -1;
    if (g->nalt == 1 && g->alts[0]->n) {
        node *f = g->alts[0]->v[0];
        if (f->t == N_CHAR && !(flags & RX_I)) r->first = f->cp;
        if (f->t == N_BOL) r->bol = 1;
    }
    return r;
}

/* ------------------------------------------------------------------ matcher */
typedef struct { const char *s; size_t len; int flags, full; size_t cap[RX_MAXG][2]; size_t end; } st;
typedef struct K { int t; const seq *sq; int i; const node *n; int count; size_t prev, start; int g; const struct K *next; } K;
enum { K_GCLOSE, K_REP };

static int ch_at(const st *S, size_t pos, int *l) {
    if (pos >= S->len) { *l = 0; return -1; }
    if (S->flags & RX_A) { *l = 1; return (unsigned char)S->s[pos]; }
    int c = u8_next(S->s + pos, l);
    if (pos + (size_t)*l > S->len) { *l = 1; c = (unsigned char)S->s[pos]; }
    return c;
}
static int ch_before(const st *S, size_t pos) {
    if (pos == 0) return -1;
    if (S->flags & RX_A) return (unsigned char)S->s[pos - 1];
    size_t p = pos - 1; int k = 0;
    while (p > 0 && k < 3 && ((unsigned char)S->s[p] & 0xc0) == 0x80) { p--; k++; }
    int l; int c = u8_next(S->s + p, &l);
    if (p + (size_t)l != pos) return (unsigned char)S->s[pos - 1];
    return c;
}
static int isw(const st *S, int c) { if (c < 0) return 0; if (S->flags & RX_A) return c < 128 && py_isword(c); return py_isword(c); }
static int iss(const st *S, int c) { if (S->flags & RX_A) return c == ' ' || (c >= 9 && c <= 13); return py_isspace(c); }
static int fold(int c) {
    if ((c >= 'A' && c <= 'Z') || (c >= 0xc0 && c <= 0xde && c != 0xd7)) return c + 32;
    return c;
}
static int cls_has(const st *S, const node *n, int c) {
    int in = 0;
    if ((n->cflags & C_SPACE) && iss(S, c)) in = 1;
    if ((n->cflags & C_NSPACE) && !iss(S, c)) in = 1;
    if ((n->cflags & C_WORD) && isw(S, c)) in = 1;
    if ((n->cflags & C_NWORD) && !isw(S, c)) in = 1;
    if ((n->cflags & C_DIGIT) && py_isdigit(c)) in = 1;
    if ((n->cflags & C_NDIGIT) && !py_isdigit(c)) in = 1;
    for (int i = 0; !in && i < n->nr; i++) {
        if (c >= n->lo[i] && c <= n->hi[i]) in = 1;
        else if (S->flags & RX_I) { int f = fold(c); if ((f >= fold(n->lo[i]) && f <= fold(n->hi[i]))) in = 1; }
    }
    return n->neg ? !in : in;
}
static int atom(const st *S, const node *n, size_t pos, int *l) {
    int c = ch_at(S, pos, l);
    if (c < 0) return 0;
    switch (n->t) {
    case N_CHAR: return c == n->cp || ((S->flags & RX_I) && fold(c) == fold(n->cp));
    case N_ANY: return c != '\n' || (S->flags & RX_S);
    case N_CLASS: return cls_has(S, n, c);
    }
    return 0;
}
static int simple(const node *n) { return n->t == N_CHAR || n->t == N_ANY || n->t == N_CLASS; }

static int run(st *S, const seq *sq, int i, size_t pos, const K *k);
static int rep(st *S, const node *n, int count, size_t pos, size_t prev, const seq *sq, int i, const K *k);

static int cont(st *S, size_t pos, const K *k) {
    if (!k) {
        if (S->full && pos != S->len) return 0;
        S->end = pos; return 1;
    }
    if (k->t == K_GCLOSE) {
        size_t o0 = 0, o1 = 0;
        if (k->g >= 0) { o0 = S->cap[k->g][0]; o1 = S->cap[k->g][1]; S->cap[k->g][0] = k->start; S->cap[k->g][1] = pos; }
        if (run(S, k->sq, k->i, pos, k->next)) return 1;
        if (k->g >= 0) { S->cap[k->g][0] = o0; S->cap[k->g][1] = o1; }
        return 0;
    }
    return rep(S, k->n, k->count + 1, pos, k->prev, k->sq, k->i, k->next);
}

static int run(st *S, const seq *sq, int i, size_t pos, const K *k) {
    int l;
    while (i < sq->n) {
        const node *n = sq->v[i];
        switch (n->t) {
        case N_CHAR: case N_ANY: case N_CLASS:
            if (!atom(S, n, pos, &l)) return 0;
            pos += (size_t)l; i++; continue;
        case N_BOL:
            if (!(pos == 0 || ((S->flags & RX_M) && S->s[pos - 1] == '\n'))) return 0;
            i++; continue;
        case N_EOL:
            if (!(pos == S->len || (S->s[pos] == '\n' && ((S->flags & RX_M) || pos + 1 == S->len)))) return 0;
            i++; continue;
        case N_WB: case N_NWB: {
            int a = isw(S, ch_before(S, pos)), b = isw(S, ch_at(S, pos, &l));
            if ((a != b) != (n->t == N_WB)) return 0;
            i++; continue;
        }
        case N_GROUP: {
            K f = { K_GCLOSE, sq, i + 1, n, 0, 0, pos, n->g, k };
            for (int a = 0; a < n->nalt; a++) if (run(S, n->alts[a], 0, pos, &f)) return 1;
            return 0;
        }
        case N_REP:
            return rep(S, n, 0, pos, (size_t)-1, sq, i + 1, k);
        }
    }
    return cont(S, pos, k);
}

static int rep(st *S, const node *n, int count, size_t pos, size_t prev, const seq *sq, int i, const K *k) {
    const node *c = n->child; int l;
    if (simple(c)) {
        if (n->lazy) {
            int j = 0; size_t p = pos;
            for (;;) {
                if (j >= n->min && run(S, sq, i, p, k)) return 1;
                if (j >= n->max || !atom(S, c, p, &l)) return 0;
                p += (size_t)l; j++;
            }
        }
        size_t stackbuf[256]; size_t *P = stackbuf; size_t cap = 256; int cnt = 0;
        P[0] = pos; size_t p = pos;
        while (cnt < n->max && atom(S, c, p, &l)) {
            p += (size_t)l; cnt++;
            if ((size_t)cnt >= cap) {
                size_t nc = cap * 2; size_t *q = malloc(sizeof(size_t) * nc);
                if (!q) fatal("out of memory");
                memcpy(q, P, sizeof(size_t) * cap); if (P != stackbuf) free(P); P = q; cap = nc;
            }
            P[cnt] = p;
        }
        int ok = 0;
        for (int j = cnt; j >= n->min; j--) if (run(S, sq, i, P[j], k)) { ok = 1; break; }
        if (P != stackbuf) free(P);
        return ok;
    }
    if (count > 0 && pos == prev) return count >= n->min ? run(S, sq, i, pos, k) : 0;   /* an empty pass ends the loop */
    K f = { K_REP, sq, i, n, count, pos, 0, -1, k };
    if (n->lazy) {
        if (count >= n->min && run(S, sq, i, pos, k)) return 1;
        return count < n->max && run(S, n->cseq, 0, pos, &f);
    }
    if (count < n->max && run(S, n->cseq, 0, pos, &f)) return 1;
    return count >= n->min && run(S, sq, i, pos, k);
}

static int try_at(rx *r, const char *s, size_t len, size_t pos, int full, rxm *m) {
    st S; S.s = s; S.len = len; S.flags = r->flags; S.full = full;
    for (int g = 0; g < RX_MAXG; g++) S.cap[g][0] = S.cap[g][1] = (size_t)-1;
    if (!run(&S, r->top, 0, pos, NULL)) return 0;
    m->ok = 1; m->s = s; m->ng = r->ng;
    for (int g = 0; g < RX_MAXG; g++) { m->g[g][0] = S.cap[g][0]; m->g[g][1] = S.cap[g][1]; }
    m->g[0][0] = pos; m->g[0][1] = S.end;
    return 1;
}
int rx_match_n(rx *r, const char *s, size_t len, size_t pos, int full, rxm *m) { m->ok = 0; return try_at(r, s, len, pos, full, m); }
int rx_match(rx *r, const char *s, rxm *m) { return rx_match_n(r, s, strlen(s), 0, 0, m); }
int rx_fullmatch(rx *r, const char *s, rxm *m) { return rx_match_n(r, s, strlen(s), 0, 1, m); }
int rx_search_n(rx *r, const char *s, size_t len, size_t from, rxm *m) {
    m->ok = 0;
    for (size_t p = from; p <= len;) {
        int ok = 1;
        if (r->first >= 0 && r->first < 128) ok = p < len && (unsigned char)s[p] == r->first;
        else if (r->bol) ok = p == 0 || ((r->flags & RX_M) && s[p - 1] == '\n');
        if (ok && try_at(r, s, len, p, 0, m)) return 1;
        if (p == len) break;
        if (r->flags & RX_A) p++;
        else { int l; u8_next(s + p, &l); p += (size_t)l; }
    }
    return 0;
}
int rx_search(rx *r, const char *s, size_t from, rxm *m) { return rx_search_n(r, s, strlen(s), from, m); }
int rx_gset(const rxm *m, int k) { return m->g[k][0] != (size_t)-1; }
char *rx_group(const rxm *m, int k) {
    if (!rx_gset(m, k)) return xstrdup("");
    return xstrndup(m->s + m->g[k][0], m->g[k][1] - m->g[k][0]);
}
char *rx_sub(rx *r, const char *s, const char *repl, char *(*fn)(const rxm *, void *), void *ctx) {
    sbuf b; sb_init(&b); size_t len = strlen(s), pos = 0; rxm m;
    while (pos <= len && rx_search_n(r, s, len, pos, &m)) {
        sb_addn(&b, s + pos, m.g[0][0] - pos);
        sb_add(&b, fn ? fn(&m, ctx) : repl);
        if (m.g[0][1] == m.g[0][0]) {
            if (m.g[0][0] >= len) { pos = len + 1; break; }
            int l; u8_next(s + m.g[0][0], &l);
            sb_addn(&b, s + m.g[0][0], (size_t)l); pos = m.g[0][0] + (size_t)l;
        } else pos = m.g[0][1];
    }
    if (pos <= len) sb_add(&b, s + pos);
    return sb_take(&b);
}

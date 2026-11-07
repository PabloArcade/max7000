/* run.c -- the emit and decode commands: one fuse file in; emit writes an output folder and REPORT.txt, decode runs the
 * same steps with every file kept in memory and prints the equations.
 *
 * REPORT.txt lines: tool and credit; input name and sha256; part; decode status (DECODED, or REFUSED <NAME> with the
 * detail); build input files; RACING registers (a register whose clock and next state share a signal: its behaviour
 * at that instant depends on device timing); pins or pads not covered, with the reason; the documentation view.
 * The same lines are printed on standard output.
 */
#include "atf.h"
#include "jedec.h"
#include "max.h"
#include "pof.h"
#include "rx.h"
#include "sha256.h"
#include "util.h"
#include <stdlib.h>
#include <string.h>

int input_class(const unsigned char *d, size_t n, const char **why);
void atf_part_pkg(const unsigned char *data, size_t n, const char *part, const char *pkg, char **dev_out, char **pkg_out);


/* the output name: the input's base name without extension, runs of non-word characters -> "_", "_" stripped at
 * both ends, lower case; "design" when nothing is left */
static char *out_name(const char *inp) {
    static rx *rnw;
    if (!rnw) rnw = rx_compile("\\W+", 0);
    const char *b = path_base(inp);
    const char *dot = strrchr(b, '.');
    char *stem = xstrdup(b);
    if (dot) for (const char *q = b; q < dot; q++) if (*q != '.') { stem = xstrndup(b, (size_t)(dot - b)); break; }
    char *s = py_lower(py_strip_chars(rx_sub(rnw, stem, "_", NULL, NULL), "_", 1, 1));
    return *s ? s : xstrdup("design");
}

/* where the output files go: a folder (emit), or memory (decode: dir == NULL) */
static slist mem_files, mem_texts;
static void write_or_die(const char *dir, const char *file, const char *text) {
    if (!dir) { sl_push(&mem_files, file); sl_push(&mem_texts, text); return; }
    char *p = path_join(dir, file);
    if (write_text_file(p, text) != 0) fatal("cannot write %s", p);
}
static const char *mem_file(const char *file) {
    int k = sl_find(&mem_files, file);
    return k >= 0 ? mem_texts.v[k] : NULL;
}

static void run_max(const unsigned char *data, size_t n, const char *name, const char *out, const char *part, slist *R) {
    struct pof P; pof_parse(data, n, &P);
    if (!P.device) fatal("the POF names no device");
    sl_push_own(R, fmt("part (POF device field): %s", P.device));
    if (part && *part) {
        char *a = py_upper(P.device), *b = py_upper(part);
        if (!startswith(a, b)) refuse("PART_OVERRIDE_MISMATCH", "--part %s, POF device %s", part, P.device);
    }
    struct decoded X; max_decode(data, n, &X);
    sl_push_own(R, fmt("decode: DECODED (%s)", TOOL_VER));
    char *v, *q; max_emit(&X, name, P.device, &v, &q);
    write_or_die(out, fmt("%s.v", name), v); write_or_die(out, fmt("%s.qsf", name), q);
    sl_push_own(R, fmt("build input: %s.v, %s.qsf (%s; every macrocell frozen at its original LC)", name, name, TOOL_VER));
    struct netlist N; vo_parse(max_to_netlist(&X), &N);
    struct race rc; race_analyse(&N, &rc);
    sl_push_own(R, fmt("RACING registers: %s", rc.n ? race_report(&rc, 0) : "none"));
    if (X.n_attrs && !strcmp(X.d->D->fam, "max7000b"))
        sl_push(R, "pins not covered: dedicated input pins: bus hold / weak pull-up UNKNOWN (no fuse; no vendor statement in hand); the build input sets none; "
                   "I/O standard code 3 covers LVTTL / LVCMOS / 2.5 V / PCI alike; the build input names the code, not the standard");
    else sl_push(R, "pins not covered: none named for this part");
    struct catcher c;
    if (TRY(c)) {
        char *doc = max_doc_view(v);
        ENDTRY(c);
        write_or_die(out, fmt("%s_doc.v", name), doc);
        sl_push_own(R, fmt("documentation view: %s_doc.v (documentation, not the build input; equivalence to the build input not checked for this file)", name));
    } else {
        ENDTRY(c);
        sl_push_own(R, fmt("documentation view: WITHHELD (%s)", u8_prefix(fmt("UNPROVEN\t%s: %s", refusal_name, refusal_detail), 200)));
    }
}

/* pin names from a CUPL source: lines "pin <n> = [!]<name>;" */
static void read_names(const char *path, ilist *pins, slist *names) {
    static rx *rp;
    if (!rp) rp = rx_compile("^\\s*pin\\s+(\\d+)\\s*=\\s*!?(\\w+)\\s*;", RX_M | RX_I);
    size_t n; unsigned char *b = read_file(path, &n);
    if (!b) fatal("cannot read %s", path);
    /* text as the reference reads it: line ends normalised, bytes that are not UTF-8 replaced */
    sbuf t; sb_init(&t);
    for (size_t i = 0; i < n;) {
        if (b[i] == '\r') { sb_addc(&t, '\n'); i += (i + 1 < n && b[i + 1] == '\n') ? 2 : 1; continue; }
        int l; int cp = u8_next((const char *)b + i, &l);
        if (b[i] >= 0x80 && (l == 1 || i + (size_t)l > n)) { sb_add(&t, "\xef\xbf\xbd"); i++; continue; }
        (void)cp; sb_addn(&t, (const char *)b + i, (size_t)l); i += (size_t)l;
    }
    char *s = sb_take(&t);
    size_t len = strlen(s), pos = 0; rxm m;
    while (pos <= len && rx_search_n(rp, s, len, pos, &m)) {
        int p = atoi(rx_group(&m, 1));
        int k = il_find(pins, p);
        if (k >= 0) names->v[k] = rx_group(&m, 2); else { il_push(pins, p); sl_push_own(names, rx_group(&m, 2)); }
        pos = m.g[0][1] > m.g[0][0] ? m.g[0][1] : m.g[0][1] + 1;
    }
}

static int cmp_int(const void *a, const void *b) { return *(const int *)a - *(const int *)b; }

static void run_atf(const unsigned char *data, size_t n, const char *name, const char *out, const char *part, const char *pkg0, const char *names_src, slist *R) {
    char *dev, *pkg; atf_part_pkg(data, n, part, pkg0, &dev, &pkg);
    sl_push_own(R, fmt("part: %s %s", dev, pkg));
    struct anet N; atf_decode(data, n, dev, pkg, &N);
    sl_push_own(R, fmt("decode: DECODED (%s)", TOOL_VER));
    const struct atfdev *D = N.D;
    /* each pad's class from its own fields: U1 output enable GND, U2 enable from an empty term 5, U3 enable from term 5
     * routed to the sum, else driven */
    const struct amc **mcs = xmalloc(sizeof(void *) * (size_t)(D->n_mcs + 1));
    for (int i = 0; i < D->n_mcs; i++) mcs[i] = &D->mcs[i];
    for (int i = 1; i < D->n_mcs; i++) for (int j = i; j > 0 && mcs[j - 1]->num > mcs[j]->num; j--) { const struct amc *t = mcs[j]; mcs[j] = mcs[j - 1]; mcs[j - 1] = t; }
    slist cpad = { 0 }, ccls = { 0 };
    for (int i = 0; i < D->n_mcs; i++) {
        const struct amc *e = mcs[i];
        if (e->pad.num == -2) continue;
        char *pad = atf_padname(&e->pad);
        if (!atf_pin(&N, pad)) continue;
        const char *o[3] = { "oe_mux", "pt5_func", "pt5_mux" }; char *v[3] = { 0, 0, 0 };
        for (int k = 0; k < 3; k++) for (int q = 0; q < e->n_opts; q++) if (!strcmp(e->opts[q].name, o[k])) v[k] = atf_opt(N.F, &e->opts[q].f, o[k]);
        if (!v[0] || !v[1] || !v[2]) fatal("MC%d lacks an output-enable field", e->num);
        const struct ptr *p5 = NULL;
        for (int q = 0; q < e->n_pt; q++) if (!strcmp(e->pt[q].name, "PT5")) p5 = &e->pt[q];
        if (!p5) fatal("MC%d has no PT5", e->num);
        int all1 = 1, any1 = 0;
        for (int f = p5->a; f < p5->b; f++) { if (N.F[f]) any1 = 1; else all1 = 0; }
        const char *pz = all1 ? "ones" : !any1 ? "zeros" : "lits";
        const char *c = !strcmp(v[0], "GND") ? "U1"
                      : (!strcmp(v[0], "VCC_pt5") && !strcmp(v[1], "oe") && !strcmp(v[2], "as_oe") && !strcmp(pz, "zeros")) ? "U2"
                      : (!strcmp(v[0], "VCC_pt5") && !strcmp(v[1], "oe") && !strcmp(v[2], "sum")) ? "U3" : "DRIVEN";
        int k = sl_find(&cpad, pad);
        if (k >= 0) ccls.v[k] = xstrdup(c); else { sl_push(&cpad, pad); sl_push(&ccls, c); }
    }
    ilist pins = { 0 }; slist names = { 0 };
    if (names_src) read_names(names_src, &pins, &names);
    else {
        /* the pads a designer could have declared: read inputs, and driven pads that are not U2 / U3 */
        slist cand = { 0 };
        for (int i = 0; i < N.inputs.n; i++) sl_push(&cand, N.inputs.v[i]);
        for (int i = 0; i < N.no; i++) {
            int k = sl_find(&cpad, N.out[i].pad);
            const char *c = k >= 0 ? ccls.v[k] : "";
            if (strcmp(c, "U2") && strcmp(c, "U3")) sl_push(&cand, N.out[i].pad);
        }
        for (int i = 0; i < cand.n; i++) {
            int p = atf_pin(&N, cand.v[i]);
            if (p && il_find(&pins, p) < 0) { il_push(&pins, p); sl_push_own(&names, fmt("p%d", p)); }
        }
    }
    char *design = u8_prefix(py_upper(name), 8);
    if (!*design) design = xstrdup("DEC");
    char *src = atf_emit(&N, pins.v, names.v, pins.n, design);
    write_or_die(out, fmt("%s.pld", name), src);
    sl_push_own(R, fmt("build input: %s.pld (%s)", name, TOOL_VER));
    /* undeclared pads by class */
    const char *K[4] = { "OTHER", "U1", "U2", "U3" };
    ilist cl[4] = { { 0 }, { 0 }, { 0 }, { 0 } };
    for (int i = 0; i < cpad.n; i++) {
        int p = atf_pin(&N, cpad.v[i]);
        if (il_find(&pins, p) >= 0) continue;
        int k = !strcmp(ccls.v[i], "DRIVEN") ? 0 : !strcmp(ccls.v[i], "U1") ? 1 : !strcmp(ccls.v[i], "U2") ? 2 : 3;
        il_push(&cl[k], p);
    }
    slist parts = { 0 };
    for (int k = 0; k < 4; k++) {
        if (!cl[k].n) continue;
        qsort(cl[k].v, (size_t)cl[k].n, sizeof(int), cmp_int);
        sbuf b; sb_init(&b); sb_printf(&b, "%s ", K[k]);
        for (int i = 0; i < cl[k].n; i++) sb_printf(&b, i ? ",%d" : "%d", cl[k].v[i]);
        sl_push_own(&parts, sb_take(&b));
    }
    sl_push_own(R, fmt("undeclared pads (left unconnected): %s", parts.n ? sl_join(&parts, "; ") : "none"));
    if (cl[3].n || cl[0].n) {
        ilist nc = { 0 };
        for (int i = 0; i < cl[3].n; i++) il_push(&nc, cl[3].v[i]);
        for (int i = 0; i < cl[0].n; i++) il_push(&nc, cl[0].v[i]);
        qsort(nc.v, (size_t)nc.n, sizeof(int), cmp_int);
        sbuf b; sb_init(&b);
        for (int i = 0; i < nc.n; i++) sb_printf(&b, i ? ",%d" : "%d", nc.v[i]);
        sl_push_own(R, fmt("pins not covered: %s (U3: output enable from a product term in the sum, no vendor statement of the pad's behaviour; "
                           "keep these pads unconnected on the board as the vendor report's NC legend asks)", sb_take(&b)));
    } else sl_push(R, "pins not covered: none");
}

/* the decode and emit step; a refusal ends it with the REFUSED line (kept in its own function so that no local of the
 * caller lives across the jump) */
static int decode_into(const char *part, const char *pkg, const char *names, const unsigned char *data, size_t n, const char *name, const char *out, slist *R) {
    struct catcher c;
    if (TRY(c)) {
        const char *why = NULL; int k = input_class(data, n, &why);
        if (k == 1) run_max(data, n, name, out, part, R);
        else if (k == 2) run_atf(data, n, name, out, part, pkg, names, R);
        else refuse("INPUT_FORMAT_NOT_MODELLED", "%s", why);
        ENDTRY(c);
        return 0;
    }
    ENDTRY(c);
    sl_push_own(R, fmt("decode: REFUSED %s (%s)", refusal_name, u8_prefix(fmt("%s: %s", refusal_name, refusal_detail), 300)));
    sl_push(R, "no build input written");
    return 3;
}

/* emit: the output folder, REPORT.txt, and the report lines on standard output */
int run_emit(const char *inp, const char *out, const char *part, const char *pkg, const char *names) {
    if (mkdir_p(out) != 0) fatal("cannot create the output folder %s", out);
    size_t n; unsigned char *data = read_file(inp, &n);
    if (!data) fatal("cannot read %s", inp);
    char *name = out_name(inp);
    char hex[65]; sha256_hex(data, n, hex);
    slist R = { 0 };
    sl_push_own(&R, fmt("%s; credit: %s", TOOL_VER, CREDIT));
    sl_push_own(&R, fmt("input: %s sha256 %s", path_base(inp), hex));
    int rc = decode_into(part, pkg, names, data, n, name, out, &R);
    char *rep = fmt("%s\n", sl_join(&R, "\n"));
    write_or_die(out, "REPORT.txt", rep);
    put_text(stdout, rep);
    return rc;
}

/* decode: the steps of emit with every file kept in memory. Printed: the report lines that do not name an output file,
 * then the equations -- MAX: the documentation view's notation note, its ports and its body (per macrocell its sum,
 * data, clock, enable, clear and preset, then per pin its value and output enable); ATF: the CUPL source's pin lines
 * and equations. */
int run_decode(const char *inp, const char *part, const char *pkg) {
    size_t n; unsigned char *data = read_file(inp, &n);
    if (!data) fatal("cannot read %s", inp);
    char *name = out_name(inp);
    char hex[65]; sha256_hex(data, n, hex);
    slist R = { 0 };
    sl_push_own(&R, fmt("input: %s sha256 %s", path_base(inp), hex));
    int rc = decode_into(part, pkg, NULL, data, n, name, NULL, &R);
    slist O = { 0 };
    const char *withheld = NULL;
    for (int i = 0; i < R.n; i++) {
        const char *l = R.v[i];
        if (startswith(l, "build input:") || !strcmp(l, "no build input written")) continue;
        if (startswith(l, "documentation view:")) { if (strstr(l, ": WITHHELD (")) withheld = strstr(l, "WITHHELD ("); continue; }
        if (!strcmp(l, fmt("decode: DECODED (%s)", TOOL_VER))) { sl_push(&O, "decode: DECODED"); continue; }
        sl_push(&O, l);
    }
    if (rc == 0) {
        const char *doc = mem_file(fmt("%s_doc.v", name)), *pld = mem_file(fmt("%s.pld", name));
        if (doc) {
            sl_push(&O, "equations:");
            char *t = xstrdup(doc);
            slist L = { 0 }, ins = { 0 }, outs = { 0 }, ios = { 0 }, body = { 0 };
            for (char *p = t, *e; *p; p = e + 1) {
                e = strchr(p, '\n'); if (!e) e = p + strlen(p);
                sl_push_own(&L, xstrndup(p, (size_t)(e - p)));
                if (!*e) break;
            }
            int in_module = 0;
            for (int i = 0; i < L.n; i++) {
                const char *l = L.v[i];
                if (startswith(l, "// Notation:") || (i > 0 && startswith(L.v[i - 1], "// Notation:")) || (i > 1 && startswith(L.v[i - 2], "// Notation:"))) { if (!in_module) sl_push(&O, l); continue; }
                if (startswith(l, "module ")) { in_module = 1; continue; }
                if (!in_module || !strcmp(l, "endmodule")) continue;
                if (startswith(l, "  input ")) { sl_push_own(&ins, py_strip_chars(xstrdup(l + 8), ";", 0, 1)); continue; }
                if (startswith(l, "  output ")) { sl_push_own(&outs, py_strip_chars(xstrdup(l + 9), ";", 0, 1)); continue; }
                if (startswith(l, "  inout ")) { sl_push_own(&ios, py_strip_chars(xstrdup(l + 8), ";", 0, 1)); continue; }
                sl_push(&body, l);
            }
            sl_push_own(&O, fmt("inputs: %s", ins.n ? sl_join(&ins, " ") : "none"));
            sl_push_own(&O, fmt("outputs: %s", outs.n ? sl_join(&outs, " ") : "none"));
            if (ios.n) sl_push_own(&O, fmt("bidirectional: %s", sl_join(&ios, " ")));
            for (int i = 0; i < body.n; i++) sl_push(&O, body.v[i]);
        } else if (pld) {
            sl_push(&O, "equations:");
            const char *p = strstr(pld, "\n/* emitted by ");
            if (!p) fatal("the CUPL source has no header comment");
            p = strchr(p + 1, '\n');
            if (p) { char *rest = xstrdup(p + 1); size_t k = strlen(rest); while (k && rest[k - 1] == '\n') rest[--k] = 0; sl_push(&O, rest); }
        } else sl_push_own(&O, fmt("equations: %s", withheld ? withheld : "WITHHELD"));
    }
    put_text(stdout, fmt("%s\n", sl_join(&O, "\n")));
    return rc;
}

/* max7000dump -- MAX 7000S / AE / B (.pof) and ATF1502AS / ATF1504AS / ATF1508AS (.jed) fuse map decompiler.
 *
 *   max7000dump decode --viewlist                          supported parts + packages
 *   max7000dump decode [--part P] [--package PKG] <file>   equations per macrocell, to stdout
 *   max7000dump emit [--part P] [--package PKG] [--names F.pld] <file> <outdir>
 *   max7000dump dump [--part P] [--package PKG] <file>     structural report (decoded fields)
 *   max7000dump diff [--part P] [--package PKG] <a> <b>    macrocell-level diff of two files of one part
 *   max7000dump devices                                    supported parts, packages and notes
 *   max7000dump --version
 *
 * emit writes one output folder:
 *   MAX: <name>.v + <name>.qsf  build input (atom-level Verilog + pin / option constraints) for Quartus II 13.0sp1
 *        <name>_doc.v           documentation view (readable equations; not the build input)
 *   ATF: <name>.pld             build input (CUPL) for WinCUPL
 *   REPORT.txt                  part, decode status, refusals by name, RACING registers, pins / pads not covered
 * Anything the decoder does not handle is refused by name and no build input is written.
 * The banner goes to stderr, first, on every run, so that it never mixes with a redirected output.
 * Exit codes: 0 done, 3 refused by name, 2 usage, 1 error (and no arguments at all).
 */
#include "atf.h"
#include "max.h"
#include "pof.h"
#include "rx.h"
#include "sha256.h"
#include "util.h"

#include <stdlib.h>
#include <string.h>

int run_emit(const char *inp, const char *out, const char *part, const char *pkg, const char *names);
int run_decode(const char *inp, const char *part, const char *pkg);
int run_diff(const char *a, const char *b, const char *part, const char *pkg);
int run_jed2pof(const char *in, const char *out, const char *part, const char *pkg, const char *device);
int run_pof2jed(const char *in, const char *out, const char *part);
void print_devices(void);

static void banner(void) { fprintf(stderr, "%s\n", BANNER); }

static void usage(void) {
    fprintf(stderr,
        PROG_NAME " " TOOL_VERSION " -- MAX 7000S / AE / B and ATF1502AS / ATF1504AS / ATF1508AS fuse map decompiler by Darksoft\n"
        "\n"
        "Commands:\n"
        "  " PROG_NAME " decode --viewlist                          list supported parts + packages\n"
        "  " PROG_NAME " decode [--part P] [--package PKG] <file>   equations per macrocell, to stdout\n"
        "  " PROG_NAME " emit [--part P] [--package PKG] [--names F.pld] <file> <outdir>\n"
        "                                                         build input + documentation view + REPORT.txt\n"
        "  " PROG_NAME " dump [--part P] [--package PKG] <file>     structural report (decoded fields)\n"
        "  " PROG_NAME " diff [--part P] [--package PKG] <a> <b>    macrocell-level diff of two files of one part\n"
        "  " PROG_NAME " jed2pof [--part P] [--package PKG] [--device D] <in.jed> <out.pof>\n"
        "                                                         ATF150x JEDEC -> MAX 7000S / AE POF of the same pins\n"
        "  " PROG_NAME " pof2jed [--part P] <in.pof> <out.jed>\n"
        "                                                         MAX 7000S / AE POF -> ATF150x JEDEC of the same pins\n"
        "  " PROG_NAME " devices                                    supported parts, packages and notes\n"
        "  " PROG_NAME " --version                                  version line only\n"
        "\n"
        "Device selection: a .pof is decoded for the device named in its own device field; a .jed for the part\n"
        "named in its \"JEDEC file for:\" header line (--part overrides it). An ATF package comes from the JEDEC\n"
        "pin count, or from --package when the pin count does not decide it (44-pin parts). --part on a .pof\n"
        "must match its device field. A file naming a part this tool does not support is refused by name.\n"
        "jed2pof writes ATF1502AS / ATF1504AS / ATF1508AS for the 5 V EPM7032S / EPM7064S / EPM7128S; a JEDEC does not\n"
        "say AS or ASV, so a 3.3 V target needs --part ATF150xASV or --device EPM70xxAE. The part comes from the fuse\n"
        "count; 44-pin files need --package (PLCC44 or TQFP44).\n"
        "pof2jed writes the ATF1502 / ATF1504 / ATF1508 of the same pins for an EPM7032 / EPM7064 / EPM7128 S or AE; the\n"
        "AS part (5 V) for an S device, the ASV part (3.3 V) for an AE device; --part ATF150xAS or ATF150xASV chooses.\n"
        "Options may stand anywhere on the line; option names are not case-sensitive; --opt=value also works.\n"
        "\n"
        "Exit codes: 0 done, 3 refused by name (the reason is printed), 2 usage, 1 error.\n");
}

/* the input class, as the decompile step decides it: 1 POF, 2 ATF JEDEC, 0 neither (detail in *why) */
int input_class(const unsigned char *d, size_t n, const char **why) {
    static rx *rqf;
    if (!rqf) rqf = rx_compile("\\bQF\\d+", RX_A);
    if (n >= 3 && !memcmp(d, "POF", 3)) return 1;
    size_t a = n < 4096 ? n : 4096, b = n < 8192 ? n : 8192;
    rxm m;
    if (memchr(d, '*', a) && rx_search_n(rqf, (const char *)d, b, 0, &m)) {
        size_t c = n < 400 ? n : 400;
        int jf = 0, epm = 0;
        for (size_t i = 0; i + 15 <= c; i++) if (!memcmp(d + i, "JEDEC file for:", 15)) jf = 1;
        for (size_t i = 0; i + 4 <= c; i++) if (!memcmp(d + i, "EPM7", 4)) epm = 1;
        if (jf && epm) { *why = "MAX JEDEC: give the .pof"; return 0; }
        return 2;
    }
    *why = "neither a POF nor a JEDEC fuse file";
    return 0;
}

/* the ATF part and package: --part, else the JEDEC header; --package, else the pin count (refuses by name) */
void atf_part_pkg(const unsigned char *data, size_t n, const char *part, const char *pkg, char **dev_out, char **pkg_out) {
    static rx *rh, *rqp;
    if (!rh) { rh = rx_compile("JEDEC file for:\\s*(\\S+)", 0); rqp = rx_compile("\\*\\s*QP(\\d+)", 0); }
    char *t = latin1_to_u8(data, n);
    rxm mh, mq;
    int hh = rx_search(rh, t, 0, &mh), hq = rx_search(rqp, t, 0, &mq);
    char *dev = py_upper(part && *part ? part : hh ? rx_group(&mh, 1) : "");
    if (strcmp(dev, "ATF1502AS") && strcmp(dev, "ATF1504AS") && strcmp(dev, "ATF1508AS"))
        refuse("DEVICE_NOT_MODELLED", "JEDEC part %s", py_repr_str(dev));
    char *qp = hq ? rx_group(&mq, 1) : xstrdup("");
    char *pk = pkg && *pkg ? xstrdup(pkg) : NULL;
    if (!pk) {
        static const char *T[][3] = { { "ATF1504AS", "84", "PLCC84" }, { "ATF1504AS", "100", "TQFP100" }, { "ATF1508AS", "84", "PLCC84" },
                                      { "ATF1508AS", "160", "PQFP160" } };
        for (size_t i = 0; i < sizeof T / sizeof T[0]; i++) if (!strcmp(T[i][0], dev) && !strcmp(T[i][1], qp)) pk = xstrdup(T[i][2]);
        if (!pk) refuse("PACKAGE_NOT_DETERMINED", "%s with QP%s: give --package (e.g. PLCC44 / TQFP44)", dev, hq ? qp : "?");
    }
    *dev_out = dev; *pkg_out = pk;
}

static int run_dump(const char *inp, const char *part, const char *pkg) {
    size_t n; unsigned char *data = read_file(inp, &n);
    if (!data) fatal("cannot read %s", inp);
    struct catcher c; int rc = 0;
    if (TRY(c)) {
        const char *why = NULL; int k = input_class(data, n, &why);
        if (k == 1) {
            struct pof P; pof_parse(data, n, &P);
            if (!P.device) fatal("the POF names no device");
            if (part && *part) {
                char *a = py_upper(P.device), *b = py_upper(part);
                if (!startswith(a, b)) refuse("PART_OVERRIDE_MISMATCH", "--part %s, POF device %s", part, P.device);
            }
            struct decoded X; max_decode(data, n, &X);
            put_text(stdout, fmt("device %s\n%s", P.device, max_dump(&X)));
        } else if (k == 2) {
            char *dev, *pk; atf_part_pkg(data, n, part, pkg, &dev, &pk);
            struct anet N; atf_decode(data, n, dev, pk, &N);
            put_text(stdout, atf_dump(&N));
        } else refuse("INPUT_FORMAT_NOT_MODELLED", "%s", why);
        ENDTRY(c);
    } else {
        ENDTRY(c);
        put_text(stdout, fmt("REFUSED %s: %s\n", refusal_name, refusal_detail)); rc = 3;
    }
    return rc;
}

/* ---- command line: options are taken out of the argument list wherever they stand (names not case-sensitive,
 * "--opt VALUE" or "--opt=VALUE"); the first occurrence of an option counts. What is left is the command and its files. */
static int eq_nocase(const char *a, const char *b) {
    for (; *a && *b; a++, b++) {
        int x = (unsigned char)*a, y = (unsigned char)*b;
        if (x >= 'A' && x <= 'Z') x += 32;
        if (y >= 'A' && y <= 'Z') y += 32;
        if (x != y) return 0;
    }
    return *a == *b;
}
static int prefix_nocase(const char *s, const char *p, const char **rest) {
    size_t k = strlen(p);
    if (strlen(s) < k) return 0;
    char *h = xstrndup(s, k);
    if (!eq_nocase(h, p)) return 0;
    *rest = s + k; return 1;
}

int main(int argc, char **argv) {
    banner();
    if (argc < 2) { usage(); return 1; }
    static const char *vopt[4] = { "--part", "--package", "--names", "--device" };
    const char *val[4] = { NULL, NULL, NULL, NULL };
    int viewlist = 0, version = 0, np = 0;
    char **pos = xcalloc((size_t)argc + 1, sizeof(char *));
    for (int i = 1; i < argc; i++) {
        const char *a = argv[i], *rest;
        int done = 0;
        if (eq_nocase(a, "--viewlist")) { viewlist = 1; continue; }
        if (eq_nocase(a, "--version")) { version = 1; continue; }
        for (int k = 0; k < 4 && !done; k++) {
            if (eq_nocase(a, vopt[k])) {
                if (i + 1 >= argc) { fprintf(stderr, PROG_NAME ": option %s needs a value\n", vopt[k]); return 2; }
                if (!val[k]) val[k] = argv[i + 1];
                i++; done = 1;
            } else if (prefix_nocase(a, vopt[k], &rest) && *rest == '=') {
                if (!val[k]) val[k] = rest + 1;
                done = 1;
            }
        }
        if (done) continue;
        if (!strncmp(a, "--", 2)) {
            fprintf(stderr, PROG_NAME ": unrecognised option \"%s\"\n  Run with no arguments for the usage text.\n", a);
            return 2;
        }
        pos[np++] = argv[i];
    }
    int rc;
    const char *cmd = np ? pos[0] : "";
    if (version) rc = 0;                                         /* the banner is the version line */
    else if (!strcmp(cmd, "devices") && np == 1 && !viewlist && !val[0] && !val[1] && !val[2] && !val[3]) { print_devices(); rc = 0; }
    else if (!strcmp(cmd, "decode") && viewlist && np == 1 && !val[0] && !val[1] && !val[2] && !val[3]) { print_devices(); rc = 0; }
    else if (!strcmp(cmd, "decode") && np == 2 && !viewlist && !val[2] && !val[3]) rc = run_decode(pos[1], val[0], val[1]);
    else if (!strcmp(cmd, "emit") && np == 3 && !viewlist && !val[3]) rc = run_emit(pos[1], pos[2], val[0], val[1], val[2]);
    else if (!strcmp(cmd, "jed2pof") && np == 3 && !viewlist && !val[2]) rc = run_jed2pof(pos[1], pos[2], val[0], val[1], val[3]);
    else if (!strcmp(cmd, "pof2jed") && np == 3 && !viewlist && !val[1] && !val[2] && !val[3]) rc = run_pof2jed(pos[1], pos[2], val[0]);
    else if (!strcmp(cmd, "dump") && np == 2 && !viewlist && !val[2] && !val[3]) rc = run_dump(pos[1], val[0], val[1]);
    else if (!strcmp(cmd, "diff") && np == 3 && !viewlist && !val[2] && !val[3]) rc = run_diff(pos[1], pos[2], val[0], val[1]);
    else { usage(); rc = 2; }
    fflush(stdout);
    mem_release_all();
    return rc;
}

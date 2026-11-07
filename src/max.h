/* max.h -- MAX 7000S / AE / B: device resolution, fuse decoding, the decoded atom netlist.
 *
 * A decoded design is a set of macrocells (LC1..LCn), shared expanders (SEXPn: the NAND of product term 1 of LCn when
 * that term is steered to the expander), and I/O cells. Only what an output pin can reach is decoded: a work list
 * starts from every enabled output and its output enable and follows every literal back to its source.
 *
 * Settings of a macrocell (index into cell.st): product-term steering of terms 1..5 (1 -> shared expander, 2 -> XOR
 * operand, 3 -> clear, 4 -> clock or clock enable, 5 -> preset; a term not steered is ORed into the sum), clock select
 * (0 product-term clock, 1 / 2 global clock network, code 3 = none), clear select (1 = the global clear pin),
 * or-invert (0 normal, 1 XOR / invert, 2 T flip-flop, 3 invert + T where the part accepts it), parallel-expander
 * allocation (1 = this cell's sum goes to the next cell), register output (raw 0 = registered), turbo bit.
 * A setting value >= 0 is a setting number; a raw code with no setting number is stored as SV_CODE(code).
 */
#ifndef MAX_H
#define MAX_H
#include "tables.h"
#include "util.h"

#define SV_CODE(c) (-1000 - (c))
#define SV_IS_CODE(v) ((v) <= -1000)
char *sv_str(int v);                       /* "3" or "code3" */

struct maxctx {
    const struct maxdev *D; const struct padtable *P;
    char *dev, *fam, *part, *pkg;
    int n_io; int *io_key, *io_val;        /* I/O cell index -> pin code, in the pad table's order */
    int n_ib; int ib_key[8], ib_val[8];    /* dedicated input number -> pin code */
    int jtag_in[3], jtag_pins[4];
    int n_s7, s7_addr[7];
};
struct maxctx *max_device(const char *devname);   /* refuses DEVICE_NOT_MODELLED / PACKAGE_NOT_MODELLED */
int max_io_pin(const struct maxctx *d, int io);   /* 0 = not bonded */
int max_ib_pin(const struct maxctx *d, int ib);   /* stops (no report) when absent, as the reference does */
const struct setting *max_find_setting(const struct maxctx *d, const char *name);   /* NULL = not on this part */
int max_raw(const struct maxctx *d, const unsigned char *bits, const char *name, int inst);
int max_setting(const struct maxctx *d, const unsigned char *bits, const char *name, int inst);

enum { S_INV_SEXP, S_INV_XOR, S_CLR, S_CLK_CE, S_PRE, S_CLOCK_SELECT, S_CLEAR_SELECT, S_OR_INV, S_PEXP, S_REG_OUT, S_TURBO, S_N };
struct row { int st; slist l; };           /* st 0 not decoded, 1 constant 0 (GND), 2 literal list (empty = constant 1) */
struct cell { int st[S_N]; int reg, donor, pu, fpin; struct row rows[5]; };   /* pu 0 low, 1 high, -1 unknown */
struct io { int pin, lc, od, has_clamp, clamp, clamp_lc; char *oe; };       /* lc 0 = none; oe "vcc" or [!]net */
struct attr { int pin, bus_hold, weak_pull_up, iost, delay; };
struct decoded {
    struct maxctx *d;
    unsigned char *bits; int nbits;
    int gcc[3]; int n_gclk, gclk_net[4], gclk_ib[4], gclk_inv[4];
    int jtag;
    struct cell *cells;                    /* 1..n_mc */
    int n_ios; struct io *ios;             /* in decoding order (I/O cell index) */
    int n_attrs; struct attr *attrs;
    char *live, *live_sexp;                /* flags 1..n_mc */
    slist used;                            /* nets read by the decoded logic */
};
void max_decode(const unsigned char *data, size_t n, struct decoded *X);   /* refuses by name */
const struct io *max_io_at(const struct decoded *X, int pin);
const struct attr *max_attr_at(const struct decoded *X, int pin);
int max_used(const struct decoded *X, const char *net);
char *max_to_netlist(const struct decoded *X);       /* the decoded design as an atom netlist (refuses by name) */
char *max_oe_text(const struct io *io);
char *max_dump(const struct decoded *X);

/* atom netlist reader (the dialect max_to_netlist writes and the emitter's reader form) */
struct port { char *name, *val; };
struct par { char *name, *val; };
struct inst { char *loc, *kind, *name; int np; struct port *p; int npar; struct par *par; };
struct netlist { char *top; slist inputs, outputs, inouts; int nw; char **wr, **ww; int ni; struct inst *in; };
void vo_parse(const char *text, struct netlist *N);
const char *vo_port(const struct inst *x, const char *name);     /* NULL = no such port */
const char *vo_par(const struct inst *x, const char *name);      /* NULL = no such parameter */
int vo_is_reg(const struct inst *x);
int vo_const_clear(const struct inst *x);
char *vo_regkey(const struct inst *x);

/* RACING lint */
struct race { int n; char **key; char **line; };     /* registers whose clock meets their data, enable or async logic */
void race_analyse(const struct netlist *N, struct race *R);
char *race_report(const struct race *R, int by_number);

/* emitters */
void max_emit(const struct decoded *X, const char *top, const char *device, char **v, char **qsf);
char *max_doc_view(const char *em);                  /* refuses by name (DOC_NET, DOC_MODE, VO_*) */
char *pinname(int code);                              /* pin code -> pin number, or ball name (row letter + column) */
#endif

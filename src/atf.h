/* atf.h -- ATF1502AS / ATF1504AS / ATF1508AS: fuse map -> decoded netlist, and the CUPL emitter.
 *
 * Product term: fuse 0 = column connected; a column is a UIM switch (true or complement) or a macrocell's foldback
 * line. All fuses 1 = constant 1, all 0 = constant 0. Each UIM switch selects one source (a pad, a macrocell feedback,
 * or ground) through its mux field. Per macrocell, terms 1..5 go to the sum or to their special use (1 foldback,
 * 2 XOR operand, 3 asynchronous reset, 4 clock / clock enable, 5 preset / output enable); the XOR gate's second input,
 * its inversion and the cascade link follow the macrocell's mux fields. Signals of the netlist are Verilog-style
 * expressions over pad_<PAD>, uim_UIM<n>, mc<n>_pt<k>, mc<n>_q / _xt / _st, fb_MC<n>, flb_MC<n>, casout_MC<n>, g_<global>.
 */
#ifndef ATF_H
#define ATF_H
#include "tables.h"
#include "util.h"

struct named { char *name, *val; };
struct aout { char *pad, *a, *en; int od; };
struct areg { char *mc, *clk, *ce, *ar, *as, *d; };
struct ainfo {                       /* per macrocell, for the emitter */
    char *mc; int n_o; struct named *o;
    char *pt[5]; char *ptname[5]; int n_pt;
    char *xt, *clk, *ce, *ar, *as, *en, *a, *fb, *pad;
    int pt1_flb, pt2_xor, xb, inv_on;
};
struct anet {
    const struct atfdev *D; const struct apkg *K; char *dev, *pkg;
    int security, jtag_mode;
    slist notes;
    int nw; struct named *w;         /* wires, in order */
    int nr; struct areg *r;
    int no; struct aout *out;        /* driven pads, in order */
    slist inputs;                    /* pads read (unordered set; sorted when listed) */
    slist latches;
    int njd; struct named *jd;       /* JTAG pads driven in test-port mode: pad -> macrocell */
    int nu; struct named *und;       /* pads whose output enable the fuse map does not determine: pad -> reason */
    int ni; struct ainfo *info;      /* in macrocell order */
    unsigned char *F;
};
const struct atfdev *atf_find(const char *dev);
int atf_pin(const struct anet *N, const char *pad);       /* package pin of a pad, 0 = not bonded */
char *atf_padname(const struct padname *p);
char *atf_opt(const unsigned char *F, const struct afield *f, const char *where);
void atf_decode(const unsigned char *data, size_t n, const char *dev, const char *pkg, struct anet *N);
const char *atf_wire(const struct anet *N, const char *name);   /* NULL = no wire of that name (last one wins) */
const struct aout *atf_output(const struct anet *N, const char *pad);
const char *atf_info_opt(const struct ainfo *I, const char *name);
char *atf_dump(const struct anet *N);
void atf_check_jtag_declared(const struct anet *N, const int *names_pin, int nn);
/* names: pin number -> port name; returns the CUPL source */
char *atf_emit(const struct anet *N, const int *pins, char **names, int nn, const char *design);
#endif

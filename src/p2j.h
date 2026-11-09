/* p2j.h -- data for the POF -> JEDEC converter (tab_p2j.c).
 *
 * The fitter's JEDEC file lists, after the product-term rows, the macrocell configuration groups, the UIM pairs, the
 * global output enables, the device configuration, the JTAG bits, the user (UES) fuses and the reserved bits, each with
 * its own NOTE lines and line breaks. A segment is either literal text, or a fuse field: "L<addr>" + pre + bit runs,
 * each run followed by its separator text. */
#ifndef P2J_H
#define P2J_H
#include <stddef.h>
struct p2jseg { const char *text; int addr; const char *pre; int n_runs; const int *runs; const char *const *seps; };
struct p2jdev {
    const char *name, *ruler;
    const struct p2jseg *segs; int n_segs;
    const int *user; int n_user;           /* user fuses, bit k of the user code = user[k] */
    const int *pinfix; int n_pinfix;       /* (fuse, value) of the per-pin configuration fields */
    const char *clk3;                      /* the CLK3 pad's source name */
};
extern const struct p2jdev p2j_devices[]; extern const int n_p2j_devices;
/* per MAX source part: the IO_USE_FINP_BUF bit of each I/O cell (-1 = none) */
struct p2jfinp { const char *part; int n; const int *addr; };
extern const struct p2jfinp p2j_finp[]; extern const int n_p2j_finp;
extern const char *const p2j_forms[][2]; extern const int n_p2j_forms;
#endif

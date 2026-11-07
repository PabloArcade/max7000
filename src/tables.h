/* tables.h -- layout of the compiled device tables (tab_max.c, tab_pads.c, tab_atf.c).
 *
 * MAX 7000S / AE / B: one struct maxdev per device. Fuse addresses are bit indices into the POF's
 * Logical_Address_and_Data_32 packet. The product-term array follows the formula
 *     address = cols * n_pt * mc + cols * pt + col
 * (cols = 2 * lines + 16: a true and a complement column per PIA line, then the 16 shared expanders of the LAB).
 * PIA buffer b select bits: pia_base + b * pia_w + 0 .. pbw-1 (one-cold); on parts with 8 or more LABs a 2x2 bit
 * at pia_base + pbw + slot * pia_w picks one of two buffers for the slot. Global output enable k: select bits at
 * oe_base + k * oe_w + 0 .. poe_pbw-1, invert bit right after them.
 */
#ifndef TABLES_H
#define TABLES_H

/* a named configuration setting: n_inst instances of n_bits bits; addr[inst * n_bits + bit], -1 = no such bit.
 * n_map >= 0: the bit pattern is an index into map[] (raw code -> setting number); n_map = -1: the raw value is the
 * setting. n_inst = 0: the setting does not exist on this part. */
struct setting { const char *name; int n_inst, n_bits, n_map; const int *map; const int *addr; };

/* source kinds of a routing mux: macrocell output, I/O pin, dedicated input, PIA buffer */
enum { SRC_MCELL = 0, SRC_IO = 1, SRC_FINPBUF = 2, SRC_PIABUF = 3 };

struct maxdev {
    const char *name, *fam;
    int n_mc, n_lab, lines, cols, n_pt, has2x2, pia_base, pia_w, pbw, oe_base, oe_w, poe_pbw, n_poe, nbuf, total;
    const int *slotsel; int n_slotsel;     /* (slot, 2x2 value, buffer) triples */
    const int *bufsrc; int n_bufsrc;       /* (buffer, position, kind, index) */
    const int *poesrc; int n_poesrc;       /* (global OE, position, kind, index) */
    int gcc[3]; int n_gcc; int s7_first;   /* global clock configuration bits; S parts: 7 more bits from s7_first */
    const struct setting *settings; int n_settings;
    const int *s7set; int n_s7set;         /* accepted patterns of the 7 extra S bits: length, bits... */
    const int *gcc_t; int n_gcc_t;         /* (b0, b1, b2, network, dedicated input, inverted) */
    const int *donor; int n_donor;         /* (or-invert setting, reg-out raw, clocked, op mode, output mode, register mode) */
    const int *puh; int n_puh;             /* (raw, high) power-up encoding; n_puh = 0: always low */
    int gndclamp_on;                       /* raw value of the ground-clamp bit that means "on"; -1 none */
    const int *iost; int n_iost;           /* accepted I/O standard codes (B parts) */
    const int *delay; int n_delay;         /* accepted delay-chain values (B parts) */
    int or3;                               /* or-invert setting 3 accepted (invert + T flip-flop) */
};
enum { OPM_VCC = 0, OPM_NORMAL = 1, OPM_INVERT = 2, OPM_XOR = 3 };

struct strpair { const char *a, *b; };

/* package pins: (pin code, kind, index) triples; kind 0 dedicated input (index = input number), 1 I/O cell
 * (index = LAB * 16 + cell), 2 supply. Pin code = the pin number, or letter * 1000 + number for a ball. */
struct padtable { const char *part, *pkg; const int *pads; int n; int jtag[4]; /* TDI, TMS, TCK, TDO */ };

extern const struct maxdev max_devices[]; extern const int n_max_devices;
extern const struct strpair max_pkg_codes[]; extern const int n_max_pkg_codes;
extern const struct padtable max_padtables[]; extern const int n_max_padtables;

/* ATF1502AS / ATF1504AS / ATF1508AS. A field is a list of fuses (bit n of the value = fuse fuses[n]) and its named
 * values, in the order the fuse map lists them. */
struct fval { const char *name; int code; };
struct afield { const int *fuses; int n_fuses; const struct fval *vals; int n_vals; };
struct fopt { const char *name; struct afield f; };
struct ptr { const char *name; int a, b; };              /* product term: fuse range [a, b) */
struct padname { const char *pre; int num; };            /* pad "M12" = {"M", 12}; "R" = {"R", -1}; none = {"", -2} */
struct amc { int num; const char *block; struct padname pad; const struct ptr *pt; int n_pt; const struct fopt *opts; int n_opts; };
struct ablk { const char *name; const int *cols; int w; }; /* per row offset: (kind, number): 1 UIM true, 2 UIM complement, 3 foldback, -1 none, 0 unknown */
struct asw { const char *name; const char *block; struct afield mux; };
struct apin { struct padname pad; int pin; };
struct apkg { const char *name; const struct apin *pins; int n; };
enum { G_GCLK1_MUX, G_GCLK2_MUX, G_GCLK3_MUX, G_GCLK1_INV, G_GCLK2_INV, G_GCLK3_INV, G_GCLR_INV,
       G_GOE1_MUX, G_GOE2_MUX, G_GOE3_MUX, G_GOE4_MUX, G_GOE5_MUX, G_GOE6_MUX, G_N };
struct atfdev {
    const char *name; int qf, goe_inv0;
    const struct amc *mcs; int n_mcs;
    const struct ablk *blks; int n_blks;
    const struct asw *sws; int n_sws;
    const struct afield *glob;               /* G_N fields */
    const struct fopt *cfg; int n_cfg;
    const struct apkg *pkgs; int n_pkgs;
    const int *uncov; int n_uncov;           /* (fuse, required value or -1) for fuses outside every field */
    struct padname spec[4];                  /* TDI, TMS, TCK, TDO pads */
};
extern const struct atfdev atf_devices[]; extern const int n_atf_devices;

#endif

# max7000dump v3.28 -- source

MAX 7000S / AE / B (.pof) and ATF1502AS / ATF1504AS / ATF1508AS (.jed) fuse map decompiler by Darksoft.
Reads one fuse file and writes a design that the vendor tool compiles back into a part that behaves the same.
Built for arcade PCB preservation. Use at your own risk.

## Commands

```
max7000dump v3.28 -- MAX 7000S / AE / B and ATF1502AS / ATF1504AS / ATF1508AS fuse map decompiler by Darksoft

Commands:
  max7000dump decode --viewlist                          list supported parts + packages
  max7000dump decode [--part P] [--package PKG] <file>   equations per macrocell, to stdout
  max7000dump emit [--part P] [--package PKG] [--names F.pld] <file> <outdir>
                                                         build input + documentation view + REPORT.txt
  max7000dump dump [--part P] [--package PKG] <file>     structural report (decoded fields)
  max7000dump diff [--part P] [--package PKG] <a> <b>    macrocell-level diff of two files of one part
  max7000dump jed2pof [--part P] [--package PKG] [--device D] <in.jed> <out.pof>
                                                         ATF150x JEDEC -> MAX 7000S / AE POF of the same pins
  max7000dump pof2jed [--part P] <in.pof> <out.jed>
                                                         MAX 7000S / AE POF -> ATF150x JEDEC of the same pins
  max7000dump devices                                    supported parts, packages and notes
  max7000dump --version                                  version line only

Device selection: a .pof is decoded for the device named in its own device field; a .jed for the part
named in its "JEDEC file for:" header line (--part overrides it). An ATF package comes from the JEDEC
pin count, or from --package when the pin count does not decide it (44-pin parts). --part on a .pof
must match its device field. A file naming a part this tool does not support is refused by name.
jed2pof writes ATF1502AS / ATF1504AS / ATF1508AS for the 5 V EPM7032S / EPM7064S / EPM7128S; a JEDEC does not
say AS or ASV, so a 3.3 V target needs --part ATF150xASV or --device EPM70xxAE. The part comes from the fuse
count; 44-pin files need --package (PLCC44 or TQFP44).
pof2jed writes the ATF1502 / ATF1504 / ATF1508 of the same pins for an EPM7032 / EPM7064 / EPM7128 S or AE; the
AS part (5 V) for an S device, the ASV part (3.3 V) for an AE device; --part ATF150xAS or ATF150xASV chooses.
Options may stand anywhere on the line; option names are not case-sensitive; --opt=value also works.

Exit codes: 0 done, 3 refused by name (the reason is printed), 2 usage, 1 error.
```

Examples:

```
max7000dump decode chip.pof                                  equations on standard output
max7000dump decode chip.jed --package PLCC44
max7000dump emit chip.pof out                                build input + documentation view + REPORT.txt in out/
max7000dump emit chip.jed out --package PLCC44 --names original.pld      pin names taken from a CUPL source
max7000dump dump chip.pof                                    decoded fields + decoded netlist
max7000dump diff old.jed new.jed --package PLCC44            what changed, by macrocell and field
max7000dump devices                                          supported parts and packages (also: decode --viewlist)
max7000dump jed2pof chip.jed chip.pof --package PLCC44       ATF1502AS .jed -> EPM7032S .pof (5 V)
max7000dump jed2pof chip.jed chip.pof --part ATF1504ASV --package TQFP44      3.3 V: -> EPM7064AE
max7000dump pof2jed chip.pof chip.jed                        EPM7032S .pof -> ATF1502AS .jed (EPM7032AE -> ATF1502ASV)
max7000dump --version
```

- **decode** prints the report lines (input, part, decode status, RACING registers, pins not covered) and then the
  equations: for MAX parts the documentation view's body (per macrocell its sum, data, clock, enable, clear, preset and
  register; per pin its value and output enable); for ATF parts the CUPL pin lines and equations.
- **emit** writes the output folder: MAX `<name>.v` + `<name>.qsf` (build input, atom-level Verilog with every macrocell
  at its original location, for Quartus II 13.0sp1), `<name>_doc.v` (documentation view); ATF `<name>.pld` (build
  input, CUPL, for WinCUPL); `REPORT.txt`. The report lines are also printed on standard output.
- **dump** prints the decoded fields per macrocell and I/O cell and the decoded netlist.
- **diff** decodes two files of the same part and prints every decoded field that differs, as
  `<macrocell or pin> <field>: <value in a> -> <value in b>`, then `differences: <n>`. Files of different parts are
  refused (PARTS_DIFFER).
- **devices** lists the supported parts and packages and the parts that are not supported.

The banner `max7000 decoder tool by Darksoft v3.28. Use at your own risk.` goes to standard error, first, on every run.
Exit codes: 0 done, 3 refused by name, 2 usage, 1 error (and no arguments at all).

## Supported parts and packages (51; each decoded, re-compiled by the vendor tool and checked)

    ATF1502AS  PLCC44, TQFP44
    ATF1504AS  PLCC44, PLCC68, PLCC84, PQFP100, TQFP100, TQFP44
    ATF1508AS  PLCC84, PQFP100, PQFP160, TQFP100
    EPM7032AE  PLCC44, TQFP44
    EPM7032B   TQFP44
    EPM7032S   PLCC44, TQFP44
    EPM7064AE  FBGA100, PLCC44, TQFP100, TQFP44
    EPM7064B   FBGA100, TQFP100, TQFP44
    EPM7064S   PLCC44, PLCC84, TQFP100, TQFP44
    EPM7128AE  FBGA100, TQFP100, TQFP144
    EPM7128B   TQFP100, TQFP144
    EPM7128S   PLCC84, PQFP100, PQFP160, TQFP100
    EPM7160S   PLCC84, PQFP160, TQFP100
    EPM7192S   PQFP160
    EPM7256AE  FBGA100, FBGA256, PQFP208, TQFP100, TQFP144
    EPM7256B   FBGA256, TQFP100, TQFP144
    EPM7256S   PQFP208, RQFP208

Not supported: original (non-S) MAX 7000 and EPM7096 (no vendor tool in hand compiles them), EPM7512AE / EPM7512B.

## jed2pof

jed2pof -- ATF150x JEDEC to MAX 7000 POF

    max7000dump jed2pof chip.jed chip.pof --package PLCC44

Reads the .jed of an ATF1502AS, ATF1504AS or ATF1508AS and writes a .pof for the MAX part with the same pins:
ATF1502AS -> EPM7032S, ATF1504AS -> EPM7064S, ATF1508AS -> EPM7128S (5 V parts), in the same package. The MAX part
programmed with that .pof behaves as the ATF part programmed with the .jed: the same outputs, enables and register
states for every input sequence from power-up. No other program is needed.

Target voltage. A JEDEC file does not say whether it is for the 5 V part (ATF150xAS) or the 3.3 V part (ATF150xASV);
the same file programs both. By default jed2pof writes for the 5 V MAX part. For a 3.3 V board give --part ATF1502ASV
(or ATF1504ASV / ATF1508ASV) or --device EPM7032AE (EPM7064AE / EPM7128AE). 44-pin files also need --package PLCC44
or --package TQFP44, because the two packages number their pins differently. A part / package pair with no MAX part
(for example ATF1504AS PLCC68 or PQFP100) is refused (NO_TARGET_PACKAGE).

Power-up. The ATF150x registers start low after power-up (Atmel ATF1502AS and ATF1508AS datasheets); the MAX 7000S
registers start low too, and on the AE parts jed2pof sets every register to start low. For the ATF1504AS no datasheet
revision found states the value for that part itself; its files are converted on the strength of the family statement.

Pin keeper. The ATF150x parts can hold an undriven pin at its last level (pin keeper). The MAX 7000S and 7000AE parts
have no such circuit, so the keeper setting is dropped. An input pin left floating on the board may therefore behave
differently on the MAX part: tie unused or floating inputs to a fixed level. jed2pof prints "DROPPED PIN_KEEPER" for
every file whose keeper is on (93 of the 95 fitter and board files tested).

What is written. jed2pof writes a .pof only when every feature the file uses is one of the forms checked on the test
designs, and only after its own MAX decoder has read the new .pof back. Anything else is refused by name (exit code 3)
and no file is written. Refused features, with the number of the 194 test files refused for each:

    PAD_ENABLE_UNDETERMINED       66
    OUTPUT_FEEDBACK_SPLIT         12
    GOE_PT_SOURCE_NOT_MODELLED    10
    CASCADE_DONOR_FORM             9
    PAD_NOT_BONDED                 6
    NO_TARGET_PACKAGE              5
    POWER_DOWN_PIN                 4
    GLOBAL_CLEAR_POLARITY          2
    PIA_UNROUTABLE                 2
    FAST_INPUT_NOT_MODELLED        1

Also refused by name, not met in the test files: latches whose value reaches a pin (LATCH_NO_MAX_COUNTERPART),
output enables computed by a product term (PT_OE_NO_MAX_COUNTERPART), a third global clock or an inverted second one
(GLOBAL_CLOCK_FORM), and any form not in the checked set (FORM_NOT_PROVEN). No 3.3 V ATF1504 test design was
converted, so every EPM7064AE conversion is refused (FORM_NOT_PROVEN).

How it was checked. 194 test JEDECs: 105 written by the Atmel fitter, 87 written by Atmel's POF2JED from Quartus .pof
files, and the two Batman Forever U42 files. 117 were refused by name. The other 77 were converted, and for each one a
formal equivalence check between the decoded .jed and the decoded .pof, from power-up with every register low, found
them equal on every pin. By target part:

    target     equivalent   equivalent, timing     relies on the family
                                        hazard                statement
    EPM7032S           38                    5                        0
    EPM7064S            7                    1                       15
    EPM7128S            1                    8                        0
    EPM7032AE           1                    0                        0
    EPM7064AE           0                    0                        0
    EPM7128AE           1                    0                        0

"equivalent, timing hazard" = a register whose clock and data come from the same input, so its behaviour when both
change together depends on the chip's timing, on the ATF part as on the MAX part. "relies on the family statement" =
ATF1504AS files whose equality depends on the power-up value (see Power-up). The C program was compared byte for byte
with the checked reference on all 194 files and on 20000 corrupted files, on Linux and on Windows.
Read back by the vendor tools. Quartus II 13.0sp1 (quartus_cpf) opened each of the 77 converted .pof files and
converted it to .jam and .svf with no error and no warning: 77 of 77. Atmel's POF2JED 4.45.1 converted each one
back to an ATF .jed, and that .jed was compared formally with the original input .jed on every pin, from power-up.
By target part:

    target      files equivalent   equivalent, timing   not decided  different
                                               hazard                         
    EPM7032S       43         38                    5             0          0
    EPM7064S       23         22                    1             0          0
    EPM7128S        9          1                    6             2          0
    EPM7032AE       1          1                    0             0          0
    EPM7128AE       1          0                    0             1          0

None came back different. In the 3 files counted as "not decided", POF2JED's .jed connects a pad to logic lines
that nothing reads: an unconnected pad of the 84-pin PLCC package (2 files; POF2JED itself reports that its
conversion may not be correct) or the JTAG TDI pin (1 file). The decoder in this tool refuses such a file; with that
pad left free to take any value, all three are equal to the original as well (the two PLCC files with the timing
hazard described above).


## pof2jed

pof2jed -- MAX 7000 POF to ATF150x JEDEC

    max7000dump pof2jed chip.pof chip.jed

Reads the .pof of an EPM7032S, EPM7064S or EPM7128S (5 V) or an EPM7032AE, EPM7064AE or EPM7128AE (3.3 V) and writes
the .jed for the Atmel / Microchip part with the same pins, in the same package: EPM7032 -> ATF1502, EPM7064 ->
ATF1504, EPM7128 -> ATF1508; an S part gives the 5 V ATF150xAS, an AE part the 3.3 V ATF150xASV. The ATF part
programmed with that .jed behaves as the MAX part programmed with the .pof: the same outputs, enables and register
states for every input sequence from power-up. The .jed is laid out as the Atmel fitter writes it. No other program is
needed. Every other MAX part (EPM7160S and up, all 7000B parts) is refused (NO_ATF_TARGET), as is a package with no ATF
part of the same pins, such as FBGA100 or TQFP144 (NO_ATF_TARGET_PACKAGE).

Target voltage. The same .jed programs the AS and the ASV part; the first line pof2jed prints names the part it wrote
for. --part ATF150xAS or --part ATF150xASV chooses the other voltage of the same density; the file is then written only
if every feature it uses was checked for that exact part, otherwise it is refused (FORM_NOT_PROVEN).

What has no ATF counterpart.
  - Registers of an AE part set to start high at power-up: refused (REGISTER_POWERS_UP_HIGH). The ATF150x registers
    start low and the ATF has no inverter after the register. 153 of the 188 AE test files use them.
  - Ground pins of an AE part (unused pins tied low): written as ATF outputs driving 0. If the macrocell behind such a
    pin also carries logic the file is refused (GND_CLAMP_MACROCELL_USED).
  - Unused pins of an S part: Quartus writes an unused logic cell (a register that never clocks and whose clear is
    always on) and enables the pin's output. pof2jed writes the ATF pin driving 0, which is what that cell does when read
    from the image, and prints for every such pin
        pin <n> drives 0: level read from the image, not stated by the vendor
    because no Altera document states the level such a pin has on the board.
  - User code bits 17 to 32 of an AE part, and bit 16 on the EPM7128: the ATF part has no fuse for them; dropped, and
    named when not 1. The turbo bit becomes the ATF low-power setting. The ATF pin keeper is written off (the MAX parts
    have none).

What is written. pof2jed writes a .jed only when every feature the file uses is one of the forms checked on the test
designs, and only after its own ATF decoder has read the new .jed back. Anything else is refused by name (exit code 3)
and no file is written. Refused, with the number of the 478 test files refused for each:

    REGISTER_POWERS_UP_HIGH          153
    PT_OE_CONFLICT                    21
    PEXP_DONOR_MODE_UNWITNESSED       15
    NO_ATF_TARGET_PACKAGE             10
    GND_CLAMP_MACROCELL_USED           7
    GLOBAL_CLOCK_CONFIG_UNWITNESSED    4
    PEXP_ACROSS_LAB                    3
    JTAG_PIN_CONFLICT                  1
    OR_OUTPUT_INVERT_UNWITNESSED       1
    S_GLOBAL_UNWITNESSED               1

PT_OE_CONFLICT: a pin enable driven by a logic cell needs the pin's own enable term, which the file already uses.
Also refused by name, not met in the test files: UIM_UNROUTABLE, DONOR_FORM, DONOR_SEXP_FORM, DONOR_AT_BLOCK_EDGE,
PART_PAIR_MISMATCH (a --part of another density), and any form not in the checked set (FORM_NOT_PROVEN).

How it was checked. 478 test .pof files written by Quartus II 13.0sp1 for these six parts. 216 were refused by name.
The other 262 were converted, and for each one a formal equivalence check between the decoded .pof and the decoded .jed,
from power-up, found them equal on every pin it compared; no file was found different. By target part:

    target       equal     timing   family   unused  refused
                           hazard statement     pins         
    ATF1502AS       50         14        0       25       10   (from EPM7032S, 99 files)
    ATF1504AS        0          3       13       40       18   (from EPM7064S, 74 files)
    ATF1508AS       36          9        0       63        9   (from EPM7128S, 117 files)
    ATF1502ASV       4          0        0        0       62   (from EPM7032AE, 66 files)
    ATF1504ASV       0          0        3        0       53   (from EPM7064AE, 56 files)
    ATF1508ASV       2          0        0        0       64   (from EPM7128AE, 66 files)

"equal" = equal on every pin. "timing hazard" = a register whose clock and data come from the same input, so its
behaviour when both change together depends on the chip's timing, on either part. "family statement" = ATF1504AS files
whose equality depends on the power-up value: no datasheet revision found states it for that part itself, only for its
family. "unused pins" = equal on every pin except the S-part unused pins above, whose level rests on reading the image;
the report names them. The C program was compared byte for byte with the checked reference on all 478 files and on
20000 corrupted files, on Linux and on Windows. Converting back with jed2pof gives a .pof equal to the original
wherever jed2pof writes (163 files).

Atmel's POF2JED. POF2JED 4.45.1 was run on the same files. On a pin whose behaviour the vendor states, its .jed did
not behave as the .pof in these files:
    EPM7032S 4, EPM7064S 6, EPM7128S 0, EPM7032AE 39, EPM7064AE 27, EPM7128AE 7
Among the differences: AE registers that start high (POF2JED drops the setting; pof2jed refuses such files), AE ground
pins that POF2JED leaves undriven, and pin values that differ from power-up. In further files the only difference is
on the S-part unused pins above, which POF2JED leaves undriven:
    EPM7032S 25, EPM7064S 27, EPM7128S 24
What level the MAX part puts on those pins is not stated by any vendor file, so these are not counted as POF2JED errors.


## Known limits

- ATF1508AS: decoded designs are proven equivalent on their declared pins only; undeclared pads whose macrocell holds
  buried logic (class U3) are not proven. Two latch designs emit as UNPROVEN.
- ATF1502AS / ATF1504AS / ATF1508AS, class U3: an undeclared pad whose output enable comes from a product term routed
  into the sum has no documented behaviour (no vendor statement exists for such a pad; the ATF1508 fitter never leaves
  that state on a pad it reads). These pads are listed in REPORT.txt as not covered; keep them unconnected on the board.
  26 of the test designs carry such pads.
- ATF latch designs: the two test designs with latch storage (F_latch on ATF1502AS and ATF1504AS) emit a build input
  whose re-compiled fuse map leaves pads in the U3 state above, so their equivalence is not proven: UNPROVEN.
- ATF1508AS: a macrocell whose term 5 goes to the sum while its output enable is "VCC_pt5" with the "oe" function is
  refused PT5_GROUP_NOT_MODELLED (2 test designs, e.g. MC96 of F_latch_a).
- ATF parts: the power-down pin function (pd1 / pd2 set to "pd") is refused POWER_DOWN_PIN: no tool in hand produces
  its behaviour (4 test designs).
- MAX: a parallel-expander donor at the edge of a LAB is refused PEXP_ACROSS_LAB (7 test designs: EPM7064S, EPM7128AE,
  EPM7128B, EPM7256B, EPM7256S).
- MAX: a parallel-expander donor in a setting combination the vendor tool was never seen to produce is refused
  PEXP_DONOR_MODE_UNWITNESSED (3 test designs: EPM7032AE, EPM7032B, EPM7064AE).
- MAX: a POF that enables boundary scan (JTAG) while the I/O cells of the test-port pins are set as outputs is refused
  JTAG_PIN_CONFLICT: the vendor's POF and the vendor's own netlist of the same compile disagree on what such a part
  does (1 test design, EPM7032S).
- MAX, very full designs (EPM7160S, EPM7192S, EPM7256S, EPM7256AE, EPM7256B): the design decodes, but the build input does not
  fit in Quartus II 13.0sp1 (errors 163105 / 163106 at 89-100 % use, with the same interconnect demand per LAB as the
  original): 21 test designs.
- MAX, one completely full EPM7032S design: writing a pin as a constant costs one macrocell more than the part has
  (Quartus error 163017, 34 of 32 macrocells): 1 test design.
- MAX 7000B: the dedicated input pins' bus hold / weak pull-up is UNKNOWN (no fuse found, no vendor statement); the
  build input sets none.
- MAX 7000B: I/O standard code 3 covers LVTTL / LVCMOS / 2.5 V / PCI alike; the build input names the code, not the
  standard.
- RACING registers: a register whose clock and next state (data, enable, clear or preset) share a signal behaves, at
  the instant that signal changes, as the device timing decides; no tool in hand gives that timing. Every such register
  is named in REPORT.txt and in a comment of the build input.
- Original (non-S) MAX 7000 and EPM7096: not supported (no vendor tool in hand compiles them). EPM7512AE / EPM7512B:
  not supported (stops with an error, exit code 1).
- No measurement on a programmed part or a board is part of this release.

## Documentation view

The tool writes the documentation view (`<name>_doc.v`) for every decoded MAX design and does not check it against the
build input when it runs; the header of each `<name>_doc.v` says so. The view was checked equal to its build input, by a
formal equivalence check, for all 436 of 436 MAX test designs that decode. It is documentation; the build input is the
`.v` + `.qsf` pair.

## Rebuild

```
cd src && make             Linux (any c99 compiler, C library only): max7000dump
cd src && make windows     Windows x86-64 with x86_64-w64-mingw32-gcc: max7000dump.exe
```

The Windows binary is linked with a 32 MB stack (`-Wl,--stack,33554432`, needed by the largest designs), stripped and
without a link time stamp, so a rebuild gives the same bytes. Every device table is compiled in (`src/tab_*.c`,
generated data; `src/TABLES.sha256` lists their hashes): decoding needs no other program.

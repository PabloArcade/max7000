# max7000dump v3.25 -- source

MAX 7000S / AE / B (.pof) and ATF1502AS / ATF1504AS / ATF1508AS (.jed) fuse map decompiler by Darksoft.
Reads one fuse file and writes a design that the vendor tool compiles back into a part that behaves the same.
Built for arcade PCB preservation. Use at your own risk.

## Commands

```
max7000dump v3.25 -- MAX 7000S / AE / B and ATF1502AS / ATF1504AS / ATF1508AS fuse map decompiler by Darksoft

Commands:
  max7000dump decode --viewlist                          list supported parts + packages
  max7000dump decode [--part P] [--package PKG] <file>   equations per macrocell, to stdout
  max7000dump emit [--part P] [--package PKG] [--names F.pld] <file> <outdir>
                                                         build input + documentation view + REPORT.txt
  max7000dump dump [--part P] [--package PKG] <file>     structural report (decoded fields)
  max7000dump diff [--part P] [--package PKG] <a> <b>    macrocell-level diff of two files of one part
  max7000dump devices                                    supported parts, packages and notes
  max7000dump --version                                  version line only

Device selection: a .pof is decoded for the device named in its own device field; a .jed for the part
named in its "JEDEC file for:" header line (--part overrides it). An ATF package comes from the JEDEC
pin count, or from --package when the pin count does not decide it (44-pin parts). --part on a .pof
must match its device field. A file naming a part this tool does not support is refused by name.
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

The banner `max7000 decoder tool by Darksoft v3.25. Use at your own risk.` goes to standard error, first, on every run.
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

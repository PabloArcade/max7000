# max7000dump -- build with any c99 compiler and the C library only.
#   make            Linux (or any POSIX) binary: max7000dump
#   make windows    Windows x86-64 binary with mingw-w64: max7000dump.exe
# The Windows binary is linked with a 32 MB stack reserve: the regular-expression matcher recurses over long
# netlist lines and the default 1 MB stack of a Windows thread is not enough for the largest designs. The
# Windows binary is stripped (the toolchain's runtime libraries carry debug sections otherwise) and carries no link
# time stamp, so that a rebuild gives the same bytes.
CC      = cc
CFLAGS  = -std=c99 -Wall -Wextra -O2
TARGET  = max7000dump
SRCS    = main.c run.c diff.c devices.c jed2pof.c util.c rx.c sha256.c pof.c jedec.c maxdev.c maxdec.c maxnet.c vonet.c maxemit.c maxdoc.c atfdec.c atfemit.c tab_max.c tab_pads.c tab_atf.c tab_devices.c tab_forms.c
OBJS    = $(SRCS:.c=.o)
HDRS    = util.h rx.h sha256.h pof.h jedec.h max.h atf.h tables.h

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

%.o: %.c $(HDRS)
	$(CC) $(CFLAGS) -c -o $@ $<

WINCC     = x86_64-w64-mingw32-gcc
WINSTACK  = 33554432
WINTARGET = max7000dump.exe

windows:
	$(WINCC) $(CFLAGS) -Wl,--strip-all -Wl,--no-insert-timestamp -Wl,--stack,$(WINSTACK) -o $(WINTARGET) $(SRCS)

clean:
	rm -f $(OBJS) $(TARGET) $(WINTARGET)

.PHONY: clean windows

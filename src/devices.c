/* devices.c -- the devices command (also decode --viewlist): the supported part + package pairs and the parts that
 * are not supported. */
#include "util.h"

extern const char *const dev_pairs[][2];
extern const int n_dev_pairs;

void print_devices(void) {
    sbuf b; sb_init(&b);
    sb_printf(&b, "Supported parts and packages (%d):\n", n_dev_pairs);
    for (int i = 0; i < n_dev_pairs; i++) sb_printf(&b, "  %-11s %s\n", dev_pairs[i][0], dev_pairs[i][1]);
    sb_add(&b, "Not supported: original (non-S) MAX 7000 and EPM7096 (no vendor tool in hand compiles them), EPM7512AE / EPM7512B.\n"
               "Note: ATF1508AS: decoded designs are proven equivalent on their declared pins only; undeclared pads whose\n"
               "macrocell holds buried logic (class U3) are not proven.\n");
    put_text(stdout, sb_take(&b));
}

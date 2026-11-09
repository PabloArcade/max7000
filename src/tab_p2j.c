/* tab_p2j.c -- data of the POF -> JEDEC converter (do not edit by hand).
 * Per ATF part: the JEDEC layout the fitter writes after the product-term rows (literal text and fuse fields), the
 * user fuses, the per-pin configuration fuses and their values, the CLK3 pad; and the forms pof2jed may write, each
 * of which occurs in a converted reference design whose ATF file was checked equivalent to the MAX image. */
#include "p2j.h"

static const int atf1502as_r1[] = {16};
static const char *const atf1502as_s1[] = {""};
static const int atf1502as_r3[] = {32};
static const char *const atf1502as_s3[] = {""};
static const int atf1502as_r5[] = {32};
static const char *const atf1502as_s5[] = {""};
static const int atf1502as_r7[] = {16};
static const char *const atf1502as_s7[] = {""};
static const int atf1502as_r9[] = {32};
static const char *const atf1502as_s9[] = {""};
static const int atf1502as_r11[] = {32};
static const char *const atf1502as_s11[] = {""};
static const int atf1502as_r13[] = {16};
static const char *const atf1502as_s13[] = {""};
static const int atf1502as_r15[] = {32};
static const char *const atf1502as_s15[] = {""};
static const int atf1502as_r17[] = {32};
static const char *const atf1502as_s17[] = {""};
static const int atf1502as_r19[] = {16};
static const char *const atf1502as_s19[] = {""};
static const int atf1502as_r21[] = {32};
static const char *const atf1502as_s21[] = {""};
static const int atf1502as_r23[] = {32};
static const char *const atf1502as_s23[] = {""};
static const int atf1502as_r25[] = {16};
static const char *const atf1502as_s25[] = {""};
static const int atf1502as_r27[] = {32};
static const char *const atf1502as_s27[] = {""};
static const int atf1502as_r29[] = {32};
static const char *const atf1502as_s29[] = {""};
static const int atf1502as_r31[] = {16};
static const char *const atf1502as_s31[] = {""};
static const int atf1502as_r33[] = {32};
static const char *const atf1502as_s33[] = {""};
static const int atf1502as_r35[] = {32};
static const char *const atf1502as_s35[] = {""};
static const int atf1502as_r37[] = {16};
static const char *const atf1502as_s37[] = {""};
static const int atf1502as_r39[] = {32};
static const char *const atf1502as_s39[] = {""};
static const int atf1502as_r41[] = {32};
static const char *const atf1502as_s41[] = {""};
static const int atf1502as_r43[] = {16};
static const char *const atf1502as_s43[] = {""};
static const int atf1502as_r45[] = {32};
static const char *const atf1502as_s45[] = {""};
static const int atf1502as_r47[] = {32};
static const char *const atf1502as_s47[] = {""};
static const int atf1502as_r49[] = {16};
static const char *const atf1502as_s49[] = {""};
static const int atf1502as_r51[] = {32};
static const char *const atf1502as_s51[] = {""};
static const int atf1502as_r53[] = {32};
static const char *const atf1502as_s53[] = {""};
static const int atf1502as_r55[] = {16};
static const char *const atf1502as_s55[] = {""};
static const int atf1502as_r57[] = {32};
static const char *const atf1502as_s57[] = {""};
static const int atf1502as_r59[] = {32};
static const char *const atf1502as_s59[] = {""};
static const int atf1502as_r61[] = {16};
static const char *const atf1502as_s61[] = {""};
static const int atf1502as_r63[] = {32};
static const char *const atf1502as_s63[] = {""};
static const int atf1502as_r65[] = {32};
static const char *const atf1502as_s65[] = {""};
static const int atf1502as_r67[] = {16};
static const char *const atf1502as_s67[] = {""};
static const int atf1502as_r69[] = {32};
static const char *const atf1502as_s69[] = {""};
static const int atf1502as_r71[] = {32};
static const char *const atf1502as_s71[] = {""};
static const int atf1502as_r73[] = {5};
static const char *const atf1502as_s73[] = {""};
static const int atf1502as_r75[] = {5};
static const char *const atf1502as_s75[] = {""};
static const int atf1502as_r77[] = {5};
static const char *const atf1502as_s77[] = {""};
static const int atf1502as_r79[] = {5};
static const char *const atf1502as_s79[] = {""};
static const int atf1502as_r81[] = {5};
static const char *const atf1502as_s81[] = {""};
static const int atf1502as_r83[] = {5};
static const char *const atf1502as_s83[] = {""};
static const int atf1502as_r85[] = {5};
static const char *const atf1502as_s85[] = {""};
static const int atf1502as_r87[] = {5};
static const char *const atf1502as_s87[] = {""};
static const int atf1502as_r89[] = {5};
static const char *const atf1502as_s89[] = {""};
static const int atf1502as_r91[] = {5};
static const char *const atf1502as_s91[] = {""};
static const int atf1502as_r93[] = {5};
static const char *const atf1502as_s93[] = {""};
static const int atf1502as_r95[] = {5};
static const char *const atf1502as_s95[] = {""};
static const int atf1502as_r97[] = {5};
static const char *const atf1502as_s97[] = {""};
static const int atf1502as_r99[] = {5};
static const char *const atf1502as_s99[] = {""};
static const int atf1502as_r101[] = {5};
static const char *const atf1502as_s101[] = {""};
static const int atf1502as_r103[] = {5};
static const char *const atf1502as_s103[] = {""};
static const int atf1502as_r105[] = {5};
static const char *const atf1502as_s105[] = {""};
static const int atf1502as_r107[] = {5};
static const char *const atf1502as_s107[] = {""};
static const int atf1502as_r109[] = {5};
static const char *const atf1502as_s109[] = {""};
static const int atf1502as_r111[] = {5};
static const char *const atf1502as_s111[] = {""};
static const int atf1502as_r113[] = {5};
static const char *const atf1502as_s113[] = {""};
static const int atf1502as_r115[] = {5};
static const char *const atf1502as_s115[] = {""};
static const int atf1502as_r117[] = {5};
static const char *const atf1502as_s117[] = {""};
static const int atf1502as_r119[] = {5};
static const char *const atf1502as_s119[] = {""};
static const int atf1502as_r121[] = {5};
static const char *const atf1502as_s121[] = {""};
static const int atf1502as_r123[] = {5};
static const char *const atf1502as_s123[] = {""};
static const int atf1502as_r125[] = {5};
static const char *const atf1502as_s125[] = {""};
static const int atf1502as_r127[] = {5};
static const char *const atf1502as_s127[] = {""};
static const int atf1502as_r129[] = {5};
static const char *const atf1502as_s129[] = {""};
static const int atf1502as_r131[] = {5};
static const char *const atf1502as_s131[] = {""};
static const int atf1502as_r133[] = {5};
static const char *const atf1502as_s133[] = {""};
static const int atf1502as_r135[] = {5};
static const char *const atf1502as_s135[] = {""};
static const int atf1502as_r137[] = {5};
static const char *const atf1502as_s137[] = {""};
static const int atf1502as_r139[] = {5};
static const char *const atf1502as_s139[] = {""};
static const int atf1502as_r141[] = {5};
static const char *const atf1502as_s141[] = {""};
static const int atf1502as_r143[] = {5};
static const char *const atf1502as_s143[] = {""};
static const int atf1502as_r145[] = {5};
static const char *const atf1502as_s145[] = {""};
static const int atf1502as_r147[] = {5};
static const char *const atf1502as_s147[] = {""};
static const int atf1502as_r149[] = {5};
static const char *const atf1502as_s149[] = {""};
static const int atf1502as_r151[] = {5};
static const char *const atf1502as_s151[] = {""};
static const int atf1502as_r153[] = {5};
static const char *const atf1502as_s153[] = {""};
static const int atf1502as_r155[] = {5};
static const char *const atf1502as_s155[] = {""};
static const int atf1502as_r157[] = {5};
static const char *const atf1502as_s157[] = {""};
static const int atf1502as_r159[] = {5};
static const char *const atf1502as_s159[] = {""};
static const int atf1502as_r161[] = {5};
static const char *const atf1502as_s161[] = {""};
static const int atf1502as_r163[] = {5};
static const char *const atf1502as_s163[] = {""};
static const int atf1502as_r165[] = {5};
static const char *const atf1502as_s165[] = {""};
static const int atf1502as_r167[] = {5};
static const char *const atf1502as_s167[] = {""};
static const int atf1502as_r169[] = {5};
static const char *const atf1502as_s169[] = {""};
static const int atf1502as_r171[] = {5};
static const char *const atf1502as_s171[] = {""};
static const int atf1502as_r173[] = {5};
static const char *const atf1502as_s173[] = {""};
static const int atf1502as_r175[] = {5};
static const char *const atf1502as_s175[] = {""};
static const int atf1502as_r177[] = {5};
static const char *const atf1502as_s177[] = {""};
static const int atf1502as_r179[] = {5};
static const char *const atf1502as_s179[] = {""};
static const int atf1502as_r181[] = {5};
static const char *const atf1502as_s181[] = {""};
static const int atf1502as_r183[] = {5};
static const char *const atf1502as_s183[] = {""};
static const int atf1502as_r185[] = {5};
static const char *const atf1502as_s185[] = {""};
static const int atf1502as_r187[] = {5};
static const char *const atf1502as_s187[] = {""};
static const int atf1502as_r189[] = {5};
static const char *const atf1502as_s189[] = {""};
static const int atf1502as_r191[] = {5};
static const char *const atf1502as_s191[] = {""};
static const int atf1502as_r193[] = {5};
static const char *const atf1502as_s193[] = {""};
static const int atf1502as_r195[] = {5};
static const char *const atf1502as_s195[] = {""};
static const int atf1502as_r197[] = {5};
static const char *const atf1502as_s197[] = {""};
static const int atf1502as_r199[] = {5};
static const char *const atf1502as_s199[] = {""};
static const int atf1502as_r201[] = {5};
static const char *const atf1502as_s201[] = {""};
static const int atf1502as_r203[] = {5};
static const char *const atf1502as_s203[] = {""};
static const int atf1502as_r205[] = {5};
static const char *const atf1502as_s205[] = {""};
static const int atf1502as_r207[] = {5};
static const char *const atf1502as_s207[] = {""};
static const int atf1502as_r209[] = {5};
static const char *const atf1502as_s209[] = {""};
static const int atf1502as_r211[] = {5};
static const char *const atf1502as_s211[] = {""};
static const int atf1502as_r213[] = {5};
static const char *const atf1502as_s213[] = {""};
static const int atf1502as_r215[] = {5};
static const char *const atf1502as_s215[] = {""};
static const int atf1502as_r217[] = {5};
static const char *const atf1502as_s217[] = {""};
static const int atf1502as_r219[] = {5};
static const char *const atf1502as_s219[] = {""};
static const int atf1502as_r221[] = {5};
static const char *const atf1502as_s221[] = {""};
static const int atf1502as_r223[] = {5};
static const char *const atf1502as_s223[] = {""};
static const int atf1502as_r225[] = {5};
static const char *const atf1502as_s225[] = {""};
static const int atf1502as_r227[] = {5};
static const char *const atf1502as_s227[] = {""};
static const int atf1502as_r229[] = {5};
static const char *const atf1502as_s229[] = {""};
static const int atf1502as_r231[] = {5};
static const char *const atf1502as_s231[] = {""};
static const int atf1502as_r233[] = {5,5,5,5,5,5};
static const char *const atf1502as_s233[] = {"\r\n","\r\n","\r\n","\r\n","\r\n","\r\n"};
static const int atf1502as_r235[] = {32};
static const char *const atf1502as_s235[] = {""};
static const int atf1502as_r237[] = {4};
static const char *const atf1502as_s237[] = {""};
static const int atf1502as_r239[] = {16};
static const char *const atf1502as_s239[] = {""};
static const int atf1502as_r241[] = {6};
static const char *const atf1502as_s241[] = {""};
static const struct p2jseg atf1502as_segs[] = {
    {"NOTE macrocell configurations\r\n        0         0         0         0 *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15360, " ", 1, atf1502as_r1, atf1502as_s1},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15376, " ", 1, atf1502as_r3, atf1502as_s3},
    {"*  NOTE S16,S12 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15408, " ", 1, atf1502as_r5, atf1502as_s5},
    {"*  NOTE S14,S11 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15440, " ", 1, atf1502as_r7, atf1502as_s7},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15456, " ", 1, atf1502as_r9, atf1502as_s9},
    {"*  NOTE S9 ,S6  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15488, " ", 1, atf1502as_r11, atf1502as_s11},
    {"*  NOTE S13,S10 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15520, " ", 1, atf1502as_r13, atf1502as_s13},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15536, " ", 1, atf1502as_r15, atf1502as_s15},
    {"*  NOTE S20,S18 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15568, " ", 1, atf1502as_r17, atf1502as_s17},
    {"*  NOTE S8 ,S21 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15600, " ", 1, atf1502as_r19, atf1502as_s19},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15616, " ", 1, atf1502as_r21, atf1502as_s21},
    {"*  NOTE S7 ,S19 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15648, " ", 1, atf1502as_r23, atf1502as_s23},
    {"*  NOTE S22,S5  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15680, " ", 1, atf1502as_r25, atf1502as_s25},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15696, " ", 1, atf1502as_r27, atf1502as_s27},
    {"*  NOTE S23,S4  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15728, " ", 1, atf1502as_r29, atf1502as_s29},
    {"*  NOTE S3 ,S15 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15760, " ", 1, atf1502as_r31, atf1502as_s31},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15776, " ", 1, atf1502as_r33, atf1502as_s33},
    {"*  NOTE S0 ,S1  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15808, " ", 1, atf1502as_r35, atf1502as_s35},
    {"*  NOTE S17,S2  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15840, " ", 1, atf1502as_r37, atf1502as_s37},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15856, " ", 1, atf1502as_r39, atf1502as_s39},
    {"*  NOTE S16,S12 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15888, " ", 1, atf1502as_r41, atf1502as_s41},
    {"*  NOTE S14,S11 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15920, " ", 1, atf1502as_r43, atf1502as_s43},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15936, " ", 1, atf1502as_r45, atf1502as_s45},
    {"*  NOTE S9 ,S6  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 15968, " ", 1, atf1502as_r47, atf1502as_s47},
    {"*  NOTE S13,S10 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16000, " ", 1, atf1502as_r49, atf1502as_s49},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16016, " ", 1, atf1502as_r51, atf1502as_s51},
    {"*  NOTE S20,S18 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16048, " ", 1, atf1502as_r53, atf1502as_s53},
    {"*  NOTE S8 ,S21 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16080, " ", 1, atf1502as_r55, atf1502as_s55},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16096, " ", 1, atf1502as_r57, atf1502as_s57},
    {"*  NOTE S7 ,S19 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16128, " ", 1, atf1502as_r59, atf1502as_s59},
    {"*  NOTE S22,S5  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16160, " ", 1, atf1502as_r61, atf1502as_s61},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16176, " ", 1, atf1502as_r63, atf1502as_s63},
    {"*  NOTE S23,S4  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16208, " ", 1, atf1502as_r65, atf1502as_s65},
    {"*  NOTE S3 ,S15 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16240, " ", 1, atf1502as_r67, atf1502as_s67},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16256, " ", 1, atf1502as_r69, atf1502as_s69},
    {"*  NOTE S0 ,S1  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16288, " ", 1, atf1502as_r71, atf1502as_s71},
    {"*  NOTE S17,S2  of block B * \r\n\r\nNOTE   UIM for block A and B *\r\nNOTE         0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16320, " ", 1, atf1502as_r73, atf1502as_s73},
    {"*  NOTE Mux-39 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16325, " ", 1, atf1502as_r75, atf1502as_s75},
    {"*  NOTE Mux-39 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16330, " ", 1, atf1502as_r77, atf1502as_s77},
    {"*  NOTE Mux-38 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16335, " ", 1, atf1502as_r79, atf1502as_s79},
    {"*  NOTE Mux-38 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16340, " ", 1, atf1502as_r81, atf1502as_s81},
    {"*  NOTE Mux-37 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16345, " ", 1, atf1502as_r83, atf1502as_s83},
    {"*  NOTE Mux-37 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16350, " ", 1, atf1502as_r85, atf1502as_s85},
    {"*  NOTE Mux-36 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16355, " ", 1, atf1502as_r87, atf1502as_s87},
    {"*  NOTE Mux-36 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16360, " ", 1, atf1502as_r89, atf1502as_s89},
    {"*  NOTE Mux-35 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16365, " ", 1, atf1502as_r91, atf1502as_s91},
    {"*  NOTE Mux-35 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16370, " ", 1, atf1502as_r93, atf1502as_s93},
    {"*  NOTE Mux-34 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16375, " ", 1, atf1502as_r95, atf1502as_s95},
    {"*  NOTE Mux-34 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16380, " ", 1, atf1502as_r97, atf1502as_s97},
    {"*  NOTE Mux-33 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16385, " ", 1, atf1502as_r99, atf1502as_s99},
    {"*  NOTE Mux-33 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16390, " ", 1, atf1502as_r101, atf1502as_s101},
    {"*  NOTE Mux-32 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16395, " ", 1, atf1502as_r103, atf1502as_s103},
    {"*  NOTE Mux-32 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16400, " ", 1, atf1502as_r105, atf1502as_s105},
    {"*  NOTE Mux-31 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16405, " ", 1, atf1502as_r107, atf1502as_s107},
    {"*  NOTE Mux-31 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16410, " ", 1, atf1502as_r109, atf1502as_s109},
    {"*  NOTE Mux-30 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16415, " ", 1, atf1502as_r111, atf1502as_s111},
    {"*  NOTE Mux-30 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16420, " ", 1, atf1502as_r113, atf1502as_s113},
    {"*  NOTE Mux-29 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16425, " ", 1, atf1502as_r115, atf1502as_s115},
    {"*  NOTE Mux-29 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16430, " ", 1, atf1502as_r117, atf1502as_s117},
    {"*  NOTE Mux-28 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16435, " ", 1, atf1502as_r119, atf1502as_s119},
    {"*  NOTE Mux-28 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16440, " ", 1, atf1502as_r121, atf1502as_s121},
    {"*  NOTE Mux-27 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16445, " ", 1, atf1502as_r123, atf1502as_s123},
    {"*  NOTE Mux-27 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16450, " ", 1, atf1502as_r125, atf1502as_s125},
    {"*  NOTE Mux-26 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16455, " ", 1, atf1502as_r127, atf1502as_s127},
    {"*  NOTE Mux-26 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16460, " ", 1, atf1502as_r129, atf1502as_s129},
    {"*  NOTE Mux-25 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16465, " ", 1, atf1502as_r131, atf1502as_s131},
    {"*  NOTE Mux-25 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16470, " ", 1, atf1502as_r133, atf1502as_s133},
    {"*  NOTE Mux-24 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16475, " ", 1, atf1502as_r135, atf1502as_s135},
    {"*  NOTE Mux-24 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16480, " ", 1, atf1502as_r137, atf1502as_s137},
    {"*  NOTE Mux-23 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16485, " ", 1, atf1502as_r139, atf1502as_s139},
    {"*  NOTE Mux-23 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16490, " ", 1, atf1502as_r141, atf1502as_s141},
    {"*  NOTE Mux-22 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16495, " ", 1, atf1502as_r143, atf1502as_s143},
    {"*  NOTE Mux-22 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16500, " ", 1, atf1502as_r145, atf1502as_s145},
    {"*  NOTE Mux-21 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16505, " ", 1, atf1502as_r147, atf1502as_s147},
    {"*  NOTE Mux-21 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16510, " ", 1, atf1502as_r149, atf1502as_s149},
    {"*  NOTE Mux-20 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16515, " ", 1, atf1502as_r151, atf1502as_s151},
    {"*  NOTE Mux-20 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16520, " ", 1, atf1502as_r153, atf1502as_s153},
    {"*  NOTE Mux-19 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16525, " ", 1, atf1502as_r155, atf1502as_s155},
    {"*  NOTE Mux-19 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16530, " ", 1, atf1502as_r157, atf1502as_s157},
    {"*  NOTE Mux-18 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16535, " ", 1, atf1502as_r159, atf1502as_s159},
    {"*  NOTE Mux-18 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16540, " ", 1, atf1502as_r161, atf1502as_s161},
    {"*  NOTE Mux-17 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16545, " ", 1, atf1502as_r163, atf1502as_s163},
    {"*  NOTE Mux-17 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16550, " ", 1, atf1502as_r165, atf1502as_s165},
    {"*  NOTE Mux-16 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16555, " ", 1, atf1502as_r167, atf1502as_s167},
    {"*  NOTE Mux-16 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16560, " ", 1, atf1502as_r169, atf1502as_s169},
    {"*  NOTE Mux-15 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16565, " ", 1, atf1502as_r171, atf1502as_s171},
    {"*  NOTE Mux-15 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16570, " ", 1, atf1502as_r173, atf1502as_s173},
    {"*  NOTE Mux-14 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16575, " ", 1, atf1502as_r175, atf1502as_s175},
    {"*  NOTE Mux-14 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16580, " ", 1, atf1502as_r177, atf1502as_s177},
    {"*  NOTE Mux-13 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16585, " ", 1, atf1502as_r179, atf1502as_s179},
    {"*  NOTE Mux-13 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16590, " ", 1, atf1502as_r181, atf1502as_s181},
    {"*  NOTE Mux-12 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16595, " ", 1, atf1502as_r183, atf1502as_s183},
    {"*  NOTE Mux-12 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16600, " ", 1, atf1502as_r185, atf1502as_s185},
    {"*  NOTE Mux-11 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16605, " ", 1, atf1502as_r187, atf1502as_s187},
    {"*  NOTE Mux-11 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16610, " ", 1, atf1502as_r189, atf1502as_s189},
    {"*  NOTE Mux-10 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16615, " ", 1, atf1502as_r191, atf1502as_s191},
    {"*  NOTE Mux-10 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16620, " ", 1, atf1502as_r193, atf1502as_s193},
    {"*  NOTE Mux-9 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16625, " ", 1, atf1502as_r195, atf1502as_s195},
    {"*  NOTE Mux-9 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16630, " ", 1, atf1502as_r197, atf1502as_s197},
    {"*  NOTE Mux-8 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16635, " ", 1, atf1502as_r199, atf1502as_s199},
    {"*  NOTE Mux-8 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16640, " ", 1, atf1502as_r201, atf1502as_s201},
    {"*  NOTE Mux-7 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16645, " ", 1, atf1502as_r203, atf1502as_s203},
    {"*  NOTE Mux-7 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16650, " ", 1, atf1502as_r205, atf1502as_s205},
    {"*  NOTE Mux-6 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16655, " ", 1, atf1502as_r207, atf1502as_s207},
    {"*  NOTE Mux-6 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16660, " ", 1, atf1502as_r209, atf1502as_s209},
    {"*  NOTE Mux-5 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16665, " ", 1, atf1502as_r211, atf1502as_s211},
    {"*  NOTE Mux-5 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16670, " ", 1, atf1502as_r213, atf1502as_s213},
    {"*  NOTE Mux-4 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16675, " ", 1, atf1502as_r215, atf1502as_s215},
    {"*  NOTE Mux-4 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16680, " ", 1, atf1502as_r217, atf1502as_s217},
    {"*  NOTE Mux-3 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16685, " ", 1, atf1502as_r219, atf1502as_s219},
    {"*  NOTE Mux-3 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16690, " ", 1, atf1502as_r221, atf1502as_s221},
    {"*  NOTE Mux-2 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16695, " ", 1, atf1502as_r223, atf1502as_s223},
    {"*  NOTE Mux-2 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16700, " ", 1, atf1502as_r225, atf1502as_s225},
    {"*  NOTE Mux-1 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16705, " ", 1, atf1502as_r227, atf1502as_s227},
    {"*  NOTE Mux-1 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16710, " ", 1, atf1502as_r229, atf1502as_s229},
    {"*  NOTE Mux-0 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16715, " ", 1, atf1502as_r231, atf1502as_s231},
    {"*  NOTE Mux-0 of block B*\r\n\r\nNOTE  6 global OE \r\n0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16720, "\r\n", 6, atf1502as_r233, atf1502as_s233},
    {"*\r\n\r\nNOTE  device configuration bits*\r\nNOTE   0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16750, " ", 1, atf1502as_r235, atf1502as_s235},
    {"*\r\n\r\nNOTE  Special Purpose Bits (JTAG) *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16782, " ", 1, atf1502as_r237, atf1502as_s237},
    {"*\r\n\r\nNOTE  UES bits*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16786, " ", 1, atf1502as_r239, atf1502as_s239},
    {"*\r\n\r\nNOTE  Reserved bits *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 16802, " ", 1, atf1502as_r241, atf1502as_s241},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
};
static const int atf1502as_user[] = {
    16786,16787,16788,16789,16790,16791,16792,16793,16794,16795,16796,16797,16798,16799,16800,16801,
};
static const int atf1502as_pinfix[] = {
    16770,0,16771,0,16772,0,16773,1,16774,1,
};
static const int atf1504as_r1[] = {16};
static const char *const atf1504as_s1[] = {""};
static const int atf1504as_r3[] = {32};
static const char *const atf1504as_s3[] = {""};
static const int atf1504as_r5[] = {32};
static const char *const atf1504as_s5[] = {""};
static const int atf1504as_r7[] = {16};
static const char *const atf1504as_s7[] = {""};
static const int atf1504as_r9[] = {32};
static const char *const atf1504as_s9[] = {""};
static const int atf1504as_r11[] = {32};
static const char *const atf1504as_s11[] = {""};
static const int atf1504as_r13[] = {16};
static const char *const atf1504as_s13[] = {""};
static const int atf1504as_r15[] = {32};
static const char *const atf1504as_s15[] = {""};
static const int atf1504as_r17[] = {32};
static const char *const atf1504as_s17[] = {""};
static const int atf1504as_r19[] = {16};
static const char *const atf1504as_s19[] = {""};
static const int atf1504as_r21[] = {32};
static const char *const atf1504as_s21[] = {""};
static const int atf1504as_r23[] = {32};
static const char *const atf1504as_s23[] = {""};
static const int atf1504as_r25[] = {16};
static const char *const atf1504as_s25[] = {""};
static const int atf1504as_r27[] = {32};
static const char *const atf1504as_s27[] = {""};
static const int atf1504as_r29[] = {32};
static const char *const atf1504as_s29[] = {""};
static const int atf1504as_r31[] = {16};
static const char *const atf1504as_s31[] = {""};
static const int atf1504as_r33[] = {32};
static const char *const atf1504as_s33[] = {""};
static const int atf1504as_r35[] = {32};
static const char *const atf1504as_s35[] = {""};
static const int atf1504as_r37[] = {16};
static const char *const atf1504as_s37[] = {""};
static const int atf1504as_r39[] = {32};
static const char *const atf1504as_s39[] = {""};
static const int atf1504as_r41[] = {32};
static const char *const atf1504as_s41[] = {""};
static const int atf1504as_r43[] = {16};
static const char *const atf1504as_s43[] = {""};
static const int atf1504as_r45[] = {32};
static const char *const atf1504as_s45[] = {""};
static const int atf1504as_r47[] = {32};
static const char *const atf1504as_s47[] = {""};
static const int atf1504as_r49[] = {16};
static const char *const atf1504as_s49[] = {""};
static const int atf1504as_r51[] = {32};
static const char *const atf1504as_s51[] = {""};
static const int atf1504as_r53[] = {32};
static const char *const atf1504as_s53[] = {""};
static const int atf1504as_r55[] = {16};
static const char *const atf1504as_s55[] = {""};
static const int atf1504as_r57[] = {32};
static const char *const atf1504as_s57[] = {""};
static const int atf1504as_r59[] = {32};
static const char *const atf1504as_s59[] = {""};
static const int atf1504as_r61[] = {16};
static const char *const atf1504as_s61[] = {""};
static const int atf1504as_r63[] = {32};
static const char *const atf1504as_s63[] = {""};
static const int atf1504as_r65[] = {32};
static const char *const atf1504as_s65[] = {""};
static const int atf1504as_r67[] = {16};
static const char *const atf1504as_s67[] = {""};
static const int atf1504as_r69[] = {32};
static const char *const atf1504as_s69[] = {""};
static const int atf1504as_r71[] = {32};
static const char *const atf1504as_s71[] = {""};
static const int atf1504as_r73[] = {16};
static const char *const atf1504as_s73[] = {""};
static const int atf1504as_r75[] = {32};
static const char *const atf1504as_s75[] = {""};
static const int atf1504as_r77[] = {32};
static const char *const atf1504as_s77[] = {""};
static const int atf1504as_r79[] = {16};
static const char *const atf1504as_s79[] = {""};
static const int atf1504as_r81[] = {32};
static const char *const atf1504as_s81[] = {""};
static const int atf1504as_r83[] = {32};
static const char *const atf1504as_s83[] = {""};
static const int atf1504as_r85[] = {16};
static const char *const atf1504as_s85[] = {""};
static const int atf1504as_r87[] = {32};
static const char *const atf1504as_s87[] = {""};
static const int atf1504as_r89[] = {32};
static const char *const atf1504as_s89[] = {""};
static const int atf1504as_r91[] = {16};
static const char *const atf1504as_s91[] = {""};
static const int atf1504as_r93[] = {32};
static const char *const atf1504as_s93[] = {""};
static const int atf1504as_r95[] = {32};
static const char *const atf1504as_s95[] = {""};
static const int atf1504as_r97[] = {16};
static const char *const atf1504as_s97[] = {""};
static const int atf1504as_r99[] = {32};
static const char *const atf1504as_s99[] = {""};
static const int atf1504as_r101[] = {32};
static const char *const atf1504as_s101[] = {""};
static const int atf1504as_r103[] = {16};
static const char *const atf1504as_s103[] = {""};
static const int atf1504as_r105[] = {32};
static const char *const atf1504as_s105[] = {""};
static const int atf1504as_r107[] = {32};
static const char *const atf1504as_s107[] = {""};
static const int atf1504as_r109[] = {16};
static const char *const atf1504as_s109[] = {""};
static const int atf1504as_r111[] = {32};
static const char *const atf1504as_s111[] = {""};
static const int atf1504as_r113[] = {32};
static const char *const atf1504as_s113[] = {""};
static const int atf1504as_r115[] = {16};
static const char *const atf1504as_s115[] = {""};
static const int atf1504as_r117[] = {32};
static const char *const atf1504as_s117[] = {""};
static const int atf1504as_r119[] = {32};
static const char *const atf1504as_s119[] = {""};
static const int atf1504as_r121[] = {16};
static const char *const atf1504as_s121[] = {""};
static const int atf1504as_r123[] = {32};
static const char *const atf1504as_s123[] = {""};
static const int atf1504as_r125[] = {32};
static const char *const atf1504as_s125[] = {""};
static const int atf1504as_r127[] = {16};
static const char *const atf1504as_s127[] = {""};
static const int atf1504as_r129[] = {32};
static const char *const atf1504as_s129[] = {""};
static const int atf1504as_r131[] = {32};
static const char *const atf1504as_s131[] = {""};
static const int atf1504as_r133[] = {16};
static const char *const atf1504as_s133[] = {""};
static const int atf1504as_r135[] = {32};
static const char *const atf1504as_s135[] = {""};
static const int atf1504as_r137[] = {32};
static const char *const atf1504as_s137[] = {""};
static const int atf1504as_r139[] = {16};
static const char *const atf1504as_s139[] = {""};
static const int atf1504as_r141[] = {32};
static const char *const atf1504as_s141[] = {""};
static const int atf1504as_r143[] = {32};
static const char *const atf1504as_s143[] = {""};
static const int atf1504as_r145[] = {9};
static const char *const atf1504as_s145[] = {""};
static const int atf1504as_r147[] = {9};
static const char *const atf1504as_s147[] = {""};
static const int atf1504as_r149[] = {9};
static const char *const atf1504as_s149[] = {""};
static const int atf1504as_r151[] = {9};
static const char *const atf1504as_s151[] = {""};
static const int atf1504as_r153[] = {9};
static const char *const atf1504as_s153[] = {""};
static const int atf1504as_r155[] = {9};
static const char *const atf1504as_s155[] = {""};
static const int atf1504as_r157[] = {9};
static const char *const atf1504as_s157[] = {""};
static const int atf1504as_r159[] = {9};
static const char *const atf1504as_s159[] = {""};
static const int atf1504as_r161[] = {9};
static const char *const atf1504as_s161[] = {""};
static const int atf1504as_r163[] = {9};
static const char *const atf1504as_s163[] = {""};
static const int atf1504as_r165[] = {9};
static const char *const atf1504as_s165[] = {""};
static const int atf1504as_r167[] = {9};
static const char *const atf1504as_s167[] = {""};
static const int atf1504as_r169[] = {9};
static const char *const atf1504as_s169[] = {""};
static const int atf1504as_r171[] = {9};
static const char *const atf1504as_s171[] = {""};
static const int atf1504as_r173[] = {9};
static const char *const atf1504as_s173[] = {""};
static const int atf1504as_r175[] = {9};
static const char *const atf1504as_s175[] = {""};
static const int atf1504as_r177[] = {9};
static const char *const atf1504as_s177[] = {""};
static const int atf1504as_r179[] = {9};
static const char *const atf1504as_s179[] = {""};
static const int atf1504as_r181[] = {9};
static const char *const atf1504as_s181[] = {""};
static const int atf1504as_r183[] = {9};
static const char *const atf1504as_s183[] = {""};
static const int atf1504as_r185[] = {9};
static const char *const atf1504as_s185[] = {""};
static const int atf1504as_r187[] = {9};
static const char *const atf1504as_s187[] = {""};
static const int atf1504as_r189[] = {9};
static const char *const atf1504as_s189[] = {""};
static const int atf1504as_r191[] = {9};
static const char *const atf1504as_s191[] = {""};
static const int atf1504as_r193[] = {9};
static const char *const atf1504as_s193[] = {""};
static const int atf1504as_r195[] = {9};
static const char *const atf1504as_s195[] = {""};
static const int atf1504as_r197[] = {9};
static const char *const atf1504as_s197[] = {""};
static const int atf1504as_r199[] = {9};
static const char *const atf1504as_s199[] = {""};
static const int atf1504as_r201[] = {9};
static const char *const atf1504as_s201[] = {""};
static const int atf1504as_r203[] = {9};
static const char *const atf1504as_s203[] = {""};
static const int atf1504as_r205[] = {9};
static const char *const atf1504as_s205[] = {""};
static const int atf1504as_r207[] = {9};
static const char *const atf1504as_s207[] = {""};
static const int atf1504as_r209[] = {9};
static const char *const atf1504as_s209[] = {""};
static const int atf1504as_r211[] = {9};
static const char *const atf1504as_s211[] = {""};
static const int atf1504as_r213[] = {9};
static const char *const atf1504as_s213[] = {""};
static const int atf1504as_r215[] = {9};
static const char *const atf1504as_s215[] = {""};
static const int atf1504as_r217[] = {9};
static const char *const atf1504as_s217[] = {""};
static const int atf1504as_r219[] = {9};
static const char *const atf1504as_s219[] = {""};
static const int atf1504as_r221[] = {9};
static const char *const atf1504as_s221[] = {""};
static const int atf1504as_r223[] = {9};
static const char *const atf1504as_s223[] = {""};
static const int atf1504as_r225[] = {9};
static const char *const atf1504as_s225[] = {""};
static const int atf1504as_r227[] = {9};
static const char *const atf1504as_s227[] = {""};
static const int atf1504as_r229[] = {9};
static const char *const atf1504as_s229[] = {""};
static const int atf1504as_r231[] = {9};
static const char *const atf1504as_s231[] = {""};
static const int atf1504as_r233[] = {9};
static const char *const atf1504as_s233[] = {""};
static const int atf1504as_r235[] = {9};
static const char *const atf1504as_s235[] = {""};
static const int atf1504as_r237[] = {9};
static const char *const atf1504as_s237[] = {""};
static const int atf1504as_r239[] = {9};
static const char *const atf1504as_s239[] = {""};
static const int atf1504as_r241[] = {9};
static const char *const atf1504as_s241[] = {""};
static const int atf1504as_r243[] = {9};
static const char *const atf1504as_s243[] = {""};
static const int atf1504as_r245[] = {9};
static const char *const atf1504as_s245[] = {""};
static const int atf1504as_r247[] = {9};
static const char *const atf1504as_s247[] = {""};
static const int atf1504as_r249[] = {9};
static const char *const atf1504as_s249[] = {""};
static const int atf1504as_r251[] = {9};
static const char *const atf1504as_s251[] = {""};
static const int atf1504as_r253[] = {9};
static const char *const atf1504as_s253[] = {""};
static const int atf1504as_r255[] = {9};
static const char *const atf1504as_s255[] = {""};
static const int atf1504as_r257[] = {9};
static const char *const atf1504as_s257[] = {""};
static const int atf1504as_r259[] = {9};
static const char *const atf1504as_s259[] = {""};
static const int atf1504as_r261[] = {9};
static const char *const atf1504as_s261[] = {""};
static const int atf1504as_r263[] = {9};
static const char *const atf1504as_s263[] = {""};
static const int atf1504as_r265[] = {9};
static const char *const atf1504as_s265[] = {""};
static const int atf1504as_r267[] = {9};
static const char *const atf1504as_s267[] = {""};
static const int atf1504as_r269[] = {9};
static const char *const atf1504as_s269[] = {""};
static const int atf1504as_r271[] = {9};
static const char *const atf1504as_s271[] = {""};
static const int atf1504as_r273[] = {9};
static const char *const atf1504as_s273[] = {""};
static const int atf1504as_r275[] = {9};
static const char *const atf1504as_s275[] = {""};
static const int atf1504as_r277[] = {9};
static const char *const atf1504as_s277[] = {""};
static const int atf1504as_r279[] = {9};
static const char *const atf1504as_s279[] = {""};
static const int atf1504as_r281[] = {9};
static const char *const atf1504as_s281[] = {""};
static const int atf1504as_r283[] = {9};
static const char *const atf1504as_s283[] = {""};
static const int atf1504as_r285[] = {9};
static const char *const atf1504as_s285[] = {""};
static const int atf1504as_r287[] = {9};
static const char *const atf1504as_s287[] = {""};
static const int atf1504as_r289[] = {9};
static const char *const atf1504as_s289[] = {""};
static const int atf1504as_r291[] = {9};
static const char *const atf1504as_s291[] = {""};
static const int atf1504as_r293[] = {9};
static const char *const atf1504as_s293[] = {""};
static const int atf1504as_r295[] = {9};
static const char *const atf1504as_s295[] = {""};
static const int atf1504as_r297[] = {9};
static const char *const atf1504as_s297[] = {""};
static const int atf1504as_r299[] = {9};
static const char *const atf1504as_s299[] = {""};
static const int atf1504as_r301[] = {9};
static const char *const atf1504as_s301[] = {""};
static const int atf1504as_r303[] = {9};
static const char *const atf1504as_s303[] = {""};
static const int atf1504as_r305[] = {9};
static const char *const atf1504as_s305[] = {""};
static const int atf1504as_r307[] = {9};
static const char *const atf1504as_s307[] = {""};
static const int atf1504as_r309[] = {9};
static const char *const atf1504as_s309[] = {""};
static const int atf1504as_r311[] = {9};
static const char *const atf1504as_s311[] = {""};
static const int atf1504as_r313[] = {9};
static const char *const atf1504as_s313[] = {""};
static const int atf1504as_r315[] = {9};
static const char *const atf1504as_s315[] = {""};
static const int atf1504as_r317[] = {9};
static const char *const atf1504as_s317[] = {""};
static const int atf1504as_r319[] = {9};
static const char *const atf1504as_s319[] = {""};
static const int atf1504as_r321[] = {9};
static const char *const atf1504as_s321[] = {""};
static const int atf1504as_r323[] = {9};
static const char *const atf1504as_s323[] = {""};
static const int atf1504as_r325[] = {9};
static const char *const atf1504as_s325[] = {""};
static const int atf1504as_r327[] = {9};
static const char *const atf1504as_s327[] = {""};
static const int atf1504as_r329[] = {9};
static const char *const atf1504as_s329[] = {""};
static const int atf1504as_r331[] = {9};
static const char *const atf1504as_s331[] = {""};
static const int atf1504as_r333[] = {9};
static const char *const atf1504as_s333[] = {""};
static const int atf1504as_r335[] = {9};
static const char *const atf1504as_s335[] = {""};
static const int atf1504as_r337[] = {9};
static const char *const atf1504as_s337[] = {""};
static const int atf1504as_r339[] = {9};
static const char *const atf1504as_s339[] = {""};
static const int atf1504as_r341[] = {9};
static const char *const atf1504as_s341[] = {""};
static const int atf1504as_r343[] = {9};
static const char *const atf1504as_s343[] = {""};
static const int atf1504as_r345[] = {9};
static const char *const atf1504as_s345[] = {""};
static const int atf1504as_r347[] = {9};
static const char *const atf1504as_s347[] = {""};
static const int atf1504as_r349[] = {9};
static const char *const atf1504as_s349[] = {""};
static const int atf1504as_r351[] = {9};
static const char *const atf1504as_s351[] = {""};
static const int atf1504as_r353[] = {9};
static const char *const atf1504as_s353[] = {""};
static const int atf1504as_r355[] = {9};
static const char *const atf1504as_s355[] = {""};
static const int atf1504as_r357[] = {9};
static const char *const atf1504as_s357[] = {""};
static const int atf1504as_r359[] = {9};
static const char *const atf1504as_s359[] = {""};
static const int atf1504as_r361[] = {9};
static const char *const atf1504as_s361[] = {""};
static const int atf1504as_r363[] = {9};
static const char *const atf1504as_s363[] = {""};
static const int atf1504as_r365[] = {9};
static const char *const atf1504as_s365[] = {""};
static const int atf1504as_r367[] = {9};
static const char *const atf1504as_s367[] = {""};
static const int atf1504as_r369[] = {9};
static const char *const atf1504as_s369[] = {""};
static const int atf1504as_r371[] = {9};
static const char *const atf1504as_s371[] = {""};
static const int atf1504as_r373[] = {9};
static const char *const atf1504as_s373[] = {""};
static const int atf1504as_r375[] = {9};
static const char *const atf1504as_s375[] = {""};
static const int atf1504as_r377[] = {9};
static const char *const atf1504as_s377[] = {""};
static const int atf1504as_r379[] = {9};
static const char *const atf1504as_s379[] = {""};
static const int atf1504as_r381[] = {9};
static const char *const atf1504as_s381[] = {""};
static const int atf1504as_r383[] = {9};
static const char *const atf1504as_s383[] = {""};
static const int atf1504as_r385[] = {9};
static const char *const atf1504as_s385[] = {""};
static const int atf1504as_r387[] = {9};
static const char *const atf1504as_s387[] = {""};
static const int atf1504as_r389[] = {9};
static const char *const atf1504as_s389[] = {""};
static const int atf1504as_r391[] = {9};
static const char *const atf1504as_s391[] = {""};
static const int atf1504as_r393[] = {9};
static const char *const atf1504as_s393[] = {""};
static const int atf1504as_r395[] = {9};
static const char *const atf1504as_s395[] = {""};
static const int atf1504as_r397[] = {9};
static const char *const atf1504as_s397[] = {""};
static const int atf1504as_r399[] = {9};
static const char *const atf1504as_s399[] = {""};
static const int atf1504as_r401[] = {9};
static const char *const atf1504as_s401[] = {""};
static const int atf1504as_r403[] = {9};
static const char *const atf1504as_s403[] = {""};
static const int atf1504as_r405[] = {9};
static const char *const atf1504as_s405[] = {""};
static const int atf1504as_r407[] = {9};
static const char *const atf1504as_s407[] = {""};
static const int atf1504as_r409[] = {9};
static const char *const atf1504as_s409[] = {""};
static const int atf1504as_r411[] = {9};
static const char *const atf1504as_s411[] = {""};
static const int atf1504as_r413[] = {9};
static const char *const atf1504as_s413[] = {""};
static const int atf1504as_r415[] = {9};
static const char *const atf1504as_s415[] = {""};
static const int atf1504as_r417[] = {9};
static const char *const atf1504as_s417[] = {""};
static const int atf1504as_r419[] = {9};
static const char *const atf1504as_s419[] = {""};
static const int atf1504as_r421[] = {9};
static const char *const atf1504as_s421[] = {""};
static const int atf1504as_r423[] = {9};
static const char *const atf1504as_s423[] = {""};
static const int atf1504as_r425[] = {9};
static const char *const atf1504as_s425[] = {""};
static const int atf1504as_r427[] = {9};
static const char *const atf1504as_s427[] = {""};
static const int atf1504as_r429[] = {9};
static const char *const atf1504as_s429[] = {""};
static const int atf1504as_r431[] = {9};
static const char *const atf1504as_s431[] = {""};
static const int atf1504as_r433[] = {9};
static const char *const atf1504as_s433[] = {""};
static const int atf1504as_r435[] = {9};
static const char *const atf1504as_s435[] = {""};
static const int atf1504as_r437[] = {9};
static const char *const atf1504as_s437[] = {""};
static const int atf1504as_r439[] = {9};
static const char *const atf1504as_s439[] = {""};
static const int atf1504as_r441[] = {9};
static const char *const atf1504as_s441[] = {""};
static const int atf1504as_r443[] = {9};
static const char *const atf1504as_s443[] = {""};
static const int atf1504as_r445[] = {9};
static const char *const atf1504as_s445[] = {""};
static const int atf1504as_r447[] = {9};
static const char *const atf1504as_s447[] = {""};
static const int atf1504as_r449[] = {9};
static const char *const atf1504as_s449[] = {""};
static const int atf1504as_r451[] = {9};
static const char *const atf1504as_s451[] = {""};
static const int atf1504as_r453[] = {9};
static const char *const atf1504as_s453[] = {""};
static const int atf1504as_r455[] = {9};
static const char *const atf1504as_s455[] = {""};
static const int atf1504as_r457[] = {9};
static const char *const atf1504as_s457[] = {""};
static const int atf1504as_r459[] = {9};
static const char *const atf1504as_s459[] = {""};
static const int atf1504as_r461[] = {9};
static const char *const atf1504as_s461[] = {""};
static const int atf1504as_r463[] = {9};
static const char *const atf1504as_s463[] = {""};
static const int atf1504as_r465[] = {9,9,9,9,9,9};
static const char *const atf1504as_s465[] = {"\r\n","\r\n","\r\n","\r\n","\r\n","\r\n"};
static const int atf1504as_r467[] = {32};
static const char *const atf1504as_s467[] = {""};
static const int atf1504as_r469[] = {4};
static const char *const atf1504as_s469[] = {""};
static const int atf1504as_r471[] = {16};
static const char *const atf1504as_s471[] = {""};
static const int atf1504as_r473[] = {6};
static const char *const atf1504as_s473[] = {""};
static const struct p2jseg atf1504as_segs[] = {
    {"NOTE macrocell configurations\r\n        0         0         0         0 *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30720, " ", 1, atf1504as_r1, atf1504as_s1},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30736, " ", 1, atf1504as_r3, atf1504as_s3},
    {"*  NOTE S16,S12 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30768, " ", 1, atf1504as_r5, atf1504as_s5},
    {"*  NOTE S14,S11 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30800, " ", 1, atf1504as_r7, atf1504as_s7},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30816, " ", 1, atf1504as_r9, atf1504as_s9},
    {"*  NOTE S16,S12 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30848, " ", 1, atf1504as_r11, atf1504as_s11},
    {"*  NOTE S14,S11 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30880, " ", 1, atf1504as_r13, atf1504as_s13},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30896, " ", 1, atf1504as_r15, atf1504as_s15},
    {"*  NOTE S9 ,S6  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30928, " ", 1, atf1504as_r17, atf1504as_s17},
    {"*  NOTE S13,S10 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30960, " ", 1, atf1504as_r19, atf1504as_s19},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 30976, " ", 1, atf1504as_r21, atf1504as_s21},
    {"*  NOTE S9 ,S6  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31008, " ", 1, atf1504as_r23, atf1504as_s23},
    {"*  NOTE S13,S10 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31040, " ", 1, atf1504as_r25, atf1504as_s25},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31056, " ", 1, atf1504as_r27, atf1504as_s27},
    {"*  NOTE S20,S18 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31088, " ", 1, atf1504as_r29, atf1504as_s29},
    {"*  NOTE S8 ,S21 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31120, " ", 1, atf1504as_r31, atf1504as_s31},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31136, " ", 1, atf1504as_r33, atf1504as_s33},
    {"*  NOTE S20,S18 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31168, " ", 1, atf1504as_r35, atf1504as_s35},
    {"*  NOTE S8 ,S21 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31200, " ", 1, atf1504as_r37, atf1504as_s37},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31216, " ", 1, atf1504as_r39, atf1504as_s39},
    {"*  NOTE S7 ,S19 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31248, " ", 1, atf1504as_r41, atf1504as_s41},
    {"*  NOTE S22,S5  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31280, " ", 1, atf1504as_r43, atf1504as_s43},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31296, " ", 1, atf1504as_r45, atf1504as_s45},
    {"*  NOTE S7 ,S19 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31328, " ", 1, atf1504as_r47, atf1504as_s47},
    {"*  NOTE S22,S5  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31360, " ", 1, atf1504as_r49, atf1504as_s49},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31376, " ", 1, atf1504as_r51, atf1504as_s51},
    {"*  NOTE S23,S4  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31408, " ", 1, atf1504as_r53, atf1504as_s53},
    {"*  NOTE S3 ,S15 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31440, " ", 1, atf1504as_r55, atf1504as_s55},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31456, " ", 1, atf1504as_r57, atf1504as_s57},
    {"*  NOTE S23,S4  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31488, " ", 1, atf1504as_r59, atf1504as_s59},
    {"*  NOTE S3 ,S15 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31520, " ", 1, atf1504as_r61, atf1504as_s61},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31536, " ", 1, atf1504as_r63, atf1504as_s63},
    {"*  NOTE S0 ,S1  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31568, " ", 1, atf1504as_r65, atf1504as_s65},
    {"*  NOTE S17,S2  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31600, " ", 1, atf1504as_r67, atf1504as_s67},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31616, " ", 1, atf1504as_r69, atf1504as_s69},
    {"*  NOTE S0 ,S1  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31648, " ", 1, atf1504as_r71, atf1504as_s71},
    {"*  NOTE S17,S2  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31680, " ", 1, atf1504as_r73, atf1504as_s73},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31696, " ", 1, atf1504as_r75, atf1504as_s75},
    {"*  NOTE S16,S12 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31728, " ", 1, atf1504as_r77, atf1504as_s77},
    {"*  NOTE S14,S11 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31760, " ", 1, atf1504as_r79, atf1504as_s79},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31776, " ", 1, atf1504as_r81, atf1504as_s81},
    {"*  NOTE S16,S12 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31808, " ", 1, atf1504as_r83, atf1504as_s83},
    {"*  NOTE S14,S11 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31840, " ", 1, atf1504as_r85, atf1504as_s85},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31856, " ", 1, atf1504as_r87, atf1504as_s87},
    {"*  NOTE S9 ,S6  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31888, " ", 1, atf1504as_r89, atf1504as_s89},
    {"*  NOTE S13,S10 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31920, " ", 1, atf1504as_r91, atf1504as_s91},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31936, " ", 1, atf1504as_r93, atf1504as_s93},
    {"*  NOTE S9 ,S6  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 31968, " ", 1, atf1504as_r95, atf1504as_s95},
    {"*  NOTE S13,S10 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32000, " ", 1, atf1504as_r97, atf1504as_s97},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32016, " ", 1, atf1504as_r99, atf1504as_s99},
    {"*  NOTE S20,S18 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32048, " ", 1, atf1504as_r101, atf1504as_s101},
    {"*  NOTE S8 ,S21 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32080, " ", 1, atf1504as_r103, atf1504as_s103},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32096, " ", 1, atf1504as_r105, atf1504as_s105},
    {"*  NOTE S20,S18 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32128, " ", 1, atf1504as_r107, atf1504as_s107},
    {"*  NOTE S8 ,S21 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32160, " ", 1, atf1504as_r109, atf1504as_s109},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32176, " ", 1, atf1504as_r111, atf1504as_s111},
    {"*  NOTE S7 ,S19 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32208, " ", 1, atf1504as_r113, atf1504as_s113},
    {"*  NOTE S22,S5  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32240, " ", 1, atf1504as_r115, atf1504as_s115},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32256, " ", 1, atf1504as_r117, atf1504as_s117},
    {"*  NOTE S7 ,S19 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32288, " ", 1, atf1504as_r119, atf1504as_s119},
    {"*  NOTE S22,S5  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32320, " ", 1, atf1504as_r121, atf1504as_s121},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32336, " ", 1, atf1504as_r123, atf1504as_s123},
    {"*  NOTE S23,S4  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32368, " ", 1, atf1504as_r125, atf1504as_s125},
    {"*  NOTE S3 ,S15 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32400, " ", 1, atf1504as_r127, atf1504as_s127},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32416, " ", 1, atf1504as_r129, atf1504as_s129},
    {"*  NOTE S23,S4  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32448, " ", 1, atf1504as_r131, atf1504as_s131},
    {"*  NOTE S3 ,S15 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32480, " ", 1, atf1504as_r133, atf1504as_s133},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32496, " ", 1, atf1504as_r135, atf1504as_s135},
    {"*  NOTE S0 ,S1  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32528, " ", 1, atf1504as_r137, atf1504as_s137},
    {"*  NOTE S17,S2  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32560, " ", 1, atf1504as_r139, atf1504as_s139},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32576, " ", 1, atf1504as_r141, atf1504as_s141},
    {"*  NOTE S0 ,S1  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32608, " ", 1, atf1504as_r143, atf1504as_s143},
    {"*  NOTE S17,S2  of block C * \r\n\r\nNOTE   UIM for block A and D *\r\nNOTE         0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32640, " ", 1, atf1504as_r145, atf1504as_s145},
    {"*  NOTE Mux-0 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32649, " ", 1, atf1504as_r147, atf1504as_s147},
    {"*  NOTE Mux-1 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32658, " ", 1, atf1504as_r149, atf1504as_s149},
    {"*  NOTE Mux-0 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32667, " ", 1, atf1504as_r151, atf1504as_s151},
    {"*  NOTE Mux-1 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32676, " ", 1, atf1504as_r153, atf1504as_s153},
    {"*  NOTE Mux-2 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32685, " ", 1, atf1504as_r155, atf1504as_s155},
    {"*  NOTE Mux-3 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32694, " ", 1, atf1504as_r157, atf1504as_s157},
    {"*  NOTE Mux-2 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32703, " ", 1, atf1504as_r159, atf1504as_s159},
    {"*  NOTE Mux-3 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32712, " ", 1, atf1504as_r161, atf1504as_s161},
    {"*  NOTE Mux-4 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32721, " ", 1, atf1504as_r163, atf1504as_s163},
    {"*  NOTE Mux-5 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32730, " ", 1, atf1504as_r165, atf1504as_s165},
    {"*  NOTE Mux-4 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32739, " ", 1, atf1504as_r167, atf1504as_s167},
    {"*  NOTE Mux-5 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32748, " ", 1, atf1504as_r169, atf1504as_s169},
    {"*  NOTE Mux-6 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32757, " ", 1, atf1504as_r171, atf1504as_s171},
    {"*  NOTE Mux-7 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32766, " ", 1, atf1504as_r173, atf1504as_s173},
    {"*  NOTE Mux-6 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32775, " ", 1, atf1504as_r175, atf1504as_s175},
    {"*  NOTE Mux-7 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32784, " ", 1, atf1504as_r177, atf1504as_s177},
    {"*  NOTE Mux-8 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32793, " ", 1, atf1504as_r179, atf1504as_s179},
    {"*  NOTE Mux-9 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32802, " ", 1, atf1504as_r181, atf1504as_s181},
    {"*  NOTE Mux-8 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32811, " ", 1, atf1504as_r183, atf1504as_s183},
    {"*  NOTE Mux-9 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32820, " ", 1, atf1504as_r185, atf1504as_s185},
    {"*  NOTE Mux-10 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32829, " ", 1, atf1504as_r187, atf1504as_s187},
    {"*  NOTE Mux-11 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32838, " ", 1, atf1504as_r189, atf1504as_s189},
    {"*  NOTE Mux-10 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32847, " ", 1, atf1504as_r191, atf1504as_s191},
    {"*  NOTE Mux-11 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32856, " ", 1, atf1504as_r193, atf1504as_s193},
    {"*  NOTE Mux-12 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32865, " ", 1, atf1504as_r195, atf1504as_s195},
    {"*  NOTE Mux-13 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32874, " ", 1, atf1504as_r197, atf1504as_s197},
    {"*  NOTE Mux-12 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32883, " ", 1, atf1504as_r199, atf1504as_s199},
    {"*  NOTE Mux-13 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32892, " ", 1, atf1504as_r201, atf1504as_s201},
    {"*  NOTE Mux-14 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32901, " ", 1, atf1504as_r203, atf1504as_s203},
    {"*  NOTE Mux-15 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32910, " ", 1, atf1504as_r205, atf1504as_s205},
    {"*  NOTE Mux-14 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32919, " ", 1, atf1504as_r207, atf1504as_s207},
    {"*  NOTE Mux-15 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32928, " ", 1, atf1504as_r209, atf1504as_s209},
    {"*  NOTE Mux-16 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32937, " ", 1, atf1504as_r211, atf1504as_s211},
    {"*  NOTE Mux-17 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32946, " ", 1, atf1504as_r213, atf1504as_s213},
    {"*  NOTE Mux-16 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32955, " ", 1, atf1504as_r215, atf1504as_s215},
    {"*  NOTE Mux-17 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32964, " ", 1, atf1504as_r217, atf1504as_s217},
    {"*  NOTE Mux-18 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32973, " ", 1, atf1504as_r219, atf1504as_s219},
    {"*  NOTE Mux-19 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32982, " ", 1, atf1504as_r221, atf1504as_s221},
    {"*  NOTE Mux-18 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 32991, " ", 1, atf1504as_r223, atf1504as_s223},
    {"*  NOTE Mux-19 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33000, " ", 1, atf1504as_r225, atf1504as_s225},
    {"*  NOTE Mux-20 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33009, " ", 1, atf1504as_r227, atf1504as_s227},
    {"*  NOTE Mux-21 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33018, " ", 1, atf1504as_r229, atf1504as_s229},
    {"*  NOTE Mux-20 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33027, " ", 1, atf1504as_r231, atf1504as_s231},
    {"*  NOTE Mux-21 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33036, " ", 1, atf1504as_r233, atf1504as_s233},
    {"*  NOTE Mux-22 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33045, " ", 1, atf1504as_r235, atf1504as_s235},
    {"*  NOTE Mux-23 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33054, " ", 1, atf1504as_r237, atf1504as_s237},
    {"*  NOTE Mux-22 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33063, " ", 1, atf1504as_r239, atf1504as_s239},
    {"*  NOTE Mux-23 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33072, " ", 1, atf1504as_r241, atf1504as_s241},
    {"*  NOTE Mux-24 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33081, " ", 1, atf1504as_r243, atf1504as_s243},
    {"*  NOTE Mux-25 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33090, " ", 1, atf1504as_r245, atf1504as_s245},
    {"*  NOTE Mux-24 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33099, " ", 1, atf1504as_r247, atf1504as_s247},
    {"*  NOTE Mux-25 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33108, " ", 1, atf1504as_r249, atf1504as_s249},
    {"*  NOTE Mux-26 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33117, " ", 1, atf1504as_r251, atf1504as_s251},
    {"*  NOTE Mux-27 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33126, " ", 1, atf1504as_r253, atf1504as_s253},
    {"*  NOTE Mux-26 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33135, " ", 1, atf1504as_r255, atf1504as_s255},
    {"*  NOTE Mux-27 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33144, " ", 1, atf1504as_r257, atf1504as_s257},
    {"*  NOTE Mux-28 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33153, " ", 1, atf1504as_r259, atf1504as_s259},
    {"*  NOTE Mux-29 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33162, " ", 1, atf1504as_r261, atf1504as_s261},
    {"*  NOTE Mux-28 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33171, " ", 1, atf1504as_r263, atf1504as_s263},
    {"*  NOTE Mux-29 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33180, " ", 1, atf1504as_r265, atf1504as_s265},
    {"*  NOTE Mux-30 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33189, " ", 1, atf1504as_r267, atf1504as_s267},
    {"*  NOTE Mux-31 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33198, " ", 1, atf1504as_r269, atf1504as_s269},
    {"*  NOTE Mux-30 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33207, " ", 1, atf1504as_r271, atf1504as_s271},
    {"*  NOTE Mux-31 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33216, " ", 1, atf1504as_r273, atf1504as_s273},
    {"*  NOTE Mux-32 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33225, " ", 1, atf1504as_r275, atf1504as_s275},
    {"*  NOTE Mux-33 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33234, " ", 1, atf1504as_r277, atf1504as_s277},
    {"*  NOTE Mux-32 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33243, " ", 1, atf1504as_r279, atf1504as_s279},
    {"*  NOTE Mux-33 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33252, " ", 1, atf1504as_r281, atf1504as_s281},
    {"*  NOTE Mux-34 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33261, " ", 1, atf1504as_r283, atf1504as_s283},
    {"*  NOTE Mux-35 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33270, " ", 1, atf1504as_r285, atf1504as_s285},
    {"*  NOTE Mux-34 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33279, " ", 1, atf1504as_r287, atf1504as_s287},
    {"*  NOTE Mux-35 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33288, " ", 1, atf1504as_r289, atf1504as_s289},
    {"*  NOTE Mux-36 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33297, " ", 1, atf1504as_r291, atf1504as_s291},
    {"*  NOTE Mux-37 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33306, " ", 1, atf1504as_r293, atf1504as_s293},
    {"*  NOTE Mux-36 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33315, " ", 1, atf1504as_r295, atf1504as_s295},
    {"*  NOTE Mux-37 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33324, " ", 1, atf1504as_r297, atf1504as_s297},
    {"*  NOTE Mux-38 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33333, " ", 1, atf1504as_r299, atf1504as_s299},
    {"*  NOTE Mux-39 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33342, " ", 1, atf1504as_r301, atf1504as_s301},
    {"*  NOTE Mux-38 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33351, " ", 1, atf1504as_r303, atf1504as_s303},
    {"*  NOTE Mux-39 of block A*\r\n\r\nNOTE   UIM for block B and C *\r\nNOTE         0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33360, " ", 1, atf1504as_r305, atf1504as_s305},
    {"*  NOTE Mux-0 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33369, " ", 1, atf1504as_r307, atf1504as_s307},
    {"*  NOTE Mux-1 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33378, " ", 1, atf1504as_r309, atf1504as_s309},
    {"*  NOTE Mux-0 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33387, " ", 1, atf1504as_r311, atf1504as_s311},
    {"*  NOTE Mux-1 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33396, " ", 1, atf1504as_r313, atf1504as_s313},
    {"*  NOTE Mux-2 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33405, " ", 1, atf1504as_r315, atf1504as_s315},
    {"*  NOTE Mux-3 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33414, " ", 1, atf1504as_r317, atf1504as_s317},
    {"*  NOTE Mux-2 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33423, " ", 1, atf1504as_r319, atf1504as_s319},
    {"*  NOTE Mux-3 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33432, " ", 1, atf1504as_r321, atf1504as_s321},
    {"*  NOTE Mux-4 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33441, " ", 1, atf1504as_r323, atf1504as_s323},
    {"*  NOTE Mux-5 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33450, " ", 1, atf1504as_r325, atf1504as_s325},
    {"*  NOTE Mux-4 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33459, " ", 1, atf1504as_r327, atf1504as_s327},
    {"*  NOTE Mux-5 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33468, " ", 1, atf1504as_r329, atf1504as_s329},
    {"*  NOTE Mux-6 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33477, " ", 1, atf1504as_r331, atf1504as_s331},
    {"*  NOTE Mux-7 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33486, " ", 1, atf1504as_r333, atf1504as_s333},
    {"*  NOTE Mux-6 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33495, " ", 1, atf1504as_r335, atf1504as_s335},
    {"*  NOTE Mux-7 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33504, " ", 1, atf1504as_r337, atf1504as_s337},
    {"*  NOTE Mux-8 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33513, " ", 1, atf1504as_r339, atf1504as_s339},
    {"*  NOTE Mux-9 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33522, " ", 1, atf1504as_r341, atf1504as_s341},
    {"*  NOTE Mux-8 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33531, " ", 1, atf1504as_r343, atf1504as_s343},
    {"*  NOTE Mux-9 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33540, " ", 1, atf1504as_r345, atf1504as_s345},
    {"*  NOTE Mux-10 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33549, " ", 1, atf1504as_r347, atf1504as_s347},
    {"*  NOTE Mux-11 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33558, " ", 1, atf1504as_r349, atf1504as_s349},
    {"*  NOTE Mux-10 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33567, " ", 1, atf1504as_r351, atf1504as_s351},
    {"*  NOTE Mux-11 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33576, " ", 1, atf1504as_r353, atf1504as_s353},
    {"*  NOTE Mux-12 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33585, " ", 1, atf1504as_r355, atf1504as_s355},
    {"*  NOTE Mux-13 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33594, " ", 1, atf1504as_r357, atf1504as_s357},
    {"*  NOTE Mux-12 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33603, " ", 1, atf1504as_r359, atf1504as_s359},
    {"*  NOTE Mux-13 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33612, " ", 1, atf1504as_r361, atf1504as_s361},
    {"*  NOTE Mux-14 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33621, " ", 1, atf1504as_r363, atf1504as_s363},
    {"*  NOTE Mux-15 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33630, " ", 1, atf1504as_r365, atf1504as_s365},
    {"*  NOTE Mux-14 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33639, " ", 1, atf1504as_r367, atf1504as_s367},
    {"*  NOTE Mux-15 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33648, " ", 1, atf1504as_r369, atf1504as_s369},
    {"*  NOTE Mux-16 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33657, " ", 1, atf1504as_r371, atf1504as_s371},
    {"*  NOTE Mux-17 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33666, " ", 1, atf1504as_r373, atf1504as_s373},
    {"*  NOTE Mux-16 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33675, " ", 1, atf1504as_r375, atf1504as_s375},
    {"*  NOTE Mux-17 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33684, " ", 1, atf1504as_r377, atf1504as_s377},
    {"*  NOTE Mux-18 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33693, " ", 1, atf1504as_r379, atf1504as_s379},
    {"*  NOTE Mux-19 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33702, " ", 1, atf1504as_r381, atf1504as_s381},
    {"*  NOTE Mux-18 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33711, " ", 1, atf1504as_r383, atf1504as_s383},
    {"*  NOTE Mux-19 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33720, " ", 1, atf1504as_r385, atf1504as_s385},
    {"*  NOTE Mux-20 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33729, " ", 1, atf1504as_r387, atf1504as_s387},
    {"*  NOTE Mux-21 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33738, " ", 1, atf1504as_r389, atf1504as_s389},
    {"*  NOTE Mux-20 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33747, " ", 1, atf1504as_r391, atf1504as_s391},
    {"*  NOTE Mux-21 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33756, " ", 1, atf1504as_r393, atf1504as_s393},
    {"*  NOTE Mux-22 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33765, " ", 1, atf1504as_r395, atf1504as_s395},
    {"*  NOTE Mux-23 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33774, " ", 1, atf1504as_r397, atf1504as_s397},
    {"*  NOTE Mux-22 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33783, " ", 1, atf1504as_r399, atf1504as_s399},
    {"*  NOTE Mux-23 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33792, " ", 1, atf1504as_r401, atf1504as_s401},
    {"*  NOTE Mux-24 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33801, " ", 1, atf1504as_r403, atf1504as_s403},
    {"*  NOTE Mux-25 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33810, " ", 1, atf1504as_r405, atf1504as_s405},
    {"*  NOTE Mux-24 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33819, " ", 1, atf1504as_r407, atf1504as_s407},
    {"*  NOTE Mux-25 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33828, " ", 1, atf1504as_r409, atf1504as_s409},
    {"*  NOTE Mux-26 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33837, " ", 1, atf1504as_r411, atf1504as_s411},
    {"*  NOTE Mux-27 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33846, " ", 1, atf1504as_r413, atf1504as_s413},
    {"*  NOTE Mux-26 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33855, " ", 1, atf1504as_r415, atf1504as_s415},
    {"*  NOTE Mux-27 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33864, " ", 1, atf1504as_r417, atf1504as_s417},
    {"*  NOTE Mux-28 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33873, " ", 1, atf1504as_r419, atf1504as_s419},
    {"*  NOTE Mux-29 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33882, " ", 1, atf1504as_r421, atf1504as_s421},
    {"*  NOTE Mux-28 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33891, " ", 1, atf1504as_r423, atf1504as_s423},
    {"*  NOTE Mux-29 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33900, " ", 1, atf1504as_r425, atf1504as_s425},
    {"*  NOTE Mux-30 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33909, " ", 1, atf1504as_r427, atf1504as_s427},
    {"*  NOTE Mux-31 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33918, " ", 1, atf1504as_r429, atf1504as_s429},
    {"*  NOTE Mux-30 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33927, " ", 1, atf1504as_r431, atf1504as_s431},
    {"*  NOTE Mux-31 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33936, " ", 1, atf1504as_r433, atf1504as_s433},
    {"*  NOTE Mux-32 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33945, " ", 1, atf1504as_r435, atf1504as_s435},
    {"*  NOTE Mux-33 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33954, " ", 1, atf1504as_r437, atf1504as_s437},
    {"*  NOTE Mux-32 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33963, " ", 1, atf1504as_r439, atf1504as_s439},
    {"*  NOTE Mux-33 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33972, " ", 1, atf1504as_r441, atf1504as_s441},
    {"*  NOTE Mux-34 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33981, " ", 1, atf1504as_r443, atf1504as_s443},
    {"*  NOTE Mux-35 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33990, " ", 1, atf1504as_r445, atf1504as_s445},
    {"*  NOTE Mux-34 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 33999, " ", 1, atf1504as_r447, atf1504as_s447},
    {"*  NOTE Mux-35 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34008, " ", 1, atf1504as_r449, atf1504as_s449},
    {"*  NOTE Mux-36 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34017, " ", 1, atf1504as_r451, atf1504as_s451},
    {"*  NOTE Mux-37 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34026, " ", 1, atf1504as_r453, atf1504as_s453},
    {"*  NOTE Mux-36 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34035, " ", 1, atf1504as_r455, atf1504as_s455},
    {"*  NOTE Mux-37 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34044, " ", 1, atf1504as_r457, atf1504as_s457},
    {"*  NOTE Mux-38 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34053, " ", 1, atf1504as_r459, atf1504as_s459},
    {"*  NOTE Mux-39 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34062, " ", 1, atf1504as_r461, atf1504as_s461},
    {"*  NOTE Mux-38 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34071, " ", 1, atf1504as_r463, atf1504as_s463},
    {"*  NOTE Mux-39 of block B*\r\n\r\nNOTE  6 global OE \r\n0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34080, "\r\n", 6, atf1504as_r465, atf1504as_s465},
    {"*\r\n\r\nNOTE  device configuration bits*\r\nNOTE   0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34134, " ", 1, atf1504as_r467, atf1504as_s467},
    {"*\r\n\r\nNOTE  Special Purpose Bits (JTAG) *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34166, " ", 1, atf1504as_r469, atf1504as_s469},
    {"*\r\n\r\nNOTE  UES bits*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34170, " ", 1, atf1504as_r471, atf1504as_s471},
    {"*\r\n\r\nNOTE  Reserved bits *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 34186, " ", 1, atf1504as_r473, atf1504as_s473},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
};
static const int atf1504as_user[] = {
    34170,34171,34172,34173,34174,34175,34176,34177,34178,34179,34180,34181,34182,34183,34184,34185,
};
static const int atf1504as_pinfix[] = {
    34154,0,34155,0,34156,0,34157,1,34158,1,
};
static const int atf1508as_r1[] = {16};
static const char *const atf1508as_s1[] = {""};
static const int atf1508as_r3[] = {32};
static const char *const atf1508as_s3[] = {""};
static const int atf1508as_r5[] = {32};
static const char *const atf1508as_s5[] = {""};
static const int atf1508as_r7[] = {16};
static const char *const atf1508as_s7[] = {""};
static const int atf1508as_r9[] = {32};
static const char *const atf1508as_s9[] = {""};
static const int atf1508as_r11[] = {32};
static const char *const atf1508as_s11[] = {""};
static const int atf1508as_r13[] = {16};
static const char *const atf1508as_s13[] = {""};
static const int atf1508as_r15[] = {32};
static const char *const atf1508as_s15[] = {""};
static const int atf1508as_r17[] = {32};
static const char *const atf1508as_s17[] = {""};
static const int atf1508as_r19[] = {16};
static const char *const atf1508as_s19[] = {""};
static const int atf1508as_r21[] = {32};
static const char *const atf1508as_s21[] = {""};
static const int atf1508as_r23[] = {32};
static const char *const atf1508as_s23[] = {""};
static const int atf1508as_r25[] = {16};
static const char *const atf1508as_s25[] = {""};
static const int atf1508as_r27[] = {32};
static const char *const atf1508as_s27[] = {""};
static const int atf1508as_r29[] = {32};
static const char *const atf1508as_s29[] = {""};
static const int atf1508as_r31[] = {16};
static const char *const atf1508as_s31[] = {""};
static const int atf1508as_r33[] = {32};
static const char *const atf1508as_s33[] = {""};
static const int atf1508as_r35[] = {32};
static const char *const atf1508as_s35[] = {""};
static const int atf1508as_r37[] = {16};
static const char *const atf1508as_s37[] = {""};
static const int atf1508as_r39[] = {32};
static const char *const atf1508as_s39[] = {""};
static const int atf1508as_r41[] = {32};
static const char *const atf1508as_s41[] = {""};
static const int atf1508as_r43[] = {16};
static const char *const atf1508as_s43[] = {""};
static const int atf1508as_r45[] = {32};
static const char *const atf1508as_s45[] = {""};
static const int atf1508as_r47[] = {32};
static const char *const atf1508as_s47[] = {""};
static const int atf1508as_r49[] = {16};
static const char *const atf1508as_s49[] = {""};
static const int atf1508as_r51[] = {32};
static const char *const atf1508as_s51[] = {""};
static const int atf1508as_r53[] = {32};
static const char *const atf1508as_s53[] = {""};
static const int atf1508as_r55[] = {16};
static const char *const atf1508as_s55[] = {""};
static const int atf1508as_r57[] = {32};
static const char *const atf1508as_s57[] = {""};
static const int atf1508as_r59[] = {32};
static const char *const atf1508as_s59[] = {""};
static const int atf1508as_r61[] = {16};
static const char *const atf1508as_s61[] = {""};
static const int atf1508as_r63[] = {32};
static const char *const atf1508as_s63[] = {""};
static const int atf1508as_r65[] = {32};
static const char *const atf1508as_s65[] = {""};
static const int atf1508as_r67[] = {16};
static const char *const atf1508as_s67[] = {""};
static const int atf1508as_r69[] = {32};
static const char *const atf1508as_s69[] = {""};
static const int atf1508as_r71[] = {32};
static const char *const atf1508as_s71[] = {""};
static const int atf1508as_r73[] = {16};
static const char *const atf1508as_s73[] = {""};
static const int atf1508as_r75[] = {32};
static const char *const atf1508as_s75[] = {""};
static const int atf1508as_r77[] = {32};
static const char *const atf1508as_s77[] = {""};
static const int atf1508as_r79[] = {16};
static const char *const atf1508as_s79[] = {""};
static const int atf1508as_r81[] = {32};
static const char *const atf1508as_s81[] = {""};
static const int atf1508as_r83[] = {32};
static const char *const atf1508as_s83[] = {""};
static const int atf1508as_r85[] = {16};
static const char *const atf1508as_s85[] = {""};
static const int atf1508as_r87[] = {32};
static const char *const atf1508as_s87[] = {""};
static const int atf1508as_r89[] = {32};
static const char *const atf1508as_s89[] = {""};
static const int atf1508as_r91[] = {16};
static const char *const atf1508as_s91[] = {""};
static const int atf1508as_r93[] = {32};
static const char *const atf1508as_s93[] = {""};
static const int atf1508as_r95[] = {32};
static const char *const atf1508as_s95[] = {""};
static const int atf1508as_r97[] = {16};
static const char *const atf1508as_s97[] = {""};
static const int atf1508as_r99[] = {32};
static const char *const atf1508as_s99[] = {""};
static const int atf1508as_r101[] = {32};
static const char *const atf1508as_s101[] = {""};
static const int atf1508as_r103[] = {16};
static const char *const atf1508as_s103[] = {""};
static const int atf1508as_r105[] = {32};
static const char *const atf1508as_s105[] = {""};
static const int atf1508as_r107[] = {32};
static const char *const atf1508as_s107[] = {""};
static const int atf1508as_r109[] = {16};
static const char *const atf1508as_s109[] = {""};
static const int atf1508as_r111[] = {32};
static const char *const atf1508as_s111[] = {""};
static const int atf1508as_r113[] = {32};
static const char *const atf1508as_s113[] = {""};
static const int atf1508as_r115[] = {16};
static const char *const atf1508as_s115[] = {""};
static const int atf1508as_r117[] = {32};
static const char *const atf1508as_s117[] = {""};
static const int atf1508as_r119[] = {32};
static const char *const atf1508as_s119[] = {""};
static const int atf1508as_r121[] = {16};
static const char *const atf1508as_s121[] = {""};
static const int atf1508as_r123[] = {32};
static const char *const atf1508as_s123[] = {""};
static const int atf1508as_r125[] = {32};
static const char *const atf1508as_s125[] = {""};
static const int atf1508as_r127[] = {16};
static const char *const atf1508as_s127[] = {""};
static const int atf1508as_r129[] = {32};
static const char *const atf1508as_s129[] = {""};
static const int atf1508as_r131[] = {32};
static const char *const atf1508as_s131[] = {""};
static const int atf1508as_r133[] = {16};
static const char *const atf1508as_s133[] = {""};
static const int atf1508as_r135[] = {32};
static const char *const atf1508as_s135[] = {""};
static const int atf1508as_r137[] = {32};
static const char *const atf1508as_s137[] = {""};
static const int atf1508as_r139[] = {16};
static const char *const atf1508as_s139[] = {""};
static const int atf1508as_r141[] = {32};
static const char *const atf1508as_s141[] = {""};
static const int atf1508as_r143[] = {32};
static const char *const atf1508as_s143[] = {""};
static const int atf1508as_r145[] = {16};
static const char *const atf1508as_s145[] = {""};
static const int atf1508as_r147[] = {32};
static const char *const atf1508as_s147[] = {""};
static const int atf1508as_r149[] = {32};
static const char *const atf1508as_s149[] = {""};
static const int atf1508as_r151[] = {16};
static const char *const atf1508as_s151[] = {""};
static const int atf1508as_r153[] = {32};
static const char *const atf1508as_s153[] = {""};
static const int atf1508as_r155[] = {32};
static const char *const atf1508as_s155[] = {""};
static const int atf1508as_r157[] = {16};
static const char *const atf1508as_s157[] = {""};
static const int atf1508as_r159[] = {32};
static const char *const atf1508as_s159[] = {""};
static const int atf1508as_r161[] = {32};
static const char *const atf1508as_s161[] = {""};
static const int atf1508as_r163[] = {16};
static const char *const atf1508as_s163[] = {""};
static const int atf1508as_r165[] = {32};
static const char *const atf1508as_s165[] = {""};
static const int atf1508as_r167[] = {32};
static const char *const atf1508as_s167[] = {""};
static const int atf1508as_r169[] = {16};
static const char *const atf1508as_s169[] = {""};
static const int atf1508as_r171[] = {32};
static const char *const atf1508as_s171[] = {""};
static const int atf1508as_r173[] = {32};
static const char *const atf1508as_s173[] = {""};
static const int atf1508as_r175[] = {16};
static const char *const atf1508as_s175[] = {""};
static const int atf1508as_r177[] = {32};
static const char *const atf1508as_s177[] = {""};
static const int atf1508as_r179[] = {32};
static const char *const atf1508as_s179[] = {""};
static const int atf1508as_r181[] = {16};
static const char *const atf1508as_s181[] = {""};
static const int atf1508as_r183[] = {32};
static const char *const atf1508as_s183[] = {""};
static const int atf1508as_r185[] = {32};
static const char *const atf1508as_s185[] = {""};
static const int atf1508as_r187[] = {16};
static const char *const atf1508as_s187[] = {""};
static const int atf1508as_r189[] = {32};
static const char *const atf1508as_s189[] = {""};
static const int atf1508as_r191[] = {32};
static const char *const atf1508as_s191[] = {""};
static const int atf1508as_r193[] = {16};
static const char *const atf1508as_s193[] = {""};
static const int atf1508as_r195[] = {32};
static const char *const atf1508as_s195[] = {""};
static const int atf1508as_r197[] = {32};
static const char *const atf1508as_s197[] = {""};
static const int atf1508as_r199[] = {16};
static const char *const atf1508as_s199[] = {""};
static const int atf1508as_r201[] = {32};
static const char *const atf1508as_s201[] = {""};
static const int atf1508as_r203[] = {32};
static const char *const atf1508as_s203[] = {""};
static const int atf1508as_r205[] = {16};
static const char *const atf1508as_s205[] = {""};
static const int atf1508as_r207[] = {32};
static const char *const atf1508as_s207[] = {""};
static const int atf1508as_r209[] = {32};
static const char *const atf1508as_s209[] = {""};
static const int atf1508as_r211[] = {16};
static const char *const atf1508as_s211[] = {""};
static const int atf1508as_r213[] = {32};
static const char *const atf1508as_s213[] = {""};
static const int atf1508as_r215[] = {32};
static const char *const atf1508as_s215[] = {""};
static const int atf1508as_r217[] = {16};
static const char *const atf1508as_s217[] = {""};
static const int atf1508as_r219[] = {32};
static const char *const atf1508as_s219[] = {""};
static const int atf1508as_r221[] = {32};
static const char *const atf1508as_s221[] = {""};
static const int atf1508as_r223[] = {16};
static const char *const atf1508as_s223[] = {""};
static const int atf1508as_r225[] = {32};
static const char *const atf1508as_s225[] = {""};
static const int atf1508as_r227[] = {32};
static const char *const atf1508as_s227[] = {""};
static const int atf1508as_r229[] = {16};
static const char *const atf1508as_s229[] = {""};
static const int atf1508as_r231[] = {32};
static const char *const atf1508as_s231[] = {""};
static const int atf1508as_r233[] = {32};
static const char *const atf1508as_s233[] = {""};
static const int atf1508as_r235[] = {16};
static const char *const atf1508as_s235[] = {""};
static const int atf1508as_r237[] = {32};
static const char *const atf1508as_s237[] = {""};
static const int atf1508as_r239[] = {32};
static const char *const atf1508as_s239[] = {""};
static const int atf1508as_r241[] = {16};
static const char *const atf1508as_s241[] = {""};
static const int atf1508as_r243[] = {32};
static const char *const atf1508as_s243[] = {""};
static const int atf1508as_r245[] = {32};
static const char *const atf1508as_s245[] = {""};
static const int atf1508as_r247[] = {16};
static const char *const atf1508as_s247[] = {""};
static const int atf1508as_r249[] = {32};
static const char *const atf1508as_s249[] = {""};
static const int atf1508as_r251[] = {32};
static const char *const atf1508as_s251[] = {""};
static const int atf1508as_r253[] = {16};
static const char *const atf1508as_s253[] = {""};
static const int atf1508as_r255[] = {32};
static const char *const atf1508as_s255[] = {""};
static const int atf1508as_r257[] = {32};
static const char *const atf1508as_s257[] = {""};
static const int atf1508as_r259[] = {16};
static const char *const atf1508as_s259[] = {""};
static const int atf1508as_r261[] = {32};
static const char *const atf1508as_s261[] = {""};
static const int atf1508as_r263[] = {32};
static const char *const atf1508as_s263[] = {""};
static const int atf1508as_r265[] = {16};
static const char *const atf1508as_s265[] = {""};
static const int atf1508as_r267[] = {32};
static const char *const atf1508as_s267[] = {""};
static const int atf1508as_r269[] = {32};
static const char *const atf1508as_s269[] = {""};
static const int atf1508as_r271[] = {16};
static const char *const atf1508as_s271[] = {""};
static const int atf1508as_r273[] = {32};
static const char *const atf1508as_s273[] = {""};
static const int atf1508as_r275[] = {32};
static const char *const atf1508as_s275[] = {""};
static const int atf1508as_r277[] = {16};
static const char *const atf1508as_s277[] = {""};
static const int atf1508as_r279[] = {32};
static const char *const atf1508as_s279[] = {""};
static const int atf1508as_r281[] = {32};
static const char *const atf1508as_s281[] = {""};
static const int atf1508as_r283[] = {16};
static const char *const atf1508as_s283[] = {""};
static const int atf1508as_r285[] = {32};
static const char *const atf1508as_s285[] = {""};
static const int atf1508as_r287[] = {32};
static const char *const atf1508as_s287[] = {""};
static const int atf1508as_r289[] = {27};
static const char *const atf1508as_s289[] = {""};
static const int atf1508as_r291[] = {27};
static const char *const atf1508as_s291[] = {""};
static const int atf1508as_r293[] = {27};
static const char *const atf1508as_s293[] = {""};
static const int atf1508as_r295[] = {27};
static const char *const atf1508as_s295[] = {""};
static const int atf1508as_r297[] = {27};
static const char *const atf1508as_s297[] = {""};
static const int atf1508as_r299[] = {27};
static const char *const atf1508as_s299[] = {""};
static const int atf1508as_r301[] = {27};
static const char *const atf1508as_s301[] = {""};
static const int atf1508as_r303[] = {27};
static const char *const atf1508as_s303[] = {""};
static const int atf1508as_r305[] = {27};
static const char *const atf1508as_s305[] = {""};
static const int atf1508as_r307[] = {27};
static const char *const atf1508as_s307[] = {""};
static const int atf1508as_r309[] = {27};
static const char *const atf1508as_s309[] = {""};
static const int atf1508as_r311[] = {27};
static const char *const atf1508as_s311[] = {""};
static const int atf1508as_r313[] = {27};
static const char *const atf1508as_s313[] = {""};
static const int atf1508as_r315[] = {27};
static const char *const atf1508as_s315[] = {""};
static const int atf1508as_r317[] = {27};
static const char *const atf1508as_s317[] = {""};
static const int atf1508as_r319[] = {27};
static const char *const atf1508as_s319[] = {""};
static const int atf1508as_r321[] = {27};
static const char *const atf1508as_s321[] = {""};
static const int atf1508as_r323[] = {27};
static const char *const atf1508as_s323[] = {""};
static const int atf1508as_r325[] = {27};
static const char *const atf1508as_s325[] = {""};
static const int atf1508as_r327[] = {27};
static const char *const atf1508as_s327[] = {""};
static const int atf1508as_r329[] = {27};
static const char *const atf1508as_s329[] = {""};
static const int atf1508as_r331[] = {27};
static const char *const atf1508as_s331[] = {""};
static const int atf1508as_r333[] = {27};
static const char *const atf1508as_s333[] = {""};
static const int atf1508as_r335[] = {27};
static const char *const atf1508as_s335[] = {""};
static const int atf1508as_r337[] = {27};
static const char *const atf1508as_s337[] = {""};
static const int atf1508as_r339[] = {27};
static const char *const atf1508as_s339[] = {""};
static const int atf1508as_r341[] = {27};
static const char *const atf1508as_s341[] = {""};
static const int atf1508as_r343[] = {27};
static const char *const atf1508as_s343[] = {""};
static const int atf1508as_r345[] = {27};
static const char *const atf1508as_s345[] = {""};
static const int atf1508as_r347[] = {27};
static const char *const atf1508as_s347[] = {""};
static const int atf1508as_r349[] = {27};
static const char *const atf1508as_s349[] = {""};
static const int atf1508as_r351[] = {27};
static const char *const atf1508as_s351[] = {""};
static const int atf1508as_r353[] = {27};
static const char *const atf1508as_s353[] = {""};
static const int atf1508as_r355[] = {27};
static const char *const atf1508as_s355[] = {""};
static const int atf1508as_r357[] = {27};
static const char *const atf1508as_s357[] = {""};
static const int atf1508as_r359[] = {27};
static const char *const atf1508as_s359[] = {""};
static const int atf1508as_r361[] = {27};
static const char *const atf1508as_s361[] = {""};
static const int atf1508as_r363[] = {27};
static const char *const atf1508as_s363[] = {""};
static const int atf1508as_r365[] = {27};
static const char *const atf1508as_s365[] = {""};
static const int atf1508as_r367[] = {27};
static const char *const atf1508as_s367[] = {""};
static const int atf1508as_r369[] = {27};
static const char *const atf1508as_s369[] = {""};
static const int atf1508as_r371[] = {27};
static const char *const atf1508as_s371[] = {""};
static const int atf1508as_r373[] = {27};
static const char *const atf1508as_s373[] = {""};
static const int atf1508as_r375[] = {27};
static const char *const atf1508as_s375[] = {""};
static const int atf1508as_r377[] = {27};
static const char *const atf1508as_s377[] = {""};
static const int atf1508as_r379[] = {27};
static const char *const atf1508as_s379[] = {""};
static const int atf1508as_r381[] = {27};
static const char *const atf1508as_s381[] = {""};
static const int atf1508as_r383[] = {27};
static const char *const atf1508as_s383[] = {""};
static const int atf1508as_r385[] = {27};
static const char *const atf1508as_s385[] = {""};
static const int atf1508as_r387[] = {27};
static const char *const atf1508as_s387[] = {""};
static const int atf1508as_r389[] = {27};
static const char *const atf1508as_s389[] = {""};
static const int atf1508as_r391[] = {27};
static const char *const atf1508as_s391[] = {""};
static const int atf1508as_r393[] = {27};
static const char *const atf1508as_s393[] = {""};
static const int atf1508as_r395[] = {27};
static const char *const atf1508as_s395[] = {""};
static const int atf1508as_r397[] = {27};
static const char *const atf1508as_s397[] = {""};
static const int atf1508as_r399[] = {27};
static const char *const atf1508as_s399[] = {""};
static const int atf1508as_r401[] = {27};
static const char *const atf1508as_s401[] = {""};
static const int atf1508as_r403[] = {27};
static const char *const atf1508as_s403[] = {""};
static const int atf1508as_r405[] = {27};
static const char *const atf1508as_s405[] = {""};
static const int atf1508as_r407[] = {27};
static const char *const atf1508as_s407[] = {""};
static const int atf1508as_r409[] = {27};
static const char *const atf1508as_s409[] = {""};
static const int atf1508as_r411[] = {27};
static const char *const atf1508as_s411[] = {""};
static const int atf1508as_r413[] = {27};
static const char *const atf1508as_s413[] = {""};
static const int atf1508as_r415[] = {27};
static const char *const atf1508as_s415[] = {""};
static const int atf1508as_r417[] = {27};
static const char *const atf1508as_s417[] = {""};
static const int atf1508as_r419[] = {27};
static const char *const atf1508as_s419[] = {""};
static const int atf1508as_r421[] = {27};
static const char *const atf1508as_s421[] = {""};
static const int atf1508as_r423[] = {27};
static const char *const atf1508as_s423[] = {""};
static const int atf1508as_r425[] = {27};
static const char *const atf1508as_s425[] = {""};
static const int atf1508as_r427[] = {27};
static const char *const atf1508as_s427[] = {""};
static const int atf1508as_r429[] = {27};
static const char *const atf1508as_s429[] = {""};
static const int atf1508as_r431[] = {27};
static const char *const atf1508as_s431[] = {""};
static const int atf1508as_r433[] = {27};
static const char *const atf1508as_s433[] = {""};
static const int atf1508as_r435[] = {27};
static const char *const atf1508as_s435[] = {""};
static const int atf1508as_r437[] = {27};
static const char *const atf1508as_s437[] = {""};
static const int atf1508as_r439[] = {27};
static const char *const atf1508as_s439[] = {""};
static const int atf1508as_r441[] = {27};
static const char *const atf1508as_s441[] = {""};
static const int atf1508as_r443[] = {27};
static const char *const atf1508as_s443[] = {""};
static const int atf1508as_r445[] = {27};
static const char *const atf1508as_s445[] = {""};
static const int atf1508as_r447[] = {27};
static const char *const atf1508as_s447[] = {""};
static const int atf1508as_r449[] = {27};
static const char *const atf1508as_s449[] = {""};
static const int atf1508as_r451[] = {27};
static const char *const atf1508as_s451[] = {""};
static const int atf1508as_r453[] = {27};
static const char *const atf1508as_s453[] = {""};
static const int atf1508as_r455[] = {27};
static const char *const atf1508as_s455[] = {""};
static const int atf1508as_r457[] = {27};
static const char *const atf1508as_s457[] = {""};
static const int atf1508as_r459[] = {27};
static const char *const atf1508as_s459[] = {""};
static const int atf1508as_r461[] = {27};
static const char *const atf1508as_s461[] = {""};
static const int atf1508as_r463[] = {27};
static const char *const atf1508as_s463[] = {""};
static const int atf1508as_r465[] = {27};
static const char *const atf1508as_s465[] = {""};
static const int atf1508as_r467[] = {27};
static const char *const atf1508as_s467[] = {""};
static const int atf1508as_r469[] = {27};
static const char *const atf1508as_s469[] = {""};
static const int atf1508as_r471[] = {27};
static const char *const atf1508as_s471[] = {""};
static const int atf1508as_r473[] = {27};
static const char *const atf1508as_s473[] = {""};
static const int atf1508as_r475[] = {27};
static const char *const atf1508as_s475[] = {""};
static const int atf1508as_r477[] = {27};
static const char *const atf1508as_s477[] = {""};
static const int atf1508as_r479[] = {27};
static const char *const atf1508as_s479[] = {""};
static const int atf1508as_r481[] = {27};
static const char *const atf1508as_s481[] = {""};
static const int atf1508as_r483[] = {27};
static const char *const atf1508as_s483[] = {""};
static const int atf1508as_r485[] = {27};
static const char *const atf1508as_s485[] = {""};
static const int atf1508as_r487[] = {27};
static const char *const atf1508as_s487[] = {""};
static const int atf1508as_r489[] = {27};
static const char *const atf1508as_s489[] = {""};
static const int atf1508as_r491[] = {27};
static const char *const atf1508as_s491[] = {""};
static const int atf1508as_r493[] = {27};
static const char *const atf1508as_s493[] = {""};
static const int atf1508as_r495[] = {27};
static const char *const atf1508as_s495[] = {""};
static const int atf1508as_r497[] = {27};
static const char *const atf1508as_s497[] = {""};
static const int atf1508as_r499[] = {27};
static const char *const atf1508as_s499[] = {""};
static const int atf1508as_r501[] = {27};
static const char *const atf1508as_s501[] = {""};
static const int atf1508as_r503[] = {27};
static const char *const atf1508as_s503[] = {""};
static const int atf1508as_r505[] = {27};
static const char *const atf1508as_s505[] = {""};
static const int atf1508as_r507[] = {27};
static const char *const atf1508as_s507[] = {""};
static const int atf1508as_r509[] = {27};
static const char *const atf1508as_s509[] = {""};
static const int atf1508as_r511[] = {27};
static const char *const atf1508as_s511[] = {""};
static const int atf1508as_r513[] = {27};
static const char *const atf1508as_s513[] = {""};
static const int atf1508as_r515[] = {27};
static const char *const atf1508as_s515[] = {""};
static const int atf1508as_r517[] = {27};
static const char *const atf1508as_s517[] = {""};
static const int atf1508as_r519[] = {27};
static const char *const atf1508as_s519[] = {""};
static const int atf1508as_r521[] = {27};
static const char *const atf1508as_s521[] = {""};
static const int atf1508as_r523[] = {27};
static const char *const atf1508as_s523[] = {""};
static const int atf1508as_r525[] = {27};
static const char *const atf1508as_s525[] = {""};
static const int atf1508as_r527[] = {27};
static const char *const atf1508as_s527[] = {""};
static const int atf1508as_r529[] = {27};
static const char *const atf1508as_s529[] = {""};
static const int atf1508as_r531[] = {27};
static const char *const atf1508as_s531[] = {""};
static const int atf1508as_r533[] = {27};
static const char *const atf1508as_s533[] = {""};
static const int atf1508as_r535[] = {27};
static const char *const atf1508as_s535[] = {""};
static const int atf1508as_r537[] = {27};
static const char *const atf1508as_s537[] = {""};
static const int atf1508as_r539[] = {27};
static const char *const atf1508as_s539[] = {""};
static const int atf1508as_r541[] = {27};
static const char *const atf1508as_s541[] = {""};
static const int atf1508as_r543[] = {27};
static const char *const atf1508as_s543[] = {""};
static const int atf1508as_r545[] = {27};
static const char *const atf1508as_s545[] = {""};
static const int atf1508as_r547[] = {27};
static const char *const atf1508as_s547[] = {""};
static const int atf1508as_r549[] = {27};
static const char *const atf1508as_s549[] = {""};
static const int atf1508as_r551[] = {27};
static const char *const atf1508as_s551[] = {""};
static const int atf1508as_r553[] = {27};
static const char *const atf1508as_s553[] = {""};
static const int atf1508as_r555[] = {27};
static const char *const atf1508as_s555[] = {""};
static const int atf1508as_r557[] = {27};
static const char *const atf1508as_s557[] = {""};
static const int atf1508as_r559[] = {27};
static const char *const atf1508as_s559[] = {""};
static const int atf1508as_r561[] = {27};
static const char *const atf1508as_s561[] = {""};
static const int atf1508as_r563[] = {27};
static const char *const atf1508as_s563[] = {""};
static const int atf1508as_r565[] = {27};
static const char *const atf1508as_s565[] = {""};
static const int atf1508as_r567[] = {27};
static const char *const atf1508as_s567[] = {""};
static const int atf1508as_r569[] = {27};
static const char *const atf1508as_s569[] = {""};
static const int atf1508as_r571[] = {27};
static const char *const atf1508as_s571[] = {""};
static const int atf1508as_r573[] = {27};
static const char *const atf1508as_s573[] = {""};
static const int atf1508as_r575[] = {27};
static const char *const atf1508as_s575[] = {""};
static const int atf1508as_r577[] = {27};
static const char *const atf1508as_s577[] = {""};
static const int atf1508as_r579[] = {27};
static const char *const atf1508as_s579[] = {""};
static const int atf1508as_r581[] = {27};
static const char *const atf1508as_s581[] = {""};
static const int atf1508as_r583[] = {27};
static const char *const atf1508as_s583[] = {""};
static const int atf1508as_r585[] = {27};
static const char *const atf1508as_s585[] = {""};
static const int atf1508as_r587[] = {27};
static const char *const atf1508as_s587[] = {""};
static const int atf1508as_r589[] = {27};
static const char *const atf1508as_s589[] = {""};
static const int atf1508as_r591[] = {27};
static const char *const atf1508as_s591[] = {""};
static const int atf1508as_r593[] = {27};
static const char *const atf1508as_s593[] = {""};
static const int atf1508as_r595[] = {27};
static const char *const atf1508as_s595[] = {""};
static const int atf1508as_r597[] = {27};
static const char *const atf1508as_s597[] = {""};
static const int atf1508as_r599[] = {27};
static const char *const atf1508as_s599[] = {""};
static const int atf1508as_r601[] = {27};
static const char *const atf1508as_s601[] = {""};
static const int atf1508as_r603[] = {27};
static const char *const atf1508as_s603[] = {""};
static const int atf1508as_r605[] = {27};
static const char *const atf1508as_s605[] = {""};
static const int atf1508as_r607[] = {27};
static const char *const atf1508as_s607[] = {""};
static const int atf1508as_r609[] = {27};
static const char *const atf1508as_s609[] = {""};
static const int atf1508as_r611[] = {27};
static const char *const atf1508as_s611[] = {""};
static const int atf1508as_r613[] = {27};
static const char *const atf1508as_s613[] = {""};
static const int atf1508as_r615[] = {27};
static const char *const atf1508as_s615[] = {""};
static const int atf1508as_r617[] = {27};
static const char *const atf1508as_s617[] = {""};
static const int atf1508as_r619[] = {27};
static const char *const atf1508as_s619[] = {""};
static const int atf1508as_r621[] = {27};
static const char *const atf1508as_s621[] = {""};
static const int atf1508as_r623[] = {27};
static const char *const atf1508as_s623[] = {""};
static const int atf1508as_r625[] = {27};
static const char *const atf1508as_s625[] = {""};
static const int atf1508as_r627[] = {27};
static const char *const atf1508as_s627[] = {""};
static const int atf1508as_r629[] = {27};
static const char *const atf1508as_s629[] = {""};
static const int atf1508as_r631[] = {27};
static const char *const atf1508as_s631[] = {""};
static const int atf1508as_r633[] = {27};
static const char *const atf1508as_s633[] = {""};
static const int atf1508as_r635[] = {27};
static const char *const atf1508as_s635[] = {""};
static const int atf1508as_r637[] = {27};
static const char *const atf1508as_s637[] = {""};
static const int atf1508as_r639[] = {27};
static const char *const atf1508as_s639[] = {""};
static const int atf1508as_r641[] = {27};
static const char *const atf1508as_s641[] = {""};
static const int atf1508as_r643[] = {27};
static const char *const atf1508as_s643[] = {""};
static const int atf1508as_r645[] = {27};
static const char *const atf1508as_s645[] = {""};
static const int atf1508as_r647[] = {27};
static const char *const atf1508as_s647[] = {""};
static const int atf1508as_r649[] = {27};
static const char *const atf1508as_s649[] = {""};
static const int atf1508as_r651[] = {27};
static const char *const atf1508as_s651[] = {""};
static const int atf1508as_r653[] = {27};
static const char *const atf1508as_s653[] = {""};
static const int atf1508as_r655[] = {27};
static const char *const atf1508as_s655[] = {""};
static const int atf1508as_r657[] = {27};
static const char *const atf1508as_s657[] = {""};
static const int atf1508as_r659[] = {27};
static const char *const atf1508as_s659[] = {""};
static const int atf1508as_r661[] = {27};
static const char *const atf1508as_s661[] = {""};
static const int atf1508as_r663[] = {27};
static const char *const atf1508as_s663[] = {""};
static const int atf1508as_r665[] = {27};
static const char *const atf1508as_s665[] = {""};
static const int atf1508as_r667[] = {27};
static const char *const atf1508as_s667[] = {""};
static const int atf1508as_r669[] = {27};
static const char *const atf1508as_s669[] = {""};
static const int atf1508as_r671[] = {27};
static const char *const atf1508as_s671[] = {""};
static const int atf1508as_r673[] = {27};
static const char *const atf1508as_s673[] = {""};
static const int atf1508as_r675[] = {27};
static const char *const atf1508as_s675[] = {""};
static const int atf1508as_r677[] = {27};
static const char *const atf1508as_s677[] = {""};
static const int atf1508as_r679[] = {27};
static const char *const atf1508as_s679[] = {""};
static const int atf1508as_r681[] = {27};
static const char *const atf1508as_s681[] = {""};
static const int atf1508as_r683[] = {27};
static const char *const atf1508as_s683[] = {""};
static const int atf1508as_r685[] = {27};
static const char *const atf1508as_s685[] = {""};
static const int atf1508as_r687[] = {27};
static const char *const atf1508as_s687[] = {""};
static const int atf1508as_r689[] = {27};
static const char *const atf1508as_s689[] = {""};
static const int atf1508as_r691[] = {27};
static const char *const atf1508as_s691[] = {""};
static const int atf1508as_r693[] = {27};
static const char *const atf1508as_s693[] = {""};
static const int atf1508as_r695[] = {27};
static const char *const atf1508as_s695[] = {""};
static const int atf1508as_r697[] = {27};
static const char *const atf1508as_s697[] = {""};
static const int atf1508as_r699[] = {27};
static const char *const atf1508as_s699[] = {""};
static const int atf1508as_r701[] = {27};
static const char *const atf1508as_s701[] = {""};
static const int atf1508as_r703[] = {27};
static const char *const atf1508as_s703[] = {""};
static const int atf1508as_r705[] = {27};
static const char *const atf1508as_s705[] = {""};
static const int atf1508as_r707[] = {27};
static const char *const atf1508as_s707[] = {""};
static const int atf1508as_r709[] = {27};
static const char *const atf1508as_s709[] = {""};
static const int atf1508as_r711[] = {27};
static const char *const atf1508as_s711[] = {""};
static const int atf1508as_r713[] = {27};
static const char *const atf1508as_s713[] = {""};
static const int atf1508as_r715[] = {27};
static const char *const atf1508as_s715[] = {""};
static const int atf1508as_r717[] = {27};
static const char *const atf1508as_s717[] = {""};
static const int atf1508as_r719[] = {27};
static const char *const atf1508as_s719[] = {""};
static const int atf1508as_r721[] = {27};
static const char *const atf1508as_s721[] = {""};
static const int atf1508as_r723[] = {27};
static const char *const atf1508as_s723[] = {""};
static const int atf1508as_r725[] = {27};
static const char *const atf1508as_s725[] = {""};
static const int atf1508as_r727[] = {27};
static const char *const atf1508as_s727[] = {""};
static const int atf1508as_r729[] = {27};
static const char *const atf1508as_s729[] = {""};
static const int atf1508as_r731[] = {27};
static const char *const atf1508as_s731[] = {""};
static const int atf1508as_r733[] = {27};
static const char *const atf1508as_s733[] = {""};
static const int atf1508as_r735[] = {27};
static const char *const atf1508as_s735[] = {""};
static const int atf1508as_r737[] = {27};
static const char *const atf1508as_s737[] = {""};
static const int atf1508as_r739[] = {27};
static const char *const atf1508as_s739[] = {""};
static const int atf1508as_r741[] = {27};
static const char *const atf1508as_s741[] = {""};
static const int atf1508as_r743[] = {27};
static const char *const atf1508as_s743[] = {""};
static const int atf1508as_r745[] = {27};
static const char *const atf1508as_s745[] = {""};
static const int atf1508as_r747[] = {27};
static const char *const atf1508as_s747[] = {""};
static const int atf1508as_r749[] = {27};
static const char *const atf1508as_s749[] = {""};
static const int atf1508as_r751[] = {27};
static const char *const atf1508as_s751[] = {""};
static const int atf1508as_r753[] = {27};
static const char *const atf1508as_s753[] = {""};
static const int atf1508as_r755[] = {27};
static const char *const atf1508as_s755[] = {""};
static const int atf1508as_r757[] = {27};
static const char *const atf1508as_s757[] = {""};
static const int atf1508as_r759[] = {27};
static const char *const atf1508as_s759[] = {""};
static const int atf1508as_r761[] = {27};
static const char *const atf1508as_s761[] = {""};
static const int atf1508as_r763[] = {27};
static const char *const atf1508as_s763[] = {""};
static const int atf1508as_r765[] = {27};
static const char *const atf1508as_s765[] = {""};
static const int atf1508as_r767[] = {27};
static const char *const atf1508as_s767[] = {""};
static const int atf1508as_r769[] = {27};
static const char *const atf1508as_s769[] = {""};
static const int atf1508as_r771[] = {27};
static const char *const atf1508as_s771[] = {""};
static const int atf1508as_r773[] = {27};
static const char *const atf1508as_s773[] = {""};
static const int atf1508as_r775[] = {27};
static const char *const atf1508as_s775[] = {""};
static const int atf1508as_r777[] = {27};
static const char *const atf1508as_s777[] = {""};
static const int atf1508as_r779[] = {27};
static const char *const atf1508as_s779[] = {""};
static const int atf1508as_r781[] = {27};
static const char *const atf1508as_s781[] = {""};
static const int atf1508as_r783[] = {27};
static const char *const atf1508as_s783[] = {""};
static const int atf1508as_r785[] = {27};
static const char *const atf1508as_s785[] = {""};
static const int atf1508as_r787[] = {27};
static const char *const atf1508as_s787[] = {""};
static const int atf1508as_r789[] = {27};
static const char *const atf1508as_s789[] = {""};
static const int atf1508as_r791[] = {27};
static const char *const atf1508as_s791[] = {""};
static const int atf1508as_r793[] = {27};
static const char *const atf1508as_s793[] = {""};
static const int atf1508as_r795[] = {27};
static const char *const atf1508as_s795[] = {""};
static const int atf1508as_r797[] = {27};
static const char *const atf1508as_s797[] = {""};
static const int atf1508as_r799[] = {27};
static const char *const atf1508as_s799[] = {""};
static const int atf1508as_r801[] = {27};
static const char *const atf1508as_s801[] = {""};
static const int atf1508as_r803[] = {27};
static const char *const atf1508as_s803[] = {""};
static const int atf1508as_r805[] = {27};
static const char *const atf1508as_s805[] = {""};
static const int atf1508as_r807[] = {27};
static const char *const atf1508as_s807[] = {""};
static const int atf1508as_r809[] = {27};
static const char *const atf1508as_s809[] = {""};
static const int atf1508as_r811[] = {27};
static const char *const atf1508as_s811[] = {""};
static const int atf1508as_r813[] = {27};
static const char *const atf1508as_s813[] = {""};
static const int atf1508as_r815[] = {27};
static const char *const atf1508as_s815[] = {""};
static const int atf1508as_r817[] = {27};
static const char *const atf1508as_s817[] = {""};
static const int atf1508as_r819[] = {27};
static const char *const atf1508as_s819[] = {""};
static const int atf1508as_r821[] = {27};
static const char *const atf1508as_s821[] = {""};
static const int atf1508as_r823[] = {27};
static const char *const atf1508as_s823[] = {""};
static const int atf1508as_r825[] = {27};
static const char *const atf1508as_s825[] = {""};
static const int atf1508as_r827[] = {27};
static const char *const atf1508as_s827[] = {""};
static const int atf1508as_r829[] = {27};
static const char *const atf1508as_s829[] = {""};
static const int atf1508as_r831[] = {27};
static const char *const atf1508as_s831[] = {""};
static const int atf1508as_r833[] = {27};
static const char *const atf1508as_s833[] = {""};
static const int atf1508as_r835[] = {27};
static const char *const atf1508as_s835[] = {""};
static const int atf1508as_r837[] = {27};
static const char *const atf1508as_s837[] = {""};
static const int atf1508as_r839[] = {27};
static const char *const atf1508as_s839[] = {""};
static const int atf1508as_r841[] = {27};
static const char *const atf1508as_s841[] = {""};
static const int atf1508as_r843[] = {27};
static const char *const atf1508as_s843[] = {""};
static const int atf1508as_r845[] = {27};
static const char *const atf1508as_s845[] = {""};
static const int atf1508as_r847[] = {27};
static const char *const atf1508as_s847[] = {""};
static const int atf1508as_r849[] = {27};
static const char *const atf1508as_s849[] = {""};
static const int atf1508as_r851[] = {27};
static const char *const atf1508as_s851[] = {""};
static const int atf1508as_r853[] = {27};
static const char *const atf1508as_s853[] = {""};
static const int atf1508as_r855[] = {27};
static const char *const atf1508as_s855[] = {""};
static const int atf1508as_r857[] = {27};
static const char *const atf1508as_s857[] = {""};
static const int atf1508as_r859[] = {27};
static const char *const atf1508as_s859[] = {""};
static const int atf1508as_r861[] = {27};
static const char *const atf1508as_s861[] = {""};
static const int atf1508as_r863[] = {27};
static const char *const atf1508as_s863[] = {""};
static const int atf1508as_r865[] = {27};
static const char *const atf1508as_s865[] = {""};
static const int atf1508as_r867[] = {27};
static const char *const atf1508as_s867[] = {""};
static const int atf1508as_r869[] = {27};
static const char *const atf1508as_s869[] = {""};
static const int atf1508as_r871[] = {27};
static const char *const atf1508as_s871[] = {""};
static const int atf1508as_r873[] = {27};
static const char *const atf1508as_s873[] = {""};
static const int atf1508as_r875[] = {27};
static const char *const atf1508as_s875[] = {""};
static const int atf1508as_r877[] = {27};
static const char *const atf1508as_s877[] = {""};
static const int atf1508as_r879[] = {27};
static const char *const atf1508as_s879[] = {""};
static const int atf1508as_r881[] = {27};
static const char *const atf1508as_s881[] = {""};
static const int atf1508as_r883[] = {27};
static const char *const atf1508as_s883[] = {""};
static const int atf1508as_r885[] = {27};
static const char *const atf1508as_s885[] = {""};
static const int atf1508as_r887[] = {27};
static const char *const atf1508as_s887[] = {""};
static const int atf1508as_r889[] = {27};
static const char *const atf1508as_s889[] = {""};
static const int atf1508as_r891[] = {27};
static const char *const atf1508as_s891[] = {""};
static const int atf1508as_r893[] = {27};
static const char *const atf1508as_s893[] = {""};
static const int atf1508as_r895[] = {27};
static const char *const atf1508as_s895[] = {""};
static const int atf1508as_r897[] = {27};
static const char *const atf1508as_s897[] = {""};
static const int atf1508as_r899[] = {27};
static const char *const atf1508as_s899[] = {""};
static const int atf1508as_r901[] = {27};
static const char *const atf1508as_s901[] = {""};
static const int atf1508as_r903[] = {27};
static const char *const atf1508as_s903[] = {""};
static const int atf1508as_r905[] = {27};
static const char *const atf1508as_s905[] = {""};
static const int atf1508as_r907[] = {27};
static const char *const atf1508as_s907[] = {""};
static const int atf1508as_r909[] = {27};
static const char *const atf1508as_s909[] = {""};
static const int atf1508as_r911[] = {27};
static const char *const atf1508as_s911[] = {""};
static const int atf1508as_r913[] = {27};
static const char *const atf1508as_s913[] = {""};
static const int atf1508as_r915[] = {27};
static const char *const atf1508as_s915[] = {""};
static const int atf1508as_r917[] = {27};
static const char *const atf1508as_s917[] = {""};
static const int atf1508as_r919[] = {27};
static const char *const atf1508as_s919[] = {""};
static const int atf1508as_r921[] = {27};
static const char *const atf1508as_s921[] = {""};
static const int atf1508as_r923[] = {27};
static const char *const atf1508as_s923[] = {""};
static const int atf1508as_r925[] = {27};
static const char *const atf1508as_s925[] = {""};
static const int atf1508as_r927[] = {27};
static const char *const atf1508as_s927[] = {""};
static const int atf1508as_r929[] = {27,27,27,27,27,27};
static const char *const atf1508as_s929[] = {"\r\n","\r\n","\r\n","\r\n","\r\n","\r\n"};
static const int atf1508as_r931[] = {32};
static const char *const atf1508as_s931[] = {""};
static const int atf1508as_r933[] = {4};
static const char *const atf1508as_s933[] = {""};
static const int atf1508as_r935[] = {16};
static const char *const atf1508as_s935[] = {""};
static const int atf1508as_r937[] = {2};
static const char *const atf1508as_s937[] = {""};
static const struct p2jseg atf1508as_segs[] = {
    {"NOTE macrocell configurations\r\n        0         0         0         0 *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61440, " ", 1, atf1508as_r1, atf1508as_s1},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61456, " ", 1, atf1508as_r3, atf1508as_s3},
    {"*  NOTE S16,S12 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61488, " ", 1, atf1508as_r5, atf1508as_s5},
    {"*  NOTE S14,S11 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61520, " ", 1, atf1508as_r7, atf1508as_s7},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61536, " ", 1, atf1508as_r9, atf1508as_s9},
    {"*  NOTE S16,S12 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61568, " ", 1, atf1508as_r11, atf1508as_s11},
    {"*  NOTE S14,S11 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61600, " ", 1, atf1508as_r13, atf1508as_s13},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61616, " ", 1, atf1508as_r15, atf1508as_s15},
    {"*  NOTE S16,S12 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61648, " ", 1, atf1508as_r17, atf1508as_s17},
    {"*  NOTE S14,S11 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61680, " ", 1, atf1508as_r19, atf1508as_s19},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61696, " ", 1, atf1508as_r21, atf1508as_s21},
    {"*  NOTE S16,S12 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61728, " ", 1, atf1508as_r23, atf1508as_s23},
    {"*  NOTE S14,S11 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61760, " ", 1, atf1508as_r25, atf1508as_s25},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61776, " ", 1, atf1508as_r27, atf1508as_s27},
    {"*  NOTE S9 ,S6  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61808, " ", 1, atf1508as_r29, atf1508as_s29},
    {"*  NOTE S13,S10 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61840, " ", 1, atf1508as_r31, atf1508as_s31},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61856, " ", 1, atf1508as_r33, atf1508as_s33},
    {"*  NOTE S9 ,S6  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61888, " ", 1, atf1508as_r35, atf1508as_s35},
    {"*  NOTE S13,S10 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61920, " ", 1, atf1508as_r37, atf1508as_s37},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61936, " ", 1, atf1508as_r39, atf1508as_s39},
    {"*  NOTE S9 ,S6  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 61968, " ", 1, atf1508as_r41, atf1508as_s41},
    {"*  NOTE S13,S10 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62000, " ", 1, atf1508as_r43, atf1508as_s43},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62016, " ", 1, atf1508as_r45, atf1508as_s45},
    {"*  NOTE S9 ,S6  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62048, " ", 1, atf1508as_r47, atf1508as_s47},
    {"*  NOTE S13,S10 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62080, " ", 1, atf1508as_r49, atf1508as_s49},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62096, " ", 1, atf1508as_r51, atf1508as_s51},
    {"*  NOTE S20,S18 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62128, " ", 1, atf1508as_r53, atf1508as_s53},
    {"*  NOTE S8 ,S21 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62160, " ", 1, atf1508as_r55, atf1508as_s55},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62176, " ", 1, atf1508as_r57, atf1508as_s57},
    {"*  NOTE S20,S18 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62208, " ", 1, atf1508as_r59, atf1508as_s59},
    {"*  NOTE S8 ,S21 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62240, " ", 1, atf1508as_r61, atf1508as_s61},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62256, " ", 1, atf1508as_r63, atf1508as_s63},
    {"*  NOTE S20,S18 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62288, " ", 1, atf1508as_r65, atf1508as_s65},
    {"*  NOTE S8 ,S21 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62320, " ", 1, atf1508as_r67, atf1508as_s67},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62336, " ", 1, atf1508as_r69, atf1508as_s69},
    {"*  NOTE S20,S18 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62368, " ", 1, atf1508as_r71, atf1508as_s71},
    {"*  NOTE S8 ,S21 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62400, " ", 1, atf1508as_r73, atf1508as_s73},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62416, " ", 1, atf1508as_r75, atf1508as_s75},
    {"*  NOTE S7 ,S19 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62448, " ", 1, atf1508as_r77, atf1508as_s77},
    {"*  NOTE S22,S5  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62480, " ", 1, atf1508as_r79, atf1508as_s79},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62496, " ", 1, atf1508as_r81, atf1508as_s81},
    {"*  NOTE S7 ,S19 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62528, " ", 1, atf1508as_r83, atf1508as_s83},
    {"*  NOTE S22,S5  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62560, " ", 1, atf1508as_r85, atf1508as_s85},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62576, " ", 1, atf1508as_r87, atf1508as_s87},
    {"*  NOTE S7 ,S19 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62608, " ", 1, atf1508as_r89, atf1508as_s89},
    {"*  NOTE S22,S5  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62640, " ", 1, atf1508as_r91, atf1508as_s91},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62656, " ", 1, atf1508as_r93, atf1508as_s93},
    {"*  NOTE S7 ,S19 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62688, " ", 1, atf1508as_r95, atf1508as_s95},
    {"*  NOTE S22,S5  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62720, " ", 1, atf1508as_r97, atf1508as_s97},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62736, " ", 1, atf1508as_r99, atf1508as_s99},
    {"*  NOTE S23,S4  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62768, " ", 1, atf1508as_r101, atf1508as_s101},
    {"*  NOTE S3 ,S15 of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62800, " ", 1, atf1508as_r103, atf1508as_s103},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62816, " ", 1, atf1508as_r105, atf1508as_s105},
    {"*  NOTE S23,S4  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62848, " ", 1, atf1508as_r107, atf1508as_s107},
    {"*  NOTE S3 ,S15 of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62880, " ", 1, atf1508as_r109, atf1508as_s109},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62896, " ", 1, atf1508as_r111, atf1508as_s111},
    {"*  NOTE S23,S4  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62928, " ", 1, atf1508as_r113, atf1508as_s113},
    {"*  NOTE S3 ,S15 of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62960, " ", 1, atf1508as_r115, atf1508as_s115},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 62976, " ", 1, atf1508as_r117, atf1508as_s117},
    {"*  NOTE S23,S4  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63008, " ", 1, atf1508as_r119, atf1508as_s119},
    {"*  NOTE S3 ,S15 of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63040, " ", 1, atf1508as_r121, atf1508as_s121},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63056, " ", 1, atf1508as_r123, atf1508as_s123},
    {"*  NOTE S0 ,S1  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63088, " ", 1, atf1508as_r125, atf1508as_s125},
    {"*  NOTE S17,S2  of block A * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63120, " ", 1, atf1508as_r127, atf1508as_s127},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63136, " ", 1, atf1508as_r129, atf1508as_s129},
    {"*  NOTE S0 ,S1  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63168, " ", 1, atf1508as_r131, atf1508as_s131},
    {"*  NOTE S17,S2  of block B * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63200, " ", 1, atf1508as_r133, atf1508as_s133},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63216, " ", 1, atf1508as_r135, atf1508as_s135},
    {"*  NOTE S0 ,S1  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63248, " ", 1, atf1508as_r137, atf1508as_s137},
    {"*  NOTE S17,S2  of block C * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63280, " ", 1, atf1508as_r139, atf1508as_s139},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63296, " ", 1, atf1508as_r141, atf1508as_s141},
    {"*  NOTE S0 ,S1  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63328, " ", 1, atf1508as_r143, atf1508as_s143},
    {"*  NOTE S17,S2  of block D * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63360, " ", 1, atf1508as_r145, atf1508as_s145},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63376, " ", 1, atf1508as_r147, atf1508as_s147},
    {"*  NOTE S16,S12 of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63408, " ", 1, atf1508as_r149, atf1508as_s149},
    {"*  NOTE S14,S11 of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63440, " ", 1, atf1508as_r151, atf1508as_s151},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63456, " ", 1, atf1508as_r153, atf1508as_s153},
    {"*  NOTE S16,S12 of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63488, " ", 1, atf1508as_r155, atf1508as_s155},
    {"*  NOTE S14,S11 of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63520, " ", 1, atf1508as_r157, atf1508as_s157},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63536, " ", 1, atf1508as_r159, atf1508as_s159},
    {"*  NOTE S16,S12 of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63568, " ", 1, atf1508as_r161, atf1508as_s161},
    {"*  NOTE S14,S11 of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63600, " ", 1, atf1508as_r163, atf1508as_s163},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63616, " ", 1, atf1508as_r165, atf1508as_s165},
    {"*  NOTE S16,S12 of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63648, " ", 1, atf1508as_r167, atf1508as_s167},
    {"*  NOTE S14,S11 of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63680, " ", 1, atf1508as_r169, atf1508as_s169},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63696, " ", 1, atf1508as_r171, atf1508as_s171},
    {"*  NOTE S9 ,S6  of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63728, " ", 1, atf1508as_r173, atf1508as_s173},
    {"*  NOTE S13,S10 of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63760, " ", 1, atf1508as_r175, atf1508as_s175},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63776, " ", 1, atf1508as_r177, atf1508as_s177},
    {"*  NOTE S9 ,S6  of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63808, " ", 1, atf1508as_r179, atf1508as_s179},
    {"*  NOTE S13,S10 of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63840, " ", 1, atf1508as_r181, atf1508as_s181},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63856, " ", 1, atf1508as_r183, atf1508as_s183},
    {"*  NOTE S9 ,S6  of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63888, " ", 1, atf1508as_r185, atf1508as_s185},
    {"*  NOTE S13,S10 of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63920, " ", 1, atf1508as_r187, atf1508as_s187},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63936, " ", 1, atf1508as_r189, atf1508as_s189},
    {"*  NOTE S9 ,S6  of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 63968, " ", 1, atf1508as_r191, atf1508as_s191},
    {"*  NOTE S13,S10 of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64000, " ", 1, atf1508as_r193, atf1508as_s193},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64016, " ", 1, atf1508as_r195, atf1508as_s195},
    {"*  NOTE S20,S18 of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64048, " ", 1, atf1508as_r197, atf1508as_s197},
    {"*  NOTE S8 ,S21 of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64080, " ", 1, atf1508as_r199, atf1508as_s199},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64096, " ", 1, atf1508as_r201, atf1508as_s201},
    {"*  NOTE S20,S18 of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64128, " ", 1, atf1508as_r203, atf1508as_s203},
    {"*  NOTE S8 ,S21 of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64160, " ", 1, atf1508as_r205, atf1508as_s205},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64176, " ", 1, atf1508as_r207, atf1508as_s207},
    {"*  NOTE S20,S18 of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64208, " ", 1, atf1508as_r209, atf1508as_s209},
    {"*  NOTE S8 ,S21 of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64240, " ", 1, atf1508as_r211, atf1508as_s211},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64256, " ", 1, atf1508as_r213, atf1508as_s213},
    {"*  NOTE S20,S18 of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64288, " ", 1, atf1508as_r215, atf1508as_s215},
    {"*  NOTE S8 ,S21 of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64320, " ", 1, atf1508as_r217, atf1508as_s217},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64336, " ", 1, atf1508as_r219, atf1508as_s219},
    {"*  NOTE S7 ,S19 of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64368, " ", 1, atf1508as_r221, atf1508as_s221},
    {"*  NOTE S22,S5  of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64400, " ", 1, atf1508as_r223, atf1508as_s223},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64416, " ", 1, atf1508as_r225, atf1508as_s225},
    {"*  NOTE S7 ,S19 of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64448, " ", 1, atf1508as_r227, atf1508as_s227},
    {"*  NOTE S22,S5  of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64480, " ", 1, atf1508as_r229, atf1508as_s229},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64496, " ", 1, atf1508as_r231, atf1508as_s231},
    {"*  NOTE S7 ,S19 of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64528, " ", 1, atf1508as_r233, atf1508as_s233},
    {"*  NOTE S22,S5  of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64560, " ", 1, atf1508as_r235, atf1508as_s235},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64576, " ", 1, atf1508as_r237, atf1508as_s237},
    {"*  NOTE S7 ,S19 of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64608, " ", 1, atf1508as_r239, atf1508as_s239},
    {"*  NOTE S22,S5  of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64640, " ", 1, atf1508as_r241, atf1508as_s241},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64656, " ", 1, atf1508as_r243, atf1508as_s243},
    {"*  NOTE S23,S4  of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64688, " ", 1, atf1508as_r245, atf1508as_s245},
    {"*  NOTE S3 ,S15 of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64720, " ", 1, atf1508as_r247, atf1508as_s247},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64736, " ", 1, atf1508as_r249, atf1508as_s249},
    {"*  NOTE S23,S4  of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64768, " ", 1, atf1508as_r251, atf1508as_s251},
    {"*  NOTE S3 ,S15 of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64800, " ", 1, atf1508as_r253, atf1508as_s253},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64816, " ", 1, atf1508as_r255, atf1508as_s255},
    {"*  NOTE S23,S4  of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64848, " ", 1, atf1508as_r257, atf1508as_s257},
    {"*  NOTE S3 ,S15 of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64880, " ", 1, atf1508as_r259, atf1508as_s259},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64896, " ", 1, atf1508as_r261, atf1508as_s261},
    {"*  NOTE S23,S4  of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64928, " ", 1, atf1508as_r263, atf1508as_s263},
    {"*  NOTE S3 ,S15 of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64960, " ", 1, atf1508as_r265, atf1508as_s265},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 64976, " ", 1, atf1508as_r267, atf1508as_s267},
    {"*  NOTE S0 ,S1  of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65008, " ", 1, atf1508as_r269, atf1508as_s269},
    {"*  NOTE S17,S2  of block H * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65040, " ", 1, atf1508as_r271, atf1508as_s271},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65056, " ", 1, atf1508as_r273, atf1508as_s273},
    {"*  NOTE S0 ,S1  of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65088, " ", 1, atf1508as_r275, atf1508as_s275},
    {"*  NOTE S17,S2  of block G * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65120, " ", 1, atf1508as_r277, atf1508as_s277},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65136, " ", 1, atf1508as_r279, atf1508as_s279},
    {"*  NOTE S0 ,S1  of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65168, " ", 1, atf1508as_r281, atf1508as_s281},
    {"*  NOTE S17,S2  of block F * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65200, " ", 1, atf1508as_r283, atf1508as_s283},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65216, " ", 1, atf1508as_r285, atf1508as_s285},
    {"*  NOTE S0 ,S1  of block E * \r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65248, " ", 1, atf1508as_r287, atf1508as_s287},
    {"*  NOTE S17,S2  of block E * \r\n\r\nNOTE   UIM for block A and H *\r\nNOTE         0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65280, " ", 1, atf1508as_r289, atf1508as_s289},
    {"*  NOTE Mux-0 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65307, " ", 1, atf1508as_r291, atf1508as_s291},
    {"*  NOTE Mux-1 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65334, " ", 1, atf1508as_r293, atf1508as_s293},
    {"*  NOTE Mux-0 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65361, " ", 1, atf1508as_r295, atf1508as_s295},
    {"*  NOTE Mux-1 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65388, " ", 1, atf1508as_r297, atf1508as_s297},
    {"*  NOTE Mux-2 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65415, " ", 1, atf1508as_r299, atf1508as_s299},
    {"*  NOTE Mux-3 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65442, " ", 1, atf1508as_r301, atf1508as_s301},
    {"*  NOTE Mux-2 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65469, " ", 1, atf1508as_r303, atf1508as_s303},
    {"*  NOTE Mux-3 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65496, " ", 1, atf1508as_r305, atf1508as_s305},
    {"*  NOTE Mux-4 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65523, " ", 1, atf1508as_r307, atf1508as_s307},
    {"*  NOTE Mux-5 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65550, " ", 1, atf1508as_r309, atf1508as_s309},
    {"*  NOTE Mux-4 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65577, " ", 1, atf1508as_r311, atf1508as_s311},
    {"*  NOTE Mux-5 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65604, " ", 1, atf1508as_r313, atf1508as_s313},
    {"*  NOTE Mux-6 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65631, " ", 1, atf1508as_r315, atf1508as_s315},
    {"*  NOTE Mux-7 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65658, " ", 1, atf1508as_r317, atf1508as_s317},
    {"*  NOTE Mux-6 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65685, " ", 1, atf1508as_r319, atf1508as_s319},
    {"*  NOTE Mux-7 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65712, " ", 1, atf1508as_r321, atf1508as_s321},
    {"*  NOTE Mux-8 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65739, " ", 1, atf1508as_r323, atf1508as_s323},
    {"*  NOTE Mux-9 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65766, " ", 1, atf1508as_r325, atf1508as_s325},
    {"*  NOTE Mux-8 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65793, " ", 1, atf1508as_r327, atf1508as_s327},
    {"*  NOTE Mux-9 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65820, " ", 1, atf1508as_r329, atf1508as_s329},
    {"*  NOTE Mux-10 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65847, " ", 1, atf1508as_r331, atf1508as_s331},
    {"*  NOTE Mux-11 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65874, " ", 1, atf1508as_r333, atf1508as_s333},
    {"*  NOTE Mux-10 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65901, " ", 1, atf1508as_r335, atf1508as_s335},
    {"*  NOTE Mux-11 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65928, " ", 1, atf1508as_r337, atf1508as_s337},
    {"*  NOTE Mux-12 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65955, " ", 1, atf1508as_r339, atf1508as_s339},
    {"*  NOTE Mux-13 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 65982, " ", 1, atf1508as_r341, atf1508as_s341},
    {"*  NOTE Mux-12 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66009, " ", 1, atf1508as_r343, atf1508as_s343},
    {"*  NOTE Mux-13 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66036, " ", 1, atf1508as_r345, atf1508as_s345},
    {"*  NOTE Mux-14 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66063, " ", 1, atf1508as_r347, atf1508as_s347},
    {"*  NOTE Mux-15 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66090, " ", 1, atf1508as_r349, atf1508as_s349},
    {"*  NOTE Mux-14 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66117, " ", 1, atf1508as_r351, atf1508as_s351},
    {"*  NOTE Mux-15 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66144, " ", 1, atf1508as_r353, atf1508as_s353},
    {"*  NOTE Mux-16 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66171, " ", 1, atf1508as_r355, atf1508as_s355},
    {"*  NOTE Mux-17 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66198, " ", 1, atf1508as_r357, atf1508as_s357},
    {"*  NOTE Mux-16 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66225, " ", 1, atf1508as_r359, atf1508as_s359},
    {"*  NOTE Mux-17 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66252, " ", 1, atf1508as_r361, atf1508as_s361},
    {"*  NOTE Mux-18 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66279, " ", 1, atf1508as_r363, atf1508as_s363},
    {"*  NOTE Mux-19 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66306, " ", 1, atf1508as_r365, atf1508as_s365},
    {"*  NOTE Mux-18 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66333, " ", 1, atf1508as_r367, atf1508as_s367},
    {"*  NOTE Mux-19 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66360, " ", 1, atf1508as_r369, atf1508as_s369},
    {"*  NOTE Mux-20 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66387, " ", 1, atf1508as_r371, atf1508as_s371},
    {"*  NOTE Mux-21 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66414, " ", 1, atf1508as_r373, atf1508as_s373},
    {"*  NOTE Mux-20 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66441, " ", 1, atf1508as_r375, atf1508as_s375},
    {"*  NOTE Mux-21 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66468, " ", 1, atf1508as_r377, atf1508as_s377},
    {"*  NOTE Mux-22 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66495, " ", 1, atf1508as_r379, atf1508as_s379},
    {"*  NOTE Mux-23 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66522, " ", 1, atf1508as_r381, atf1508as_s381},
    {"*  NOTE Mux-22 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66549, " ", 1, atf1508as_r383, atf1508as_s383},
    {"*  NOTE Mux-23 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66576, " ", 1, atf1508as_r385, atf1508as_s385},
    {"*  NOTE Mux-24 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66603, " ", 1, atf1508as_r387, atf1508as_s387},
    {"*  NOTE Mux-25 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66630, " ", 1, atf1508as_r389, atf1508as_s389},
    {"*  NOTE Mux-24 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66657, " ", 1, atf1508as_r391, atf1508as_s391},
    {"*  NOTE Mux-25 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66684, " ", 1, atf1508as_r393, atf1508as_s393},
    {"*  NOTE Mux-26 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66711, " ", 1, atf1508as_r395, atf1508as_s395},
    {"*  NOTE Mux-27 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66738, " ", 1, atf1508as_r397, atf1508as_s397},
    {"*  NOTE Mux-26 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66765, " ", 1, atf1508as_r399, atf1508as_s399},
    {"*  NOTE Mux-27 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66792, " ", 1, atf1508as_r401, atf1508as_s401},
    {"*  NOTE Mux-28 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66819, " ", 1, atf1508as_r403, atf1508as_s403},
    {"*  NOTE Mux-29 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66846, " ", 1, atf1508as_r405, atf1508as_s405},
    {"*  NOTE Mux-28 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66873, " ", 1, atf1508as_r407, atf1508as_s407},
    {"*  NOTE Mux-29 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66900, " ", 1, atf1508as_r409, atf1508as_s409},
    {"*  NOTE Mux-30 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66927, " ", 1, atf1508as_r411, atf1508as_s411},
    {"*  NOTE Mux-31 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66954, " ", 1, atf1508as_r413, atf1508as_s413},
    {"*  NOTE Mux-30 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 66981, " ", 1, atf1508as_r415, atf1508as_s415},
    {"*  NOTE Mux-31 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67008, " ", 1, atf1508as_r417, atf1508as_s417},
    {"*  NOTE Mux-32 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67035, " ", 1, atf1508as_r419, atf1508as_s419},
    {"*  NOTE Mux-33 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67062, " ", 1, atf1508as_r421, atf1508as_s421},
    {"*  NOTE Mux-32 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67089, " ", 1, atf1508as_r423, atf1508as_s423},
    {"*  NOTE Mux-33 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67116, " ", 1, atf1508as_r425, atf1508as_s425},
    {"*  NOTE Mux-34 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67143, " ", 1, atf1508as_r427, atf1508as_s427},
    {"*  NOTE Mux-35 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67170, " ", 1, atf1508as_r429, atf1508as_s429},
    {"*  NOTE Mux-34 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67197, " ", 1, atf1508as_r431, atf1508as_s431},
    {"*  NOTE Mux-35 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67224, " ", 1, atf1508as_r433, atf1508as_s433},
    {"*  NOTE Mux-36 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67251, " ", 1, atf1508as_r435, atf1508as_s435},
    {"*  NOTE Mux-37 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67278, " ", 1, atf1508as_r437, atf1508as_s437},
    {"*  NOTE Mux-36 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67305, " ", 1, atf1508as_r439, atf1508as_s439},
    {"*  NOTE Mux-37 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67332, " ", 1, atf1508as_r441, atf1508as_s441},
    {"*  NOTE Mux-38 of block A*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67359, " ", 1, atf1508as_r443, atf1508as_s443},
    {"*  NOTE Mux-39 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67386, " ", 1, atf1508as_r445, atf1508as_s445},
    {"*  NOTE Mux-38 of block H*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67413, " ", 1, atf1508as_r447, atf1508as_s447},
    {"*  NOTE Mux-39 of block A*\r\n\r\nNOTE   UIM for block B and G *\r\nNOTE         0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67440, " ", 1, atf1508as_r449, atf1508as_s449},
    {"*  NOTE Mux-0 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67467, " ", 1, atf1508as_r451, atf1508as_s451},
    {"*  NOTE Mux-1 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67494, " ", 1, atf1508as_r453, atf1508as_s453},
    {"*  NOTE Mux-0 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67521, " ", 1, atf1508as_r455, atf1508as_s455},
    {"*  NOTE Mux-1 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67548, " ", 1, atf1508as_r457, atf1508as_s457},
    {"*  NOTE Mux-2 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67575, " ", 1, atf1508as_r459, atf1508as_s459},
    {"*  NOTE Mux-3 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67602, " ", 1, atf1508as_r461, atf1508as_s461},
    {"*  NOTE Mux-2 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67629, " ", 1, atf1508as_r463, atf1508as_s463},
    {"*  NOTE Mux-3 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67656, " ", 1, atf1508as_r465, atf1508as_s465},
    {"*  NOTE Mux-4 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67683, " ", 1, atf1508as_r467, atf1508as_s467},
    {"*  NOTE Mux-5 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67710, " ", 1, atf1508as_r469, atf1508as_s469},
    {"*  NOTE Mux-4 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67737, " ", 1, atf1508as_r471, atf1508as_s471},
    {"*  NOTE Mux-5 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67764, " ", 1, atf1508as_r473, atf1508as_s473},
    {"*  NOTE Mux-6 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67791, " ", 1, atf1508as_r475, atf1508as_s475},
    {"*  NOTE Mux-7 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67818, " ", 1, atf1508as_r477, atf1508as_s477},
    {"*  NOTE Mux-6 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67845, " ", 1, atf1508as_r479, atf1508as_s479},
    {"*  NOTE Mux-7 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67872, " ", 1, atf1508as_r481, atf1508as_s481},
    {"*  NOTE Mux-8 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67899, " ", 1, atf1508as_r483, atf1508as_s483},
    {"*  NOTE Mux-9 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67926, " ", 1, atf1508as_r485, atf1508as_s485},
    {"*  NOTE Mux-8 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67953, " ", 1, atf1508as_r487, atf1508as_s487},
    {"*  NOTE Mux-9 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 67980, " ", 1, atf1508as_r489, atf1508as_s489},
    {"*  NOTE Mux-10 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68007, " ", 1, atf1508as_r491, atf1508as_s491},
    {"*  NOTE Mux-11 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68034, " ", 1, atf1508as_r493, atf1508as_s493},
    {"*  NOTE Mux-10 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68061, " ", 1, atf1508as_r495, atf1508as_s495},
    {"*  NOTE Mux-11 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68088, " ", 1, atf1508as_r497, atf1508as_s497},
    {"*  NOTE Mux-12 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68115, " ", 1, atf1508as_r499, atf1508as_s499},
    {"*  NOTE Mux-13 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68142, " ", 1, atf1508as_r501, atf1508as_s501},
    {"*  NOTE Mux-12 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68169, " ", 1, atf1508as_r503, atf1508as_s503},
    {"*  NOTE Mux-13 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68196, " ", 1, atf1508as_r505, atf1508as_s505},
    {"*  NOTE Mux-14 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68223, " ", 1, atf1508as_r507, atf1508as_s507},
    {"*  NOTE Mux-15 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68250, " ", 1, atf1508as_r509, atf1508as_s509},
    {"*  NOTE Mux-14 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68277, " ", 1, atf1508as_r511, atf1508as_s511},
    {"*  NOTE Mux-15 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68304, " ", 1, atf1508as_r513, atf1508as_s513},
    {"*  NOTE Mux-16 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68331, " ", 1, atf1508as_r515, atf1508as_s515},
    {"*  NOTE Mux-17 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68358, " ", 1, atf1508as_r517, atf1508as_s517},
    {"*  NOTE Mux-16 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68385, " ", 1, atf1508as_r519, atf1508as_s519},
    {"*  NOTE Mux-17 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68412, " ", 1, atf1508as_r521, atf1508as_s521},
    {"*  NOTE Mux-18 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68439, " ", 1, atf1508as_r523, atf1508as_s523},
    {"*  NOTE Mux-19 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68466, " ", 1, atf1508as_r525, atf1508as_s525},
    {"*  NOTE Mux-18 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68493, " ", 1, atf1508as_r527, atf1508as_s527},
    {"*  NOTE Mux-19 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68520, " ", 1, atf1508as_r529, atf1508as_s529},
    {"*  NOTE Mux-20 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68547, " ", 1, atf1508as_r531, atf1508as_s531},
    {"*  NOTE Mux-21 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68574, " ", 1, atf1508as_r533, atf1508as_s533},
    {"*  NOTE Mux-20 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68601, " ", 1, atf1508as_r535, atf1508as_s535},
    {"*  NOTE Mux-21 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68628, " ", 1, atf1508as_r537, atf1508as_s537},
    {"*  NOTE Mux-22 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68655, " ", 1, atf1508as_r539, atf1508as_s539},
    {"*  NOTE Mux-23 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68682, " ", 1, atf1508as_r541, atf1508as_s541},
    {"*  NOTE Mux-22 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68709, " ", 1, atf1508as_r543, atf1508as_s543},
    {"*  NOTE Mux-23 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68736, " ", 1, atf1508as_r545, atf1508as_s545},
    {"*  NOTE Mux-24 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68763, " ", 1, atf1508as_r547, atf1508as_s547},
    {"*  NOTE Mux-25 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68790, " ", 1, atf1508as_r549, atf1508as_s549},
    {"*  NOTE Mux-24 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68817, " ", 1, atf1508as_r551, atf1508as_s551},
    {"*  NOTE Mux-25 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68844, " ", 1, atf1508as_r553, atf1508as_s553},
    {"*  NOTE Mux-26 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68871, " ", 1, atf1508as_r555, atf1508as_s555},
    {"*  NOTE Mux-27 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68898, " ", 1, atf1508as_r557, atf1508as_s557},
    {"*  NOTE Mux-26 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68925, " ", 1, atf1508as_r559, atf1508as_s559},
    {"*  NOTE Mux-27 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68952, " ", 1, atf1508as_r561, atf1508as_s561},
    {"*  NOTE Mux-28 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 68979, " ", 1, atf1508as_r563, atf1508as_s563},
    {"*  NOTE Mux-29 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69006, " ", 1, atf1508as_r565, atf1508as_s565},
    {"*  NOTE Mux-28 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69033, " ", 1, atf1508as_r567, atf1508as_s567},
    {"*  NOTE Mux-29 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69060, " ", 1, atf1508as_r569, atf1508as_s569},
    {"*  NOTE Mux-30 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69087, " ", 1, atf1508as_r571, atf1508as_s571},
    {"*  NOTE Mux-31 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69114, " ", 1, atf1508as_r573, atf1508as_s573},
    {"*  NOTE Mux-30 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69141, " ", 1, atf1508as_r575, atf1508as_s575},
    {"*  NOTE Mux-31 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69168, " ", 1, atf1508as_r577, atf1508as_s577},
    {"*  NOTE Mux-32 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69195, " ", 1, atf1508as_r579, atf1508as_s579},
    {"*  NOTE Mux-33 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69222, " ", 1, atf1508as_r581, atf1508as_s581},
    {"*  NOTE Mux-32 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69249, " ", 1, atf1508as_r583, atf1508as_s583},
    {"*  NOTE Mux-33 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69276, " ", 1, atf1508as_r585, atf1508as_s585},
    {"*  NOTE Mux-34 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69303, " ", 1, atf1508as_r587, atf1508as_s587},
    {"*  NOTE Mux-35 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69330, " ", 1, atf1508as_r589, atf1508as_s589},
    {"*  NOTE Mux-34 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69357, " ", 1, atf1508as_r591, atf1508as_s591},
    {"*  NOTE Mux-35 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69384, " ", 1, atf1508as_r593, atf1508as_s593},
    {"*  NOTE Mux-36 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69411, " ", 1, atf1508as_r595, atf1508as_s595},
    {"*  NOTE Mux-37 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69438, " ", 1, atf1508as_r597, atf1508as_s597},
    {"*  NOTE Mux-36 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69465, " ", 1, atf1508as_r599, atf1508as_s599},
    {"*  NOTE Mux-37 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69492, " ", 1, atf1508as_r601, atf1508as_s601},
    {"*  NOTE Mux-38 of block B*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69519, " ", 1, atf1508as_r603, atf1508as_s603},
    {"*  NOTE Mux-39 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69546, " ", 1, atf1508as_r605, atf1508as_s605},
    {"*  NOTE Mux-38 of block G*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69573, " ", 1, atf1508as_r607, atf1508as_s607},
    {"*  NOTE Mux-39 of block B*\r\n\r\nNOTE   UIM for block C and F *\r\nNOTE         0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69600, " ", 1, atf1508as_r609, atf1508as_s609},
    {"*  NOTE Mux-0 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69627, " ", 1, atf1508as_r611, atf1508as_s611},
    {"*  NOTE Mux-1 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69654, " ", 1, atf1508as_r613, atf1508as_s613},
    {"*  NOTE Mux-0 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69681, " ", 1, atf1508as_r615, atf1508as_s615},
    {"*  NOTE Mux-1 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69708, " ", 1, atf1508as_r617, atf1508as_s617},
    {"*  NOTE Mux-2 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69735, " ", 1, atf1508as_r619, atf1508as_s619},
    {"*  NOTE Mux-3 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69762, " ", 1, atf1508as_r621, atf1508as_s621},
    {"*  NOTE Mux-2 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69789, " ", 1, atf1508as_r623, atf1508as_s623},
    {"*  NOTE Mux-3 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69816, " ", 1, atf1508as_r625, atf1508as_s625},
    {"*  NOTE Mux-4 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69843, " ", 1, atf1508as_r627, atf1508as_s627},
    {"*  NOTE Mux-5 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69870, " ", 1, atf1508as_r629, atf1508as_s629},
    {"*  NOTE Mux-4 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69897, " ", 1, atf1508as_r631, atf1508as_s631},
    {"*  NOTE Mux-5 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69924, " ", 1, atf1508as_r633, atf1508as_s633},
    {"*  NOTE Mux-6 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69951, " ", 1, atf1508as_r635, atf1508as_s635},
    {"*  NOTE Mux-7 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 69978, " ", 1, atf1508as_r637, atf1508as_s637},
    {"*  NOTE Mux-6 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70005, " ", 1, atf1508as_r639, atf1508as_s639},
    {"*  NOTE Mux-7 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70032, " ", 1, atf1508as_r641, atf1508as_s641},
    {"*  NOTE Mux-8 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70059, " ", 1, atf1508as_r643, atf1508as_s643},
    {"*  NOTE Mux-9 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70086, " ", 1, atf1508as_r645, atf1508as_s645},
    {"*  NOTE Mux-8 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70113, " ", 1, atf1508as_r647, atf1508as_s647},
    {"*  NOTE Mux-9 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70140, " ", 1, atf1508as_r649, atf1508as_s649},
    {"*  NOTE Mux-10 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70167, " ", 1, atf1508as_r651, atf1508as_s651},
    {"*  NOTE Mux-11 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70194, " ", 1, atf1508as_r653, atf1508as_s653},
    {"*  NOTE Mux-10 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70221, " ", 1, atf1508as_r655, atf1508as_s655},
    {"*  NOTE Mux-11 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70248, " ", 1, atf1508as_r657, atf1508as_s657},
    {"*  NOTE Mux-12 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70275, " ", 1, atf1508as_r659, atf1508as_s659},
    {"*  NOTE Mux-13 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70302, " ", 1, atf1508as_r661, atf1508as_s661},
    {"*  NOTE Mux-12 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70329, " ", 1, atf1508as_r663, atf1508as_s663},
    {"*  NOTE Mux-13 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70356, " ", 1, atf1508as_r665, atf1508as_s665},
    {"*  NOTE Mux-14 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70383, " ", 1, atf1508as_r667, atf1508as_s667},
    {"*  NOTE Mux-15 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70410, " ", 1, atf1508as_r669, atf1508as_s669},
    {"*  NOTE Mux-14 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70437, " ", 1, atf1508as_r671, atf1508as_s671},
    {"*  NOTE Mux-15 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70464, " ", 1, atf1508as_r673, atf1508as_s673},
    {"*  NOTE Mux-16 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70491, " ", 1, atf1508as_r675, atf1508as_s675},
    {"*  NOTE Mux-17 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70518, " ", 1, atf1508as_r677, atf1508as_s677},
    {"*  NOTE Mux-16 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70545, " ", 1, atf1508as_r679, atf1508as_s679},
    {"*  NOTE Mux-17 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70572, " ", 1, atf1508as_r681, atf1508as_s681},
    {"*  NOTE Mux-18 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70599, " ", 1, atf1508as_r683, atf1508as_s683},
    {"*  NOTE Mux-19 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70626, " ", 1, atf1508as_r685, atf1508as_s685},
    {"*  NOTE Mux-18 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70653, " ", 1, atf1508as_r687, atf1508as_s687},
    {"*  NOTE Mux-19 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70680, " ", 1, atf1508as_r689, atf1508as_s689},
    {"*  NOTE Mux-20 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70707, " ", 1, atf1508as_r691, atf1508as_s691},
    {"*  NOTE Mux-21 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70734, " ", 1, atf1508as_r693, atf1508as_s693},
    {"*  NOTE Mux-20 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70761, " ", 1, atf1508as_r695, atf1508as_s695},
    {"*  NOTE Mux-21 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70788, " ", 1, atf1508as_r697, atf1508as_s697},
    {"*  NOTE Mux-22 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70815, " ", 1, atf1508as_r699, atf1508as_s699},
    {"*  NOTE Mux-23 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70842, " ", 1, atf1508as_r701, atf1508as_s701},
    {"*  NOTE Mux-22 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70869, " ", 1, atf1508as_r703, atf1508as_s703},
    {"*  NOTE Mux-23 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70896, " ", 1, atf1508as_r705, atf1508as_s705},
    {"*  NOTE Mux-24 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70923, " ", 1, atf1508as_r707, atf1508as_s707},
    {"*  NOTE Mux-25 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70950, " ", 1, atf1508as_r709, atf1508as_s709},
    {"*  NOTE Mux-24 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 70977, " ", 1, atf1508as_r711, atf1508as_s711},
    {"*  NOTE Mux-25 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71004, " ", 1, atf1508as_r713, atf1508as_s713},
    {"*  NOTE Mux-26 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71031, " ", 1, atf1508as_r715, atf1508as_s715},
    {"*  NOTE Mux-27 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71058, " ", 1, atf1508as_r717, atf1508as_s717},
    {"*  NOTE Mux-26 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71085, " ", 1, atf1508as_r719, atf1508as_s719},
    {"*  NOTE Mux-27 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71112, " ", 1, atf1508as_r721, atf1508as_s721},
    {"*  NOTE Mux-28 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71139, " ", 1, atf1508as_r723, atf1508as_s723},
    {"*  NOTE Mux-29 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71166, " ", 1, atf1508as_r725, atf1508as_s725},
    {"*  NOTE Mux-28 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71193, " ", 1, atf1508as_r727, atf1508as_s727},
    {"*  NOTE Mux-29 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71220, " ", 1, atf1508as_r729, atf1508as_s729},
    {"*  NOTE Mux-30 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71247, " ", 1, atf1508as_r731, atf1508as_s731},
    {"*  NOTE Mux-31 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71274, " ", 1, atf1508as_r733, atf1508as_s733},
    {"*  NOTE Mux-30 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71301, " ", 1, atf1508as_r735, atf1508as_s735},
    {"*  NOTE Mux-31 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71328, " ", 1, atf1508as_r737, atf1508as_s737},
    {"*  NOTE Mux-32 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71355, " ", 1, atf1508as_r739, atf1508as_s739},
    {"*  NOTE Mux-33 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71382, " ", 1, atf1508as_r741, atf1508as_s741},
    {"*  NOTE Mux-32 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71409, " ", 1, atf1508as_r743, atf1508as_s743},
    {"*  NOTE Mux-33 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71436, " ", 1, atf1508as_r745, atf1508as_s745},
    {"*  NOTE Mux-34 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71463, " ", 1, atf1508as_r747, atf1508as_s747},
    {"*  NOTE Mux-35 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71490, " ", 1, atf1508as_r749, atf1508as_s749},
    {"*  NOTE Mux-34 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71517, " ", 1, atf1508as_r751, atf1508as_s751},
    {"*  NOTE Mux-35 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71544, " ", 1, atf1508as_r753, atf1508as_s753},
    {"*  NOTE Mux-36 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71571, " ", 1, atf1508as_r755, atf1508as_s755},
    {"*  NOTE Mux-37 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71598, " ", 1, atf1508as_r757, atf1508as_s757},
    {"*  NOTE Mux-36 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71625, " ", 1, atf1508as_r759, atf1508as_s759},
    {"*  NOTE Mux-37 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71652, " ", 1, atf1508as_r761, atf1508as_s761},
    {"*  NOTE Mux-38 of block C*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71679, " ", 1, atf1508as_r763, atf1508as_s763},
    {"*  NOTE Mux-39 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71706, " ", 1, atf1508as_r765, atf1508as_s765},
    {"*  NOTE Mux-38 of block F*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71733, " ", 1, atf1508as_r767, atf1508as_s767},
    {"*  NOTE Mux-39 of block C*\r\n\r\nNOTE   UIM for block D and E *\r\nNOTE         0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71760, " ", 1, atf1508as_r769, atf1508as_s769},
    {"*  NOTE Mux-0 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71787, " ", 1, atf1508as_r771, atf1508as_s771},
    {"*  NOTE Mux-1 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71814, " ", 1, atf1508as_r773, atf1508as_s773},
    {"*  NOTE Mux-0 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71841, " ", 1, atf1508as_r775, atf1508as_s775},
    {"*  NOTE Mux-1 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71868, " ", 1, atf1508as_r777, atf1508as_s777},
    {"*  NOTE Mux-2 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71895, " ", 1, atf1508as_r779, atf1508as_s779},
    {"*  NOTE Mux-3 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71922, " ", 1, atf1508as_r781, atf1508as_s781},
    {"*  NOTE Mux-2 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71949, " ", 1, atf1508as_r783, atf1508as_s783},
    {"*  NOTE Mux-3 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 71976, " ", 1, atf1508as_r785, atf1508as_s785},
    {"*  NOTE Mux-4 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72003, " ", 1, atf1508as_r787, atf1508as_s787},
    {"*  NOTE Mux-5 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72030, " ", 1, atf1508as_r789, atf1508as_s789},
    {"*  NOTE Mux-4 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72057, " ", 1, atf1508as_r791, atf1508as_s791},
    {"*  NOTE Mux-5 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72084, " ", 1, atf1508as_r793, atf1508as_s793},
    {"*  NOTE Mux-6 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72111, " ", 1, atf1508as_r795, atf1508as_s795},
    {"*  NOTE Mux-7 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72138, " ", 1, atf1508as_r797, atf1508as_s797},
    {"*  NOTE Mux-6 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72165, " ", 1, atf1508as_r799, atf1508as_s799},
    {"*  NOTE Mux-7 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72192, " ", 1, atf1508as_r801, atf1508as_s801},
    {"*  NOTE Mux-8 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72219, " ", 1, atf1508as_r803, atf1508as_s803},
    {"*  NOTE Mux-9 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72246, " ", 1, atf1508as_r805, atf1508as_s805},
    {"*  NOTE Mux-8 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72273, " ", 1, atf1508as_r807, atf1508as_s807},
    {"*  NOTE Mux-9 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72300, " ", 1, atf1508as_r809, atf1508as_s809},
    {"*  NOTE Mux-10 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72327, " ", 1, atf1508as_r811, atf1508as_s811},
    {"*  NOTE Mux-11 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72354, " ", 1, atf1508as_r813, atf1508as_s813},
    {"*  NOTE Mux-10 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72381, " ", 1, atf1508as_r815, atf1508as_s815},
    {"*  NOTE Mux-11 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72408, " ", 1, atf1508as_r817, atf1508as_s817},
    {"*  NOTE Mux-12 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72435, " ", 1, atf1508as_r819, atf1508as_s819},
    {"*  NOTE Mux-13 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72462, " ", 1, atf1508as_r821, atf1508as_s821},
    {"*  NOTE Mux-12 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72489, " ", 1, atf1508as_r823, atf1508as_s823},
    {"*  NOTE Mux-13 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72516, " ", 1, atf1508as_r825, atf1508as_s825},
    {"*  NOTE Mux-14 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72543, " ", 1, atf1508as_r827, atf1508as_s827},
    {"*  NOTE Mux-15 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72570, " ", 1, atf1508as_r829, atf1508as_s829},
    {"*  NOTE Mux-14 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72597, " ", 1, atf1508as_r831, atf1508as_s831},
    {"*  NOTE Mux-15 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72624, " ", 1, atf1508as_r833, atf1508as_s833},
    {"*  NOTE Mux-16 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72651, " ", 1, atf1508as_r835, atf1508as_s835},
    {"*  NOTE Mux-17 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72678, " ", 1, atf1508as_r837, atf1508as_s837},
    {"*  NOTE Mux-16 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72705, " ", 1, atf1508as_r839, atf1508as_s839},
    {"*  NOTE Mux-17 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72732, " ", 1, atf1508as_r841, atf1508as_s841},
    {"*  NOTE Mux-18 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72759, " ", 1, atf1508as_r843, atf1508as_s843},
    {"*  NOTE Mux-19 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72786, " ", 1, atf1508as_r845, atf1508as_s845},
    {"*  NOTE Mux-18 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72813, " ", 1, atf1508as_r847, atf1508as_s847},
    {"*  NOTE Mux-19 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72840, " ", 1, atf1508as_r849, atf1508as_s849},
    {"*  NOTE Mux-20 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72867, " ", 1, atf1508as_r851, atf1508as_s851},
    {"*  NOTE Mux-21 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72894, " ", 1, atf1508as_r853, atf1508as_s853},
    {"*  NOTE Mux-20 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72921, " ", 1, atf1508as_r855, atf1508as_s855},
    {"*  NOTE Mux-21 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72948, " ", 1, atf1508as_r857, atf1508as_s857},
    {"*  NOTE Mux-22 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 72975, " ", 1, atf1508as_r859, atf1508as_s859},
    {"*  NOTE Mux-23 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73002, " ", 1, atf1508as_r861, atf1508as_s861},
    {"*  NOTE Mux-22 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73029, " ", 1, atf1508as_r863, atf1508as_s863},
    {"*  NOTE Mux-23 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73056, " ", 1, atf1508as_r865, atf1508as_s865},
    {"*  NOTE Mux-24 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73083, " ", 1, atf1508as_r867, atf1508as_s867},
    {"*  NOTE Mux-25 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73110, " ", 1, atf1508as_r869, atf1508as_s869},
    {"*  NOTE Mux-24 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73137, " ", 1, atf1508as_r871, atf1508as_s871},
    {"*  NOTE Mux-25 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73164, " ", 1, atf1508as_r873, atf1508as_s873},
    {"*  NOTE Mux-26 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73191, " ", 1, atf1508as_r875, atf1508as_s875},
    {"*  NOTE Mux-27 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73218, " ", 1, atf1508as_r877, atf1508as_s877},
    {"*  NOTE Mux-26 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73245, " ", 1, atf1508as_r879, atf1508as_s879},
    {"*  NOTE Mux-27 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73272, " ", 1, atf1508as_r881, atf1508as_s881},
    {"*  NOTE Mux-28 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73299, " ", 1, atf1508as_r883, atf1508as_s883},
    {"*  NOTE Mux-29 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73326, " ", 1, atf1508as_r885, atf1508as_s885},
    {"*  NOTE Mux-28 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73353, " ", 1, atf1508as_r887, atf1508as_s887},
    {"*  NOTE Mux-29 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73380, " ", 1, atf1508as_r889, atf1508as_s889},
    {"*  NOTE Mux-30 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73407, " ", 1, atf1508as_r891, atf1508as_s891},
    {"*  NOTE Mux-31 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73434, " ", 1, atf1508as_r893, atf1508as_s893},
    {"*  NOTE Mux-30 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73461, " ", 1, atf1508as_r895, atf1508as_s895},
    {"*  NOTE Mux-31 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73488, " ", 1, atf1508as_r897, atf1508as_s897},
    {"*  NOTE Mux-32 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73515, " ", 1, atf1508as_r899, atf1508as_s899},
    {"*  NOTE Mux-33 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73542, " ", 1, atf1508as_r901, atf1508as_s901},
    {"*  NOTE Mux-32 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73569, " ", 1, atf1508as_r903, atf1508as_s903},
    {"*  NOTE Mux-33 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73596, " ", 1, atf1508as_r905, atf1508as_s905},
    {"*  NOTE Mux-34 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73623, " ", 1, atf1508as_r907, atf1508as_s907},
    {"*  NOTE Mux-35 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73650, " ", 1, atf1508as_r909, atf1508as_s909},
    {"*  NOTE Mux-34 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73677, " ", 1, atf1508as_r911, atf1508as_s911},
    {"*  NOTE Mux-35 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73704, " ", 1, atf1508as_r913, atf1508as_s913},
    {"*  NOTE Mux-36 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73731, " ", 1, atf1508as_r915, atf1508as_s915},
    {"*  NOTE Mux-37 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73758, " ", 1, atf1508as_r917, atf1508as_s917},
    {"*  NOTE Mux-36 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73785, " ", 1, atf1508as_r919, atf1508as_s919},
    {"*  NOTE Mux-37 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73812, " ", 1, atf1508as_r921, atf1508as_s921},
    {"*  NOTE Mux-38 of block D*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73839, " ", 1, atf1508as_r923, atf1508as_s923},
    {"*  NOTE Mux-39 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73866, " ", 1, atf1508as_r925, atf1508as_s925},
    {"*  NOTE Mux-38 of block E*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73893, " ", 1, atf1508as_r927, atf1508as_s927},
    {"*  NOTE Mux-39 of block D*\r\n\r\nNOTE  6 global OE \r\n0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 73920, "\r\n", 6, atf1508as_r929, atf1508as_s929},
    {"*\r\n\r\nNOTE  device configuration bits*\r\nNOTE   0         0         0*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 74082, " ", 1, atf1508as_r931, atf1508as_s931},
    {"*\r\n\r\nNOTE  Special Purpose Bits (JTAG) *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 74114, " ", 1, atf1508as_r933, atf1508as_s933},
    {"*\r\n\r\nNOTE  UES bits*\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 74118, " ", 1, atf1508as_r935, atf1508as_s935},
    {"*\r\n\r\nNOTE  Reserved bits *\r\n", 0, NULL, 0, NULL, NULL},
    {NULL, 74134, " ", 1, atf1508as_r937, atf1508as_s937},
    {"*\r\n", 0, NULL, 0, NULL, NULL},
};
static const int atf1508as_user[] = {
    74118,74119,74120,74121,74122,74123,74124,74125,74126,74127,74128,74129,74130,74131,74132,74133,
};
static const int atf1508as_pinfix[] = {
    74102,0,74103,0,74104,0,74106,1,74105,1,
};
const struct p2jdev p2j_devices[] = {
    {"ATF1502AS", "0         0         0         0         0         *\r\n", atf1502as_segs, 243, atf1502as_user, 16, atf1502as_pinfix, 5, "M17_PAD"},
    {"ATF1504AS", "0         0         0         0         0         *\r\n", atf1504as_segs, 475, atf1504as_user, 16, atf1504as_pinfix, 5, "M64_PAD"},
    {"ATF1508AS", "0         0         0         0         0         *\r\n", atf1508as_segs, 939, atf1508as_user, 16, atf1508as_pinfix, 5, "M128_PAD"},
};
const int n_p2j_devices = 3;
static const int finp_epm7032s[] = {
    14818,14824,14830,14836,14842,14848,14854,14860,14866,14872,14878,14884,14890,14896,14902,14908,
    14914,14920,14926,14932,14938,14944,14950,14956,14962,14968,14974,14980,14986,14992,14998,15004,
};
static const int finp_epm7064s[] = {
    30202,30208,30214,30220,30226,30232,30238,30244,30250,30256,30262,30268,30274,30280,30286,30292,
    30298,30304,30310,30316,30322,30328,30334,30340,30346,30352,30358,30364,30370,30376,30382,30388,
    30394,30400,30406,30412,30418,30424,30430,30436,30442,30448,30454,30460,30466,30472,30478,30484,
    30490,30496,30502,30508,30514,30520,30526,30532,30538,30544,30550,30556,30562,30568,30574,30580,
};
static const int finp_epm7128s[] = {
    62104,-1,62110,62116,62122,62128,-1,62134,62140,-1,62146,62152,62158,62164,-1,62170,
    62176,-1,62182,62188,62194,62200,-1,62206,62212,-1,62218,62224,62230,62236,-1,62242,
    62248,-1,62254,62260,62266,62272,-1,62278,62284,-1,62290,62296,62302,62308,-1,62314,
    62320,-1,62326,62332,62338,62344,-1,62350,62356,-1,62362,62368,62374,62380,-1,62386,
    62392,-1,62398,62404,62410,62416,-1,62422,62428,-1,62434,62440,62446,62452,-1,62458,
    62464,-1,62470,62476,62482,62488,-1,62494,62500,-1,62506,62512,62518,62524,-1,62530,
    62536,-1,62542,62548,62554,62560,-1,62566,62572,-1,62578,62584,62590,62596,-1,62602,
    62608,-1,62614,62620,62626,62632,-1,62638,62644,-1,62650,62656,62662,62668,-1,62674,
};
static const int finp_epm7032ae[] = {
    14847,14854,14861,14868,14875,14882,14889,14896,14903,14910,14917,14924,14931,14938,14945,14952,
    14959,14966,14973,14980,14987,14994,15001,15008,15015,15022,15029,15036,15043,15050,15057,15064,
};
static const int finp_epm7064ae[] = {
    30263,30270,30277,30284,30291,30298,30305,30312,30319,30326,30333,30340,30347,30354,30361,30368,
    30375,30382,30389,30396,30403,30410,30417,30424,30431,30438,30445,30452,30459,30466,30473,30480,
    30487,30494,30501,30508,30515,30522,30529,30536,30543,30550,30557,30564,30571,30578,30585,30592,
    30599,30606,30613,30620,30627,30634,30641,30648,30655,30662,30669,30676,30683,30690,30697,30704,
};
static const int finp_epm7128ae[] = {
    62229,-1,62236,62243,62250,62257,-1,62264,62271,-1,62278,62285,62292,62299,-1,62306,
    62313,-1,62320,62327,62334,62341,-1,62348,62355,-1,62362,62369,62376,62383,-1,62390,
    62397,-1,62404,62411,62418,62425,-1,62432,62439,-1,62446,62453,62460,62467,-1,62474,
    62481,-1,62488,62495,62502,62509,-1,62516,62523,-1,62530,62537,62544,62551,-1,62558,
    62565,-1,62572,62579,62586,62593,-1,62600,62607,-1,62614,62621,62628,62635,-1,62642,
    62649,-1,62656,62663,62670,62677,-1,62684,62691,-1,62698,62705,62712,62719,-1,62726,
    62733,-1,62740,62747,62754,62761,-1,62768,62775,-1,62782,62789,62796,62803,-1,62810,
    62817,-1,62824,62831,62838,62845,-1,62852,62859,-1,62866,62873,62880,62887,-1,62894,
};
const struct p2jfinp p2j_finp[] = {
    {"EPM7032S", 32, finp_epm7032s},
    {"EPM7064S", 64, finp_epm7064s},
    {"EPM7128S", 128, finp_epm7128s},
    {"EPM7032AE", 32, finp_epm7032ae},
    {"EPM7064AE", 64, finp_epm7064ae},
    {"EPM7128AE", 128, finp_epm7128ae},
};
const int n_p2j_finp = 6;
const char *const p2j_forms[][2] = {
    {"ATF1502AS", "CELL flb sum ar clk_ce ce as_oe as GND clocked"},
    {"ATF1502AS", "CELL flb sum ar clk_ce ce sum as GND clocked"},
    {"ATF1502AS", "CELL flb sum ar clk_ce clk sum as GND clocked"},
    {"ATF1502AS", "CELL flb sum ar sum ce sum as GND clocked"},
    {"ATF1502AS", "CELL flb sum ar sum clk sum as GND unclocked"},
    {"ATF1502AS", "CELL flb sum sum clk_ce ce as_oe as GCLR clocked"},
    {"ATF1502AS", "CELL flb sum sum clk_ce ce sum as GCLR clocked"},
    {"ATF1502AS", "CELL flb sum sum clk_ce ce sum as GND clocked"},
    {"ATF1502AS", "CELL flb sum sum clk_ce clk sum as GND clocked"},
    {"ATF1502AS", "CELL flb sum sum sum ce as_oe as GCLR clocked"},
    {"ATF1502AS", "CELL flb sum sum sum ce as_oe as GND clocked"},
    {"ATF1502AS", "CELL flb sum sum sum ce sum as GCLR clocked"},
    {"ATF1502AS", "CELL flb sum sum sum ce sum as GND clocked"},
    {"ATF1502AS", "CELL flb sum sum sum clk as_oe oe GND unclocked"},
    {"ATF1502AS", "CELL flb sum sum sum clk sum as GND unclocked"},
    {"ATF1502AS", "CELL flb xor ar sum ce sum as GND clocked"},
    {"ATF1502AS", "CELL flb xor ar sum clk sum as GND unclocked"},
    {"ATF1502AS", "CELL flb xor sum clk_ce ce sum as GND clocked"},
    {"ATF1502AS", "CELL flb xor sum sum clk sum as GND unclocked"},
    {"ATF1502AS", "CELL sum sum ar sum ce sum as GND clocked"},
    {"ATF1502AS", "CELL sum sum sum clk_ce clk sum as GND clocked"},
    {"ATF1502AS", "CELL sum sum sum sum ce as_oe as GND clocked"},
    {"ATF1502AS", "CELL sum sum sum sum ce sum as GND clocked"},
    {"ATF1502AS", "CELL sum sum sum sum clk as_oe oe GND unclocked"},
    {"ATF1502AS", "CELL sum sum sum sum clk sum as GND unclocked"},
    {"ATF1502AS", "CELL sum xor ar sum clk sum as GND unclocked"},
    {"ATF1502AS", "CELL sum xor sum clk_ce clk sum as GND clocked"},
    {"ATF1502AS", "CELL sum xor sum sum ce sum as GND clocked"},
    {"ATF1502AS", "CELL sum xor sum sum clk sum as GND unclocked"},
    {"ATF1502AS", "DONOR invert comb dff"},
    {"ATF1502AS", "DONOR normal comb dff"},
    {"ATF1502AS", "DONOR normal reg dff"},
    {"ATF1502AS", "DONOR vcc comb dff"},
    {"ATF1502AS", "DONOR vcc reg tff"},
    {"ATF1502AS", "GCLK 1:IB0"},
    {"ATF1502AS", "GCLK 1:IB0 2:IB3"},
    {"ATF1502AS", "GCLK 1:IB0 2:~IB0"},
    {"ATF1502AS", "GCLR"},
    {"ATF1502AS", "GOE False"},
    {"ATF1502AS", "GOE True"},
    {"ATF1502AS", "JTAG False"},
    {"ATF1502AS", "JTAG True"},
    {"ATF1502AS", "OD False"},
    {"ATF1502AS", "OD True"},
    {"ATF1502AS", "PT5OE False"},
    {"ATF1502AS", "PT5OE True"},
    {"ATF1502AS", "RELOC 1"},
    {"ATF1502AS", "XOR 0 False comb"},
    {"ATF1502AS", "XOR 0 False sync"},
    {"ATF1502AS", "XOR 0 True sync"},
    {"ATF1502AS", "XOR 1 False comb"},
    {"ATF1502AS", "XOR 1 False sync"},
    {"ATF1502AS", "XOR 1 True comb"},
    {"ATF1502AS", "XOR 1 True sync"},
    {"ATF1502AS", "XOR 2 False sync"},
    {"ATF1502AS", "XOR 3 False sync"},
    {"ATF1502ASV", "CELL flb sum ar clk_ce ce as_oe as GND clocked"},
    {"ATF1502ASV", "CELL flb sum ar clk_ce ce sum as GND clocked"},
    {"ATF1502ASV", "CELL flb sum ar clk_ce clk sum as GND clocked"},
    {"ATF1502ASV", "CELL flb sum ar sum clk sum as GND unclocked"},
    {"ATF1502ASV", "CELL flb sum sum clk_ce ce sum as GCLR clocked"},
    {"ATF1502ASV", "CELL flb sum sum sum ce sum as GCLR clocked"},
    {"ATF1502ASV", "CELL flb sum sum sum clk sum as GND unclocked"},
    {"ATF1502ASV", "CELL flb xor ar sum ce sum as GND clocked"},
    {"ATF1502ASV", "CELL flb xor sum clk_ce ce sum as GND clocked"},
    {"ATF1502ASV", "CELL flb xor sum sum clk sum as GND unclocked"},
    {"ATF1502ASV", "DONOR vcc reg tff"},
    {"ATF1502ASV", "GCLK 1:IB0"},
    {"ATF1502ASV", "GCLR"},
    {"ATF1502ASV", "GOE True"},
    {"ATF1502ASV", "GROUND_PIN"},
    {"ATF1502ASV", "JTAG False"},
    {"ATF1502ASV", "JTAG True"},
    {"ATF1502ASV", "OD False"},
    {"ATF1502ASV", "XOR 0 False comb"},
    {"ATF1502ASV", "XOR 0 False sync"},
    {"ATF1502ASV", "XOR 1 False comb"},
    {"ATF1502ASV", "XOR 1 False sync"},
    {"ATF1502ASV", "XOR 1 True comb"},
    {"ATF1502ASV", "XOR 1 True sync"},
    {"ATF1502ASV", "XOR 2 False sync"},
    {"ATF1504AS", "CELL flb sum ar clk_ce ce sum as GND clocked"},
    {"ATF1504AS", "CELL flb sum ar clk_ce clk sum as GND clocked"},
    {"ATF1504AS", "CELL flb sum ar sum ce sum as GND clocked"},
    {"ATF1504AS", "CELL flb sum ar sum clk sum as GND unclocked"},
    {"ATF1504AS", "CELL flb sum sum clk_ce ce as_oe as GCLR clocked"},
    {"ATF1504AS", "CELL flb sum sum clk_ce ce as_oe as GND clocked"},
    {"ATF1504AS", "CELL flb sum sum clk_ce ce sum as GCLR clocked"},
    {"ATF1504AS", "CELL flb sum sum clk_ce ce sum as GND clocked"},
    {"ATF1504AS", "CELL flb sum sum clk_ce clk sum as GND clocked"},
    {"ATF1504AS", "CELL flb sum sum sum ce as_oe as GCLR clocked"},
    {"ATF1504AS", "CELL flb sum sum sum ce as_oe as GND clocked"},
    {"ATF1504AS", "CELL flb sum sum sum ce sum as GCLR clocked"},
    {"ATF1504AS", "CELL flb sum sum sum ce sum as GND clocked"},
    {"ATF1504AS", "CELL flb sum sum sum clk as_oe oe GND unclocked"},
    {"ATF1504AS", "CELL flb sum sum sum clk sum as GND unclocked"},
    {"ATF1504AS", "CELL flb xor ar sum clk sum as GND unclocked"},
    {"ATF1504AS", "CELL flb xor sum clk_ce clk sum as GND clocked"},
    {"ATF1504AS", "CELL flb xor sum sum ce sum as GND clocked"},
    {"ATF1504AS", "CELL flb xor sum sum clk sum as GND unclocked"},
    {"ATF1504AS", "CELL sum sum ar sum ce sum as GND clocked"},
    {"ATF1504AS", "CELL sum sum sum clk_ce ce sum as GND clocked"},
    {"ATF1504AS", "CELL sum sum sum clk_ce clk sum as GND clocked"},
    {"ATF1504AS", "CELL sum sum sum sum ce as_oe as GCLR clocked"},
    {"ATF1504AS", "CELL sum sum sum sum ce as_oe as GND clocked"},
    {"ATF1504AS", "CELL sum sum sum sum ce sum as GND clocked"},
    {"ATF1504AS", "CELL sum sum sum sum clk as_oe oe GND unclocked"},
    {"ATF1504AS", "CELL sum sum sum sum clk sum as GND unclocked"},
    {"ATF1504AS", "CELL sum xor sum clk_ce clk sum as GND clocked"},
    {"ATF1504AS", "CELL sum xor sum sum clk sum as GND unclocked"},
    {"ATF1504AS", "DONOR vcc comb dff"},
    {"ATF1504AS", "GCLK 1:IB0"},
    {"ATF1504AS", "GCLK 1:IB0 2:IB3"},
    {"ATF1504AS", "GCLK 1:IB0 2:~IB0"},
    {"ATF1504AS", "GCLR"},
    {"ATF1504AS", "GOE False"},
    {"ATF1504AS", "GOE True"},
    {"ATF1504AS", "JTAG False"},
    {"ATF1504AS", "JTAG True"},
    {"ATF1504AS", "OD False"},
    {"ATF1504AS", "OD True"},
    {"ATF1504AS", "PT5OE False"},
    {"ATF1504AS", "PT5OE True"},
    {"ATF1504AS", "RELOC 1"},
    {"ATF1504AS", "XOR 0 False comb"},
    {"ATF1504AS", "XOR 0 False sync"},
    {"ATF1504AS", "XOR 0 True sync"},
    {"ATF1504AS", "XOR 1 False sync"},
    {"ATF1504AS", "XOR 1 True comb"},
    {"ATF1504AS", "XOR 1 True sync"},
    {"ATF1504AS", "XOR 2 False sync"},
    {"ATF1504ASV", "CELL flb sum ar clk_ce ce sum as GND clocked"},
    {"ATF1504ASV", "CELL flb sum ar sum clk sum as GND unclocked"},
    {"ATF1504ASV", "CELL flb sum sum clk_ce ce sum as GCLR clocked"},
    {"ATF1504ASV", "CELL flb sum sum sum ce sum as GCLR clocked"},
    {"ATF1504ASV", "CELL flb sum sum sum clk sum as GND unclocked"},
    {"ATF1504ASV", "GCLK 1:IB0"},
    {"ATF1504ASV", "GCLR"},
    {"ATF1504ASV", "GROUND_PIN"},
    {"ATF1504ASV", "JTAG False"},
    {"ATF1504ASV", "JTAG True"},
    {"ATF1504ASV", "OD False"},
    {"ATF1504ASV", "XOR 0 False comb"},
    {"ATF1504ASV", "XOR 0 False sync"},
    {"ATF1504ASV", "XOR 1 False sync"},
    {"ATF1508AS", "CELL flb sum ar clk_ce ce as_oe as GND clocked"},
    {"ATF1508AS", "CELL flb sum ar clk_ce ce sum as GCLR clocked"},
    {"ATF1508AS", "CELL flb sum ar clk_ce ce sum as GND clocked"},
    {"ATF1508AS", "CELL flb sum ar clk_ce clk sum as GND clocked"},
    {"ATF1508AS", "CELL flb sum ar sum ce sum as GND clocked"},
    {"ATF1508AS", "CELL flb sum ar sum clk sum as GND unclocked"},
    {"ATF1508AS", "CELL flb sum sum clk_ce ce as_oe as GCLR clocked"},
    {"ATF1508AS", "CELL flb sum sum clk_ce ce as_oe as GND clocked"},
    {"ATF1508AS", "CELL flb sum sum clk_ce ce sum as GCLR clocked"},
    {"ATF1508AS", "CELL flb sum sum clk_ce ce sum as GND clocked"},
    {"ATF1508AS", "CELL flb sum sum clk_ce clk sum as GND clocked"},
    {"ATF1508AS", "CELL flb sum sum sum ce as_oe as GCLR clocked"},
    {"ATF1508AS", "CELL flb sum sum sum ce as_oe as GND clocked"},
    {"ATF1508AS", "CELL flb sum sum sum ce sum as GCLR clocked"},
    {"ATF1508AS", "CELL flb sum sum sum ce sum as GND clocked"},
    {"ATF1508AS", "CELL flb sum sum sum clk as_oe oe GND unclocked"},
    {"ATF1508AS", "CELL flb sum sum sum clk sum as GND unclocked"},
    {"ATF1508AS", "CELL flb xor ar sum ce sum as GND clocked"},
    {"ATF1508AS", "CELL flb xor ar sum clk sum as GND unclocked"},
    {"ATF1508AS", "CELL flb xor sum clk_ce ce sum as GND clocked"},
    {"ATF1508AS", "CELL flb xor sum clk_ce clk sum as GND clocked"},
    {"ATF1508AS", "CELL flb xor sum sum ce sum as GND clocked"},
    {"ATF1508AS", "CELL flb xor sum sum clk sum as GND unclocked"},
    {"ATF1508AS", "CELL sum sum ar sum ce sum as GND clocked"},
    {"ATF1508AS", "CELL sum sum sum clk_ce ce sum as GND clocked"},
    {"ATF1508AS", "CELL sum sum sum clk_ce clk sum as GND clocked"},
    {"ATF1508AS", "CELL sum sum sum sum ce as_oe as GCLR clocked"},
    {"ATF1508AS", "CELL sum sum sum sum ce as_oe as GND clocked"},
    {"ATF1508AS", "CELL sum sum sum sum ce sum as GND clocked"},
    {"ATF1508AS", "CELL sum sum sum sum clk as_oe oe GND unclocked"},
    {"ATF1508AS", "CELL sum sum sum sum clk sum as GND unclocked"},
    {"ATF1508AS", "CELL sum xor sum clk_ce clk sum as GND clocked"},
    {"ATF1508AS", "CELL sum xor sum sum ce sum as GND clocked"},
    {"ATF1508AS", "CELL sum xor sum sum clk sum as GND unclocked"},
    {"ATF1508AS", "DONOR vcc comb dff"},
    {"ATF1508AS", "DONOR vcc reg tff"},
    {"ATF1508AS", "GCLK"},
    {"ATF1508AS", "GCLK 1:IB0"},
    {"ATF1508AS", "GCLK 1:IB0 2:IB3"},
    {"ATF1508AS", "GCLK 1:IB0 2:~IB0"},
    {"ATF1508AS", "GCLR"},
    {"ATF1508AS", "GOE False"},
    {"ATF1508AS", "GOE True"},
    {"ATF1508AS", "JTAG False"},
    {"ATF1508AS", "JTAG True"},
    {"ATF1508AS", "OD False"},
    {"ATF1508AS", "OD True"},
    {"ATF1508AS", "PT5OE False"},
    {"ATF1508AS", "PT5OE True"},
    {"ATF1508AS", "RELOC 1"},
    {"ATF1508AS", "XOR 0 False comb"},
    {"ATF1508AS", "XOR 0 False sync"},
    {"ATF1508AS", "XOR 0 True sync"},
    {"ATF1508AS", "XOR 1 False comb"},
    {"ATF1508AS", "XOR 1 False sync"},
    {"ATF1508AS", "XOR 1 True comb"},
    {"ATF1508AS", "XOR 1 True sync"},
    {"ATF1508AS", "XOR 2 False sync"},
    {"ATF1508AS", "XOR 3 False sync"},
    {"ATF1508ASV", "CELL flb sum ar clk_ce ce as_oe as GND clocked"},
    {"ATF1508ASV", "CELL flb sum ar clk_ce ce sum as GND clocked"},
    {"ATF1508ASV", "CELL flb sum ar clk_ce clk sum as GND clocked"},
    {"ATF1508ASV", "CELL flb sum ar sum clk sum as GND unclocked"},
    {"ATF1508ASV", "CELL flb sum sum sum clk sum as GND unclocked"},
    {"ATF1508ASV", "CELL flb xor ar sum ce sum as GND clocked"},
    {"ATF1508ASV", "CELL flb xor sum clk_ce ce sum as GND clocked"},
    {"ATF1508ASV", "CELL flb xor sum sum clk sum as GND unclocked"},
    {"ATF1508ASV", "CELL sum sum sum sum clk sum as GND unclocked"},
    {"ATF1508ASV", "DONOR vcc comb dff"},
    {"ATF1508ASV", "DONOR vcc reg tff"},
    {"ATF1508ASV", "GCLK 1:IB0"},
    {"ATF1508ASV", "GOE True"},
    {"ATF1508ASV", "GROUND_PIN"},
    {"ATF1508ASV", "JTAG True"},
    {"ATF1508ASV", "OD False"},
    {"ATF1508ASV", "XOR 0 False comb"},
    {"ATF1508ASV", "XOR 0 False sync"},
    {"ATF1508ASV", "XOR 1 False sync"},
    {"ATF1508ASV", "XOR 1 True comb"},
    {"ATF1508ASV", "XOR 1 True sync"},
    {"ATF1508ASV", "XOR 2 False sync"},
};
const int n_p2j_forms = 226;

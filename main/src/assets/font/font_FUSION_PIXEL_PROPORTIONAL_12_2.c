/*******************************************************************************
 * Size: 12 px
 * Bpp: 2
 * Opts: --bpp 2 --size 12 --no-compress --stride 1 --align 1 --font FUSION-PIXEL-12PX-PROPORTIONAL-ZH_HANS.OTF --range 32,37,40,41,44,45,46,47,48-57,58,65-90,97-122,176,937 --format lvgl -o font_FUSION_PIXEL_PROPORTIONAL_12_2.c
 ******************************************************************************/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#ifdef LV_LVGL_H_INCLUDE_SIMPLE
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif



#ifndef FONT_FUSION_PIXEL_PROPORTIONAL_12_2
#define FONT_FUSION_PIXEL_PROPORTIONAL_12_2 1
#endif

#if FONT_FUSION_PIXEL_PROPORTIONAL_12_2

/*-----------------
 *    BITMAPS
 *----------------*/

/*Store the image of the glyphs*/
static LV_ATTRIBUTE_LARGE_CONST const uint8_t glyph_bitmap[] = {
    /* U+0020 " " */

    /* U+0025 "%" */
    0x30, 0x33, 0x30, 0xc3, 0xc, 0x0, 0x30, 0x3,
    0x0, 0x30, 0x0, 0xc3, 0xc, 0x33, 0x30, 0x30,

    /* U+0028 "(" */
    0xc, 0xc3, 0x30, 0xc3, 0xc, 0x30, 0x30, 0xc0,
    0xc0,

    /* U+0029 ")" */
    0xc0, 0xc3, 0x3, 0xc, 0x30, 0xc3, 0x30, 0xcc,
    0x0,

    /* U+002C "," */
    0x33, 0xc0,

    /* U+002D "-" */
    0xff, 0xc0,

    /* U+002E "." */
    0xc0,

    /* U+002F "/" */
    0x0, 0xc0, 0x30, 0x30, 0xc, 0xc, 0x3, 0x0,
    0xc0, 0xc0, 0x30, 0x30, 0xc, 0x0,

    /* U+0030 "0" */
    0x3f, 0x30, 0x3c, 0xf, 0xf, 0xcc, 0xfc, 0x3c,
    0xf, 0x3, 0x3f, 0x0,

    /* U+0031 "1" */
    0xc, 0xf, 0xc, 0xc0, 0x30, 0xc, 0x3, 0x0,
    0xc0, 0x30, 0xff, 0xc0,

    /* U+0032 "2" */
    0x3f, 0x30, 0x30, 0xc, 0x3, 0x3, 0x3, 0x3,
    0x3, 0x0, 0xff, 0xc0,

    /* U+0033 "3" */
    0x3f, 0x30, 0x30, 0xc, 0x3, 0xf, 0x0, 0x30,
    0xf, 0x3, 0x3f, 0x0,

    /* U+0034 "4" */
    0xf, 0xc, 0xc3, 0x30, 0xcc, 0xc3, 0x30, 0xcf,
    0xfc, 0xc, 0x3, 0x0,

    /* U+0035 "5" */
    0xff, 0xf0, 0xc, 0x3, 0x0, 0xff, 0x0, 0x30,
    0xf, 0x3, 0x3f, 0x0,

    /* U+0036 "6" */
    0x3f, 0x30, 0x3c, 0x3, 0x0, 0xff, 0x30, 0x3c,
    0xf, 0x3, 0x3f, 0x0,

    /* U+0037 "7" */
    0xff, 0xc0, 0x30, 0xc, 0xc, 0x3, 0x3, 0x0,
    0xc0, 0x30, 0xc, 0x0,

    /* U+0038 "8" */
    0x3f, 0x30, 0x3c, 0xf, 0x3, 0x3f, 0x30, 0x3c,
    0xf, 0x3, 0x3f, 0x0,

    /* U+0039 "9" */
    0x3f, 0x30, 0x3c, 0xf, 0x3, 0x3f, 0xc0, 0x30,
    0xf, 0x3, 0x3f, 0x0,

    /* U+003A ":" */
    0xc0, 0x30,

    /* U+0041 "A" */
    0x3, 0x0, 0xc, 0x0, 0xcc, 0x3, 0x30, 0x30,
    0x30, 0xff, 0xc3, 0x3, 0x30, 0x3, 0xc0, 0xc,

    /* U+0042 "B" */
    0xff, 0xcc, 0x3, 0xc0, 0x3c, 0x3, 0xff, 0xcc,
    0x3, 0xc0, 0x3c, 0x3, 0xff, 0xc0,

    /* U+0043 "C" */
    0xf, 0xc3, 0x3, 0xc0, 0xc, 0x0, 0xc0, 0xc,
    0x0, 0xc0, 0x3, 0x3, 0xf, 0xc0,

    /* U+0044 "D" */
    0xff, 0xc, 0xc, 0xc0, 0x3c, 0x3, 0xc0, 0x3c,
    0x3, 0xc0, 0x3c, 0xc, 0xff, 0x0,

    /* U+0045 "E" */
    0xff, 0xfc, 0x0, 0xc0, 0xc, 0x0, 0xff, 0xcc,
    0x0, 0xc0, 0xc, 0x0, 0xff, 0xf0,

    /* U+0046 "F" */
    0xff, 0xfc, 0x0, 0xc0, 0xc, 0x0, 0xff, 0xcc,
    0x0, 0xc0, 0xc, 0x0, 0xc0, 0x0,

    /* U+0047 "G" */
    0xf, 0xc3, 0x3, 0xc0, 0xc, 0x0, 0xc0, 0xc,
    0xf, 0xc0, 0x33, 0x3, 0xf, 0xf0,

    /* U+0048 "H" */
    0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xff, 0xfc,
    0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x30,

    /* U+0049 "I" */
    0xfc, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0xfc,

    /* U+004A "J" */
    0x0, 0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x3c,
    0xf, 0x3, 0x3f, 0x0,

    /* U+004B "K" */
    0xc0, 0x3c, 0xc, 0xc3, 0xc, 0xc0, 0xf0, 0xc,
    0xc0, 0xc3, 0xc, 0xc, 0xc0, 0x30,

    /* U+004C "L" */
    0xc0, 0x30, 0xc, 0x3, 0x0, 0xc0, 0x30, 0xc,
    0x3, 0x0, 0xff, 0xc0,

    /* U+004D "M" */
    0xc0, 0x0, 0xfc, 0x0, 0xff, 0x0, 0x3f, 0x30,
    0x33, 0xcc, 0xc, 0xf0, 0xcc, 0x3c, 0x33, 0xf,
    0x3, 0x3, 0xc0, 0xc0, 0xc0,

    /* U+004E "N" */
    0xc0, 0x3f, 0x3, 0xcc, 0x3c, 0xc3, 0xc3, 0x3c,
    0x33, 0xc0, 0xfc, 0xf, 0xc0, 0x30,

    /* U+004F "O" */
    0xf, 0xc0, 0xc0, 0xcc, 0x0, 0xf0, 0x3, 0xc0,
    0xf, 0x0, 0x3c, 0x0, 0xcc, 0xc, 0xf, 0xc0,

    /* U+0050 "P" */
    0xff, 0xcc, 0x3, 0xc0, 0x3c, 0x3, 0xff, 0xcc,
    0x0, 0xc0, 0xc, 0x0, 0xc0, 0x0,

    /* U+0051 "Q" */
    0xf, 0xc0, 0xc0, 0xcc, 0x0, 0xf0, 0x3, 0xc0,
    0xf, 0x0, 0x3c, 0xc, 0xcc, 0xc, 0xf, 0xcc,

    /* U+0052 "R" */
    0xff, 0xcc, 0x3, 0xc0, 0x3c, 0x3, 0xff, 0xcc,
    0xc0, 0xc3, 0xc, 0xc, 0xc0, 0x30,

    /* U+0053 "S" */
    0x3f, 0xcc, 0x3, 0xc0, 0x3, 0x0, 0xf, 0x0,
    0xc, 0x0, 0x3c, 0x3, 0x3f, 0xc0,

    /* U+0054 "T" */
    0xff, 0xfc, 0xc, 0x0, 0x30, 0x0, 0xc0, 0x3,
    0x0, 0xc, 0x0, 0x30, 0x0, 0xc0, 0x3, 0x0,

    /* U+0055 "U" */
    0xc0, 0x3c, 0x3, 0xc0, 0x3c, 0x3, 0xc0, 0x3c,
    0x3, 0xc0, 0x3c, 0x3, 0x3f, 0xc0,

    /* U+0056 "V" */
    0xc0, 0xf, 0x0, 0x33, 0x3, 0xc, 0xc, 0x30,
    0x30, 0x33, 0x0, 0xcc, 0x0, 0xc0, 0x3, 0x0,

    /* U+0057 "W" */
    0xc0, 0xc0, 0xf0, 0x30, 0x3c, 0xc, 0xc, 0xcc,
    0xcc, 0x33, 0x33, 0xc, 0xcc, 0xc0, 0xc0, 0xc0,
    0x30, 0x30, 0xc, 0xc, 0x0,

    /* U+0058 "X" */
    0xc0, 0xc, 0xc0, 0xc3, 0x3, 0x3, 0x30, 0x3,
    0x0, 0x33, 0x3, 0x3, 0xc, 0xc, 0xc0, 0xc,

    /* U+0059 "Y" */
    0xc0, 0xc, 0xc0, 0xc3, 0x3, 0x3, 0x30, 0xc,
    0xc0, 0xc, 0x0, 0x30, 0x0, 0xc0, 0x3, 0x0,

    /* U+005A "Z" */
    0xff, 0xf0, 0x3, 0x0, 0xc0, 0x30, 0xc, 0x0,
    0xc0, 0x30, 0xc, 0x0, 0xff, 0xf0,

    /* U+0061 "a" */
    0x3f, 0x0, 0x33, 0xff, 0x3, 0xc0, 0xcf, 0xf0,

    /* U+0062 "b" */
    0xc0, 0x30, 0xc, 0x3, 0xfc, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0xff, 0x0,

    /* U+0063 "c" */
    0x3f, 0x30, 0x3c, 0x3, 0x0, 0xc0, 0xcf, 0xc0,

    /* U+0064 "d" */
    0x0, 0xc0, 0x30, 0xc, 0xff, 0xc0, 0xf0, 0x3c,
    0xf, 0x3, 0x3f, 0xc0,

    /* U+0065 "e" */
    0x3f, 0x30, 0x3f, 0xff, 0x0, 0xc0, 0xcf, 0xc0,

    /* U+0066 "f" */
    0xf, 0x30, 0x30, 0xff, 0x30, 0x30, 0x30, 0x30,
    0x30,

    /* U+0067 "g" */
    0x3f, 0xf0, 0x3c, 0xf, 0x3, 0xc0, 0xcf, 0xf0,
    0xc, 0xfc,

    /* U+0068 "h" */
    0xc0, 0x30, 0xc, 0x3, 0x3c, 0xf0, 0xf0, 0x3c,
    0xf, 0x3, 0xc0, 0xc0,

    /* U+0069 "i" */
    0x30, 0x0, 0x3c, 0x30, 0xc3, 0xc, 0xfc,

    /* U+006A "j" */
    0xc, 0x0, 0x3f, 0xc, 0x30, 0xc3, 0xc, 0x3f,
    0x0,

    /* U+006B "k" */
    0xc0, 0x30, 0xc, 0x3, 0x3, 0xcf, 0x3c, 0xc,
    0xc3, 0xc, 0xc0, 0xc0,

    /* U+006C "l" */
    0xf0, 0xc3, 0xc, 0x30, 0xc3, 0xc, 0x3c,

    /* U+006D "m" */
    0xfc, 0xf3, 0xc, 0x3c, 0x30, 0xf0, 0xc3, 0xc3,
    0xf, 0xc, 0x30,

    /* U+006E "n" */
    0xcf, 0x3c, 0x3c, 0xf, 0x3, 0xc0, 0xf0, 0x30,

    /* U+006F "o" */
    0x3f, 0x30, 0x3c, 0xf, 0x3, 0xc0, 0xcf, 0xc0,

    /* U+0070 "p" */
    0xff, 0x30, 0x3c, 0xf, 0x3, 0xc0, 0xff, 0xcc,
    0x3, 0x0,

    /* U+0071 "q" */
    0x3f, 0xf0, 0x3c, 0xf, 0x3, 0xc0, 0xcf, 0xf0,
    0xc, 0x3,

    /* U+0072 "r" */
    0xcf, 0xf0, 0xc0, 0xc0, 0xc0, 0xc0,

    /* U+0073 "s" */
    0x3f, 0x30, 0x33, 0xc0, 0xc, 0xc0, 0xcf, 0xc0,

    /* U+0074 "t" */
    0x30, 0x30, 0x30, 0xff, 0x30, 0x30, 0x30, 0x30,
    0xf,

    /* U+0075 "u" */
    0xc0, 0xf0, 0x3c, 0xf, 0x3, 0xc3, 0xcf, 0x30,

    /* U+0076 "v" */
    0xc0, 0xf0, 0x33, 0x30, 0xcc, 0xc, 0x3, 0x0,

    /* U+0077 "w" */
    0xc3, 0xf, 0xc, 0x3c, 0xcc, 0xf3, 0x33, 0x30,
    0x30, 0xc0, 0xc0,

    /* U+0078 "x" */
    0xc0, 0xcc, 0xc0, 0xc0, 0x30, 0x33, 0x30, 0x30,

    /* U+0079 "y" */
    0xc0, 0xf0, 0x3c, 0xc, 0xcc, 0x33, 0x3, 0x0,
    0xc3, 0xc0,

    /* U+007A "z" */
    0xff, 0xc0, 0xc0, 0xc0, 0xc0, 0xc0, 0x3f, 0xf0,

    /* U+00B0 "°" */
    0x3c, 0xc3, 0xc3, 0x3c,

    /* U+03A9 "Ω" */
    0xf, 0xc0, 0xc0, 0xcc, 0x0, 0xf0, 0x3, 0xc0,
    0xf, 0x0, 0x33, 0x3, 0x3, 0x30, 0xfc, 0xfc
};


/*---------------------
 *  GLYPH DESCRIPTION
 *--------------------*/

static const lv_font_fmt_txt_glyph_dsc_t glyph_dsc[] = {
    {.bitmap_index = 0, .adv_w = 0, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0} /* id = 0 reserved */,
    {.bitmap_index = 0, .adv_w = 96, .box_w = 0, .box_h = 0, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 0, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 16, .adv_w = 96, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 25, .adv_w = 96, .box_w = 3, .box_h = 11, .ofs_x = 1, .ofs_y = -1},
    {.bitmap_index = 34, .adv_w = 64, .box_w = 2, .box_h = 3, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 36, .adv_w = 96, .box_w = 5, .box_h = 1, .ofs_x = 0, .ofs_y = 4},
    {.bitmap_index = 38, .adv_w = 64, .box_w = 1, .box_h = 1, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 39, .adv_w = 96, .box_w = 5, .box_h = 11, .ofs_x = 0, .ofs_y = -1},
    {.bitmap_index = 53, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 65, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 77, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 89, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 101, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 113, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 125, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 137, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 149, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 161, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 173, .adv_w = 64, .box_w = 1, .box_h = 6, .ofs_x = 1, .ofs_y = 0},
    {.bitmap_index = 175, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 191, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 205, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 219, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 233, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 247, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 261, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 275, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 289, .adv_w = 64, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 296, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 308, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 322, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 334, .adv_w = 160, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 355, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 369, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 385, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 399, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 415, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 429, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 443, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 459, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 473, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 489, .adv_w = 160, .box_w = 9, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 510, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 526, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 542, .adv_w = 112, .box_w = 6, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 556, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 564, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 576, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 584, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 596, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 604, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 613, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 623, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 635, .adv_w = 64, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 642, .adv_w = 64, .box_w = 3, .box_h = 11, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 651, .adv_w = 96, .box_w = 5, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 663, .adv_w = 64, .box_w = 3, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 670, .adv_w = 128, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 681, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 689, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 697, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 707, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 717, .adv_w = 80, .box_w = 4, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 723, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 731, .adv_w = 80, .box_w = 4, .box_h = 9, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 740, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 748, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 756, .adv_w = 128, .box_w = 7, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 767, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 775, .adv_w = 96, .box_w = 5, .box_h = 8, .ofs_x = 0, .ofs_y = -2},
    {.bitmap_index = 785, .adv_w = 96, .box_w = 5, .box_h = 6, .ofs_x = 0, .ofs_y = 0},
    {.bitmap_index = 793, .adv_w = 80, .box_w = 4, .box_h = 4, .ofs_x = 0, .ofs_y = 6},
    {.bitmap_index = 797, .adv_w = 128, .box_w = 7, .box_h = 9, .ofs_x = 0, .ofs_y = 0}
};

/*---------------------
 *  CHARACTER MAPPING
 *--------------------*/

static const uint16_t unicode_list_0[] = {
    0x0, 0x5, 0x8, 0x9
};

static const uint16_t unicode_list_4[] = {
    0x0, 0x2f9
};

/*Collect the unicode lists and glyph_id offsets*/
static const lv_font_fmt_txt_cmap_t cmaps[] =
{
    {
        .range_start = 32, .range_length = 10, .glyph_id_start = 1,
        .unicode_list = unicode_list_0, .glyph_id_ofs_list = NULL, .list_length = 4, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    },
    {
        .range_start = 44, .range_length = 15, .glyph_id_start = 5,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 65, .range_length = 26, .glyph_id_start = 20,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 97, .range_length = 26, .glyph_id_start = 46,
        .unicode_list = NULL, .glyph_id_ofs_list = NULL, .list_length = 0, .type = LV_FONT_FMT_TXT_CMAP_FORMAT0_TINY
    },
    {
        .range_start = 176, .range_length = 762, .glyph_id_start = 72,
        .unicode_list = unicode_list_4, .glyph_id_ofs_list = NULL, .list_length = 2, .type = LV_FONT_FMT_TXT_CMAP_SPARSE_TINY
    }
};



/*--------------------
 *  ALL CUSTOM DATA
 *--------------------*/

#if LVGL_VERSION_MAJOR == 8
/*Store all the custom data of the font*/
static  lv_font_fmt_txt_glyph_cache_t cache;
#endif

#if LVGL_VERSION_MAJOR >= 8
static const lv_font_fmt_txt_dsc_t font_dsc = {
#else
static lv_font_fmt_txt_dsc_t font_dsc = {
#endif
    .glyph_bitmap = glyph_bitmap,
    .glyph_dsc = glyph_dsc,
    .cmaps = cmaps,
    .kern_dsc = NULL,
    .kern_scale = 0,
    .cmap_num = 5,
    .bpp = 2,
    .kern_classes = 0,
    .bitmap_format = 0,
#if LVGL_VERSION_MAJOR == 8
    .cache = &cache
#endif

};



/*-----------------
 *  PUBLIC FONT
 *----------------*/

/*Initialize a public general font descriptor*/
#if LVGL_VERSION_MAJOR >= 8
const lv_font_t font_FUSION_PIXEL_PROPORTIONAL_12_2 = {
#else
lv_font_t font_FUSION_PIXEL_PROPORTIONAL_12_2 = {
#endif
    .get_glyph_dsc = lv_font_get_glyph_dsc_fmt_txt,    /*Function pointer to get glyph's data*/
    .get_glyph_bitmap = lv_font_get_bitmap_fmt_txt,    /*Function pointer to get glyph's bitmap*/
    .line_height = 12,          /*The maximum line height required by the font*/
    .base_line = 2,             /*Baseline measured from the bottom of the line*/
#if !(LVGL_VERSION_MAJOR == 6 && LVGL_VERSION_MINOR == 0)
    .subpx = LV_FONT_SUBPX_NONE,
#endif
#if LV_VERSION_CHECK(7, 4, 0) || LVGL_VERSION_MAJOR >= 8
    .underline_position = 0,
    .underline_thickness = 0,
#endif
    // .static_bitmap = 0,
    .dsc = &font_dsc,          /*The custom font data. Will be accessed by `get_glyph_bitmap/dsc` */
#if LV_VERSION_CHECK(8, 2, 0) || LVGL_VERSION_MAJOR >= 9
    .fallback = NULL,
#endif
    .user_data = NULL,
};



#endif /*#if FONT_FUSION_PIXEL_PROPORTIONAL_12_2*/

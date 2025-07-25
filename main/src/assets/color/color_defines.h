/**
 * @file color_defines.h
 *
 */

#ifndef COLOR_DEFINES_H
#define COLOR_DEFINES_H


#ifdef __cplusplus
extern "C" {
#endif

#define COLOR_ffffff lv_color_white()
#define COLOR_000000 lv_color_black() //搭配 LV_OPA_TRANSP 为透明,否则为黑 
#define COLOR_333333 lv_color_hex(0x333333)
#define COLOR_2AFF80 lv_color_hex(0x2aff80)
#define COLOR_999999 lv_color_hex(0x999999)
#define COLOR_CCCCCC lv_color_hex(0xcccccc)
#define COLOR_00FF00 lv_color_hex(0x00ff00)

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*COLOR_DEFINES_H*/

#ifdef __has_include
    #if __has_include("lvgl.h")
        #ifndef LV_LVGL_H_INCLUDE_SIMPLE
            #define LV_LVGL_H_INCLUDE_SIMPLE
        #endif
    #endif
#endif

#if defined(LV_LVGL_H_INCLUDE_SIMPLE)
    #include "lvgl.h"
#else
    #include "lvgl/lvgl.h"
#endif


#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_IMG_IMG_TEXT_NONE
#define LV_ATTRIBUTE_IMG_IMG_TEXT_NONE
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_TEXT_NONE uint8_t img_text_none_map[] = {
  0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 
};

const lv_img_dsc_t img_text_none = {
  .header.cf = LV_COLOR_FORMAT_A4,
//   .header.always_zero = 0,
//   .header.reserved = 0,
  .header.w = 22,
  .header.h = 1,
  .data_size = 11,
  .data = img_text_none_map,
};

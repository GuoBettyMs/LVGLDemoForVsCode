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

#ifndef LV_ATTRIBUTE_IMG_IMG_PERIOD
#define LV_ATTRIBUTE_IMG_IMG_PERIOD
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_PERIOD uint8_t img_period_map[] = {
  0xff, 
  0xff, 
};

const lv_img_dsc_t img_period = {
  .header.cf = LV_COLOR_FORMAT_A4,
//   .header.always_zero = 0,
//   .header.reserved = 0,
  .header.w = 2,
  .header.h = 2,
  .data_size = 2,
  .data = img_period_map,
};

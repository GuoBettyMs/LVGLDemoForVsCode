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

#ifndef LV_ATTRIBUTE_IMG_IMG_1
#define LV_ATTRIBUTE_IMG_IMG_1
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_1 uint8_t img_1_map[] = {
  0xff, 0xf9, 
  0xff, 0xff, 
  0x27, 0xff, 
  0x06, 0xff, 
  0x06, 0xff, 
  0x06, 0xff, 
  0x06, 0xff, 
  0x06, 0xff, 
  0x06, 0xff, 
  0x06, 0xff, 
  0x06, 0xff, 
  0x02, 0x43, 
};

const lv_img_dsc_t img_1 = {
  .header.cf = LV_COLOR_FORMAT_A4,
//   .header.always_zero = 0,
//   .header.reserved = 0,
  .header.w = 4,
  .header.h = 12,
  .data_size = 24,
  .data = img_1_map,
};

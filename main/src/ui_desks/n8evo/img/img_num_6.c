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

#ifndef LV_ATTRIBUTE_IMG_IMG_NUM_6
#define LV_ATTRIBUTE_IMG_IMG_NUM_6
#endif

const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_IMG_NUM_6 uint8_t img_num_6_map[] = {
  0x00, 0xff, 0xf0, 
  0x00, 0xff, 0x00, 
  0x0f, 0xf0, 0x00, 
  0xff, 0xff, 0xf0, 
  0xff, 0x00, 0xff, 
  0xff, 0x00, 0xff, 
  0xff, 0x00, 0xff, 
  0xff, 0x00, 0xff, 
  0x0f, 0xff, 0xf0, 
};

const lv_img_dsc_t img_num_6 = {
  .header.cf = LV_COLOR_FORMAT_A4,
//   .header.always_zero = 0,
//   .header.reserved = 0,
  .header.w = 6,
  .header.h = 9,
  .data_size = 27,
  .data = img_num_6_map,
};

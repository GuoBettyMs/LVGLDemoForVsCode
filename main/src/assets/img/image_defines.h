#ifndef IMAGE_DEFINES_H
#define IMAGE_DEFINES_H

#ifdef __cplusplus
extern "C" {
#endif


/*********************
 *      INCLUDES
 *********************/



/*********************
 *      DEFINES
 *********************/

// 兼容v8的过渡宏（可选）
#if LVGL_VERSION_MAJOR < 9
    #define LV_IMAGE_DECLARE(img) LV_IMG_DECLARE(img)
#endif


// 声明字体
LV_FONT_DECLARE(font_04B_20_8);
LV_FONT_DECLARE(font_bahnschrift_18);
LV_FONT_DECLARE(font_bahnschrift_20);
LV_FONT_DECLARE(font_isdtyahei_18);
LV_FONT_DECLARE(font_isdtyahei_20);


// 声明图像描述符
LV_IMAGE_DECLARE(bind);
LV_IMAGE_DECLARE(bind_confirm);
LV_IMAGE_DECLARE(bind_reject);
LV_IMAGE_DECLARE(batt_close);
LV_IMAGE_DECLARE(turn);

LV_IMAGE_DECLARE(batt_none);
LV_IMAGE_DECLARE(batt_hightemp);
LV_IMAGE_DECLARE(batt_lowtemp);
LV_IMAGE_DECLARE(batt_bar_top);
LV_IMAGE_DECLARE(batt_bar_top_green);

LV_IMAGE_DECLARE(type_none);
LV_IMAGE_DECLARE(type_input);
LV_IMAGE_DECLARE(type_output);
LV_IMAGE_DECLARE(type_error);
LV_IMAGE_DECLARE(type_charge);
LV_IMAGE_DECLARE(type_average);
LV_IMAGE_DECLARE(type_discharge);
LV_IMAGE_DECLARE(type_gather);
LV_IMAGE_DECLARE(light);
LV_IMAGE_DECLARE(alert);

LV_IMAGE_DECLARE(num_none);
LV_IMAGE_DECLARE(num_0);
LV_IMAGE_DECLARE(num_1);
LV_IMAGE_DECLARE(num_2);
LV_IMAGE_DECLARE(num_3);
LV_IMAGE_DECLARE(num_4);
LV_IMAGE_DECLARE(num_5);
LV_IMAGE_DECLARE(num_6);
LV_IMAGE_DECLARE(num_7);
LV_IMAGE_DECLARE(num_8);
LV_IMAGE_DECLARE(num_9);

LV_IMAGE_DECLARE(sign_w);
LV_IMAGE_DECLARE(sign_per);

LV_IMAGE_DECLARE(port_batt_cv);
LV_IMAGE_DECLARE(port_batt_cap);
LV_IMAGE_DECLARE(port_batt_temp);
LV_IMAGE_DECLARE(port_batt_pwr);
LV_IMAGE_DECLARE(port_batt_cyc);
LV_IMAGE_DECLARE(port_num_1);
LV_IMAGE_DECLARE(port_num_2);
LV_IMAGE_DECLARE(port_num_3);
LV_IMAGE_DECLARE(port_num_4);
LV_IMAGE_DECLARE(port_batt_none);
LV_IMAGE_DECLARE(port_batt_0);

LV_IMAGE_DECLARE(cycle);
LV_IMAGE_DECLARE(signnum_2);





#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*IMAGE_DEFINES_H*/
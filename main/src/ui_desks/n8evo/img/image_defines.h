/**
 * @file image_defines.h
 *
 */

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



// 声明图像描述符
LV_IMAGE_DECLARE(img_type_auto);
LV_IMAGE_DECLARE(img_type_nimh);
LV_IMAGE_DECLARE(img_type_nicd);
LV_IMAGE_DECLARE(img_type_nizn);
LV_IMAGE_DECLARE(img_type_liion);
LV_IMAGE_DECLARE(img_type_life);
LV_IMAGE_DECLARE(img_type_lihv);
LV_IMAGE_DECLARE(img_type_eneloop);
LV_IMAGE_DECLARE(img_type_1_5vli);
LV_IMAGE_DECLARE(img_uint_a);
LV_IMAGE_DECLARE(img_uint_b);

LV_IMAGE_DECLARE(img_text_none);
LV_IMAGE_DECLARE(img_period);
LV_IMAGE_DECLARE(img_colon);
LV_IMAGE_DECLARE(img_slash);

LV_IMAGE_DECLARE(img_num_0);
LV_IMAGE_DECLARE(img_num_1);
LV_IMAGE_DECLARE(img_num_2);
LV_IMAGE_DECLARE(img_num_3);
LV_IMAGE_DECLARE(img_num_4);
LV_IMAGE_DECLARE(img_num_5);
LV_IMAGE_DECLARE(img_num_6);
LV_IMAGE_DECLARE(img_num_7);
LV_IMAGE_DECLARE(img_num_8);
LV_IMAGE_DECLARE(img_num_9);

LV_IMAGE_DECLARE(img_small_charge);
LV_IMAGE_DECLARE(img_small_cycle);
LV_IMAGE_DECLARE(img_small_error);
LV_IMAGE_DECLARE(img_small_none);

LV_IMAGE_DECLARE(img_bar_none);
LV_IMAGE_DECLARE(img_bar_charge);
LV_IMAGE_DECLARE(img_bar_cycle);
LV_IMAGE_DECLARE(img_bar_error);
LV_IMAGE_DECLARE(img_bar_setting);
LV_IMAGE_DECLARE(img_bar_discharge);
LV_IMAGE_DECLARE(img_bar_storage);
LV_IMAGE_DECLARE(img_bar_activate);
LV_IMAGE_DECLARE(img_bar_analyse);
LV_IMAGE_DECLARE(img_bar_destory);
LV_IMAGE_DECLARE(img_1);
LV_IMAGE_DECLARE(img_2);
LV_IMAGE_DECLARE(img_3);
LV_IMAGE_DECLARE(img_4);
LV_IMAGE_DECLARE(img_A);
LV_IMAGE_DECLARE(img_B);


LV_IMAGE_DECLARE(img_wifi);
LV_IMAGE_DECLARE(img_mobile_con);
LV_IMAGE_DECLARE(img_sound);
LV_IMAGE_DECLARE(img_home_lock);
LV_IMAGE_DECLARE(img_pause);
LV_IMAGE_DECLARE(img_home_menu);
LV_IMAGE_DECLARE(img_activate_chging);
LV_IMAGE_DECLARE(img_home_unlock);
LV_IMAGE_DECLARE(img_no_sound);
LV_IMAGE_DECLARE(img_play);





LV_IMAGE_DECLARE(img_arrow_back);
LV_IMAGE_DECLARE(img_info_setting);
LV_IMAGE_DECLARE(img_info_per);
LV_IMAGE_DECLARE(img_info_chart);
LV_IMAGE_DECLARE(img_info_mOhm);
LV_IMAGE_DECLARE(img_info_mwh);

// //实体键菜单
LV_IMAGE_DECLARE(img_menu);
LV_IMAGE_DECLARE(img_lock);
LV_IMAGE_DECLARE(img_unlock);
LV_IMAGE_DECLARE(img_start);
LV_IMAGE_DECLARE(img_stop);
LV_IMAGE_DECLARE(img_volume_off);
LV_IMAGE_DECLARE(img_volume_high);
LV_IMAGE_DECLARE(img_back);
LV_IMAGE_DECLARE(img_brightness);
LV_IMAGE_DECLARE(img_theme);
LV_IMAGE_DECLARE(img_capacitylimit);


//工作状态
LV_IMAGE_DECLARE(img_batt_charge);
LV_IMAGE_DECLARE(img_batt_analyse);
LV_IMAGE_DECLARE(img_batt_activate);
LV_IMAGE_DECLARE(img_batt_storage);
LV_IMAGE_DECLARE(img_batt_cycle);
LV_IMAGE_DECLARE(img_batt_destory);
LV_IMAGE_DECLARE(img_batt_discharge);

//信息行
LV_IMAGE_DECLARE(img_capacity);
LV_IMAGE_DECLARE(img_row2);
LV_IMAGE_DECLARE(img_row3);
LV_IMAGE_DECLARE(img_row4);

//侧边栏
LV_IMAGE_DECLARE(img_menu_1);

// //任务设置页面
LV_IMAGE_DECLARE(img_right_arrow);
LV_IMAGE_DECLARE(img_chemistry);
LV_IMAGE_DECLARE(img_task_selection);
LV_IMAGE_DECLARE(img_condition);
LV_IMAGE_DECLARE(img_current_setting);
LV_IMAGE_DECLARE(img_charge_current);
LV_IMAGE_DECLARE(img_discharge_current);
LV_IMAGE_DECLARE(img_task_cycle);

LV_IMAGE_DECLARE(img_current);
LV_IMAGE_DECLARE(img_task);

//底部
LV_IMAGE_DECLARE(img_small_left_arrow);
LV_IMAGE_DECLARE(img_small_right_arrow);



#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*IMAGE_DEFINES_H*/
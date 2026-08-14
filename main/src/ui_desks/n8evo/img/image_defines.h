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
//实体键菜单
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

//任务设置页面
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
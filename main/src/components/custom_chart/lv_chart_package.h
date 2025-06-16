#ifndef LV_CHART_PACKAGE_H
#define LV_CHART_PACKAGE_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "lvgl/lvgl.h"
#include "../../../lvgl_private.h" // 调用 lv_draw_triangle_dsc_t

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/

// 自定义颜色枚举
typedef enum{
    CHARGE_COLOR,     
    CHARGE_DONE_COLOR,   
    ANALYSE_COLOR,  
    ACTIVATE_COLOR,     
    STORAGE_COLOR,   
    CYCLE_COLOR, 
    DESTROY_COLOR,   
    DISCHARGE_ORANGE,  
} custom_chart_color_t;

// Define timer callback type
typedef void (*chart_timer_cb_t)(lv_timer_t* my_chart_timer);  

/**********************
 *  FUNCTION PROTOTYPES
 **********************/


/**
* Create a dynamic chart
* @param parent parent container object
* @param timer_cb timer callback function
* @param timer_period timer period
* @return created chart object pointer
*/
lv_obj_t* custom_line_chart_create(lv_obj_t* parent, chart_timer_cb_t timer_cb, uint32_t timer_period);

/**
 * spline Chart for static data
 */
void custom_static_chart_create(void);

/**
* ********************** draw Tools API **********************
*/

/**
* Custom drawing solid_area_chart callback function
*/
void draw_solid_area_chart_event_cb(lv_event_t* e);

/**
* Custom drawing faded_area_chart callback function
*/
void draw_faded_area_chart_event_cb(lv_event_t* e);

/**
* ********************** chart configuration Tools API **********************
*/

/**
* Get the color value of the area
* @param color color type
* @return the corresponding LVGL color value
*/
lv_color_t chart_area_color(custom_chart_color_t color);

/**
* Get the color value of the line
* @param color color type
* @return the corresponding LVGL color value
*/
lv_color_t chart_line_color(custom_chart_color_t color);

/**
* Dynamically modify chart colors
*/
void change_chart_color(lv_obj_t* chart, lv_event_cb_t draw_event_cb, custom_chart_color_t new_color);


/**
* Change the y-axis maximum value
*/
void custom_line_chart_set_y_max_value(float value);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
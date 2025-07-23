/**
 * @file custom_chart.h
 *
 */

#ifndef CUSTOM_CHART_H
#define CUSTOM_CHART_H

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
lv_obj_t* embedded_chart_create(lv_obj_t* parent, chart_timer_cb_t timer_cb, uint32_t timer_period);

lv_obj_t* simulator_chart_create(lv_obj_t* parent, chart_timer_cb_t timer_cb, uint32_t timer_period);

lv_obj_t* simulator_static_chart_create();

/**
* ********************** chart configuration Tools API **********************
*/

/**
* Get the color value of the line
* @param color color type
* @return the corresponding LVGL color value
*/
lv_color_t chart_line_color(custom_chart_color_t color);


/**
* Dynamically modify chart colors
*/

void update_chart_color(lv_obj_t* chart, custom_chart_color_t new_color);

/**
* set the y-axis maximum value
*/
void set_chart_y_max_value(int32_t value);



/**
* set the line width
*/
void set_chart_line_width(int32_t new_value);

#endif
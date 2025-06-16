#ifndef CUSTOM_CHART_H
#define CUSTOM_CHART_H

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
typedef struct {
    lv_coord_t *points;  // 动态数据数组
    uint16_t point_count;
    lv_coord_t min_range;
    lv_coord_t max_range;
} my_chart_data_t;


/**********************
 *  FUNCTION PROTOTYPES
 **********************/


/*
 * Create my chart.
 * @parent parent object of my chart. 
 * @copy  
 * @ret  my chart object
 */
lv_obj_t *my_chart_create(lv_obj_t *parent);

/*
 * Set the array of points and the number of points.
 * @chart display chart 
 * @points the pointer to an array storing data. 
 * @points_cnt the number of this array data.
 * @ret null
 */
void my_chart_set_points(lv_obj_t *chart, const lv_coord_t *points, uint16_t count);


// void my_chart_set_range(lv_obj_t *chart, lv_coord_t min, lv_coord_t max);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif
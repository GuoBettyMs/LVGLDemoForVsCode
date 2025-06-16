/**
 * @file custom_chart
 *
 */

/*********************
 *      INCLUDES *********************/
#include <stdio.h> // 添加标准IO头文件,用于调用 LV_LOG_USER、 printf
#include "custom_chart.h"
#include "lv_chart_package.h"

/*********************
 *      DEFINES
 *********************/

#define MY_DLOG		printf

#define MY_CHART_YMIN_DEF   0
#define MY_CHART_YMAX_DEF	100
#define MY_CHART_PNUM_DEF	200
#define _ARRAY_SIZE(buffer)   (sizeof(buffer)/sizeof(buffer[0]))



/**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  GLOBAL VARIABLES
 **********************/


/**********************
 *  STATIC VARIABLES
 **********************/
// 使用静态数组避免频繁分配
static lv_point_t line_points[100];

/**********************
 *  STATIC PROTOTYPES
 **********************/


/**********************
 *  STATIC  FUNCTIONS
 **********************/


/**********************
 *  GLOBAL  FUNCTIONS
 **********************/

void my_chart_set_points(lv_obj_t *chart, const lv_coord_t *points, uint16_t count) 
{
    if(!chart || !points || count == 0) return;

    my_chart_data_t *data = lv_obj_get_user_data(chart);
    if(!data) return;

    // 释放旧数据
    if(data->points) lv_free(data->points);

    // 分配新空间
    data->points = lv_malloc(sizeof(lv_coord_t) * count);
    if(!data->points) return;

    // 复制数据
    lv_memcpy(data->points, points, sizeof(lv_coord_t) * count);
    
    data->point_count = count;

    // 触发重绘
    lv_obj_invalidate(chart);
}

static void my_chart_set_range(lv_obj_t *chart, lv_coord_t min, lv_coord_t max) 
{
    if(!chart || min >= max) return;

    my_chart_data_t *data = lv_obj_get_user_data(chart);
    if(!data) return;

    data->min_range = min;
    data->max_range = max;
    lv_obj_invalidate(chart);
}


static void draw_event_cb(lv_event_t * e) {
    lv_obj_t * obj = lv_event_get_target(e);
    // lv_draw_ctx_t * draw_ctx = lv_event_get_draw_ctx(e);
    
    switch(lv_event_get_code(e)) {
    case LV_EVENT_DRAW_MAIN_BEGIN:
        // 初始化绘图资源
        break;
        
    case LV_EVENT_DRAW_MAIN: {
               // 检查是否是背景部分，或者其他您想要自定义的部分
        
            lv_draw_line_dsc_t line_dsc;
            lv_draw_line_dsc_init(&line_dsc);
            line_dsc.color = lv_palette_main(LV_PALETTE_RED);
            line_dsc.width = 2;

            lv_point_t points[2] = {{10, 20}, {100, 200}};
            // 使用 lv_obj_get_content_coords 获取对象的内容区域坐标，
            // 然后根据需要调整点的位置。
            lv_area_t coords;
            lv_obj_get_content_coords(obj, &coords);
            for(int i = 0; i < 2; i++) {
                points[i].x += coords.x1;
                points[i].y += coords.y1;
            }

            // // 绘制线条
            // lv_layer_t * layer = lv_obj_get_layer(obj); // 获取绘图层
            // lv_draw_line(layer, &line_dsc);
        
        // // 核心绘图逻辑
        // lv_draw_line_dsc_t line_dsc;
        // lv_draw_line_dsc_init(&line_dsc);
        // line_dsc.color = lv_palette_main(LV_PALETTE_RED);
        // line_dsc.width = 2;
        
        // // 示例：绘制简化线段
        // lv_point_t points[2] = {{10, 20}, {100, 200}};
        // lv_draw_line(draw_ctx, &line_dsc, points, 2);
        
        break;
    }
    
    case LV_EVENT_DRAW_MAIN_END:
        // 释放资源
        break;
    }
}

lv_obj_t *my_chart_create(lv_obj_t *parent) 
{
    lv_obj_update_layout(parent);
    int chart_max_h = 80 * lv_obj_get_height(parent) / 100;
    int chart_max_w = 88 * lv_obj_get_width(parent) / 100;

    // printf("Creating chart with size: %dx%d\n", chart_max_w, chart_max_h);

    // 创建基础对象
    lv_obj_t *chart = lv_obj_create(parent);
    lv_obj_set_size(chart, chart_max_w, chart_max_h);
    lv_obj_align(chart, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    
    // 设置必要的标志和样式
    lv_obj_clear_flag(chart, LV_OBJ_FLAG_SCROLLABLE);  // 禁用滚动
    
    // 设置背景和边框样式
    lv_obj_set_style_bg_color(chart, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(chart, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(chart, 1, 0);
    lv_obj_set_style_border_color(chart, lv_palette_main(LV_PALETTE_GREEN), 0);
    lv_obj_set_style_border_opa(chart, LV_OPA_COVER, 0);

    //设置范围
    my_chart_set_range(chart, 0, 100);

    // 添加绘制回调
    lv_obj_add_event_cb(chart, draw_event_cb, 
        LV_EVENT_DRAW_MAIN_BEGIN | 
        LV_EVENT_DRAW_MAIN | 
        LV_EVENT_DRAW_MAIN_END, 
        NULL);

    return chart;
}


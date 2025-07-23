/**
 * @file custom_chart
 *
 */

/*********************
 *      INCLUDES *********************/
#include <stdio.h> // 添加标准IO头文件,用于调用 LV_LOG_USER、 printf
#include "custom_chart.h"

/*********************
 *      DEFINES
 *********************/


/**********************
 *      TYPEDEFS
 **********************/


/**********************
 *  GLOBAL VARIABLES
 **********************/


/**********************
 *  STATIC VARIABLES
 **********************/

static int32_t chart_y_max = 100;
static int32_t line_w = 4;
static bool draw_dsc_inited = false;
static lv_draw_rect_dsc_t rect_dsc;
static lv_color_t area_color = LV_COLOR_MAKE(0x99, 0xff, 0x99);
static lv_color_t line_color = LV_COLOR_MAKE(0x00, 0xff, 0x00);

/*********************** STATIC CHART **************************************************/
// 定义静态数据
static float data[] = {30, 70, 30, 50, 20, 60, 40, 50, 30, 40, 50, 70,
 70, 30, 50, 20, 60, 40, 50, 30, 40, 50, 70, 30, 50, 20, 60, 40, 50, 30, 40, 30};
static int data_value_count = sizeof(data)/sizeof(data[0]);
static int current_data_index = 0;  // 当前数据索引
static lv_timer_t* static_chart_timer = NULL;  // 定时器指针
static lv_chart_series_t* static_chart_ser = NULL;  // 系列指针


/**********************
 *  STATIC PROTOTYPES
 **********************/

lv_color_t chart_line_color(custom_chart_color_t color);

/**********************
 *  STATIC  FUNCTIONS
 **********************/


/**
* Get the color value of the area
* @param color color type
* @return the corresponding LVGL color value
*/
static lv_color_t chart_area_color(custom_chart_color_t color) {
    switch (color) {
        case CHARGE_COLOR: return lv_color_hex(0xffc299);  
        case CHARGE_DONE_COLOR: return lv_color_hex(0x99ff99);  
        case ANALYSE_COLOR: return lv_color_hex(0xc299ff);  
        case ACTIVATE_COLOR: return lv_color_hex(0x87decc); 
        case STORAGE_COLOR: return lv_color_hex(0x9999ff);  
        case CYCLE_COLOR: return lv_color_hex(0x80e5ff);  
        case DESTROY_COLOR: return lv_color_hex(0xff9999);  
        case DISCHARGE_ORANGE: return lv_color_hex(0xff99c2); 
        default: return lv_color_black();  
    }
}

/**
* Add a solid color to the area from the curve to the horizontal axis
*/
static void add_solid_area(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = lv_draw_task_get_draw_dsc(draw_task);

    const lv_chart_series_t * ser = lv_chart_get_series_next(obj, NULL);
    if (!ser) return; // Exit if series is not valid

    /*Draw a triangle below the line witch some opacity gradient*/
    lv_draw_line_dsc_t * draw_line_dsc = lv_draw_task_get_draw_dsc(draw_task);
    lv_draw_triangle_dsc_t tri_dsc;

    lv_draw_triangle_dsc_init(&tri_dsc);
    tri_dsc.p[0].x = draw_line_dsc->p1.x;
    tri_dsc.p[0].y = draw_line_dsc->p1.y;
    tri_dsc.p[1].x = draw_line_dsc->p2.x;
    tri_dsc.p[1].y = draw_line_dsc->p2.y;
    tri_dsc.p[2].x = draw_line_dsc->p1.y < draw_line_dsc->p2.y ? draw_line_dsc->p1.x : draw_line_dsc->p2.x;
    tri_dsc.p[2].y = LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y);
    tri_dsc.bg_color = area_color;
    // tri_dsc.bg_opa = fill_opa;

    lv_draw_triangle(base_dsc->layer, &tri_dsc);

    // /*Draw rectangle below the triangle*/
    // lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_color = area_color;//draw_line_dsc->color;
    // rect_dsc.bg_opa = fill_opa;

    lv_area_t rect_area;
    rect_area.x1 = (int32_t)draw_line_dsc->p1.x;
    rect_area.x2 = (int32_t)draw_line_dsc->p2.x - 1;
    rect_area.y1 = (int32_t)LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - 1;
    rect_area.y2 = (int32_t)coords.y2;
    lv_draw_rect(base_dsc->layer, &rect_dsc, &rect_area);

    /*Recolor the chart line*/
    draw_line_dsc->color = line_color;
    draw_line_dsc->width = line_w; 

}

/**
* Add gradient color to the area from the curve to the horizontal axis
*/
static void add_faded_area(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);

    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = lv_draw_task_get_draw_dsc(draw_task);

    // Get the series associated with this draw task
    const lv_chart_series_t * ser = lv_chart_get_series_next(obj, NULL);
    if (!ser) return; // Exit if series is not valid

    /*Draw a triangle below the line witch some opacity gradient*/
    lv_draw_line_dsc_t * draw_line_dsc = lv_draw_task_get_draw_dsc(draw_task);
     // Coordinate boundary check
    if(draw_line_dsc->p1.x < obj->coords.x1 || draw_line_dsc->p2.x > obj->coords.x2) return;

    lv_draw_triangle_dsc_t tri_dsc;
    lv_draw_triangle_dsc_init(&tri_dsc);
    tri_dsc.p[0].x = draw_line_dsc->p1.x;
    tri_dsc.p[0].y = draw_line_dsc->p1.y;
    tri_dsc.p[1].x = draw_line_dsc->p2.x;
    tri_dsc.p[1].y = draw_line_dsc->p2.y;
    tri_dsc.p[2].x = draw_line_dsc->p1.y < draw_line_dsc->p2.y ? draw_line_dsc->p1.x : draw_line_dsc->p2.x;
    tri_dsc.p[2].y = LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y);
    tri_dsc.bg_grad.dir = LV_GRAD_DIR_VER;

    int32_t full_h = lv_obj_get_height(obj);
    int32_t fract_uppter = (int32_t)(LV_MIN(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - coords.y1) * 255 / full_h;
    int32_t fract_lower = (int32_t)(LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - coords.y1) * 255 / full_h;
    tri_dsc.bg_grad.stops[0].color = area_color;
    tri_dsc.bg_grad.stops[0].opa = 255 - fract_uppter;
    tri_dsc.bg_grad.stops[0].frac = 0;
    tri_dsc.bg_grad.stops[1].color = area_color;
    tri_dsc.bg_grad.stops[1].opa = 255 - fract_lower;
    tri_dsc.bg_grad.stops[1].frac = 255;
    lv_draw_triangle(base_dsc->layer, &tri_dsc);

    /*Draw rectangle below the triangle*/
    // lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_grad.dir = LV_GRAD_DIR_VER;
    rect_dsc.bg_grad.stops[0].color = area_color;
    rect_dsc.bg_grad.stops[0].frac = 0;
    rect_dsc.bg_grad.stops[0].opa = 255 - fract_lower;
    rect_dsc.bg_grad.stops[1].color = area_color;
    rect_dsc.bg_grad.stops[1].frac = 255;
    rect_dsc.bg_grad.stops[1].opa = 0;

    lv_area_t rect_area;
    rect_area.x1 = (int32_t)draw_line_dsc->p1.x;
    rect_area.x2 = (int32_t)draw_line_dsc->p2.x - 1;
    rect_area.y1 = (int32_t)LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - 1;
    rect_area.y2 = (int32_t)coords.y2;
    lv_draw_rect(base_dsc->layer, &rect_dsc, &rect_area);

    //Modify line color and width
    draw_line_dsc->color = line_color;
    draw_line_dsc->width = line_w; 

}

/**
* Custom drawing solid_area_chart callback function
* In embedded devices(64Kb), the chart can display  600+ points
*/
static void embedded_solid_area_chart_event_cb(lv_event_t* e) {

    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = draw_task->draw_dsc;

	    // This callback is triggered for each line segment drawn as part of LV_PART_ITEMS
    if(base_dsc->part == LV_PART_ITEMS && draw_task->type == LV_DRAW_TASK_TYPE_LINE) {
        lv_obj_t * obj = lv_event_get_target(e);	

        // Ensure we are working with the correct draw descriptor type for lines
        lv_draw_line_dsc_t * draw_line_dsc = (lv_draw_line_dsc_t *)draw_task->draw_dsc;

		// Coordinate boundary check
        if(draw_line_dsc->p1.x < obj->coords.x1 || draw_line_dsc->p2.x > obj->coords.x2) return;

			
        // Get the series associated with this draw task
        const lv_chart_series_t * ser = lv_chart_get_series_next(obj, NULL);
        if (!ser) return; // Exit if series is not valid

		//Rectangle filling drawing
        lv_draw_rect_dsc_init(&rect_dsc);
        rect_dsc.bg_color = area_color;//ser->color;
        rect_dsc.bg_opa = LV_OPA_COVER;
		rect_dsc.border_width = 0;

        lv_area_t area = {
            .x1 = (int32_t)draw_line_dsc->p1.x,
            .x2 = (int32_t)draw_line_dsc->p2.x,
            .y1 = (int32_t)LV_MIN(draw_line_dsc->p1.y, draw_line_dsc->p2.y),
            .y2 = (int32_t)obj->coords.y2
        };
        lv_draw_rect(base_dsc->layer, &rect_dsc, &area);

			/*Recolor the chart line*/
        draw_line_dsc->color = line_color;
        draw_line_dsc->width = line_w; 
	}
		
}

static void simulator_solid_area_chart_event_cb(lv_event_t* e) {

    /*Add the solid area before the lines are drawn*/
    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = lv_draw_task_get_draw_dsc(draw_task);

    if(base_dsc->part == LV_PART_ITEMS && lv_draw_task_get_type(draw_task) == LV_DRAW_TASK_TYPE_LINE) {
        add_solid_area(e); 
    }
}

static void simulator_faded_area_chart_event_cb(lv_event_t* e) {
    /*Add the faded area before the lines are drawn*/
    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = lv_draw_task_get_draw_dsc(draw_task);

    if(base_dsc->part == LV_PART_ITEMS && lv_draw_task_get_type(draw_task) == LV_DRAW_TASK_TYPE_LINE) {
        add_faded_area(e);
    }
}

static void simulator_static_chart_timer_cb(lv_timer_t* timer) {
    lv_obj_t* chart = (lv_obj_t*)timer->user_data;
    
    if(current_data_index >= data_value_count) {
        lv_timer_del(static_chart_timer);  // 数据全部添加完毕，删除定时器
        static_chart_timer = NULL;
        return;
    }

    // 添加下一个数据点
    lv_chart_set_next_value(chart, static_chart_ser, data[current_data_index]);
    current_data_index++;

    // 计算需要的缩放比例
    lv_coord_t chart_width = lv_obj_get_width(chart);
    lv_coord_t point_width = 8; // 每个点期望的像素宽度
    lv_coord_t total_width_needed = current_data_index * point_width;
    
    if(total_width_needed > chart_width) {
                // 使用变换缩放X轴
        lv_coord_t zoom_x = (chart_width * 256) / total_width_needed;
        zoom_x = LV_MAX(zoom_x, 64);  // 最小25%缩放
        
        // 仅缩放X轴
        lv_obj_set_style_transform_width(chart, zoom_x, 0);
        lv_obj_set_style_transform_height(chart, 256, 0);  // Y轴不缩放
        lv_obj_refresh_ext_draw_size(chart);

    }
    // 滚动到最右侧
    lv_obj_scroll_to_x(chart, LV_COORD_MAX, LV_ANIM_OFF);

}

/**********************
 *   GLOBAL FUNCTIONS
 **********************/

lv_color_t chart_line_color(custom_chart_color_t color) {
    switch (color) {
		case CHARGE_COLOR: return lv_color_hex(0xff6600);  
        case CHARGE_DONE_COLOR: return lv_color_hex(0x00ff00);  
        case ANALYSE_COLOR: return lv_color_hex(0x9955ff);  
        case ACTIVATE_COLOR: return lv_color_hex(0x37c8ab); 
        case STORAGE_COLOR: return lv_color_hex(0x5555ff);  
        case CYCLE_COLOR: return lv_color_hex(0x00b7e5);  
        case DESTROY_COLOR: return lv_color_hex(0xff5555);  
        case DISCHARGE_ORANGE: return lv_color_hex(0xff5599); 
        default: return lv_color_black();  // Default returns black
    }
}

void update_chart_color(lv_obj_t* chart, custom_chart_color_t new_color){

    area_color = chart_area_color(new_color);
    line_color = chart_line_color(new_color);

    // 局部刷新（仅更新图表区域）
    lv_area_t dirty_area = {
        .x1 = chart->coords.x1,
        .x2 = chart->coords.x2,
        .y1 = chart->coords.y1,
        .y2 = chart->coords.y2
    };
    lv_obj_invalidate_area(chart, &dirty_area);
}  

void set_chart_line_width(int32_t new_value){
		line_w = new_value;
}

void set_chart_y_max_value(int32_t value) {
    chart_y_max = value;
}

lv_obj_t* embedded_chart_create(lv_obj_t* parent, chart_timer_cb_t timer_cb, uint32_t timer_period) {

		// Force update of layout so that the parent object size can be obtained
    lv_obj_update_layout(parent); 
    int chart_max_h = 80 * lv_obj_get_height(parent) / 100;
    int chart_max_w = 88 * lv_obj_get_width(parent) / 100;

    // Create a chart
    lv_obj_t * line_chart = lv_chart_create(parent);
    if(!line_chart) {
       return NULL;
    }

		// Set range for Y axis
    lv_chart_set_range(line_chart, LV_CHART_AXIS_PRIMARY_Y, 0, chart_y_max); 

		// Hardware acceleration configuration
    lv_obj_add_flag(line_chart, LV_OBJ_FLAG_ADV_HITTEST);
		
	    // Style configuration
    lv_chart_set_type(line_chart, LV_CHART_TYPE_LINE);	
    lv_obj_set_size(line_chart, chart_max_w, chart_max_h);
    lv_obj_align(line_chart, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    lv_obj_set_style_bg_opa(line_chart, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(line_chart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_pad_all(line_chart, 0, 0);
    lv_obj_set_style_size(line_chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_rounded(line_chart, true, LV_PART_ITEMS);

    //Add event callback and Passing color parameters
	lv_obj_add_event_cb(line_chart, embedded_solid_area_chart_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    lv_obj_add_flag(line_chart, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

    return line_chart;
}

lv_obj_t* simulator_chart_create(lv_obj_t* parent, chart_timer_cb_t timer_cb, uint32_t timer_period) {

	// Force update of layout so that the parent object size can be obtained
    lv_obj_update_layout(parent); 
    int chart_max_h = 80 * lv_obj_get_height(parent) / 100;
    int chart_max_w = 88 * lv_obj_get_width(parent) / 100;

    printf("Creating chart with size: %dx%d\n", chart_max_w, chart_max_h);
    
    // Create a chart
    lv_obj_t * line_chart = lv_chart_create(parent);
    if(!line_chart) {
        return NULL;
    }
    lv_obj_set_size(line_chart, chart_max_w, chart_max_h);
    lv_obj_align(line_chart, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    lv_obj_set_style_bg_opa(line_chart, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(line_chart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_pad_all(line_chart, 0, 0);
    lv_obj_set_style_size(line_chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_rounded(line_chart, true, LV_PART_ITEMS);
    
    // Style configuration
    lv_chart_set_type(line_chart, LV_CHART_TYPE_LINE);
    lv_chart_set_range(line_chart, LV_CHART_AXIS_PRIMARY_Y, 0, chart_y_max); 
    lv_chart_set_point_count(line_chart, 0); 

    /* Add event callback and Passing color parameters 
        LV_EVENT_DRAW_TASK_ADDED: Triggered when LVGL generates a drawing task
        LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS：Allows an object to send drawing events
    */
    static custom_chart_color_t chart_color = CHARGE_DONE_COLOR;  
    lv_obj_add_event_cb(line_chart, simulator_solid_area_chart_event_cb, LV_EVENT_DRAW_TASK_ADDED, &chart_color);
    lv_obj_add_flag(line_chart, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

    return line_chart;
}

lv_obj_t* simulator_static_chart_create(){

    // 创建图表对象
    lv_obj_t* chart = lv_chart_create(lv_scr_act());
    lv_obj_set_size(chart, lv_pct(100), lv_pct(70));
    lv_obj_align(chart, LV_ALIGN_TOP_MID, 0, 10);
    lv_obj_set_style_bg_color(chart, lv_palette_main(LV_PALETTE_BLUE), 0);

    // 强制刷新布局
    lv_refr_now(NULL);
    int screen_w = lv_disp_get_hor_res(NULL);  // 屏幕宽度（像素）
    int screen_h = lv_disp_get_ver_res(NULL);  // 屏幕高度（像素）

    int chart_w = lv_obj_get_width(chart);   // 图表实际宽度（像素）
    int chart_h = lv_obj_get_height(chart);  // 图表实际高度（像素）

    LV_LOG_USER("Screen resolution: %dx%d", screen_w, screen_h);
    LV_LOG_USER("Chart: %dx%d (%.1f%% x %.1f%%)", 
                chart_w, chart_h,
                (double)chart_w / screen_w * 100,
                (double)chart_h / screen_h * 100);


    // 设置图表类型为样条线
    lv_chart_set_type(chart, LV_CHART_TYPE_LINE);
    
    // 设置范围
    lv_chart_set_range(chart, LV_CHART_AXIS_PRIMARY_Y, 0, 100);
    
    // 设置足够多的点以容纳所有数据
    lv_chart_set_point_count(chart, data_value_count);
    
    // 彻底清除所有网格线和背景
    // lv_obj_set_style_bg_opa(chart, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(chart, LV_OPA_COVER, LV_PART_MAIN);
    lv_obj_set_style_bg_color(chart, lv_color_white(), 0);
    lv_obj_set_style_line_width(chart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(chart, 0, LV_PART_MAIN);

    //remove padding 
    lv_obj_set_style_pad_all(chart, 0, 0);

    // 禁用数据点标记
    lv_obj_set_style_size(chart, 0, 0, LV_PART_INDICATOR);

    //使线条更平滑
    lv_obj_set_style_line_rounded(chart, true, LV_PART_ITEMS);

    // 添加数据系列
    static_chart_ser = lv_chart_add_series(chart, lv_color_hex(0x3498db), LV_CHART_AXIS_PRIMARY_Y);
    
    // 初始化数据为0
    for(int i = 0; i < data_value_count; i++) {
        lv_chart_set_next_value(chart, static_chart_ser, 0);
    }
    
    // 自定义绘制回调
    lv_obj_add_event_cb(chart, simulator_faded_area_chart_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    lv_obj_add_flag(chart, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);
    
    // 创建定时器，每秒添加一个数据点
    current_data_index = 0;  // 重置索引
    static_chart_timer = lv_timer_create(simulator_static_chart_timer_cb, 1000, chart);  // 1000ms = 1秒

    return  chart;
}
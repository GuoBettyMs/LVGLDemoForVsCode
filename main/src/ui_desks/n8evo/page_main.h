/**
 * @file page_main.h
 *
 */


#ifndef PAGE_MAIN_H
#define PAGE_MAIN_H

#ifdef __cplusplus
extern "C" {
#endif


#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>   // arc4random 在 stdlib.h 中
#include "lvgl.h"

#include "../../common.h"
#include "img/image_defines.h"

#include "User_communication.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
#include "User_gpio.h"
#include "chart_process.h"                              //嵌入式图表

#define MAX_BATT_COM_COUNT              4               //电池仓总数,从0开始
#define MAX_BOTTOM_BTN_COUNT            4               //底部按钮总数
#define MAX_BATT_TOTAL_COUNT            36              //电池总数
#define MAX_BATT_COM_CNT                6               //最大电池仓数,从0开始,横向放置2个电池或竖向放置4个电池
#define MAX_BATT_CNT_IN_COM             6               //一个电池仓内部最大电池数,从0开始
#define MAX_VER_BATT_CNT                4               //最大纵向电池数
#define MAX_INFO_MENU_COUNT             5
#define MAX_INFO_ROW_COUNT              5 
#define MAX_SETTING_ROW_COUNT           9               //含标题 "Task","启动"键
#define MAX_SETTING_ITEM_ROW_COUNT      31
#define LINE_CHART_DISPLAY_POINT        192
#define MAX_MENU_ROW_COUNT              9               //含标题 
#define RIPPLE_NUM                      3               //圆形波纹数量
#define RIPPLE_FROM                     30              //圆形波纹起始尺寸 = 图标大小
#define RIPPLE_TO                       60              //圆形波纹结束尺寸
#define RIPPLE_DUR                      1500            //圆形波纹单圈扩散时长 ms 

#define GET_BATT_INDEX(comp_idx, bar_idx) (MAX_BATT_CNT_IN_COM * (comp_idx) + (bar_idx)) // 定义电池索引计算的宏


typedef enum {
    LAYOUT_VER = 0,
    LAYOUT_HOR = 1,
    LAYOUT_COUNT
} layout_type_t;                       // 布局类型枚举

/***********************  电池数据结构**********************/
typedef struct {
    bool chg_setting;
    bool is_compressed;
    bool BatteryLoad;
    eTaskState_t TaskState;
    eBatteryType_t batt_type;          // 电池类型，如 1:"Li-ion"
    eTaskType_t work_status;           // 0:充电中,1:分析...
    uint8_t value_percent;             // 电量百分比 (0-100)
    int8_t value_tempc;
    int16_t vol_mv;
    int16_t cur_ma;
    int16_t value_tempf;
    uint16_t setting_items[7]; // 索引 0~6 对应：chemistry, task, current, condition , charge_c, discharge_c, cycle_cnt
    uint16_t line_chart_init_data;
    uint16_t line_chart_max_data;
    uint16_t line_chart_min_data;
    int16_t line_chart_data_total_cnt;
    int32_t line_chart_data[LINE_CHART_DISPLAY_POINT]; //曲线数据
    uint32_t time_s;
    uint32_t value_mah;
    uint32_t value_Ohms;
    uint32_t value_mWh;
} batt_data_t;

typedef struct {
    lv_obj_t* bg;                       // 电池整体对象
    lv_obj_t* bar_cont;                 // 进度条容器
    lv_obj_t* bar;                      // 进度条控件
    lv_obj_t* batt_body;                // 电池身体
    lv_obj_t* batt_idx_img;             // 电池索引图像
    lv_obj_t* ripples[RIPPLE_NUM];      // 动画实心圆
    lv_obj_t* workstatus_img;           // 工作状态图像
    lv_obj_t* setting_img;              // 任务设置图标
    lv_obj_t* type_label;               // 类型标签
    lv_obj_t* time_label;               // 时间标签
    lv_obj_t* volcur_label;             // 电压电流标签
} batt_t;       //电池

typedef struct {
    lv_obj_t* bg;                       // 电池仓整体对象
    lv_obj_t* light;                    // 电池仓指示灯
    lv_obj_t* batt_cont;                // 电池仓内部的整体电池容器
    layout_type_t batt_cont_layout;     // 整体电池容器的布局
    batt_t batts[MAX_BATT_CNT_IN_COM];  // 0-3: 纵向电池，4-5：横向电池
} batt_components_t;  //电池仓




typedef struct {
    lv_obj_t* cont;                 // 容器
    lv_obj_t* bar;                  // 进度条控件
    lv_obj_t* workstatus_img;       // 工作状态图像
    lv_obj_t* type_label;           // 类型标签
    lv_obj_t* time_label;           // 时间标签
    lv_obj_t* volcur_label;         // 电压电流标签
    lv_obj_t* setting_img;          // 任务设置图标
    lv_obj_t* info_arrow;
} bar_components_t;

typedef struct {
    lv_obj_t *info_icon;            // 图标
    lv_obj_t *info_label_name;      // 标签名称
    lv_obj_t *info_label_value;     // 标签值
} info_row_t;                       // 信息行的控件集合


typedef struct {
    lv_obj_t *chart;
    int battI;
} chart_timer_data_t;               // 图表定时器结构体，包含图表对象和电池索引


typedef enum {
    SET_CHEMISTRY = 1,
    SET_TASK,    
    SET_CURRENT,       
    SET_CONDITION,    
    SET_CHG_CURRENT, 
    SET_DISCHG_CURRENT,
    SET_CYC,
    SET_START,
    SET_COUNT
} task_settingrow_type_t;   //任务设置行的每一项类型

typedef struct {
    lv_obj_t* row_cont;
    lv_obj_t* title_img;
    lv_obj_t* title_label;
    lv_obj_t* value_label;
    lv_obj_t* value_img;
    lv_obj_t* line;
} setting_row_widgets_t;    //任务设置页面的每一行的控件指针
    
typedef struct {
    const lv_image_dsc_t* main_img;    // 图像指针
    const lv_image_dsc_t* selected_img;    // 图像指针
    int32_t main_text;                 // 主项文本ID
    int32_t selected_text;                 // 主项文本ID
    int32_t sub_texts[10];             // 子项ID数组，以 -1 结尾
} setting_row_display_t;    //设置行的全部显示内容


// 定义任务值到字符串ID的映射
static const struct {
    int32_t value;
    int32_t str_id;
} task_value_to_strid[] = {
    {0, STR_CHARGE},
    {1, STR_DISCHARGE},
    {2, STR_STORAGE},
    {3, STR_CYCLE},
    {4, STR_ANALYSE},
    {5, STR_ACTIVATE},
    {6, STR_DESTROY},  
};

typedef struct {
    uint8_t slave_device_count; //识别到内部电池的工作仓数量
}ui_data_t;

void page_create_v2(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*PAGE_MAIN_H*/
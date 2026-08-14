/**
 * @file page_main.c
 *
 */

#include "page_main.h"

#define MAX_BATT_COM_COUNT 4 //电池仓总数,从0开始
#define MAX_BOTTOM_BTN_COUNT 4 //底部按钮总数


#define MAX_BATT_TOTAL_COUNT 36 //电池总数
#define MAX_BATT_COM_CNT 6 //最大电池仓数,从0开始,横向放置2个电池或竖向放置4个电池
#define MAX_BATT_CNT_IN_COM 6 //一个电池仓内部最大电池数,从0开始
// #define MAX_HOR_BATT_CNT 2 //最大横向电池数
#define MAX_VER_BATT_CNT 4 //最大纵向电池数
#define MAX_INFO_MENU_COUNT 5
#define MAX_INFO_ROW_COUNT 5 
#define MAX_SETTING_ROW_COUNT 9 //含标题 "Task","启动"键
#define MAX_SETTING_ITEM_ROW_COUNT 31
#define LINE_CHART_DISPLAY_POINT 192


#define GET_BATT_INDEX(comp_idx, bar_idx) (MAX_BATT_CNT_IN_COM * (comp_idx) + (bar_idx)) // 定义电池索引计算的宏


static void bar_page_create();
static void on_language_changed();

/***********************  公共变量 **********************/
static uint8_t selected_settingrow_group_index = 1; //设置行组的索引
static bool btn_group_add_lock = 1;
static bool btn_group_activate = 0;

static uint8_t current_lang = LANGUAGE_EN;

/***********************  电池数据结构**********************/
typedef struct {
    bool chg_setting;
    bool is_compressed;
    bool BatteryLoad;
    eTaskState_t TaskState;
    eBatteryType_t batt_type; // 电池类型，如 1:"Li-ion"
    eTaskType_t work_status;      // 0:充电中,1:分析...
    uint8_t value_percent;    // 电量百分比 (0-100)
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

static batt_data_t g_batt_data[MAX_BATT_TOTAL_COUNT];

/***********************  样式 **********************/
static lv_style_t bg_style;
static lv_style_t style_selected; // 创建一个静态的样式变量，用于定义选中状态的外观
static lv_style_t style_pressed; // 创建一个用于“按下状态”的样式，以覆盖默认的灰色
static lv_style_t style_row_pressed; //行选中状态的样式变量
static lv_style_t style_row_selected; //行“按下状态”的样式，以覆盖默认的灰色



/**********************   进度条页面 **********************/

typedef enum {
    LAYOUT_VER = 0,
    LAYOUT_HOR = 1,
    LAYOUT_COUNT
} layout_type_t;// 布局类型枚举

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

/*变量*/
static int current_selected_bar_idx = 0;     //当前选中的 bar 索引

/*控件*/
static layout_type_t g_current_layout = LAYOUT_VER; // 记录当前激活的是哪个布局,默认 LAYOUT_VER
static lv_obj_t* bar_layout = NULL;   // 进度条总览容器
static bar_components_t bar_conts[LAYOUT_COUNT][4]; // 全局二维数组：第一维是布局类型，第二维是进度条索引


/**********************   信息页面 **********************/
typedef struct {
    lv_obj_t *info_icon;          // 图标
    lv_obj_t *info_name;    // 标签名称
    lv_obj_t *info_value;   // 标签值
} info_row_t;           // 信息行的控件集合


typedef struct {
    lv_obj_t *chart;
    int battI;
} chart_timer_data_t;       // 图表定时器结构体，包含图表对象和电池索引


/*变量*/
static int32_t chart_y_max = 100;
static int32_t chart_y_min = 0;
static int32_t line_w = 2;
static int chart_max_h = 28;
static int chart_max_w = 192;

/*控件*/
static lv_obj_t* info_layout = NULL;
static info_row_t g_info_rows[LAYOUT_COUNT][5]; // 全局二维数组：第一维是布局类型，第二维是行索引

static lv_obj_t* g_info_charts[LAYOUT_COUNT];
static lv_chart_series_t* line_chart_ser = NULL;
static lv_timer_t *info_chart_timer = NULL;

/**********************  总览页面 **********************/

/*变量*/
static int current_selected_batt_comp_idx = 0; //当前选中的电池仓索引

/*控件*/
lv_obj_t* stanby_page_layout;

/**********************  任务设置 **********************/

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
    const lv_image_dsc_t* main_img;     // 图像指针
    int32_t main_text;                 // 主项文本ID
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


/*变量*/
static task_settingrow_type_t current_selected_taskrow_idx = -1; //当前所选设置行的索引,0-标题行

static const uint16_t chg_conditon_value[][2] =   
{
    //min,max
    {0,0},           //Auto
    {4250,4450},     //LiHv
    {4100,4300},     //Lilon
    {3550,3750},     //LiFe
    {1800,2000},     //NiZn
    {3,15},          //NiMH  
    {0,0}            //1.5 Lilon 
};

static const uint16_t storage_conditon_value[][2] =   
{
    //min,max
    {0,0},
    {3600,4000},     //LiHv
    {3500,3900},     //Lilon
    {3000,3400},     //LiFe
    {1300,1700},     //NiZn 
    {0,0}
};

static const uint16_t dischg_conditon_value[][2] =   
{
    //min,max
    {0,0},
    {3100,3500},     //LiHv
    {2500,3300},     //Lilon
    {2700,3100},     //LiFe
    {1100,1500},     //NiZn
    {700,1100},      //NiMH  
    {0,0}
};

static const uint16_t chg_current_value[][2] =   
{
    //min,max  
    {0,3000},     //ver
    {0,6000},     //hor
};

static const uint16_t dischg_current_value[][2] =   
{
    //min,max
    {100,1000},     //ver
    {100,2000},     //hor
};

static const uint16_t storage_current_value[][2] =   
{
    //min,max  
    {100,3000},     //ver
    {100,6000},     //hor
};

static const int32_t task_value[][7] =   
{
    {STR_CHARGE, STR_DISCHARGE, STR_STORAGE, STR_CYCLE, STR_ANALYSE, STR_ACTIVATE, STR_DESTROY, -1},  
    {STR_CHARGE, STR_DESTROY, -1},    
    {STR_CHARGE, STR_DISCHARGE, STR_CYCLE, STR_ANALYSE, STR_ACTIVATE, STR_DESTROY, -1},
};

static const setting_row_display_t tasksetting_display_rows[] = 
{
    {&img_chemistry, STR_TASK, {-1}},
    {&img_chemistry, STR_BATTERY, {STR_AUTO, STR_LIHV, STR_LIHV, STR_LIFE, STR_NIZN, STR_NIMH, STR_LIHV, -1}},
    {&img_chemistry, STR_TASK_SELECTION, {task_value[2][0], -1}},
    {&img_chemistry, STR_CURRENT, {chg_current_value[0][0], -1}},
    {&img_condition, STR_CONDITION, {chg_conditon_value[0][0], -1}},
    {&img_chemistry, STR_CHARGE_CURRENT, {chg_current_value[0][0], -1}},
    {&img_chemistry, STR_DISCHARGE_CURRENT, {dischg_current_value[0][0], -1}},
    {&img_chemistry, STR_CYCLES, {5, 10, 15, 20, 25, 30, -1}},
    {&img_chemistry, STR_START, {-1}},
};//任务设置每行的显示内容,0-标题行



/*控件*/
static lv_obj_t* task_setting_text_cont;                                    // 任务主列表的内容行容器
static lv_obj_t* setting_layout;                                                   // 任务设置容器
static setting_row_widgets_t setting_mainlist[MAX_SETTING_ROW_COUNT];
static lv_obj_t* setting_sublist[MAX_SETTING_ROW_COUNT] = {NULL};


/**********************  系统设置 **********************/


/*控件*/


/**********************
 *   common methods
 **********************/

// void btn_indev_group_remove_all_obj(void)
// {
//     btn_group_activate = 0;
//     btn_group_add_lock = 1;
//     lv_group_remove_all_objs(btn_group);
// }

// void btn_indev_group_remove_obj(lv_obj_t *obj)
// {
//     lv_group_remove_obj(obj);
// }

// void btn_indev_group_add_obj(lv_obj_t *obj)
// {
//     if(obj)
//         lv_group_add_obj(btn_group, obj);
// }

/**
 * @brief 获取工作状态颜色
 * @param value 当前工作状态
 */
lv_color_t get_batt_workstatus_color(int32_t value){
    switch (value) {
        case ttCharge: return COLOR_FF6600; // 充电中
        case ttDischarge: return COLOR_FF5599; // 放电
        case ttStorage: return COLOR_5555FF; // 存储
        case ttCycle: return COLOR_00B7E5;// 循环
        case ttAnalysis: return COLOR_9955FF; // 分析
        case ttActivate: return COLOR_37C8AB; // 激活
        case ttDestroy: return COLOR_FF5555; // 损坏
        default: return COLOR_666666; // 待机或未知状态
    }
}


/**
 * @brief 更新进度条值
 * @param bar_idx 当前点击的进度条索引
 * @param value 当前进度值
 */
void update_batt_per_value(int bar_idx, int32_t value) {
    if (g_current_layout == LAYOUT_VER && (bar_idx >= MAX_VER_BATT_CNT || bar_idx < 0)) 
        return;
    if (g_current_layout == LAYOUT_HOR && (bar_idx >= (MAX_BATT_CNT_IN_COM - MAX_VER_BATT_CNT) || bar_idx < 0)) 
        return;

    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];

    int offset = 34;
    int visibleW = bar_idx >= MAX_VER_BATT_CNT ? 227 : 126;
    int newvalue = value + (100 - value) * offset / visibleW; // 15 + 85 * value / 100;
 
    lv_obj_t* bar = bar_conts[g_current_layout][bar_idx].bar;
    if (bar) {
        lv_bar_set_value(bar, data->BatteryLoad == 0 ? 0 : newvalue, LV_ANIM_OFF);
    }
    
}

/**
 * @brief 更新电压电流数据文本
 * @param bar_idx 当前点击的进度条索引
 * @param is_info true=更新信息页面的数据文本, false=更新进度条页面的数据文本
 * @param vol_mv 电压数据
 * @param cur_ma 电流数据
 */
void update_batt_volcur_text(int bar_idx, bool is_info, int32_t vol_mv, int32_t cur_ma) {
    if (g_current_layout == LAYOUT_VER && (bar_idx >= MAX_VER_BATT_CNT || bar_idx < 0)) 
        return;
    if (g_current_layout == LAYOUT_HOR && (bar_idx >= (MAX_BATT_CNT_IN_COM - MAX_VER_BATT_CNT) || bar_idx < 0)) 
        return;

    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];

    if(!is_info){
        lv_obj_t *bar_label = bar_conts[g_current_layout][bar_idx].volcur_label;
        if (bar_label)
        {
            char buf[32];
            //部分嵌入式环境 lv_snprintf 默认不启用浮点格式化支持,可改为 snprintf
            vol_mv += 50;
            vol_mv /= 100;
            if(cur_ma >= 0)
            {
                cur_ma += 50;
                cur_ma /= 100;
                snprintf(buf, sizeof(buf), "%ld.%01ldV/%ld.%01ldA", vol_mv/10,vol_mv%10, cur_ma/10,cur_ma%10);
            }
            else
            {
                cur_ma -= 50;
                cur_ma /= 100;
                cur_ma = -cur_ma;
                snprintf(buf, sizeof(buf), "%ld.%01ldV/-%ld.%01ldA", vol_mv/10,vol_mv%10, cur_ma/10,cur_ma%10);
            }
            
            lv_label_set_text(bar_label, data->BatteryLoad == 0 ? "--" : buf);
        }
    }else{
        char buf[32], buf2[32];
        lv_obj_t *label_name = g_info_rows[g_current_layout][2].info_name;
        if (label_name) {
            vol_mv += 5;
            vol_mv /= 10;
            snprintf(buf, sizeof(buf), "%ld.%02ldV", vol_mv/100,vol_mv%100); 
            lv_label_set_text(label_name, data->BatteryLoad == 0 ? "--" : buf); 
        }
        lv_obj_t *label_value = g_info_rows[g_current_layout][2].info_value;
        if (label_value) {
            if(cur_ma >= 0)
            {
                cur_ma += 5;
                cur_ma /= 10;        
                snprintf(buf2, sizeof(buf2), "%ld.%02ldA", cur_ma/100,cur_ma%100); 
            }
            else
            {
                cur_ma -= 5;
                cur_ma /= 10;
                cur_ma = -cur_ma;
                snprintf(buf2, sizeof(buf2), "-%ld.%02ldA", cur_ma/100,cur_ma%100); 
            }
            lv_label_set_text(label_value, data->BatteryLoad == 0 ? "--" : buf2); 
        }

    }
}


/**
 * @brief 更新瓦时文本
 * @param bar_idx 当前点击的进度条索引
 * @param vol_mv 电压数据
 * @param cur_ma 电流数据
 */
void update_batt_wh_text(int bar_idx, uint32_t value_mWh) {
    if (g_current_layout == LAYOUT_VER && (bar_idx >= MAX_VER_BATT_CNT || bar_idx < 0)) 
        return;
    if (g_current_layout == LAYOUT_HOR && (bar_idx >= (MAX_BATT_CNT_IN_COM - MAX_VER_BATT_CNT) || bar_idx < 0)) 
        return;

    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];

    // 直接使用全局数组，根据当前布局
    lv_obj_t *label_name = g_info_rows[g_current_layout][4].info_name;
    if (label_name) {
        char buf[32];
        if(data->BatteryLoad == 0)
        {
            lv_label_set_text(label_name,"--");
        }
        else if(value_mWh < 1000)
        {
            snprintf(buf, sizeof(buf), "%ldmWh", value_mWh);
            lv_label_set_text(label_name, buf);
        }
        else
        {
            value_mWh += 5;
            value_mWh /= 10;
            snprintf(buf, sizeof(buf), "%ld.%02ldWh", value_mWh/100,value_mWh%100);
            lv_label_set_text(label_name, buf);
        }
    }
}


/**
 * @brief 时间换算
 * @param buf 字符串文本,格式为 时:分:秒
 * @param run_time_s 时间,s
 */
static inline void format_time(char* buf, uint32_t run_time_s) {
    const uint8_t hours = run_time_s / 3600;
    const uint8_t mins = (run_time_s / 60) % 60;
    const uint8_t secs = run_time_s % 60;
    lv_snprintf(buf, 32, "%02d:%02d:%02d", hours, mins, secs);
}


/**
 * @brief 更新时间文本
 * @param bar_idx 当前点击的进度条索引
 * @param is_info true=更新信息页面的数据文本, false=更新进度条页面的数据文本
 * @param run_time_s 时间数据
 */
void update_batt_time_text(int bar_idx, bool is_info, uint32_t run_time_s) {
    if (g_current_layout == LAYOUT_VER && (bar_idx >= MAX_VER_BATT_CNT || bar_idx < 0)) 
        return;
    if (g_current_layout == LAYOUT_HOR && (bar_idx >= (MAX_BATT_CNT_IN_COM - MAX_VER_BATT_CNT) || bar_idx < 0)) 
        return;

    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    char buf[32];
    format_time(buf, run_time_s);

    if(!is_info){
        lv_obj_t* time_label = bar_conts[g_current_layout][bar_idx].time_label;
        if (time_label) {
            lv_label_set_text(time_label, data->BatteryLoad == 0 ? "--" : buf);
        }
    }else{
        lv_obj_t *label_value = g_info_rows[g_current_layout][4].info_value;
        if (label_value) {
            lv_label_set_text(label_value, data->BatteryLoad == 0 ? "--" : buf); 
        }
    }

}


/**
 * @brief 删除对象
 * @param layout 对象
 */
void lv_delete_layout(lv_obj_t **layout)
{
    if(*layout != NULL)
    {
        lv_obj_delete(*layout);
        *layout = NULL;
    }
}



/**********************
 *  任务设置
 **********************/

/**
 * @brief 获取当前操作的电池数据结构体
 */
static batt_data_t* get_current_batt_data(void) {
    int battI = GET_BATT_INDEX(current_selected_batt_comp_idx, current_selected_bar_idx);
    if (battI < 0 || battI >= MAX_BATT_TOTAL_COUNT) return NULL;
    return &g_batt_data[battI];
}    


/**
 * @brief 获取任务条件
 * @param batt_type 电池类型
 */
const int32_t* get_task_setting(eBatteryType_t batt_type)
{
    switch (batt_type)
    {
    case btAuto:
        return task_value[1];
    case btNiMHCd:
        return task_value[2];
    default:
        return task_value[0];
    }
}


/**
 * @brief 获取电流条件
 * @param work_status 工作状态
 * @param g_current_layout 当前布局
 */
uint16_t *get_current_setting(eTaskType_t work_status,layout_type_t g_current_layout)
{
    switch (work_status)
    {
    case ttCharge:
        return &chg_current_value[g_current_layout][0];
    case ttDischarge:
        return &dischg_current_value[g_current_layout][0];
    case ttStorage:
        return &storage_current_value[g_current_layout][0];
    default:
        return &chg_current_value[g_current_layout][0];
    }
}


/**
 * @brief 获取截止电压条件
 * @param batt_type 电池类型
 * @param work_status 工作状态
 */
uint16_t *get_condition_setting(eBatteryType_t batt_type,eTaskType_t work_status)
{
    switch (work_status)
    {
    case ttCharge:
        if(batt_type == btAuto || batt_type == btLi5V)
            return &chg_conditon_value[btLi5V][0];
        else
            return &chg_conditon_value[batt_type][0];
    case ttDischarge:
    case ttCycle:
    case ttAnalysis:
    case ttActivate:
        if(batt_type == btAuto || batt_type == btLi5V)
            return &dischg_conditon_value[btLi5V][0];
        else
            return &dischg_conditon_value[batt_type][0];
    case ttStorage:
        if(batt_type == btAuto || batt_type == btNiMHCd || batt_type == btLi5V)
            return &storage_conditon_value[btNiMHCd][0];
        else
            return &storage_conditon_value[batt_type][0];
    default:
        return &chg_conditon_value[batt_type][0];
    }
} 

/**
 * @brief 根据电池类型获取建议的结束条件
 */
static int32_t get_suggested_condition_by_battery_type(void) {
    batt_data_t* data = get_current_batt_data();
    if (!data) return 0;  // 错误：无数据
    
    int type = data->setting_items[0];
    int status = data->setting_items[1];
    switch(type) {
        case btLiHv: 
            if(status == ttCharge)
                return 4350;
            else if(status == ttDischarge || status == ttCycle || status == ttAnalysis || status == ttActivate)
                return 3300;
            else if(status == ttStorage)
                return 3800;
            else
                return 4350;
        case btLiIon:
            if(status == ttCharge)
                return 4200;
            else if(status == ttDischarge || status == ttCycle || status == ttAnalysis || status == ttActivate)
                return 3100;
            else if(status == ttStorage)
                return 3700;
            else
                return 4200;
        case btLiFe:
            if(status == ttCharge)
                return 3650;
            else if(status == ttDischarge || status == ttCycle || status == ttAnalysis || status == ttActivate)
                return 2900;
            else if(status == ttStorage)
                return 3200;
            else
                return 3650;
        case btNiMHCd:
            if(status == ttCharge)
                return 5;
            else if(status == ttDischarge || status == ttCycle || status == ttAnalysis || status == ttActivate)
                return 900;
            else
                return 5;
        case btNiZn:
            if(status == ttCharge)
                return 1900;
            else if(status == ttDischarge || status == ttCycle || status == ttAnalysis || status == ttActivate)
                return 1300;
            else if(status == ttStorage)
                return 1500;
            else
                return 1900;
        default: return 0;   // 未知类型
    }
}


/**
 * @brief 将任务字符串转为对应索引
 * @param str 任务字符串
 */
int32_t parse_task_string_to_int32(const char* str) {
    if (!str) return -1;
    for (size_t i = 0; i < sizeof(task_value_to_strid)/sizeof(task_value_to_strid[0]); ++i) {
        const char* localized = GetLanguageString(task_value_to_strid[i].str_id);
        if (localized && lv_strcmp(localized, str) == 0) {
            return task_value_to_strid[i].value;
        }
    }

    printf("no found task_value_to_strid[i].value,  return 0 \n" );
    return 0;  // 默认值
}


/**
 * @brief 在任务条件数组中找到对应的 ui 索引
 * @param task_value 任务索引值
 */
int find_ui_index_by_task(int32_t task_value) {
    batt_data_t* batt_data = get_current_batt_data();
    int type = batt_data->setting_items[0]; // 第0项是电池类型索引

    // 获取当前电池类型对应的任务选项ID列表（以0结尾）
    const int32_t* option_ids = get_task_setting(type);//get_current_task_options_by_battery_type();

    // 根据 task_value 查找对应的字符串ID
    int32_t target_str_id = -1;
    for (size_t i = 0; i < sizeof(task_value_to_strid) / sizeof(task_value_to_strid[0]); ++i) {
        if (task_value == task_value_to_strid[i].value) {
            target_str_id = task_value_to_strid[i].str_id;
            break;
        }
    }
    if (target_str_id == -1) {
        // printf(">>> unknown task value=%ld\n", task_value);
        return 0;
    }

    // printf("target_str_id=%ld, task_value=%ld\n", target_str_id, task_value);

    // 在 option_ids 中查找匹配的ID（数组以0结尾）
    for (int i = 0; option_ids[i] != 0; ++i) {
        if (option_ids[i] == (uint32_t)target_str_id) {
            return i;   // 返回选项索引
        }
    }

    printf(">>> task_value=%ld (str_id=%ld) not found in option_ids\n", task_value, target_str_id);
    return 0;  // 默认返回索引0（第一个选项）
}


/**
 * @brief 更新任务设置行的 value 
 * @param setting_row_idx 任务行
 * @param data 数据值
 */
void update_setting_row_value(int setting_row_idx, int data) {
    // 检查该行是否可见
    if (lv_obj_has_flag(setting_mainlist[setting_row_idx].row_cont, LV_OBJ_FLAG_HIDDEN)) {
        return;
    }

    const int32_t* option_ids;
    char new_text[32] = {0};
    const char* current_text = lv_label_get_text(setting_mainlist[setting_row_idx].value_label);
    batt_data_t* batt_data = get_current_batt_data();
    int type = batt_data->setting_items[0]; // 第0项是电池类型索引
    int status = batt_data->setting_items[1]; 

    switch (setting_row_idx) {
        case SET_CHEMISTRY: // Chemistry
            option_ids = tasksetting_display_rows[setting_row_idx].sub_texts;
            snprintf(new_text, sizeof(new_text), "%s", GetLanguageString(option_ids[data]));
            break;

        case SET_CURRENT:
        case SET_CHG_CURRENT: // Current / charge_c
        case SET_DISCHG_CURRENT:
            if (data == 0) {
                snprintf(new_text, sizeof(new_text), "%s", GetLanguageString(STR_AUTO));
            } else {
                snprintf(new_text, sizeof(new_text), "%.2fA", data * 0.001f);
            }
            break;

        case SET_TASK: // Task
            option_ids = get_task_setting(type);
            int actual_ui_idx = find_ui_index_by_task(data);
            snprintf(new_text, sizeof(new_text), "%s", GetLanguageString(option_ids[actual_ui_idx]));
            break;

        case SET_CONDITION: // Condition
            if(type != btAuto || type != btLi5V)
            {
                if(type != btNiMHCd || status != ttCharge)
                    snprintf(new_text, sizeof(new_text), "%.2fV", data / 1000.0f);
                else
                    snprintf(new_text, sizeof(new_text), "-\u2206%dmV", data);
            } 
            break;

        case SET_CYC: // cycle
            snprintf(new_text, sizeof(new_text), "%d", data);
            break;

        default:
            return;
    }

    // 仅当文本不同时才更新
    if (current_text && lv_strcmp(current_text, new_text) != 0) {
        // printf("update_setting_row_value, row = %d, data = %d \n", setting_row_idx, data);
        lv_label_set_text(setting_mainlist[setting_row_idx].value_label, new_text);
    }
}

/**
 * @brief 设置控件的可见性（封装隐藏/显示标志操作）
 * @param obj 控件对象指针
 * @param visible true=显示, false=隐藏
 */
static inline void set_obj_visibility(lv_obj_t* obj, bool visible) {
    if (obj && lv_obj_is_valid(obj)) {
        if (visible) {
            lv_obj_clear_flag(obj, LV_OBJ_FLAG_HIDDEN);
        } else {
            lv_obj_add_flag(obj, LV_OBJ_FLAG_HIDDEN);
        }
    }
}


/**
 * @brief 刷新设置行的可见性 
 */
static void refresh_setting_row_visibility(void) {
    int battI = GET_BATT_INDEX(current_selected_batt_comp_idx, current_selected_bar_idx);
    if (battI < 0 || battI >= MAX_BATT_TOTAL_COUNT) return ;

    batt_data_t* data = &g_batt_data[battI];
    int battery_type = data->setting_items[0];
    int task = data->setting_items[1]; 

    // 1. 隐藏所有设置行（从行1到行 MAX_SETTING_ROW_COUNT-2，不隐藏标题行和开始按钮）
    for (int i = 1; i < MAX_SETTING_ROW_COUNT - 1; i++) {
        set_obj_visibility(setting_mainlist[i].row_cont, false);
        set_obj_visibility(setting_mainlist[i].line, false);
    }

    // 2. 构建可见行列表（行号从1开始）
    int visible_rows[10];          // 最多存放10个行号，足够
    int visible_count = 0;

    // 辅助宏：添加行号（自动检查越界，可选）
    #define ADD_ROW(row) do { \
        if (visible_count < (int)(sizeof(visible_rows)/sizeof(visible_rows[0]))) { \
            visible_rows[visible_count++] = (row); \
        } else { \
            printf("WARNING: visible_rows overflow, row %d ignored\n", (row)); \
        } \
    } while(0)

    // 根据电池类型和任务决定显示哪些行
    switch (battery_type) {
        case btAuto:   // 自动
            if (task == ttCharge) {            // 充电
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_CURRENT); ADD_ROW(SET_START);
                lv_obj_set_style_pad_bottom(task_setting_text_cont, 41, 0);
            } else if (task == ttDestroy) {     // 销毁
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_START);
                lv_obj_set_style_pad_bottom(task_setting_text_cont, 82, 0);//实现子对象总高度小于可视高度，仍可上下滚动
            }
            break;

        case btLi5V:   // 1.5V Lilon
            lv_obj_set_style_pad_bottom(task_setting_text_cont, 0, 0);
            if (task == ttCharge || task == ttDestroy) {
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_START);
                 lv_obj_set_style_pad_bottom(task_setting_text_cont, 82, 0);
            }
            break;

        default:   // 其他类型
            lv_obj_set_style_pad_bottom(task_setting_text_cont, 0, 0);
            if (task == ttCycle) {            // 循环
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_CONDITION); ADD_ROW(SET_CHG_CURRENT); ADD_ROW(SET_DISCHG_CURRENT); ADD_ROW(SET_CYC); ADD_ROW(SET_START);
            } else if (task == ttAnalysis || task == ttActivate) {     // 分析、激活
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_CONDITION); ADD_ROW(SET_CHG_CURRENT); ADD_ROW(SET_DISCHG_CURRENT); ADD_ROW(SET_START);
            }else if (task == ttDestroy) {     // 销毁
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_START);
                 lv_obj_set_style_pad_bottom(task_setting_text_cont, 82, 0);//实现子对象总高度小于可视高度，仍可上下滚动
            } else {                    // 其他任务：充电、放电、存储等
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_CURRENT); ADD_ROW(SET_CONDITION); ADD_ROW(SET_START);
            }
            break;
    }

    #undef ADD_ROW   // 取消宏定义，避免污染


    printf("refresh_setting_row_visibility battI = %d, type = %d, task = %d, visible_count= %d\n", battI, battery_type, task, visible_count);

    // 3. 遍历可见行列表，设置为可见
    for (int i = 0; i < visible_count; i++) {
        int row = visible_rows[i];
        // 安全检查，防止越界
        if (row >= 1 && row < MAX_SETTING_ROW_COUNT) {
            set_obj_visibility(setting_mainlist[row].row_cont, true);
            set_obj_visibility(setting_mainlist[row].line, true);
        } else {
            printf("ERROR: invalid row %d in visible_rows\n", row);
        }
    }


    // 更新所有设置行的值（只更新可见行）
    for (int row = 1; row < MAX_SETTING_ROW_COUNT - 1; row++) {
        int data;
        if (row == 6)
        {
            data = 100;     //放电电流
        }else if (row == 7)
        {
            data = 5;       //循环次数
        }else
        {
            data = g_batt_data[battI].setting_items[row - 1];
        }
        update_setting_row_value(row, data);
    }

}


/**
 * @brief 获取当前选中的任务行的子列表选中索引
 * @param row 任务行
 */
static int get_task_list_idx(int row)
{
    int idx = 0;
    batt_data_t* data = get_current_batt_data();
    int type = data->setting_items[0]; // 第0项是电池类型索引
    int status = data->setting_items[1]; 
    if(row == SET_CHEMISTRY)
    {
        idx = type;
    }
    else if(row == SET_TASK)
    {
        idx = find_ui_index_by_task(data->setting_items[1]);
    }
    else if(row == SET_CURRENT)
    {
        const uint16_t* option_ids = get_current_setting(status,g_current_layout);
        idx = (data->setting_items[2] - option_ids[0]) / 100;
    }
    else if(row == SET_CONDITION)
    {
        const uint16_t* option_ids = get_condition_setting(type,status);
        if(type == btNiMHCd && status == ttCharge)
            idx = data->setting_items[3] - option_ids[0];
        else
            idx = (data->setting_items[3] - option_ids[0]) / 10;
    }
    else if(row == SET_CHG_CURRENT)
    {
        const uint16_t* option_ids = get_current_setting(ttCharge,g_current_layout);
        idx = (data->setting_items[4] - option_ids[0]) / 100;
    }
    else if(row == SET_DISCHG_CURRENT)
    {  //可能情况: 初始值 data->setting_items[5] = 0,而 option_ids[0] = 100, idx = -1
        const uint16_t* option_ids = get_current_setting(ttDischarge,g_current_layout);
        idx = (data->setting_items[5] - option_ids[0]) / 100;
    }
    else if(row == SET_CYC)
    {  ////可能情况: 初始值 data->setting_items[6] = 0,而 tasksetting_display_rows[row].sub_texts[0] = 5, idx = -1
        idx = (data->setting_items[6] -  tasksetting_display_rows[row].sub_texts[0]) / 5;
    }
    return idx == -1 ? 0 : idx;         //防呆
}


/**
 * @brief  GUI 开发中, LVGL 的非阻塞定时器事件, 延时 100ms 再销毁子列表
 */
static void my_callback(lv_timer_t* timer)
{

    lv_obj_t* sub = setting_sublist[current_selected_taskrow_idx];
    if (sub) {
        printf("after lv_refr_now, sub  = NULL \n");
        lv_obj_del(sub);
        setting_sublist[current_selected_taskrow_idx] = NULL;
    }
    // 清除父按钮的选中样式
    lv_obj_clear_state(setting_mainlist[current_selected_taskrow_idx].row_cont, LV_STATE_CHECKED);
    current_selected_taskrow_idx = -1;
}

/**
 * @brief 子列表行点击事件
 */
static void child_click_cb(lv_event_t *e) {
    lv_obj_t* child_btn = lv_event_get_target(e);
    lv_obj_t* sub_container = lv_obj_get_parent(child_btn);         // 子容器
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        // 检查当前展开的行索引是否有效
        if (current_selected_taskrow_idx < 0 || current_selected_taskrow_idx >= MAX_SETTING_ROW_COUNT) {
            printf("Invalid current_selected_taskrow_idx: %d\n", current_selected_taskrow_idx);
            return;
        }

        int current_selected_sub = get_task_list_idx(current_selected_taskrow_idx);     // 曾经选中的子项索引
        int clicked_sub_idx = (int)(intptr_t)lv_obj_get_user_data(child_btn);           // 当前点击的子项索引

        // printf("row(%d) child list olddata = %d, newdata = %d\n", 
        //     current_selected_taskrow_idx, current_selected_sub, clicked_sub_idx);

        // ====== 1. 如果点击已选中子项 → 仅折叠（删除） ======
        if (current_selected_sub == clicked_sub_idx) {
            lv_obj_t* sub = setting_sublist[current_selected_taskrow_idx];
            if (sub) {
                printf("setting_sublist  = NULL \n");
                lv_obj_del(sub);
                setting_sublist[current_selected_taskrow_idx] = NULL;     // 立即置空，防止悬空
            }
            // 清除父按钮的选中样式
            lv_obj_clear_state(setting_mainlist[current_selected_taskrow_idx].row_cont, LV_STATE_CHECKED);
            current_selected_taskrow_idx = -1;
            return;
        }

        // ====== 2. 点击不同子项 → 切换选中 ======
        // 清除旧选中
        if (current_selected_sub != -1) {
            // 注意：子容器中每个子项由 line + btn 组成，所以 btn 索引为 current_selected_sub * 2 + 1
            lv_obj_t* old_child = lv_obj_get_child(sub_container, current_selected_sub*2+1);
            if (old_child) {
                lv_obj_remove_state(old_child, LV_STATE_CHECKED);
            }
        }

        // 设置新选中
        if (child_btn && lv_obj_is_valid(child_btn)) {
            lv_obj_add_state(child_btn, LV_STATE_CHECKED);
            lv_obj_scroll_to_view(child_btn, LV_ANIM_OFF);                  //滚动定位
        }


        // 保存数据 & 更新父按钮显示值
        batt_data_t* data = get_current_batt_data();
        if (!data) return;
        int type = data->setting_items[0]; // 第0项是电池类型索引
        int status = data->setting_items[1];

        int row = current_selected_taskrow_idx;
        int32_t newdata = 0;
        const uint16_t* u16option_ids; 
        const int32_t* option_ids = get_task_setting(type);
        switch (row) {
            case SET_CHEMISTRY: 
                newdata = clicked_sub_idx;
                break;

            case SET_TASK: 
                const char* value_str = GetLanguageString(option_ids[clicked_sub_idx]);
                newdata = parse_task_string_to_int32(value_str);
                break;

            case SET_CURRENT:
                u16option_ids = get_current_setting(status,g_current_layout);
                newdata = clicked_sub_idx * 100 + u16option_ids[0];
                break;

            case SET_CHG_CURRENT:
                u16option_ids = get_current_setting(ttCharge,g_current_layout);
                newdata = clicked_sub_idx * 100 + u16option_ids[0];
                break;

            case SET_DISCHG_CURRENT:
                u16option_ids = get_current_setting(ttDischarge,g_current_layout);
                newdata = clicked_sub_idx * 100 + u16option_ids[0];
                break;

            case SET_CONDITION://condition
                u16option_ids = get_condition_setting(type,status);
                if(type == btNiMHCd && status == ttCharge)
                    newdata = u16option_ids[0] + clicked_sub_idx;
                else
                    newdata = u16option_ids[0] + clicked_sub_idx * 10;
                break;

            case SET_CYC:
                newdata = clicked_sub_idx * 5 + tasksetting_display_rows[SET_CYC].sub_texts[0];
                break;

            default:
                break;
        }

        // 持久化到电池数据结构
        data->setting_items[row - 1] = newdata;
        update_setting_row_value(row, newdata);
        printf(">>> setting row(%d) save new data = %ld\n", row, newdata);

        // 更新行上显示的文本
        if(row == SET_CHEMISTRY)
        {
            int ui_index = 0;
            ui_index = find_ui_index_by_task(data->setting_items[1]);
            const int32_t* option_ids = get_task_setting(type);
            const char* value_str = GetLanguageString(option_ids[ui_index]);
            newdata = parse_task_string_to_int32(value_str);
            data->setting_items[1] = newdata;
            update_setting_row_value(SET_TASK, newdata);

            u16option_ids = get_condition_setting(data->setting_items[0],data->setting_items[1]);
            if(data->setting_items[0] != btAuto || data->setting_items[0] != btLi5V || data->setting_items[1] != ttDestroy)
            {
                if(data->setting_items[3] < u16option_ids[0] || data->setting_items[3] > u16option_ids[1] || u16option_ids[0] == 0)
                {
                    data->setting_items[3] = get_suggested_condition_by_battery_type();
                }
                update_setting_row_value(SET_CONDITION, data->setting_items[3]);  
            }
                
        }
        else if(row == SET_TASK)
        {
            if(data->setting_items[1] == ttCharge || data->setting_items[1] == ttDischarge || data->setting_items[1] == ttStorage)
            {
                data->setting_items[2] = 1000;
                update_setting_row_value(SET_CURRENT, data->setting_items[2]);
            }
        
            u16option_ids = get_condition_setting(type,data->setting_items[1]);
            if(data->setting_items[0] != btAuto || data->setting_items[0] != btLi5V || data->setting_items[1] != ttDestroy)
            {
                if(data->setting_items[3] < u16option_ids[0] || data->setting_items[3] > u16option_ids[1] || u16option_ids[0] == 0)
                {
                    data->setting_items[3] = get_suggested_condition_by_battery_type();
                }
                update_setting_row_value(SET_CONDITION, data->setting_items[3]);
            }     
            
        }
        

        // 若更改了电池类型（索引 1），需要刷新相关设置行和结束条件
        if (current_selected_taskrow_idx == SET_CHEMISTRY || current_selected_taskrow_idx == SET_TASK) {
            batt_data_t* data = get_current_batt_data();
            if (data) {
                // 刷新所有设置行的可见性（例如某些类型需要隐藏/显示行）
                refresh_setting_row_visibility();
            }
        }
        // btn_indev_group_add_setting_rows();


        // ====== 3. 选择后自动折叠（删除子容器） ======
        // 注意：子容器索引与父按钮索引，其实数值上一致
        lv_refr_now(lv_display_get_default());      //手动强制刷新
        // vTaskDelay(100 / portTICK_PERIOD_MS);    // RTOS 环境延时100ms

        // lv_obj_t* sub = setting_sublist[current_selected_taskrow_idx];
        // if (sub) {
        //     printf("after lv_refr_now, sub  = NULL \n");
        //     // lv_obj_del_async(sub);
        //     lv_obj_del(sub);
        //     setting_sublist[current_selected_taskrow_idx] = NULL;
        // }
        // // 清除父按钮的选中样式
        // lv_obj_clear_state(setting_mainlist[current_selected_taskrow_idx].row_cont, LV_STATE_CHECKED);
        // current_selected_taskrow_idx = -1;

        //  GUI 开发中, LVGL 的非阻塞定时器事件
        lv_timer_t * timer = lv_timer_create(my_callback, 100, NULL);
        lv_timer_set_repeat_count(timer, 1);                            // 重复次数为 1，执行完自动删除
    }


}


/**
 * @brief 更新子列表每行标签文本
 * @param setting_row_idx 任务行
 * @param label 需要更新的标签
 * @param ui_idx 子列表 ui 索引
 */
void update_single_item_value(int setting_row_idx, lv_obj_t* label, int32_t ui_idx){

    // 获取子对象数量，即选项列表长度
    // printf("update row(%d) item list value = %ld \n", setting_row_idx, ui_idx);

    const int32_t* option_ids;
    const uint16_t* u16option_ids;
    batt_data_t* data = get_current_batt_data();
    int type = data->setting_items[0]; // 第0项是电池类型索引
    int status = data->setting_items[1]; 
    char buf[16];

    switch (setting_row_idx){
        case SET_CHEMISTRY://Chemistry
            option_ids = tasksetting_display_rows[setting_row_idx].sub_texts;  
            snprintf(buf, sizeof(buf), "%s",  GetLanguageString(option_ids[ui_idx]));
            break;

        case SET_TASK://Task
            option_ids = get_task_setting(type);
            snprintf(buf, sizeof(buf), "%s", GetLanguageString(option_ids[ui_idx]));
            break;

        case SET_CURRENT://3-Current
            u16option_ids = get_current_setting(status,g_current_layout);
            if (ui_idx == 0 && u16option_ids[0] == 0){
                snprintf(buf, sizeof(buf), "%s", GetLanguageString(STR_AUTO));
            }else{
                snprintf(buf, sizeof(buf), "%.2fA", ui_idx * 0.1f + u16option_ids[0] * 0.001f);
            }
            break;

        case SET_CHG_CURRENT: //5-charge_c
            u16option_ids = get_current_setting(ttCharge,g_current_layout);
            if (ui_idx == 0 && u16option_ids[0] == 0){
                snprintf(buf, sizeof(buf), "%s", GetLanguageString(STR_AUTO));
            }else{
                snprintf(buf, sizeof(buf), "%.2fA", ui_idx * 0.1f + u16option_ids[0] * 0.001f);
            }
            break;

        case SET_DISCHG_CURRENT: //6-discharge_c
            u16option_ids = get_current_setting(ttDischarge,g_current_layout);
            snprintf(buf, sizeof(buf), "%.2fA", ui_idx * 0.1f + u16option_ids[0] * 0.001f);
            break;

        case SET_CONDITION://Condition
            u16option_ids = get_condition_setting(type,status); 
            if(type != btAuto || type != btLi5V)
            {
                if(type != btNiMHCd || status != ttCharge)
                    snprintf(buf, sizeof(buf), "%.2fV", (u16option_ids[0] + 10 * ui_idx) / 1000.0f);
                else
                    snprintf(buf, sizeof(buf), "-\u2206%" PRId32 "mV", (u16option_ids[0] + ui_idx));
            }               
            break;

        case SET_CYC://7-cycle
            option_ids = tasksetting_display_rows[setting_row_idx].sub_texts;
            snprintf(buf, sizeof(buf), "%ld",  option_ids[ui_idx]);
            break;

        default:
            break;
    }

    lv_label_set_text(label, buf);

}


/**
 * @brief 获取子项ID数组长度
 * @param row_idx 任务行
 */
int get_sub_items_count(int row_idx) {
    batt_data_t* data = get_current_batt_data();
    int type = data->setting_items[0]; // 第0项是电池类型索引
    int status = data->setting_items[1]; 
    int length = 0;

    if(row_idx == SET_CHEMISTRY)
    {
        const int32_t* option_ids = tasksetting_display_rows[row_idx].sub_texts;
        while (option_ids[length] != -1) {
            length++;
        }
    }
    else if(row_idx == SET_TASK)
    {
        const int32_t* option_ids = get_task_setting(type);
        while (option_ids[length] != -1) {
            length++;
        }
    }
    else if(row_idx == SET_CURRENT)
    {
        const uint16_t* option_ids = get_current_setting(status,g_current_layout);
        length = (option_ids[1] - option_ids[0]) / 100 + 1;    
    }
    else if(row_idx == SET_CONDITION)
    {
        const uint16_t* option_ids = get_condition_setting(type,status); 
        if(type != btAuto || type != btLi5V)
        {
            if(type != btNiMHCd)
                length = (option_ids[1] - option_ids[0]) / 10 + 1;
            else
                length = (option_ids[1] - option_ids[0]) + 1;
        }         
    }
    else if(row_idx == SET_CHG_CURRENT)
    {
        const uint16_t* option_ids = get_current_setting(ttCharge,g_current_layout);
        length = (option_ids[1] - option_ids[0]) / 100 + 1;    
    }
    else if(row_idx == SET_DISCHG_CURRENT)
    {
        const uint16_t* option_ids = get_current_setting(ttDischarge,g_current_layout);
        length = (option_ids[1] - option_ids[0]) / 100 + 1;    
    }
    else if(row_idx == SET_CYC)
    {
        const int32_t* option_ids = tasksetting_display_rows[row_idx].sub_texts;
        while (option_ids[length] != -1) {
            length++;
        }  
    }
    
    // printf("sublist total count, type = %d, status = %d, length = %d\n", type, status, length);
    return length;
}


/**
 * @brief 创建单个设置行（包含分隔线、图标、标题、值、箭头）
 * @param parent 父容器指针
 * @param is_last 是否为最后一行（最后一行不显示右箭头，且值默认为空）
 */
setting_row_widgets_t create_setting_row(lv_obj_t* parent, bool is_last)
{
    setting_row_widgets_t widget;

    widget.line = lv_obj_create(parent);
    lv_obj_set_size(widget.line, 302, 1);
    lv_obj_add_style(widget.line, &bg_style, 0);
    lv_obj_set_style_bg_color(widget.line, COLOR_999999, 0);

    // 创建容器
    widget.row_cont = lv_obj_create(parent);
    lv_obj_set_size(widget.row_cont, lv_pct(100), 41);
    lv_obj_add_style(widget.row_cont, &bg_style, 0);
    lv_obj_add_style(widget.row_cont, &style_row_selected, LV_STATE_CHECKED); //添加选中状态的样式
    lv_obj_set_style_bg_color(widget.row_cont, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(widget.row_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(widget.row_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(widget.row_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

    lv_obj_add_flag(widget.row_cont, LV_OBJ_FLAG_CHECKABLE);               //允许选中 
    lv_obj_clear_flag(widget.row_cont, LV_OBJ_FLAG_CLICK_FOCUSABLE);    // 清除焦点事件，绑定点击回调


    // 图像
    widget.title_img = lv_image_create(widget.row_cont);
    lv_image_set_src(widget.title_img, &img_chemistry);
    lv_obj_set_style_image_recolor_opa(widget.title_img, LV_OPA_COVER, 0);
    lv_obj_set_style_image_recolor(widget.title_img, COLOR_333333, 0);
    // lv_obj_set_style_translate_x(widget.title_img, -8, 0);

    // 标题
    char buf[16];
    int random_row = arc4random() % 100;  // 假设范围 0~99
    widget.title_label = lv_label_create(widget.row_cont);
    lv_snprintf(buf, sizeof(buf), "title label %d", random_row);
    lv_label_set_text(widget.title_label, buf);
    lv_obj_set_style_text_color(widget.title_label, COLOR_333333, 0);
    lv_obj_set_style_text_font(widget.title_label, &font_isdtyahei_18, 0);
    // lv_obj_set_style_translate_y(widget.title_label, 1, 0);
    // lv_obj_set_style_translate_x(widget.title_label, 8, 0);
    lv_obj_set_flex_grow(widget.title_label, 1);


    if (!is_last) 
    {
        widget.value_label = lv_label_create(widget.row_cont);
        lv_label_set_text(widget.value_label, "--");
        lv_obj_set_style_text_font(widget.value_label, &font_isdtyahei_18, 0);
        lv_label_set_long_mode(widget.value_label, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
        lv_obj_set_style_translate_x(widget.value_label, -8, 0);
        // 注意：值标签不设置 flex_grow，保持内容宽度

        widget.value_img = lv_image_create(widget.row_cont);
        lv_image_set_src(widget.value_img, &img_right_arrow);
        lv_obj_set_style_translate_x(widget.value_img, -8, 0);
    }
    
    return widget;
}


/**
 * @brief 创建子列表
 */
static lv_obj_t* create_sublist(int row_i){
    int length = get_sub_items_count(row_i);                                //子列表长度
    int current_selected = row_i == -1 ? 0 : get_task_list_idx(row_i);      //选中的子项索引

    printf("fill row(%d) sublist, length = %d, current child item selected idx = %d \n", row_i, length, current_selected);

    lv_obj_t *sub_container = lv_obj_create(setting_layout);
    lv_obj_set_size(sub_container, 128, lv_pct(100));
    lv_obj_add_style(sub_container, &bg_style, 0);
    lv_obj_set_style_bg_color(sub_container, COLOR_FFFFFF, 0);
    lv_obj_set_style_clip_corner(sub_container, true, 0); // 启用圆角裁剪
    lv_obj_set_style_radius(sub_container, 8, 0);
    lv_obj_align(sub_container, LV_ALIGN_TOP_RIGHT, 0, 0);
    lv_obj_set_scrollbar_mode(sub_container, LV_SCROLLBAR_MODE_OFF);
    lv_obj_move_to_index(sub_container, -1);

    // Flex 列布局，子项顶部对齐
    lv_obj_set_style_pad_row(sub_container, 0, 0);
    lv_obj_set_flex_flow(sub_container, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(
        sub_container,                         
        LV_FLEX_ALIGN_START,              // 主轴(COLUMN)：子项从容器顶部开始排列
        LV_FLEX_ALIGN_CENTER,             // 交叉轴(ROW): 子项整体在容器内垂直居中   
        LV_FLEX_ALIGN_CENTER              // 交叉轴(ROW): 子项整体在容器内垂直居中     
    );

    for (int child_i = 0; child_i < length; child_i++) {
        setting_row_widgets_t child_btn = create_setting_row(sub_container, false);
        lv_obj_add_flag(child_btn.title_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child_btn.value_img, LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(child_btn.value_label, LV_OBJ_FLAG_HIDDEN);
        if (child_i == 0)
        {
            lv_obj_add_flag(child_btn.line, LV_OBJ_FLAG_HIDDEN);
        }
 
        //高亮已选中索引
        if (child_i == current_selected && child_btn.row_cont)
        { 
            lv_obj_add_state(child_btn.row_cont, LV_STATE_CHECKED);
            lv_obj_scroll_to_view(child_btn.row_cont, LV_ANIM_OFF);                  //滚动定位
        }

        lv_obj_set_user_data(child_btn.row_cont, (void*)(intptr_t)child_i);          //保存每个子项的对应索引
        lv_obj_add_event_cb(child_btn.row_cont, child_click_cb, LV_EVENT_ALL, NULL);

        update_single_item_value(row_i, child_btn.title_label, child_i);

    }

    return sub_container;
}


/**
 * @brief 任务设置父项行的点击事件
 */
static void row_click_event_cb(lv_event_t *e)
{
    lv_obj_t* parent_btn = lv_event_get_target(e);
    task_settingrow_type_t row_i = (task_settingrow_type_t)(intptr_t)lv_obj_get_user_data(parent_btn);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED)
    {
        // printf("current_selected_taskrow_idx = %d, row_i = %d\n", current_selected_taskrow_idx, row_i);
        // ---------- 1. 点击的是当前展开的行 → 删除子列表 ----------
        if (current_selected_taskrow_idx == row_i) {
            lv_obj_t* sub = setting_sublist[current_selected_taskrow_idx];
            if (sub) {
                lv_obj_del(sub);
                setting_sublist[current_selected_taskrow_idx] = NULL;  // 立即置空，防止悬空
            }
            // 清除父按钮的选中样式
            lv_obj_clear_state(parent_btn, LV_STATE_CHECKED);
            current_selected_taskrow_idx = -1;
            return;
        }

        // ---------- 2. 点击的是新行 → 先删除旧行 ----------
        if (current_selected_taskrow_idx != -1) {
            // 清除旧父按钮样式
            lv_obj_t* old_parent = setting_mainlist[current_selected_taskrow_idx].row_cont;
            if (old_parent) {
                lv_obj_remove_state(old_parent, LV_STATE_CHECKED);
            }

            // 删除旧的子容器
            lv_obj_t* old_sub = setting_sublist[current_selected_taskrow_idx];
            if (old_sub) {
                lv_obj_del(old_sub);
                setting_sublist[current_selected_taskrow_idx] = NULL;
            }
        }

        // ---------- 3. 创建并展开新行 ----------
        // 获取当前行的子容器（理论上应为 NULL，因为旧行已被删除）
        lv_obj_t* new_sub = setting_sublist[row_i];

        // 若意外非空（比如异步删除尚未完成），先强制删除再重建（但通常不会发生）
        if (new_sub) {
            lv_obj_del(new_sub);
            setting_sublist[row_i] = NULL;
            new_sub = NULL;
        }

        // 创建新的子容器
        new_sub = create_sublist(row_i);
        if (!new_sub) {
            printf("create_sublist failed\n");
            return;
        }
        setting_sublist[row_i] = new_sub;


        // 设置父按钮选中样式
        lv_obj_add_state(parent_btn, LV_STATE_CHECKED);
        current_selected_taskrow_idx = row_i;
    }
}


static void back_event_cb(lv_event_t *e)
{
    lv_obj_t* obj = lv_event_get_target(e); 
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        printf("back last page, last bar idx = %d \n", current_selected_bar_idx);
        // bar_page_create();
        on_language_changed();
    }

}

/**
 * @brief 创建任务设置页面
 */
void setting_page_create(){
    if (setting_layout != NULL && lv_obj_is_valid(setting_layout)) {
        printf("setting_layout != NULL && lv_obj_is_valid\n");
        return;
    }

    if (setting_layout) {
        printf("setting_layout != NULL\n");
        lv_obj_del(setting_layout);
        setting_layout = NULL;
    }

    setting_layout = lv_obj_create(lv_scr_act());
    lv_obj_set_size(setting_layout, 320, 240);
    lv_obj_set_y(setting_layout, 0);          // 垂直位置固定
    lv_obj_set_x(setting_layout, 0);  // 初始：屏幕左侧外部
    lv_obj_add_style(setting_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(setting_layout, COLOR_F9F9F9, LV_PART_MAIN); 
    lv_obj_remove_flag(setting_layout, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(setting_layout, LV_SCROLLBAR_MODE_OFF);

    // 文本行容器（可滚动）: 父容器使用 Flex 布局支持滚动
    task_setting_text_cont = lv_obj_create(setting_layout);
    lv_obj_set_size(task_setting_text_cont, lv_pct(100), 201);  //可视高度 240-1-38=201, 实现子对象总高度小于可视高度，仍可上下滚动
    lv_obj_align(task_setting_text_cont, LV_ALIGN_TOP_LEFT, 0, 41);
    lv_obj_add_style(task_setting_text_cont, &bg_style, 0);
    lv_obj_set_style_bg_color(task_setting_text_cont, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(task_setting_text_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_scrollbar_mode(task_setting_text_cont, LV_SCROLLBAR_MODE_OFF); 

    
    // Flex 列布局，子项顶部对齐
    lv_obj_set_style_pad_row(task_setting_text_cont, 0, 0);
    lv_obj_set_flex_flow(task_setting_text_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align(
        task_setting_text_cont,                         
        LV_FLEX_ALIGN_START,              // 主轴(COLUMN)：子项从容器顶部开始排列
        LV_FLEX_ALIGN_CENTER,             // 交叉轴(ROW): 子项整体在容器内垂直居中   
        LV_FLEX_ALIGN_CENTER              // 交叉轴(ROW): 子项整体在容器内垂直居中     
    );

    // 循环创建
    for (int row_i = 0; row_i < MAX_SETTING_ROW_COUNT; row_i++) {
        if (row_i == 0)
        {     
            setting_mainlist[0] = create_setting_row(setting_layout, true);
            lv_obj_add_flag(setting_mainlist[0].line, LV_OBJ_FLAG_HIDDEN);
            lv_obj_remove_flag(setting_mainlist[0].row_cont, LV_OBJ_FLAG_CHECKABLE);  
            lv_obj_remove_style(setting_mainlist[0].row_cont, &style_row_selected, LV_STATE_CHECKED);

            lv_obj_t* img_cont = lv_obj_create(setting_layout);
            lv_obj_set_size(img_cont, 41, 41);
            lv_obj_add_style(img_cont, &bg_style, 0);
            lv_obj_set_style_bg_opa(img_cont, LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_color(img_cont, COLOR_CLEAR, 0);
            lv_obj_align(img_cont, LV_ALIGN_TOP_LEFT, 0, 0);
            lv_obj_add_event_cb(img_cont, back_event_cb, LV_EVENT_ALL, NULL);

        }
        else
        {
            bool is_last = (row_i == MAX_SETTING_ROW_COUNT - 1);
            setting_mainlist[row_i] = create_setting_row(task_setting_text_cont, is_last);
            
            lv_obj_set_user_data(setting_mainlist[row_i].row_cont, (void *)(intptr_t)row_i);
            lv_obj_add_event_cb(setting_mainlist[row_i].row_cont, row_click_event_cb, LV_EVENT_ALL, NULL);
        }
        // 更新图像、标题
        const lv_image_dsc_t* icon = tasksetting_display_rows[row_i].main_img;
        const char* main_display = GetLanguageString(tasksetting_display_rows[row_i].main_text);

        lv_img_set_src(setting_mainlist[row_i].title_img, icon);
        lv_label_set_text(setting_mainlist[row_i].title_label, main_display);

    }

    refresh_setting_row_visibility();

}


/**********************
 *   系统设置
 **********************/

 /**
 * @brief 切换语言
 */
static void on_language_changed(){

    // if (current_lang ==  GetCurrentLanguage()) return; // 语言没变，不处理

    SetCurrentLanguage(GetCurrentLanguage() == LANGUAGE_CN ? LANGUAGE_EN : LANGUAGE_CN);
    current_lang = GetCurrentLanguage();

    printf("current_lang= %d, GetCurrentLanguage()= %d\n", current_lang, GetCurrentLanguage());

    int battI = GET_BATT_INDEX(current_selected_batt_comp_idx, current_selected_bar_idx);
    for (int row = 0; row < MAX_SETTING_ROW_COUNT; row++) {
        //标题
        const char* main_display = GetLanguageString(tasksetting_display_rows[row].main_text);
        lv_label_set_text(setting_mainlist[row].title_label, main_display);
        
        // value 
        if (row == 1 || row == 2 || row == 3 || row == 5 || row == 6 || row == 7)       //1-电池类型, 2-任务, 3-电流, 5-充电电流, 6-放电电流, 7-循环次数
        {
            int item_idx = g_batt_data[battI].setting_items[row - 1];                   //0-电池类型, 1-任务, 2-电流, 4-充电电流, 5-放电电流, 6-循环次数
            if (row == 6)
            {
                item_idx = 100;     //放电电流
            }else if (row == 7)
            {
                item_idx = 5;       //循环次数
            }
            update_setting_row_value(row, item_idx);
        }
    }
  
}



/**********************
 *   信息页面
 **********************/


/**
* @brief 销毁图表
* @param batt_idx 电池索引
*/
static void destroy_info_chart(int batt_idx) {
    // 直接判断指针，不再依赖容易出错的标志位
    if (info_chart_timer != NULL) {
        printf(">>> battI(%d) lv_timer_del\n", batt_idx);
        
        // 释放定时器绑定的 user_data 内存（防止内存泄漏）
        if (info_chart_timer->user_data != NULL) {
            lv_free(info_chart_timer->user_data);
            info_chart_timer->user_data = NULL;
        }
        
        lv_timer_del(info_chart_timer);
        info_chart_timer = NULL;
        if (g_info_charts[g_current_layout])
            lv_obj_add_flag(g_info_charts[g_current_layout], LV_OBJ_FLAG_HIDDEN);
    }
}

/**
* @brief 更新图表线条颜色
* @param chart 当前图表
* @param new_color 图表颜色
*/
static void update_chart_color(lv_obj_t* chart, lv_color_t new_color){

    if(line_chart_ser != NULL) lv_chart_remove_series(chart,line_chart_ser);
    line_chart_ser = lv_chart_add_series(chart, new_color, LV_CHART_AXIS_PRIMARY_Y);

    // 通过 API 获取坐标并局部刷新（仅更新图表区域）
    lv_area_t coords;
    lv_obj_get_coords(chart, &coords);
    lv_obj_invalidate_area(chart, &coords);
}  

/**
* @brief Timer callback function, 嵌入式图表定时器事件
*/
static void chart_timer_cb(lv_timer_t* timer) {

    // 从 timer->user_data 中取出你的结构体指针
    chart_timer_data_t *data = (chart_timer_data_t *)timer->user_data;
    if (data == NULL){
        lv_timer_del(info_chart_timer); // 防御性清理
        printf(">>> Invalid chart object!\n");
        return;
    } 

    //正常使用 chart 和 battI
    lv_obj_t *chart = data->chart;
    int current_battI = data->battI;

    uint32_t index = GetChannelSampleCnt(current_battI);

    if (index >= LINE_CHART_DISPLAY_POINT)	index = LINE_CHART_DISPLAY_POINT;
    eTaskState_t state = g_batt_data[current_battI].TaskState;

    if(state != tsTrickleChging && state != tsDischargDone)
    {
        int16_t sign = 1;
        if (state >= tsCurrentClimbDischging && state <= tsDischargDone)		
            sign = -1;

        for (uint32_t i = 0; i < index; i++) {
            g_batt_data[current_battI].line_chart_data[i] = (uint16_t)GetChannelPointData(current_battI, i, LINE_CHART_DISPLAY_POINT, sign);
        }
        lv_chart_refresh(chart);
    }
}


/**
* @brief 管理并启动图表定时器, 注意销毁上一个图表,可调用 destroy_info_chart()
* @param battI 电池索引
*/
static void manage_chart_timer(int battI) {
    // 1. 如果已有定时器在运行，先安全销毁（lv_timer_del 会自动处理 user_data 的释放逻辑，如果配置了的话，或者我们手动释放）
    if (info_chart_timer != NULL) {
        // 释放之前定时器绑定的内存
        if (info_chart_timer->user_data != NULL) {
            lv_free(info_chart_timer->user_data);
        }
        lv_timer_del(info_chart_timer);
        info_chart_timer = NULL;
    }

    // 2. 检查工作状态，决定是否启动新定时器
    if (g_info_charts[g_current_layout] && (battI < 0 || battI >= MAX_BATT_TOTAL_COUNT || g_batt_data[battI].BatteryLoad == 0 || g_batt_data[battI].TaskState == tsStandby)) {
        lv_obj_add_flag(g_info_charts[g_current_layout], LV_OBJ_FLAG_HIDDEN);
        return;
    }

    // 3. 显示图表并创建新定时器
    chart_timer_data_t *timer_data = (chart_timer_data_t *)lv_malloc(sizeof(chart_timer_data_t));
    if (timer_data != NULL) {
        timer_data->chart = g_info_charts[g_current_layout];
        timer_data->battI = battI;

        printf(">>> start battI(%d) info_chart_timer, workstatus=%d\n", battI, g_batt_data[battI].work_status);
        lv_chart_set_point_count(g_info_charts[g_current_layout], LINE_CHART_DISPLAY_POINT); 
        update_chart_color(g_info_charts[g_current_layout], get_batt_workstatus_color(g_batt_data[battI].work_status));
        lv_chart_set_ext_y_array(g_info_charts[g_current_layout], line_chart_ser,g_batt_data[battI].line_chart_data);
        lv_chart_set_update_mode(g_info_charts[g_current_layout], LV_CHART_UPDATE_MODE_SHIFT);
        lv_chart_refresh(g_info_charts[g_current_layout]);
        info_chart_timer = lv_timer_create(chart_timer_cb, 500, timer_data);
        lv_obj_clear_flag(g_info_charts[g_current_layout], LV_OBJ_FLAG_HIDDEN);
    }
}

/**
* @brief 将数据值转换为屏幕坐标
* @param chart 当前图表
* @param x_idx X值
* @param y_val Y值
* @param out 屏幕坐标
*/
static void chart_to_screen(lv_obj_t *chart, int32_t x_idx, int32_t y_val, lv_point_precise_t *out) {
    lv_area_t chart_area;
    lv_obj_get_coords(chart, &chart_area);
    
    int32_t y_min, y_max;
    y_min = chart_y_min;
    y_max = chart_y_max;
    
    int32_t w = lv_obj_get_width(chart);
    int32_t h = lv_obj_get_height(chart);
    uint16_t point_cnt = lv_chart_get_point_count(chart);
    
    /* X坐标：等间距分布 */
    out->x = chart_area.x1 + (x_idx * w) / (point_cnt - 1);
    /* Y坐标：数值映射到屏幕坐标（顶部最小，底部最大） */
    out->y = chart_area.y1 + h - ((y_val - y_min) * h) / (y_max - y_min);
}

/**
* @brief 嵌入式图表绘制事件
*/
static void chart_draw_event_cb(lv_event_t * e)
{
    lv_obj_t *chart = lv_event_get_target(e);
    lv_layer_t *layer = lv_event_get_layer(e);
    
    /* 获取图表坐标 */
    lv_area_t chart_area;
    lv_obj_get_coords(chart, &chart_area);
    
    /* 获取数据系列 */
    lv_chart_series_t *ser = lv_chart_get_series_next(chart, NULL);
    if (!ser) return;
    
    /* 获取数据数组（适用于 lv_chart_set_ext_y_array 绑定的情况） */
    int32_t *data = lv_chart_get_y_array(chart, ser);
    if (!data) return;
    
    uint16_t point_cnt = lv_chart_get_point_count(chart);
    if (point_cnt < 2) return;
    
    /* 获取Y轴范围 */
    int32_t y_min = chart_y_min;
    int32_t y_max = chart_y_max;
    if (y_max == y_min) return;
    
    /* 获取线条颜色 */
    lv_color_t ser_color = lv_chart_get_series_color(chart, ser);
    
    /* ---- 使用 lv_draw_triangle + lv_draw_rect 替代 lv_draw_polygon ---- */
    /* 缓存上一个有效点的坐标 */
    lv_point_precise_t prev_point;
    bool has_prev = false;
    
    for (int i = 0; i < point_cnt; i++) {
        int32_t val = data[i];
        /**********************
         * 实现： 初始数据为 0 时，不绘制曲线(以具体库为准,不同 lvgl 库所用的图表文件可能不一样)
         * lv_chart.c 文件修改 static void draw_series_line(lv_obj_t * obj, lv_layer_t * layer)
         * for(i = 0; i < chart->point_cnt; i++) { ... 
         *          if(ser->y_points[p_act] == LV_CHART_POINT_NONE) {
                        p_y = LV_DRAW_LINE_POINT_NONE;
                    }
                    else if (ser->y_points[p_act] == INT32_MIN)         //新增
                    {
                        p_y = INT32_MIN;
                    }   
                    else {
                        int32_t v = ser->y_points[p_act];
                        int32_t min_v = chart->ymin[ser->y_axis_sec];
                        int32_t max_v = chart->ymax[ser->y_axis_sec];
                        p_y = (int32_t)lv_map(v, min_v, max_v, y_ofs + h, y_ofs);
                    }
            ...
        **********************/
        /* 跳过无效数据（INT32_MIN 表示无数据） */
        if (val == INT32_MIN || val < y_min || val > y_max || val == 0) {
            has_prev = false;
            continue;
        }
        
        /* 将数据值转换为屏幕坐标 */
        lv_point_precise_t current_point;
        chart_to_screen(chart, i, val, &current_point);
        
        if (has_prev) {
            /* 1. 准备三角形绘制描述符 */
            lv_draw_triangle_dsc_t tri_dsc;
            lv_draw_triangle_dsc_init(&tri_dsc);
            
            /* 设置三个顶点坐标 */
            tri_dsc.p[0] = prev_point;
            tri_dsc.p[1] = current_point;
            /* 第三个点取较低点的垂直投影 */
            if (prev_point.y > current_point.y) {
                tri_dsc.p[2].x = prev_point.x;
                tri_dsc.p[2].y = prev_point.y;
            } else {
                tri_dsc.p[2].x = current_point.x;
                tri_dsc.p[2].y = current_point.y;
            }
            tri_dsc.bg_grad.dir = LV_GRAD_DIR_VER;
            
            /* 设置填充属性 */
            tri_dsc.bg_grad.stops[0].color = ser_color;
            tri_dsc.bg_grad.stops[0].opa = 100;//(lv_opa_t)(255 - fract_upper);
            tri_dsc.bg_grad.stops[0].frac = 255;//0;
            tri_dsc.bg_grad.stops[1].color = ser_color;
            tri_dsc.bg_grad.stops[1].opa = 100;//(lv_opa_t)(255 - fract_lower);
            tri_dsc.bg_grad.stops[1].frac = 255;
            
            /* 2. 绘制三角形 */
            lv_draw_triangle(layer, &tri_dsc);
            
            /* 3. 矩形部分（使用独立描述符） */
            lv_draw_rect_dsc_t rect_dsc;
            lv_draw_rect_dsc_init(&rect_dsc);
            rect_dsc.bg_grad.dir = LV_GRAD_DIR_VER;
            rect_dsc.bg_grad.stops[0].color = ser_color;
            rect_dsc.bg_grad.stops[0].frac = 255;//0;
            rect_dsc.bg_grad.stops[0].opa = 100;//(lv_opa_t)(255 - fract_lower);
            rect_dsc.bg_grad.stops[1].color = ser_color;
            rect_dsc.bg_grad.stops[1].frac = 255;
            rect_dsc.bg_grad.stops[1].opa = 100;//0;
            
            lv_area_t rect_area;
            rect_area.x1 = LV_MIN(prev_point.x, current_point.x);
            rect_area.x2 = LV_MAX(prev_point.x, current_point.x);
            rect_area.y1 = LV_MAX(prev_point.y, current_point.y);
            rect_area.y2 = chart_area.y2;
            
            lv_draw_rect(layer, &rect_dsc, &rect_area);
        }
        
        prev_point = current_point;
        has_prev = true;
    }

}

/**
* @brief 创建嵌入式图表
* @param parent 图表父级
* @param chart_max_h 图表高度
* @param chart_max_w 图表宽度
*/
lv_obj_t* embedded_chart_create(lv_obj_t* parent, int chart_max_h, int chart_max_w) {
	// Force update of layout so that the parent object size can be obtained
    lv_obj_update_layout(parent); 

    // Create a chart
    lv_obj_t * line_chart = lv_chart_create(parent);
    if(!line_chart) {
       return NULL;
    }

	// Set range for Y axis
    lv_chart_set_range(line_chart, LV_CHART_AXIS_PRIMARY_Y, chart_y_min, chart_y_max); 

	// Hardware acceleration configuration
    lv_obj_add_flag(line_chart, LV_OBJ_FLAG_ADV_HITTEST);
		
	// Style configuration
    lv_chart_set_type(line_chart, LV_CHART_TYPE_LINE);	
    lv_obj_set_size(line_chart, chart_max_w, chart_max_h);
    lv_obj_align(line_chart, LV_ALIGN_CENTER, 0, 0);
    lv_obj_set_style_bg_opa(line_chart, LV_OPA_TRANSP, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_INDICATOR);
    lv_obj_set_style_border_width(line_chart, 0, LV_PART_MAIN);
    lv_obj_set_style_line_width(line_chart, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_pad_all(line_chart, 0, 0);
    lv_obj_set_style_size(line_chart, 0, 0, LV_PART_INDICATOR);
    lv_obj_set_style_line_rounded(line_chart, true, LV_PART_ITEMS);

    // Add event callback and Passing color parameters
    lv_obj_add_event_cb(line_chart, chart_draw_event_cb, LV_EVENT_DRAW_MAIN_END, NULL);
    lv_obj_add_flag(line_chart, LV_OBJ_FLAG_HIDDEN);
    
    return line_chart;
}

/**
* @brief 静态图表渐变阴影绘制事件
*/
static void add_faded_area(lv_event_t * e)
{
    lv_obj_t * obj = lv_event_get_target(e);
    lv_area_t coords;
    lv_obj_get_coords(obj, &coords);

    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = lv_draw_task_get_draw_dsc(draw_task);

    const lv_chart_series_t * ser = lv_chart_get_series_next(obj, NULL);
    lv_color_t ser_color = lv_chart_get_series_color(obj, ser);

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
    tri_dsc.bg_grad.dir = LV_GRAD_DIR_VER;

    int32_t full_h = lv_obj_get_height(obj);
    int32_t fract_uppter = (int32_t)(LV_MIN(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - coords.y1) * 255 / full_h;
    int32_t fract_lower = (int32_t)(LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - coords.y1) * 255 / full_h;
    tri_dsc.bg_grad.stops[0].color = ser_color;
    tri_dsc.bg_grad.stops[0].opa = 255 - fract_uppter;
    tri_dsc.bg_grad.stops[0].frac = 0;
    tri_dsc.bg_grad.stops[1].color = ser_color;
    tri_dsc.bg_grad.stops[1].opa = 255 - fract_lower;
    tri_dsc.bg_grad.stops[1].frac = 255;

    lv_draw_triangle(base_dsc->layer, &tri_dsc);

    /*Draw rectangle below the triangle*/
    lv_draw_rect_dsc_t rect_dsc;
    lv_draw_rect_dsc_init(&rect_dsc);
    rect_dsc.bg_grad.dir = LV_GRAD_DIR_VER;
    rect_dsc.bg_grad.stops[0].color = ser_color;
    rect_dsc.bg_grad.stops[0].frac = 0;
    rect_dsc.bg_grad.stops[0].opa = 255 - fract_lower;
    rect_dsc.bg_grad.stops[1].color = ser_color;
    rect_dsc.bg_grad.stops[1].frac = 255;
    rect_dsc.bg_grad.stops[1].opa = 0;

    lv_area_t rect_area;
    rect_area.x1 = (int32_t)draw_line_dsc->p1.x;
    rect_area.x2 = (int32_t)draw_line_dsc->p2.x - 1;
    rect_area.y1 = (int32_t)LV_MAX(draw_line_dsc->p1.y, draw_line_dsc->p2.y) - 1;
    rect_area.y2 = (int32_t)coords.y2;
    lv_draw_rect(base_dsc->layer, &rect_dsc, &rect_area);
}

/**
* @brief 静态图表绘制事件
*/
static void draw_event_cb(lv_event_t * e)
{
    lv_draw_task_t * draw_task = lv_event_get_draw_task(e);
    lv_draw_dsc_base_t * base_dsc = lv_draw_task_get_draw_dsc(draw_task);

    if(base_dsc->part == LV_PART_ITEMS && lv_draw_task_get_type(draw_task) == LV_DRAW_TASK_TYPE_LINE) {
        add_faded_area(e);

    }
    // /*Hook the division lines too*/
    // if(base_dsc->part == LV_PART_MAIN && lv_draw_task_get_type(draw_task) == LV_DRAW_TASK_TYPE_LINE) {
    //     hook_division_lines(e);
    // }

}

/**
 * @brief 创建信息页面
 */
static void info_page_create(int new_bar_idx){
    if (info_layout) {
        printf("info_layout != NULL\n");
        lv_obj_del(info_layout);
        info_layout = NULL;
    }

    static const lv_coord_t text_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // 3列等宽
    static const lv_coord_t text_row_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // 5行等高


    info_layout = lv_obj_create(lv_scr_act()); 
    lv_obj_add_style(info_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(info_layout, COLOR_00FF00, LV_PART_MAIN);
    lv_obj_set_style_radius(info_layout, 10, 0);

    if (g_current_layout == LAYOUT_VER) {
        lv_obj_set_size(info_layout, lv_pct(100), 200);
        lv_obj_align(info_layout, LV_ALIGN_BOTTOM_MID, 0, 0);
    }else{
        lv_obj_set_size(info_layout, 280, lv_pct(100));
        lv_obj_align(info_layout, LV_ALIGN_LEFT_MID, 0, 0);
    }

    //创建 grid 布局： 3*5
    lv_obj_t* info_text_cont = lv_obj_create(info_layout); 
    lv_obj_set_size(info_text_cont, lv_pct(100), 120); 
    lv_obj_add_style(info_text_cont, &bg_style, 0);
    lv_obj_align(info_text_cont, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_set_style_bg_color(info_text_cont, lv_color_hex(0x00b7e5), 0); 
    //  lv_obj_set_style_bg_color(info_text_cont, COLOR_CLEAR, 0);
    // lv_obj_set_style_bg_opa(info_text_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_grid_dsc_array(info_text_cont, text_col_dsc, text_row_dsc); 
    
    for(int row_idx=0; row_idx<5;row_idx++){
        // 创建每行的三个控件（图标、名称、值）
        lv_obj_t *icon = lv_image_create(info_text_cont);  
        lv_obj_t *name = lv_label_create(info_text_cont);
        lv_obj_t *value = lv_label_create(info_text_cont);
        
        lv_image_set_src(icon, &img_batt_charge);
        lv_obj_set_style_image_recolor_opa(icon, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(icon, COLOR_FFFFFF, 0);

        lv_obj_set_style_text_color(name, COLOR_FFFFFF, 0);
        lv_label_set_text(name, "--");
        lv_obj_set_style_text_font(name, &font_isdtyahei_18, 0);

        lv_obj_set_style_text_color(value, COLOR_FFFFFF, 0);
        lv_label_set_text(value, "--");
        lv_obj_set_style_text_font(value, &font_isdtyahei_18, 0);

        // 设置它们在 Grid 中的位置（列索引 0,1,2）
        lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_STRETCH, 0, 1, LV_GRID_ALIGN_STRETCH, row_idx , 1);
        lv_obj_set_grid_cell(name, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, row_idx, 1);
        lv_obj_set_grid_cell(value, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, row_idx, 1);
        
        // 保存到全局数组
        g_info_rows[g_current_layout][row_idx].info_icon = icon;
        g_info_rows[g_current_layout][row_idx].info_name = name;
        g_info_rows[g_current_layout][row_idx].info_value = value;
    }

    //创建图表容器
    lv_obj_t* temporary_placeholder = lv_obj_create(info_layout);
    lv_obj_set_size(temporary_placeholder, lv_pct(100), 50);
    lv_obj_align_to(temporary_placeholder, info_text_cont, LV_ALIGN_OUT_BOTTOM_MID , 0, 15);
    lv_obj_add_style(temporary_placeholder, &bg_style, 0);
    lv_obj_set_style_bg_color(temporary_placeholder, COLOR_CCCCCC, 0);
    lv_obj_set_style_bg_opa(temporary_placeholder, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(temporary_placeholder, 1, 0);
    lv_obj_set_style_border_opa(temporary_placeholder, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(temporary_placeholder, COLOR_666666, 0);

    lv_obj_t *label = lv_label_create(temporary_placeholder);
    lv_label_set_text(label, "chart");
    lv_obj_center(label);


    // /** 嵌入式动态图表,以具体库为准,不同 lvgl 库所用的图表文件可能不一样 **/
    // g_info_charts[g_current_layout] = embedded_chart_create(temporary_placeholder, chart_max_h, chart_max_w);
    // line_chart_ser = lv_chart_add_series(g_info_charts[g_current_layout], lv_color_hex(0x00b7e5), LV_CHART_AXIS_PRIMARY_Y);
    
    // /** 静态示例图表 lv_example_chart_5.c ,以具体库为准,不同 lvgl 库所用的图表文件可能不一样 **/
    // lv_obj_t * chart = lv_chart_create(lv_screen_active());
    // lv_chart_set_type(chart, LV_CHART_TYPE_LINE);   /*Show lines and points too*/
    // lv_obj_set_size(chart, 200, 150);
    // lv_obj_set_style_pad_all(chart, 0, 0);
    // lv_obj_set_style_radius(chart, 0, 0);
    // lv_obj_center(chart);

    // lv_chart_set_div_line_count(chart, 5, 7);
    // g_info_charts[g_current_layout] = chart;

    // lv_obj_add_event_cb(chart, draw_event_cb, LV_EVENT_DRAW_TASK_ADDED, NULL);
    // lv_obj_add_flag(chart, LV_OBJ_FLAG_SEND_DRAW_TASK_EVENTS);

    // lv_chart_series_t * ser = lv_chart_add_series(chart, lv_palette_main(LV_PALETTE_RED), LV_CHART_AXIS_PRIMARY_Y);

    // uint32_t i;
    // for(i = 0; i < 10; i++) {
    //     lv_chart_set_next_value(chart, ser, lv_rand(10, 80));
    // }

}

/**********************
 *   进度条页面
 **********************/


void task_setting_click_event_cb(lv_event_t* e){
    printf("first create task_setting_layout\n");
    setting_page_create();
}

/**
* @brief 进度条点击事件
*/
void bar_cont_click_event_cb(lv_event_t* e) {
    lv_obj_t* cont = lv_event_get_target(e);
    int bar_idx = (int)(intptr_t)lv_obj_get_user_data(cont);
    // int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    printf("layout(0-纵向,1-横向)=%d, bar_idx=%d, current_selected_bar_idx=%d\n", g_current_layout, bar_idx, current_selected_bar_idx);

    if (info_layout && current_selected_bar_idx == bar_idx) {
        // printf("double click, info_layout != NULL, will delete info_layout\n");
        lv_obj_add_flag(bar_conts[g_current_layout][bar_idx].info_arrow, LV_OBJ_FLAG_HIDDEN);

        lv_obj_del(info_layout);
        info_layout = NULL;
    }else if (info_layout && current_selected_bar_idx != bar_idx){
        // printf("switch bar, info_layout != NULL, will translate arrow and update value\n");

        //隐藏旧箭头
        lv_obj_add_flag(bar_conts[g_current_layout][current_selected_bar_idx].info_arrow, LV_OBJ_FLAG_HIDDEN);
        //显示新箭头
        lv_obj_remove_flag(bar_conts[g_current_layout][bar_idx].info_arrow, LV_OBJ_FLAG_HIDDEN);

    }else {
        // printf("first create info_layout\n");
        lv_obj_remove_flag(bar_conts[g_current_layout][bar_idx].info_arrow, LV_OBJ_FLAG_HIDDEN);
        
        info_page_create(bar_idx);

    }

    current_selected_bar_idx = bar_idx;

}

/**
* @brief 创建进度条页面
*/
static void bar_page_create(){

    if (bar_layout) {
        printf("bar_layout != NULL\n");
        lv_obj_del(bar_layout);
        bar_layout = NULL;
    }

    bar_layout = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bar_layout, lv_pct(100), lv_pct(100));
    lv_obj_align(bar_layout, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(bar_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(bar_layout, COLOR_000000, LV_PART_MAIN);

    int idx_max = g_current_layout == LAYOUT_VER ? 4 : 2;
    for(int idx = 0; idx < idx_max; idx++){
        // 创建进度条容器
        bar_conts[g_current_layout][idx].cont = lv_obj_create(bar_layout);
        lv_obj_add_style(bar_conts[g_current_layout][idx].cont, &bg_style, 0);
        lv_obj_set_style_bg_color(bar_conts[g_current_layout][idx].cont, COLOR_000000, 0);
        lv_obj_set_user_data(bar_conts[g_current_layout][idx].cont, (void*)(intptr_t)idx);
        lv_obj_add_event_cb(bar_conts[g_current_layout][idx].cont, bar_cont_click_event_cb, LV_EVENT_CLICKED, NULL);
    

        // 创建进度条
        bar_conts[g_current_layout][idx].bar = lv_bar_create(bar_conts[g_current_layout][idx].cont);
        lv_obj_set_style_radius(bar_conts[g_current_layout][idx].bar, 17, 0);
        lv_obj_set_style_radius(bar_conts[g_current_layout][idx].bar, 17, LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(bar_conts[g_current_layout][idx].bar, COLOR_4D4D4D, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(bar_conts[g_current_layout][idx].bar, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_border_width(bar_conts[g_current_layout][idx].bar, 0, LV_PART_MAIN);
        lv_obj_set_style_bg_color(bar_conts[g_current_layout][idx].bar, lv_color_hex(0xff6600ff), LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(bar_conts[g_current_layout][idx].bar, LV_OPA_COVER, LV_PART_INDICATOR);
        lv_obj_remove_flag(bar_conts[g_current_layout][idx].bar, LV_OBJ_FLAG_CLICKABLE);
        
        //创建图像容器
        lv_obj_t* img_cont = lv_obj_create(bar_conts[g_current_layout][idx].cont);
         lv_obj_set_size(img_cont, lv_pct(100), 46);
        lv_obj_add_style(img_cont, &bg_style, 0);
        lv_obj_set_style_bg_color(img_cont, COLOR_4D4D4D, 0);
        lv_obj_set_style_radius(img_cont, 13, 0);
        // lv_obj_remove_flag(img_cont, LV_OBJ_FLAG_CLICKABLE);
        lv_obj_add_event_cb(img_cont, task_setting_click_event_cb, LV_EVENT_CLICKED, NULL);
    


        //创建工作状态图像
        bar_conts[g_current_layout][idx].workstatus_img = lv_image_create(img_cont);
        lv_img_set_src(bar_conts[g_current_layout][idx].workstatus_img, &img_batt_charge);
        lv_obj_center(bar_conts[g_current_layout][idx].workstatus_img);
        lv_obj_set_style_image_recolor_opa(bar_conts[g_current_layout][idx].workstatus_img, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(bar_conts[g_current_layout][idx].workstatus_img, COLOR_FFFFFF, 0);
    

        //创建一个标签容器
        lv_obj_t* label_cont = lv_obj_create(bar_conts[g_current_layout][idx].cont);
        lv_obj_set_size(label_cont, 79, 68); //自适应
        lv_obj_add_style(label_cont, &bg_style, 0);
        lv_obj_set_style_bg_color(label_cont, COLOR_CLEAR, 0);
        lv_obj_set_style_bg_opa(label_cont, LV_OPA_TRANSP, 0);
        lv_obj_set_style_text_align(label_cont, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_remove_flag(label_cont, LV_OBJ_FLAG_CLICKABLE); //label_cont 以及它的任何子对象都不会响应点击事件
        lv_obj_set_flex_flow(label_cont, LV_FLEX_FLOW_COLUMN);// 设置 Flex 列布局，主轴居中（垂直），交叉轴居中（水平）
        lv_obj_set_flex_align(label_cont, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);

        // 创建标签
        bar_conts[g_current_layout][idx].type_label = lv_label_create(label_cont);
        lv_obj_set_style_text_color(bar_conts[g_current_layout][idx].type_label, COLOR_FFFFFF, 0);
        lv_obj_set_style_text_font(bar_conts[g_current_layout][idx].type_label, (lv_font_t *)&font_isdtyahei_18, 0);
        lv_label_set_text(bar_conts[g_current_layout][idx].type_label, "NiMH");
        //自动换行。文本会在超过标签宽度时自动换到下一行，并自动增加标签的高度
        lv_label_set_long_mode(bar_conts[g_current_layout][idx].type_label, LV_LABEL_LONG_MODE_WRAP); 
        lv_obj_set_style_text_letter_space(bar_conts[g_current_layout][idx].type_label, -1.5, 0);// 将字符间距缩小 2 个像素


        //创建信息页面箭头
        bar_conts[g_current_layout][idx].info_arrow = lv_image_create(bar_conts[g_current_layout][idx].cont);
        lv_image_set_src(bar_conts[g_current_layout][idx].info_arrow, &img_right_arrow);
        lv_obj_set_style_image_recolor_opa(bar_conts[g_current_layout][idx].info_arrow, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(bar_conts[g_current_layout][idx].info_arrow, COLOR_FFFFFF, 0);
        lv_obj_add_flag(bar_conts[g_current_layout][idx].info_arrow, LV_OBJ_FLAG_HIDDEN);

        if (g_current_layout == LAYOUT_VER) {
            lv_obj_set_size(bar_conts[g_current_layout][idx].cont, 79, lv_pct(100));
            lv_obj_set_pos(bar_conts[g_current_layout][idx].cont, idx*79 + (idx < 1 ? idx : idx+1), 0);  // 设置X位置
            
            // 创建分割线
            if(idx < 3){
                lv_obj_t* center_line = lv_obj_create(bar_layout);
                lv_obj_set_size(center_line, 1, 209);
                lv_obj_add_style(center_line, &bg_style, 0);
                lv_obj_set_style_bg_color(center_line, COLOR_666666, 0);
                lv_obj_align(center_line, LV_ALIGN_TOP_LEFT, (idx+1)*80, 5);
            }

            lv_bar_set_orientation(bar_conts[g_current_layout][idx].bar, LV_BAR_ORIENTATION_VERTICAL);
            lv_obj_set_size(bar_conts[g_current_layout][idx].bar, 34, 126);
            lv_obj_align(bar_conts[g_current_layout][idx].bar, LV_ALIGN_TOP_MID, 0, 5);

           
            lv_obj_align(img_cont, LV_ALIGN_BOTTOM_MID, 0, -4);
                    
            // 将 label_cont 顶部对齐到进度条底侧外部,相距13个单位，垂直居中
            lv_obj_align_to(label_cont, bar_conts[g_current_layout][idx].bar, LV_ALIGN_OUT_BOTTOM_MID, 0, 13);


            lv_obj_align(bar_conts[g_current_layout][idx].info_arrow, LV_ALIGN_TOP_MID, 0, 20);
        
        }else{
            lv_obj_set_size(bar_conts[g_current_layout][idx].cont, lv_pct(100), 109);
            lv_obj_set_pos(bar_conts[g_current_layout][idx].cont, 0, idx*110);  // 设置Y位置

            // 创建分割线
            if(idx < 1){
                lv_obj_t* center_line = lv_obj_create(bar_layout);
                lv_obj_set_size(center_line, 310, 1);
                lv_obj_add_style(center_line, &bg_style, 0);
                lv_obj_set_style_bg_color(center_line, COLOR_666666, 0);
                lv_obj_align(center_line, LV_ALIGN_TOP_MID, 0, 109);
            }

            lv_bar_set_orientation(bar_conts[g_current_layout][idx].bar, LV_BAR_ORIENTATION_HORIZONTAL);
            lv_obj_set_size(bar_conts[g_current_layout][idx].bar, 227, 34);
            lv_obj_align(bar_conts[g_current_layout][idx].bar, LV_ALIGN_TOP_RIGHT, -6, 38);

            lv_obj_align(img_cont, LV_ALIGN_LEFT_MID, 4, 0);

            // 将 label_cont 右侧对齐到进度条左侧外部,相距16个单位，垂直居中
            lv_obj_align_to(label_cont, bar_conts[g_current_layout][idx].bar, LV_ALIGN_OUT_LEFT_MID, -8, 0);

            lv_obj_align(bar_conts[g_current_layout][idx].info_arrow, LV_ALIGN_RIGHT_MID, -20, 0);

        }
    }

}

/**********************
 *   待机页面
 **********************/


static void bottom_btn_event(lv_event_t* e) {
    lv_obj_t* current_obj = lv_event_get_target(e);
    int bottom_btn_idx = (int)(intptr_t)lv_obj_get_user_data(current_obj);
    lv_event_code_t code = lv_event_get_code(e);
    printf("bottom_btn_idx=%d\n", bottom_btn_idx);

}

static void show_bar_event(lv_event_t* e) {
    lv_obj_t* current_obj = lv_event_get_target(e);
    int batt_com_idx = (int)(intptr_t)lv_obj_get_user_data(current_obj);
    lv_event_code_t code = lv_event_get_code(e);
    printf("batt_com_idx=%d\n", batt_com_idx);

    current_selected_batt_comp_idx = batt_com_idx;
    bar_page_create();

}

/**
* @brief 创建待机页面
*/
void stanby_page_create(){
    const lv_color_t colors[] = {COLOR_FFFFFF, COLOR_CCCCCC, COLOR_4D4D4D, COLOR_F9F9F9};

    if (stanby_page_layout) {
        printf("stanby_page_layout != NULL\n");
        lv_obj_del(stanby_page_layout);
        stanby_page_layout = NULL;
    }


    // 定义网格列描述数组（所有行共用）：LV_GRID_TEMPLATE_LAST 是必须的结尾标记
    static const lv_coord_t col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // 2列等宽
    static const lv_coord_t row_dsc[] = {100, 100, LV_GRID_TEMPLATE_LAST}; // 2行等高
    static const lv_coord_t bottom_col_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // 4列等宽
    static const lv_coord_t bottom_row_dsc[] = {38, LV_GRID_TEMPLATE_LAST}; // 1行等高

    stanby_page_layout = lv_obj_create(lv_scr_act());
    lv_obj_set_size(stanby_page_layout, lv_pct(100), lv_pct(100)); 
    lv_obj_align(stanby_page_layout, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(stanby_page_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(stanby_page_layout, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(stanby_page_layout, LV_OPA_TRANSP, 0);

    //创建 grid 布局： 2*2
    lv_obj_t* batt_com_bg = lv_obj_create(stanby_page_layout);
    lv_obj_set_size(batt_com_bg, lv_pct(100), 201); 
    lv_obj_align(batt_com_bg, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(batt_com_bg, &bg_style, 0);
    lv_obj_set_style_bg_color(batt_com_bg, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(batt_com_bg, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_column(batt_com_bg, 1, 0);
    lv_obj_set_style_pad_row(batt_com_bg, 1, 0);
    lv_obj_set_grid_dsc_array(batt_com_bg, col_dsc, row_dsc);  
    
    for(int batt_com_idx=0;batt_com_idx<4;batt_com_idx++){
        lv_obj_t* battery_compartment = lv_obj_create(batt_com_bg);
        lv_obj_add_style(battery_compartment, &bg_style, 0);
        lv_obj_set_style_bg_color(battery_compartment, colors[batt_com_idx], 0);
        // lv_obj_set_style_bg_color(battery_compartment, COLOR_CLEAR, 0);
        // lv_obj_set_style_bg_opa(battery_compartment, LV_OPA_TRANSP, 0);

        // 不需要再设置单独的大小，由 grid 拉伸填充
        // 设置 grid 单元格位置，例如 i=0 -> 第0行第0列，i=1 -> 第0行第1列，i=2 -> 第1行第0列，i=3 -> 第1行第1列
        lv_obj_set_grid_cell(battery_compartment, LV_GRID_ALIGN_STRETCH, batt_com_idx % 2, 1, LV_GRID_ALIGN_STRETCH, batt_com_idx / 2, 1);
    
        lv_obj_set_user_data(battery_compartment, (void*)(intptr_t)batt_com_idx);//使用 intptr_t 保证可移植性
        lv_obj_add_event_cb(battery_compartment, show_bar_event, LV_EVENT_CLICKED, NULL);
    }
    

    lv_obj_t* bottom_line = lv_obj_create(stanby_page_layout);
    lv_obj_set_size(bottom_line, lv_pct(100), 1);
    lv_obj_add_style(bottom_line, &bg_style, 0);
    lv_obj_set_style_bg_color(bottom_line, COLOR_666666, 0);
    lv_obj_align(bottom_line, LV_ALIGN_BOTTOM_MID, 0, -38);

    //创建 grid 布局： 4*1
    lv_obj_t* bottom_btn_layout = lv_obj_create(stanby_page_layout);
    lv_obj_set_size(bottom_btn_layout, lv_pct(100), 38);
    lv_obj_add_style(bottom_btn_layout, &bg_style, 0);
    lv_obj_align(bottom_btn_layout, LV_ALIGN_BOTTOM_MID, 0, 0);
    lv_obj_set_style_bg_color(bottom_btn_layout, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(bottom_btn_layout, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_column(bottom_btn_layout, 1, 0);
    lv_obj_set_grid_dsc_array(bottom_btn_layout, bottom_col_dsc, bottom_row_dsc); 
    

    for(int bottom_btn_idx=0;bottom_btn_idx<MAX_BOTTOM_BTN_COUNT;bottom_btn_idx++){
        lv_obj_t* column_cont = lv_obj_create(bottom_btn_layout);
        lv_obj_add_style(column_cont, &bg_style, 0);
        lv_obj_set_style_bg_color(column_cont, colors[bottom_btn_idx], 0);
        // lv_obj_set_style_bg_color(column_cont, COLOR_CLEAR, 0);
        // lv_obj_set_style_bg_opa(column_cont, LV_OPA_TRANSP, 0);

         // 设置 grid 单元格位置，例如 i=0 -> 第0行第0列，i=1 -> 第0行第1列，i=2 -> 第0行第0列，i=3 -> 第0行第1列
        lv_obj_set_grid_cell(column_cont, LV_GRID_ALIGN_STRETCH, bottom_btn_idx, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

        lv_obj_set_user_data(column_cont, (void*)(intptr_t)bottom_btn_idx);
        lv_obj_add_event_cb(column_cont, bottom_btn_event, LV_EVENT_CLICKED, NULL);
    }


}

/**********************
 *   init
 **********************/

static void init_style(){
    lv_style_init(&bg_style);
    lv_style_set_bg_color(&bg_style, COLOR_000000);
    lv_style_set_border_opa(&bg_style, LV_OPA_TRANSP); //边框透明
    lv_style_set_border_width(&bg_style,0);
    lv_style_set_radius(&bg_style, 0);//圆角为0
    lv_style_set_pad_all(&bg_style, 0);//内边距为0
    lv_style_set_margin_all(&bg_style, 0);

    // 设置选中时的背景色
    lv_style_init(&style_row_selected);
    lv_style_set_bg_opa(&style_row_selected, LV_OPA_COVER);
    lv_style_set_bg_color(&style_row_selected, COLOR_00FF00);

    // 按下时的背景色
    lv_style_init(&style_row_pressed);
    lv_style_set_bg_opa(&style_row_pressed, LV_OPA_COVER);
    lv_style_set_bg_color(&style_row_pressed, COLOR_CCCCCC); 
    // 禁用按下时的缩放效果
    lv_style_set_transform_width(&style_row_pressed, 0);
    lv_style_set_transform_height(&style_row_pressed, 0);

    
    // 设置选中时的背景色
    lv_style_init(&style_selected);
    lv_style_set_bg_opa(&style_selected, LV_OPA_COVER);
    lv_style_set_bg_color(&style_selected, COLOR_00FF00);
    
    // 按下时的背景色
    lv_style_init(&style_pressed);
    lv_style_set_bg_opa(&style_pressed, LV_OPA_COVER);
    lv_style_set_bg_color(&style_pressed, COLOR_00FF00); 
    // 禁用按下时的缩放效果
    lv_style_set_transform_width(&style_pressed, 0);
    lv_style_set_transform_height(&style_pressed, 0);

}

void page_create_v2(void){
    init_style();
    lv_obj_add_style(lv_scr_act(), &bg_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(lv_scr_act(), COLOR_000000, LV_PART_MAIN);
    lv_obj_remove_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    
    // printf("Screen W:%ld, H:%ld\n", lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));

// info_page_create(0);
    // stanby_page_create();
    // bar_page_create();
    setting_page_create();

}
/**
 * @file page_main.c
 *
 */

#include "page_main.h"


static void bar_page_create();
static void on_language_changed();

/***********************  公共变量 **********************/
static uint8_t selected_settingrow_group_index = 1; //设置行组的索引
static bool btn_group_add_lock = 1;
static bool btn_group_activate = 0;

static uint8_t current_lang = LANGUAGE_EN;

static batt_data_t g_batt_data[MAX_BATT_TOTAL_COUNT];

/***********************  样式 **********************/
static lv_style_t bg_style;
static lv_style_t style_selected; // 创建一个静态的样式变量，用于定义选中状态的外观
static lv_style_t style_pressed; // 创建一个用于“按下状态”的样式，以覆盖默认的灰色
static lv_style_t style_row_pressed; //行选中状态的样式变量
static lv_style_t style_row_selected; //行“按下状态”的样式，以覆盖默认的灰色

static lv_style_t style_child_btn_selected; //按钮选中状态的样式变量

static lv_style_t style_text_selected;
static lv_style_t style_img_selected;
static lv_style_t style_menu_text_selected;
static lv_style_t style_menu_img_selected;


/**********************   进度条页面 **********************/

/*变量*/
static int current_selected_bar_idx = -1;     //当前选中的 bar 索引，默认未选中,0-3: 纵向电池，4-5：横向电池
static const int32_t battery_value_default[] = {STR_AUTO, STR_LIHV, STR_LILON, STR_LIFE, STR_NIZN, STR_NIMH, STR_1_5_LILON, 0};

/*控件*/
static lv_obj_t* bar_layout = NULL;   // 进度条总览容器
static batt_t bar_conts[LAYOUT_COUNT][4]; // 全局二维数组：第一维是布局类型，第二维是进度条索引 0-3: 纵向电池，0-1：横向电池


/**********************   信息页面 **********************/


/*变量*/
static info_row_t g_info_rows[LAYOUT_COUNT][5]; // 全局二维数组：第一维是布局类型，第二维是行索引
static const lv_image_dsc_t* info_imgs[5] = {&img_arrow_back, &img_info_per, &img_info_chart, &img_info_mOhm, &img_info_mwh};
static int32_t chart_y_max = 100;
static int32_t chart_y_min = 0;
static int32_t line_w = 2;
static int chart_max_h = 28;
static int chart_max_w = 192;

/*控件*/
static lv_obj_t* info_layout = NULL;
static lv_obj_t* g_info_charts[LAYOUT_COUNT];
static lv_chart_series_t* line_chart_ser = NULL;
static lv_timer_t *info_chart_timer = NULL;



/**********************  任务设置 **********************/

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
    {&img_arrow_back, NULL, STR_TASK, -1, {-1}},
    {&img_chemistry, NULL, STR_BATTERY, -1, {STR_AUTO, STR_LIHV, STR_LILON, STR_LIFE, STR_NIZN, STR_NIMH, STR_1_5_LILON, -1}},
    {&img_task_selection, NULL, STR_TASK_SELECTION, -1, {task_value[2][0], -1}},
    {&img_current_setting, NULL, STR_CURRENT, -1, {chg_current_value[0][0], -1}},
    {&img_condition, NULL, STR_CONDITION, -1, {chg_conditon_value[0][0], -1}},
    {&img_charge_current, NULL, STR_CHARGE_CURRENT, -1, {chg_current_value[0][0], -1}},
    {&img_discharge_current, NULL, STR_DISCHARGE_CURRENT, -1, {dischg_current_value[0][0], -1}},
    {&img_task_cycle, NULL, STR_CYCLES, -1, {5, 10, 15, 20, 25, 30, -1}},
    {&img_start, NULL, STR_START_TASK, -1, {-1}},
};//任务设置每行的显示内容,0-标题行



/*控件*/
lv_obj_t* setting_layout;
static lv_obj_t* task_setting_text_cont;                                    // 任务主列表的内容行容器
static setting_row_widgets_t setting_mainlist[MAX_SETTING_ROW_COUNT];       //父列表，内部每行由 create_setting_row 生成
static lv_obj_t* setting_sub_container[MAX_SETTING_ROW_COUNT] = {NULL};     //子 layout ，内部有多行, 每行由 create_setting_row 生成
static batt_t setting_bar_conts[LAYOUT_COUNT][4];                           // 全局二维数组：第一维是布局类型，第二维是进度条索引


/**********************  总览页面 **********************/


/*变量*/
static int current_selected_batt_comp_idx = 0;      //当前选中的电池仓索引
static int current_batt_com_cnt = 2;                //识别到的电池仓数量
// 定义网格列描述数组（所有行共用）：LV_GRID_TEMPLATE_LAST 是必须的结尾标记
static const lv_coord_t one_grid_dsc[] = {LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // 1行/列等高
static const lv_coord_t three_grid_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST};
static const lv_coord_t two_grid_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // 2行/列等高
static const lv_coord_t four_grid_dsc[] = {LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; // 4行/列等宽



/*控件*/
lv_obj_t* stanby_page_layout;
ui_data_t g_ui_data;
static const setting_row_display_t stanby_display_btns[] = 
{
    {&img_no_sound, &img_sound, STR_SOUND, STR_SOUND,{-1}},
    {&img_home_lock, &img_home_unlock, STR_LOCK, STR_UNLOCK, {-1}},
    {&img_pause, &img_play, STR_PAUSE, STR_START, {-1}},
    {&img_home_menu, &img_home_menu, STR_MENU, STR_MENU, {-1}}
};

static batt_components_t batt_coms[4];   //电池仓
static lv_image_dsc_t* batt_idx_imgs[6] = {&img_1, &img_2, &img_3, &img_4, &img_A, &img_B};             //电池容器的布局
static lv_obj_t* bottom_objs[5];


/**********************  菜单设置 **********************/

/*变量*/
static task_settingrow_type_t current_selected_menu_idx = 0; //当前所选设置行的索引,0-标题行
static int32_t menu_items_data[5]; //不含标题
static const setting_row_display_t menu_display_rows[] = 
{   
    {&img_arrow_back, NULL, STR_MENU, -1, {-1}},
    {&img_volume_high, NULL, STR_VOLUME, -1, {STR_OFF, STR_LOW, STR_MID, STR_HIG, -1}},
    {&img_brightness, NULL, STR_BRIGHTNESS, -1, {STR_OFF, STR_LOW, STR_MID, STR_HIG, -1}},
    {&img_theme, NULL, STR_LANGUAGE, -1, {STR_ENGLISH, STR_CHINESE, -1}},
    {&img_capacitylimit, NULL, STR_CAPACITY_LIMIT, -1, {STR_OFF, STR_ON, -1}},
    {&img_activate_chging, NULL, STR_ACTIVATE_CHARGING, -1, {STR_OFF, STR_ON, -1}},
};  //每行的显示内容,0-标题行



/*控件*/
static lv_style_t style_menu_fade_normal;     /* 默认状态：不透明 + 带过渡 */
static lv_style_t style_menu_transparent;     /* 自定义状态位 USER_1 状态：透明 */
static bool  menu_fade_inited = false;

static lv_style_t style_menu_slide_normal;      /* 默认：translate_x = 0 + transition */
static lv_style_t style_menu_slide_from_right;   /* USER_2：translate_x = -W */
static bool        menu_slide_from_right_inited = false;

static lv_style_t style_menu_slide_normal;      /* 默认：translate_x = 0 + transition */
static lv_style_t style_menu_slide_from_left;   /* USER_2：translate_x = -W */
static bool        menu_slide_from_left_inited = false;

static lv_style_t style_menu_slide_normal_y;      /* 默认：translate_x = 0 + transition */
static lv_style_t style_menu_slide_from_bottom;   /* USER_2：translate_x = -W */
static bool        menu_slide_from_bottom_inited = false;




lv_obj_t* menu_layout;
lv_obj_t* menu_text_cont;
static setting_row_widgets_t menu_mainlist[MAX_MENU_ROW_COUNT];         //父列表，内部每行由 create_setting_row 生成
static lv_obj_t* menu_sub_container[MAX_SETTING_ROW_COUNT] = {NULL};    //子 layout ，内部有多行, 每行由 create_setting_row 生成



/**********************
 *   common methods
 **********************/

 static int get_task_list_idx(int row);
int get_sub_items_count(int row_idx);
void stanby_page_create(void);
static void bar_page_create();
void task_setting_click_event_cb(lv_event_t* e);
static void menu_child_click_cb(lv_event_t *e);


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


/**
 * @brief 从左往右平移
 */
static void menu_slide_from_left_init_once(void)
{
    if (menu_slide_from_left_inited) return;
    menu_slide_from_left_inited = true;

    static const lv_style_prop_t props_x[] = { LV_STYLE_TRANSLATE_X, LV_STYLE_PROP_INV };
    static lv_style_transition_dsc_t trans_x;
    lv_style_transition_dsc_init(&trans_x,
                                 props_x,
                                 lv_anim_path_ease_out,  /* 缓出曲线；不想要可传 NULL */
                                 300, 0, NULL);

    lv_style_init(&style_menu_slide_normal);
    lv_style_set_translate_x(&style_menu_slide_normal, 0);
    lv_style_set_transition(&style_menu_slide_normal, &trans_x);

    lv_style_init(&style_menu_slide_from_left);
    lv_style_set_translate_x(&style_menu_slide_from_left, 320);    //屏幕宽度320

}

/**
 * @brief 从右往左平移
 */
static void menu_slide_from_right_init_once(void)
{
    if (menu_slide_from_right_inited) return;
    menu_slide_from_right_inited = true;

    static const lv_style_prop_t props_x[] = { LV_STYLE_TRANSLATE_X, LV_STYLE_PROP_INV };
    static lv_style_transition_dsc_t trans_x;
    lv_style_transition_dsc_init(&trans_x,
                                 props_x,
                                 lv_anim_path_ease_out,  /* 缓出曲线；不想要可传 NULL */
                                 300, 0, NULL);

    lv_style_init(&style_menu_slide_normal);
    lv_style_set_translate_x(&style_menu_slide_normal, 0);
    lv_style_set_transition(&style_menu_slide_normal, &trans_x);

    lv_style_init(&style_menu_slide_from_right);
    lv_style_set_translate_x(&style_menu_slide_from_right, lv_display_get_horizontal_resolution(NULL));    //屏幕宽度


}
/**
 * @brief 从下往上平移
 */
static void menu_slide_from_bottom_init_once(void)
{
    if (menu_slide_from_bottom_inited) return;
    menu_slide_from_bottom_inited = true;

    static const lv_style_prop_t props_y[] = { LV_STYLE_TRANSLATE_Y, LV_STYLE_PROP_INV };
    static lv_style_transition_dsc_t trans_y;
    lv_style_transition_dsc_init(&trans_y,
                                 props_y,
                                 lv_anim_path_ease_out,  /* 缓出曲线；不想要可传 NULL */
                                 300, 0, NULL);

    lv_style_init(&style_menu_slide_normal_y);
    lv_style_set_translate_x(&style_menu_slide_normal_y, 0);
    lv_style_set_transition(&style_menu_slide_normal_y, &trans_y);

    lv_style_init(&style_menu_slide_from_bottom);
    lv_style_set_translate_x(&style_menu_slide_from_bottom, 240);    //屏幕高度240
}

/* 靠 lv_style_transition_dsc_t (LVGL 就会为这个属性启动一次内部过渡，不需要写任何 lv_anim_t) + 状态切换驱动过渡:
1.自定义 USER_1 状态, 当 USER_1 状态存在时 LV_STYLE_OPA 等于 TRANSP  → 瞬间变透明（还没启动过渡）
2.自定义默认状态, 带 transition 描述, 当 USER_1 状态不存在时 LV_STYLE_OPA 等于 COVER → 自动淡入 */
static void menu_fade_init_once(void)
{
    if (menu_fade_inited) return;
    menu_fade_inited = true;

    /* 需要监视的属性列表：只监视 OPA */
    static const lv_style_prop_t props[] = { LV_STYLE_OPA, LV_STYLE_PROP_INV };

    /* transition 描述符必须 static/全局，不能放栈上 */
    static lv_style_transition_dsc_t trans;
    lv_style_transition_dsc_init(&trans,
                                 props,
                                 lv_anim_path_ease_out,  /* 缓出曲线；不想要可传 NULL */
                                 500,                    /* 时长 ms */
                                 0, NULL);


    /* 默认状态：不透明；当 OPA 属性发生变化时启动过渡 */
    lv_style_init(&style_menu_fade_normal);
    lv_style_set_opa(&style_menu_fade_normal, LV_OPA_COVER);
    lv_style_set_transition(&style_menu_fade_normal, &trans);

    /* USER_1 状态：透明，不带 transition（切换进去时不产生动画） */
    lv_style_init(&style_menu_transparent);
    lv_style_set_opa(&style_menu_transparent, LV_OPA_TRANSP);

}



// 时间换算
static inline void format_time(char* buf, uint32_t run_time) {
    const uint8_t hours = run_time / 3600;
    const uint8_t mins = (run_time / 60) % 60;
    const uint8_t secs = run_time % 60;
    lv_snprintf(buf, 32, "%02d:%02d:%02d", hours, mins, secs);
}

/**
 * @brief 获取任务条件
 * @param eBatteryType_t 电池类型
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
 * @brief 根据任务设定值找寻列表中的索引
 * @param int32_t 当前工作任务
 * @param option_ids 任务条件列表
 */
int find_task_ui_index_by_options(int32_t task_value, int32_t* option_ids){

    // 根据 task_value 查找对应的字符串ID
    int32_t target_str_id = -1;
    for (size_t i = 0; i < sizeof(task_value_to_strid) / sizeof(task_value_to_strid[0]); ++i) {
        if (task_value == task_value_to_strid[i].value) {
            target_str_id = task_value_to_strid[i].str_id;
            break;
        }
    }
    if (target_str_id == -1) {
        printf(">>> find_task_ui_index_by_options, unknown task value=%ld\n", task_value);
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
 * @brief 获取总览页面的工作状态图像
 * @param value 当前工作状态
 */
const lv_image_dsc_t* get_stanbybatt_workstatus_img(int32_t value){
    switch (value) {
        case ttCharge: return &img_small_charge;
        case ttDischarge: return &img_batt_discharge;
        case ttStorage: return &img_batt_storage; 
        case ttCycle: return &img_small_cycle;
        case ttAnalysis: return &img_batt_analyse; 
        case ttActivate: return &img_batt_activate; 
        case ttDestroy: return &img_batt_destory;
        default: return &img_small_none; 
    }
}

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
 * @brief 获取总览页面的进度条容器背景色
 * @param value 当前工作状态
 */
lv_color_t get_workstatus_bg_color(int32_t value){
    switch (value) {
        case ttCharge: return COLOR_331400; // 充电中
        // case ttDischarge: return COLOR_FF5599; // 放电
        // case ttStorage: return COLOR_5555FF; // 存储
        case ttCycle: return COLOR_00141A;// 循环
        // case ttAnalysis: return COLOR_9955FF; // 分析
        // case ttActivate: return COLOR_37C8AB; // 激活
        // case ttDestroy: return COLOR_FF5555; // 损坏
        default: return COLOR_262626; // 待机或未知状态
    }
}

/**
 * @brief 获取总览页面的进度条容器边框
 * @param value 当前工作状态
 */
lv_color_t get_workstatus_border_color(int32_t value){
    switch (value) {
        case ttCharge: return COLOR_993D00; // 充电中
        // case ttDischarge: return COLOR_FF5599; // 放电
        // case ttStorage: return COLOR_5555FF; // 存储
        case ttCycle: return COLOR_005266;// 循环
        // case ttAnalysis: return COLOR_9955FF; // 分析
        // case ttActivate: return COLOR_37C8AB; // 激活
        // case ttDestroy: return COLOR_FF5555; // 损坏
        default: return COLOR_4D4D4D; // 待机或未知状态
    }
}

/**
 * @brief 获取总览页面的进度条背景色
 * @param value 当前工作状态
 */
lv_color_t get_bar_bg_color(int32_t value){
    switch (value) {
        case ttCharge: return COLOR_4D1F00; // 充电中
        // case ttDischarge: return COLOR_FF5599; // 放电
        // case ttStorage: return COLOR_5555FF; // 存储
        case ttCycle: return COLOR_003D4D;// 循环
        // case ttAnalysis: return COLOR_9955FF; // 分析
        // case ttActivate: return COLOR_37C8AB; // 激活
        // case ttDestroy: return COLOR_FF5555; // 损坏
        default: return COLOR_262626; // 待机或未知状态
    }
}



/**
 * @brief 更新进度条值
 * @param batt_comp_idx 电池仓索引
 * @param bar_idx 进度条索引，0-3: 纵向电池，4-5：横向电池
 */
static void update_stanby_bar_value(int batt_comp_idx, int bar_idx)
{
    int batt_idx = GET_BATT_INDEX(batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[batt_comp_idx].batt_cont_layout;
    int32_t newvalue = data->value_percent;

    //  printf("->>> bar_layout = %p, stanby_page_layout = %p, setting_layout = %p\n", bar_layout,  stanby_page_layout, setting_layout);
    //  printf("->>> batt_comp_idx = %d, bar_idx = %d, BatteryLoad = %d \n", batt_comp_idx, bar_idx, data->BatteryLoad);

    if (setting_layout)
    {
        lv_obj_t* bar = setting_bar_conts[g_current_layout][g_current_layout == LAYOUT_VER ? bar_idx : bar_idx-4].bar;
        if (bar)
        {
            lv_obj_set_style_bg_color(bar, 
                data->BatteryLoad == 0 ? COLOR_1A1A1A : get_batt_workstatus_color(data->TaskState == tsStandby ? -1 : data->work_status),
                LV_PART_INDICATOR);
            lv_bar_set_value(bar, data->BatteryLoad == 0 ? 0 : data->value_percent, LV_ANIM_OFF);
        }

    }else if (bar_layout)
    {
        lv_obj_t* bar = bar_conts[g_current_layout][g_current_layout == LAYOUT_VER ? bar_idx : bar_idx-4].bar;
        if (bar)
        {
            lv_bar_set_value(bar, data->BatteryLoad == 0 ? 0 : newvalue, LV_ANIM_OFF);
        }

    }else if (stanby_page_layout)
    {
        lv_obj_t* bar = batt_coms[batt_comp_idx].batts[bar_idx].bar;
        if (bar) {
            lv_bar_set_value(bar, data->BatteryLoad == 0 ? 0 : newvalue, LV_ANIM_OFF);
        }
    } 

}


/**
 * @brief 更新类型文本
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 * @param is_info_page  true:信息页面,false:进度条页面
 */
void update_bar_batt_type_text(int bar_idx, bool is_info_page) {
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
   
    // printf("-》》》 batt_comp_idx = %d, bar page batt(%d) batt_type = %ld, data->BatteryLoad = %d\n", current_selected_batt_comp_idx,  batt_idx, batt_type, data->BatteryLoad);
    if (is_info_page)
    {
        info_row_t b = g_info_rows[g_current_layout][0];
        if (b.info_label_name) {
            //电池有装载有工作但识别不了类型、或者电池有装载没有工作，文本显示为“--”
            lv_label_set_text(b.info_label_name, data->BatteryLoad == 1 && data->batt_type < 255 ? GetLanguageString(battery_value_default[data->batt_type]) : "--"); 
        }
        if (b.info_label_value)
        {
            const int32_t* option_ids = get_task_setting(data->batt_type);
            char buf[32];
            int actual_ui_idx = find_task_ui_index_by_options(data->work_status, option_ids);
            char* work_staus = GetLanguageString(data->TaskState == tsStandby ? STR_STANDBY : option_ids[actual_ui_idx]);

            lv_snprintf(buf, sizeof(buf), " (%s)", work_staus);
            lv_label_set_text(b.info_label_value, buf);

        }
    }else{
         batt_t b = bar_conts[g_current_layout][g_current_layout == LAYOUT_VER ? bar_idx : bar_idx-4]; 
        if (b.type_label) {
            //电池有装载有工作但识别不了类型、或者电池有装载没有工作，文本显示为“--”
            lv_label_set_text(b.type_label, data->BatteryLoad == 1 && data->batt_type < 255 ? GetLanguageString(battery_value_default[data->batt_type]) : "--"); 
        }
    }

}


/**
 * @brief 更新电压电流文本
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 * @param is_info_page true:信息页面,false:进度条页面
 */
void update_bar_batt_volcur_text(int bar_idx, bool is_info_page) {
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    int32_t vol_mv = data->vol_mv;
    int32_t cur_ma = data->cur_ma;

    if (is_info_page)
    {
        char buf[32], buf2[32];
        lv_obj_t *label_name = g_info_rows[g_current_layout][2].info_label_name;
        if (label_name) {
            vol_mv += 5;
            vol_mv /= 10;
            snprintf(buf, sizeof(buf), "%ld.%02ldV", vol_mv/100, vol_mv%100); 
            lv_label_set_text(label_name, data->BatteryLoad == 0 ? "--" : buf); 
        }
        lv_obj_t *label_value = g_info_rows[g_current_layout][2].info_label_value;
        if (label_value) {
            if(cur_ma >= 0)
            {
                cur_ma += 5;
                cur_ma /= 10;        
                snprintf(buf2, sizeof(buf2), "%ld.%02ldA", cur_ma/100, cur_ma%100); 
            }
            else
            {
                cur_ma -= 5;
                cur_ma /= 10;
                cur_ma = -cur_ma;
                snprintf(buf2, sizeof(buf2), "-%ld.%02ldA", cur_ma/100, cur_ma%100); 
            }
            lv_label_set_text(label_value, data->BatteryLoad == 0 ? "--" : buf2); 
        }
    }else {
        batt_t b = bar_conts[g_current_layout][g_current_layout == LAYOUT_VER ? bar_idx : bar_idx-4]; 

        if (b.volcur_label) {
            char buf[32];
            //部分嵌入式环境 lv_snprintf 默认不启用浮点格式化支持,可改为 snprintf
            vol_mv += 50;
            vol_mv /= 100;
            if(cur_ma >= 0)
            {
                cur_ma += 50;
                cur_ma /= 100;
                snprintf(buf, sizeof(buf), "%ld.%01ldV/%ld.%01ldA", vol_mv/10, vol_mv%10, cur_ma/10, cur_ma%10);
            }
            else
            {
                cur_ma -= 50;
                cur_ma /= 100;
                cur_ma = -cur_ma;
                snprintf(buf, sizeof(buf), "%ld.%01ldV/-%ld.%01ldA", vol_mv/10, vol_mv%10, cur_ma/10, cur_ma%10);
            }
            
            lv_label_set_text(b.volcur_label, data->BatteryLoad == 0 || data->TaskState == tsStandby ? "--" : buf);
        }
    }

    
}

/**
 * @brief 更新进度条时间文本
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 * @param is_info_page true:信息页面,false:进度条页面
 */
void update_bar_batt_time_text(int bar_idx, bool is_info_page) {
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;

    
    if (is_info_page)
    {
        lv_obj_t *label_value = g_info_rows[g_current_layout][4].info_label_value;
        if (label_value) {
            char buf2[32];
            format_time(buf2, data->time_s);
            lv_label_set_text(label_value, data->BatteryLoad == 0 ? "--" : buf2); 
        }

    }else{
        batt_t b = bar_conts[g_current_layout][g_current_layout == LAYOUT_VER ? bar_idx : bar_idx-4]; 
        if (b.time_label) {
            char buf[32];
            format_time(buf, data->time_s);
            lv_label_set_text(b.time_label, data->BatteryLoad == 0 || data->TaskState == tsStandby ? "--" : buf);
        }
    }

    
}


/**
 * @brief 更新瓦时文本
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 */
void update_batt_wh_text(int bar_idx) {
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    uint32_t value_mWh = data->value_mWh;
    
    // 直接使用全局数组，根据当前布局
    lv_obj_t *label_name = g_info_rows[g_current_layout][4].info_label_name;
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
 * @brief 更新欧姆文本
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 */
void update_batt_ohms_text(int bar_idx) {
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    uint32_t value = data->value_Ohms;

    lv_obj_t *label_name = g_info_rows[g_current_layout][3].info_label_name;
    if (label_name) {
        char buf[32];
        snprintf(buf, sizeof(buf), "%ldmΩ", value); 
        if(data->BatteryLoad == 0 || value == 0)
            lv_label_set_text(label_name, "--"); 
        else
            lv_label_set_text(label_name, buf);
    }
}


/**
 * @brief 更新温度文本
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 */
void update_batt_temp_text(int bar_idx) {
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    int32_t value_tempc = data->value_tempc;
    int32_t value_tempf = data->value_tempf;

    // printf(">>> update_batt_temp_text, batt_index=%d, work_status=%d\n", batt_index, data->work_status);

    lv_obj_t *label_value = g_info_rows[g_current_layout][3].info_label_value;
    if (label_value) {
        char buf2[32];
        snprintf(buf2, sizeof(buf2), "%ld°C, %ld°F", value_tempc, value_tempf ); 
        lv_label_set_text(label_value, data->BatteryLoad == 0 ? "--" : buf2); 
    }

}


/**
 * @brief 更新毫安时文本
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 */
void update_batt_mah_text(int bar_idx) {
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    uint32_t value = data->value_mah;

    lv_obj_t *label_value = g_info_rows[g_current_layout][1].info_label_value;
    if (label_value) {
        char buf2[32];
        snprintf(buf2, sizeof(buf2), "%ldmAh", value);
        lv_label_set_text(label_value, data->BatteryLoad == 0 ? "--" : buf2); 
    }
}


/**
 * @brief 更新百分比文本
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 */
void update_batt_per_text(int bar_idx) {
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    uint8_t value = data->value_percent > 100 ? 100 : data->value_percent;

    lv_obj_t *label_value = g_info_rows[g_current_layout][1].info_label_name;
    if (label_value) {
        char buf2[32];
        snprintf(buf2, sizeof(buf2), "%d%%", value);
        lv_label_set_text(label_value, data->BatteryLoad == 0 ? "--" : buf2); 
    }
}

 /* 尺寸动画 */
static void ripple_size_cb(void *var, int32_t value)
{
    lv_obj_set_size((lv_obj_t *)var, value, value);
}

/* 背景透明度动画 */
static void ripple_opa_cb(void *var, int32_t value)
{
    lv_obj_set_style_bg_opa((lv_obj_t *)var, value, 0);
}

/* 播放结束回调：把波纹层藏起来 */
static void ripple_ready_cb(lv_anim_t *a)
{
    lv_obj_t *r = (lv_obj_t *)a->var;
    lv_obj_add_flag(r, LV_OBJ_FLAG_HIDDEN);
}

/**
 * @brief 启动波纹动画
 * @param from      起始尺寸 = 图标大小
 * @param to        结束尺寸
 * @param dur_ms    单圈扩散时长 ms 
 * @param delay_ms  每个波纹错开时间：形成一波接一波
 * @param opa_from  起始透明度
 * @param play_once 是否只播放一次
 */                        
static void ripple_start(lv_obj_t *r,
                         lv_coord_t from, lv_coord_t to,
                         uint32_t dur_ms, uint32_t delay_ms,
                         lv_opa_t opa_from,
                          bool play_once)
{
    
    lv_anim_delete(r, NULL);                     /* 清掉该对象上所有旧动画 */
    lv_obj_remove_flag(r, LV_OBJ_FLAG_HIDDEN);
    
    
    lv_anim_t a;
    /* --- 尺寸动画 --- */
    lv_anim_init(&a);
    lv_anim_set_var(&a, r);
    lv_anim_set_exec_cb(&a, ripple_size_cb);
    lv_anim_set_values(&a, from, to);
    lv_anim_set_time(&a, dur_ms);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_repeat_count(&a, play_once ? 1 : LV_ANIM_REPEAT_INFINITE);  //LV_ANIM_REPEAT_INFINITE 重复执行
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_start(&a);

    /* --- 透明度动画 --- */
    lv_anim_init(&a);
    lv_anim_set_var(&a, r);
    lv_anim_set_exec_cb(&a, ripple_opa_cb);
    lv_anim_set_values(&a, opa_from, LV_OPA_TRANSP);
    lv_anim_set_time(&a, dur_ms);
    lv_anim_set_delay(&a, delay_ms);
    lv_anim_set_repeat_count(&a, play_once ? 1 : LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    if (play_once){
        lv_anim_set_ready_cb(&a, ripple_ready_cb);   /* ★ 播完隐藏 */
    }
    lv_anim_start(&a);
}
/* 创建单个 ripple */
static lv_obj_t *ripple_create(lv_obj_t *parent, lv_color_t color)
{
    lv_obj_t *r = lv_obj_create(parent);
    lv_obj_remove_style_all(r);                                  /* 清掉默认样式 */
    lv_obj_set_size(r, 30, 30);                                  /* 初始同图标大小 */
    lv_obj_set_style_radius(r, LV_RADIUS_CIRCLE, 0);             /* 变圆 */
    lv_obj_set_style_bg_color(r, color, 0);
    lv_obj_set_style_bg_opa(r, LV_OPA_TRANSP, 0);                /* 动画里会设置 */
    lv_obj_set_style_border_width(r, 0, 0);
    lv_obj_remove_flag(r, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_remove_flag(r, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_center(r);                                            /* 居中：尺寸变化后会自动保持中心 */
    return r;
}
/* 停止某一根 bar 上的所有 ripple：删动画 + 隐藏 */
static void bar_ripple_stop(int layout, int ui_bar_i)
{
    for (int i = 0; i < RIPPLE_NUM; i++) {
        lv_obj_t *r = bar_conts[layout][ui_bar_i].ripples[i];
        lv_anim_delete(r, NULL);                 /* 尺寸 + 透明度两条一起删 */
        lv_obj_add_flag(r, LV_OBJ_FLAG_HIDDEN);  /* 关键：删完必须隐藏 */
    }
}

/* 重启某一根 bar 上的所有 ripple：先重置再启动 */
static void bar_ripple_restart(int layout, int ui_bar_i)
{
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, layout == LAYOUT_VER ? ui_bar_i : ui_bar_i+4);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];

    if (data->BatteryLoad == 0 || data->TaskState == tsStandby) return;

    for (int i = 0; i < RIPPLE_NUM; i++) {
        lv_obj_t *r = bar_conts[layout][ui_bar_i].ripples[i];

        lv_anim_delete(r, NULL);                 /*清旧动画 */

        /* 重置到初始尺寸/透明度，避免 delay 期间看到残留状态 */
        lv_obj_set_size(r, RIPPLE_FROM, RIPPLE_FROM);
        lv_obj_set_style_bg_color(r, get_batt_workstatus_color(data->work_status), 0);
                
        lv_obj_set_style_opa(r, LV_OPA_40, 0);   
        lv_obj_center(r);                        

        lv_obj_remove_flag(r, LV_OBJ_FLAG_HIDDEN); /*显示 */

        /* 错开启动，形成波纹一波接一波 */
        ripple_start(r,
                     RIPPLE_FROM, RIPPLE_TO,
                     RIPPLE_DUR, i * 200,
                     LV_OPA_40, false);
    }
}
/**
 * @brief 更新进度条页面的进度条、进度条边框颜色
 * @param g_current_layout  进度条页面布局
 * @param clicked_ui_idx    点击的进度条索引，0-3: 纵向电池，0-1：横向电池
 */        
void chang_barcont_bar_border_color(layout_type_t g_current_layout, int clicked_ui_idx){
    int bar_cnt = g_current_layout == LAYOUT_VER ? 4 : 6;
    int bar_init = g_current_layout == LAYOUT_VER ? 0 : 4;

    for (int idx = bar_init; idx < bar_cnt; idx ++)
    {
        int ui_i = g_current_layout == LAYOUT_VER ? idx : idx-4;;
        batt_t b = bar_conts[g_current_layout][ui_i]; 

        int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, idx);
        if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
        batt_data_t* old_data = &g_batt_data[batt_idx];

        if (b.bar_cont)
        {
            lv_obj_set_style_bg_color(b.bar_cont, old_data->BatteryLoad == 0 ? COLOR_1A1A1A : COLOR_262626, LV_PART_MAIN);
            if (info_layout)
            {
                
                lv_obj_set_style_border_color(b.bar_cont, old_data->BatteryLoad == 0 ? COLOR_404040 : COLOR_999999, LV_PART_MAIN);

            }else if (!info_layout){
                //当前点击的进度条边框
                lv_obj_set_style_border_color(b.bar_cont, clicked_ui_idx == ui_i ? COLOR_FFFFFF : COLOR_404040, LV_PART_MAIN);
            }
        }
        if (b.bar)
        {
            //其余进度条边框
            lv_obj_set_style_bg_color(b.bar, old_data->BatteryLoad == 0 ? COLOR_1A1A1A : COLOR_0D0D0D, LV_PART_MAIN);
            if (info_layout)
            {
                lv_obj_set_style_bg_color(b.bar, 
                    old_data->BatteryLoad == 0 ? COLOR_1A1A1A : get_batt_workstatus_color(old_data->TaskState == tsStandby ? -1 : old_data->work_status), 
                    LV_PART_INDICATOR);

            }else if (!info_layout){
                    lv_obj_set_style_bg_color(b.bar, 
                    old_data->BatteryLoad == 0 ? COLOR_1A1A1A : (clicked_ui_idx == ui_i ? get_batt_workstatus_color(old_data->work_status) : COLOR_999999), 
                    LV_PART_INDICATOR);
            }
        }

        if (!info_layout)
        {
            // printf("->>> bar_conts[%d][%d] will create info_layout, bar ripple stop\n", g_current_layout, ui_i);
            /* ★ 停掉所有 bar 的 ripple */
            bar_ripple_stop(g_current_layout, ui_i);
        }else if (info_layout)
        {
            // printf("->>> bar_conts[%d][%d] will delete info_layout, bar ripple restart\n", g_current_layout, ui_i);
            /*  恢复波纹动画 */
            bar_ripple_restart(g_current_layout, ui_i);
        }
                
        
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
 * @brief 设置控件的可见性（封装隐藏/显示标志操作）
 * @param obj 控件对象指针
 * @param visible true=显示, false=隐藏
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
                lv_obj_set_style_pad_bottom(task_setting_text_cont, 40, 0);
            } else if (task == ttDestroy) {     // 销毁
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_START);
                lv_obj_set_style_pad_bottom(task_setting_text_cont, 80, 0);//实现子对象总高度小于可视高度，仍可上下滚动
            }
            break;

        case btLi5V:   // 1.5V Lilon
            lv_obj_set_style_pad_bottom(task_setting_text_cont, 0, 0);
            if (task == ttCharge || task == ttDestroy) {
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_START);
                 lv_obj_set_style_pad_bottom(task_setting_text_cont, 80, 0);
            }
            break;

        default:   // 其他类型
            lv_obj_set_style_pad_bottom(task_setting_text_cont, 10, 0);
            if (task == ttCycle) {            // 循环
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_CONDITION); ADD_ROW(SET_CHG_CURRENT); ADD_ROW(SET_DISCHG_CURRENT); ADD_ROW(SET_CYC); ADD_ROW(SET_START);
            } else if (task == ttAnalysis || task == ttActivate) {     // 分析、激活
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_CONDITION); ADD_ROW(SET_CHG_CURRENT); ADD_ROW(SET_DISCHG_CURRENT); ADD_ROW(SET_START);
            }else if (task == ttDestroy) {     // 销毁
                ADD_ROW(SET_CHEMISTRY); ADD_ROW(SET_TASK); ADD_ROW(SET_START);
                 lv_obj_set_style_pad_bottom(task_setting_text_cont, 80, 0);//实现子对象总高度小于可视高度，仍可上下滚动
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
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
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


//取消选中的父项行背景
static void cancel_selected_setting_row(){

    if (current_selected_taskrow_idx == 0) return;
    lv_obj_set_style_bg_opa(setting_mainlist[current_selected_taskrow_idx].row_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_color(setting_mainlist[current_selected_taskrow_idx].row_cont, COLOR_CLEAR, 0);

    lv_obj_set_style_img_recolor(setting_mainlist[current_selected_taskrow_idx].title_img, COLOR_4D4D4D, 0);
    lv_obj_set_style_text_color(setting_mainlist[current_selected_taskrow_idx].title_label, COLOR_4D4D4D, 0);

}

/**
 * @brief  GUI 开发中, LVGL 的非阻塞定时器事件, 延时 100ms 再销毁子列表
 */
static void my_callback(lv_timer_t* timer)
{
    lv_obj_t* sub = setting_sub_container[current_selected_taskrow_idx];
    if (sub) {
        printf("after lv_refr_now, sub  = NULL \n");
        lv_obj_del(sub);
        setting_sub_container[current_selected_taskrow_idx] = NULL;
    }
    // 清除父按钮的选中样式
    cancel_selected_setting_row();
}

/**
 * @brief 子列表行点击事件
 */
static void child_click_cb(lv_event_t *e) {
     lv_obj_t* child_btn = lv_event_get_target(e);                   // 当前点击的子项行容器 row_cont 
    lv_obj_t* sub_container = lv_obj_get_parent(child_btn);         // 子容器 layout
    lv_event_code_t code = lv_event_get_code(e);

    if (code == LV_EVENT_CLICKED)
    {
        // 检查当前展开的父项行索引是否有效
        if (current_selected_taskrow_idx <= 0 || current_selected_taskrow_idx >= MAX_SETTING_ROW_COUNT) {
            printf("Invalid current_selected_taskrow_idx: %d\n", current_selected_taskrow_idx);
            return;
        }

        int current_selected_sub = get_task_list_idx(current_selected_taskrow_idx);     // 曾经选中的子项索引
        int clicked_sub_idx = (int)(intptr_t)lv_obj_get_user_data(child_btn);           // 当前点击的子项索引

        printf("row(%d) child list olddata = %d, newdata = %d\n", 
            current_selected_taskrow_idx, current_selected_sub, clicked_sub_idx);

        // ====== 1. 如果点击已选中子项 → 仅折叠（删除） ======
        if (current_selected_sub == clicked_sub_idx) {
            lv_delete_layout(&setting_sub_container[current_selected_taskrow_idx]);
            
            // 清除父按钮的选中样式
            cancel_selected_setting_row();
            return;
        }

        // ====== 2. 点击不同子项 → 切换选中 ======
        // 清除旧选中
        if (current_selected_sub >= 0) {
            lv_obj_t* last_obj = lv_obj_get_child(sub_container, current_selected_sub);
            if (last_obj && lv_obj_is_valid(last_obj)) {
                lv_obj_set_style_bg_opa(last_obj, LV_OPA_TRANSP, 0);
                lv_obj_set_style_text_color(lv_obj_get_child(last_obj, 0), COLOR_000000, 0);
                // printf(" cancel last_idx \n");
            }
        }

        // 设置新选中
        if (child_btn && lv_obj_is_valid(child_btn)) {
            lv_obj_set_style_bg_opa(child_btn, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(child_btn, COLOR_0066FF, 0);

            lv_obj_set_style_text_color(lv_obj_get_child(child_btn, 0), COLOR_FFFFFF, 0);
            // lv_obj_scroll_to_view(child_btn, LV_ANIM_OFF); //滚动定位
        }


        // 保存数据 & 更新父按钮显示值
        batt_data_t* data = get_current_batt_data();
        if (!data) return;
        int type = data->setting_items[0]; // 第0项是电池类型索引
        int status = data->setting_items[1];
        layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
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
        // printf(">>> setting row(%d) save new data = %ld\n", row, newdata);

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
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
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
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
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
    lv_obj_set_style_bg_color(widget.row_cont, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(widget.row_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_flex_flow(widget.row_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(widget.row_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_left(widget.row_cont, 0, 0); //title_img 与父级间距
    lv_obj_set_style_pad_right(widget.row_cont, 10, 0); 
    lv_obj_set_style_pad_column(widget.row_cont, 0, 0);

    lv_obj_add_flag(widget.row_cont, LV_OBJ_FLAG_CHECKABLE);               //允许选中 
    lv_obj_clear_flag(widget.row_cont, LV_OBJ_FLAG_CLICK_FOCUSABLE);    // 清除焦点事件，绑定点击回调

    // 图像
    widget.title_img = lv_image_create(widget.row_cont);
    lv_image_set_src(widget.title_img, &img_chemistry);
    lv_obj_set_style_image_recolor_opa(widget.title_img, LV_OPA_COVER, 0);
    lv_obj_set_style_image_recolor(widget.title_img, COLOR_333333, 0);
    lv_obj_set_style_margin_right(widget.title_img, 8, 0); //title_img 与 title_label 间距

    // 标题
    widget.title_label = lv_label_create(widget.row_cont);
    lv_obj_set_style_text_color(widget.title_label, COLOR_333333, 0);
    lv_obj_set_style_text_font(widget.title_label, &font_isdtyahei_18_2, 0);


    if (!is_last) 
    {
        lv_obj_set_flex_grow(widget.title_label, 1);    // 撑满剩余空间 → 右侧元素自动贴右

        widget.value_label = lv_label_create(widget.row_cont);
        lv_label_set_text(widget.value_label, "--");
        lv_obj_set_style_text_font(widget.value_label, &font_isdtyahei_18_2, 0);
        lv_obj_set_style_margin_right(widget.value_label, 9, 0);   // 与 value_img 间距
        // lv_label_set_long_mode(widget.value_label, LV_LABEL_LONG_MODE_SCROLL_CIRCULAR);
        // 注意：值标签不设置 flex_grow，保持内容宽度

        widget.value_img = lv_image_create(widget.row_cont);
        lv_image_set_src(widget.value_img, &img_right_arrow);
    }
    
    return widget;
}

static void anim_x_cb(void *obj, int32_t v)
{
    // printf("anim_x_cb called: v=%ld, obj=%p\n", v, obj);
    lv_obj_set_style_translate_x((lv_obj_t *)obj, v, 0);
}

/**
 * @brief 创建菜单页面子列表、任务设置子列表
 * @param row_i 父列表的点击行索引
 * @return 返回 子列表
 */
static lv_obj_t* add_item_list(int row_i)
{
    //  printf(">>> first build setting_item_list2\n");
    lv_obj_t* item_list;
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    int length = 0;                           //子列表长度
    int current_selected = 0;
    const int32_t* sub_option_ids = menu_display_rows[row_i].sub_texts;

    if (menu_layout)
    {
        item_list = menu_sub_container[row_i];
    }else if (setting_layout){
        item_list = setting_sub_container[row_i];
    }

    if (item_list != NULL && lv_obj_is_valid(item_list )) {
        printf("item_list != NULL && lv_obj_is_valid\n");
        return NULL;
    }

    if (item_list) {
        printf("item_list != NULL\n");
        lv_delete_layout(&item_list);
    }


    if (menu_layout)
    {
        if (row_i < 0 || row_i > MAX_MENU_ROW_COUNT) return NULL;
        while (sub_option_ids[length] != -1) {
            length++;
        }
        current_selected = row_i <= 0 ? 0 : menu_items_data[row_i-1];     //选中的子项索引

        item_list = lv_obj_create(menu_layout);
        lv_obj_set_size(item_list, 123, lv_pct(100)); 
        lv_obj_align(item_list, LV_ALIGN_TOP_RIGHT, 0, 0);

    }else if (setting_layout){
        length = get_sub_items_count(row_i);
        current_selected = row_i == -1 ? 0 : get_task_list_idx(row_i);      //选中的子项索引
        
        item_list = lv_obj_create(setting_layout);
        lv_obj_set_size(item_list, 123, g_current_layout == LAYOUT_VER ? 226 : 236); //实际高度228 : 实际高度236
        lv_obj_align(item_list, LV_ALIGN_TOP_RIGHT, g_current_layout == LAYOUT_VER ? 0 : -10, g_current_layout == LAYOUT_VER ? 10 : 0);
    }

    lv_obj_add_style(item_list, &bg_style, 0);
    lv_obj_set_style_bg_color(item_list, COLOR_D9D9D9, 0);
    lv_obj_set_style_border_width(item_list, 4, 0);
    lv_obj_set_style_border_color(item_list, COLOR_D9D9D9, 0);
    lv_obj_set_style_border_opa(item_list, LV_OPA_100, 0);
    lv_obj_set_style_clip_corner(item_list, true, 0); // 启用圆角裁剪
    lv_obj_set_style_radius(item_list, 10, 0);
    lv_obj_set_scrollbar_mode(item_list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_flex_flow(item_list, LV_FLEX_FLOW_COLUMN);    // Flex 列布局，子项 list_btn 顶部对齐
    lv_obj_set_flex_align(
        item_list,                         
        LV_FLEX_ALIGN_START,              // 主轴(COLUMN)：子项从容器顶部开始排列
        LV_FLEX_ALIGN_CENTER,             // 交叉轴(ROW): 子项整体在容器内垂直居中   
        LV_FLEX_ALIGN_CENTER              // 交叉轴(ROW): 子项整体在容器内垂直居中     
    );

    for (int child_i = 0; child_i < length; child_i ++) {
        lv_obj_t* btn = lv_list_add_button(item_list, NULL, "item");
        lv_obj_set_size(btn, lv_pct(100), 40);
        lv_obj_remove_style(btn, NULL, LV_STATE_PRESSED);
        lv_obj_add_style(btn, &bg_style, 0);
        lv_obj_set_style_radius(btn, 10, 0);
        lv_obj_set_flex_flow(btn, LV_FLEX_FLOW_ROW);         /* 让按钮内部的 label 在水平+垂直方向都居中 */
        lv_obj_set_flex_align(btn,
                            LV_FLEX_ALIGN_CENTER,   /* 主轴：水平居中 */
                            LV_FLEX_ALIGN_CENTER,   /* 交叉轴：垂直居中 */
                            LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_text_font(btn, &font_isdtyahei_18_2, 0);
        lv_obj_set_style_text_align(btn, LV_TEXT_ALIGN_CENTER, 0);
        lv_obj_set_user_data(btn, (void*)(intptr_t)child_i);

        if (child_i==current_selected) {
            lv_obj_set_style_bg_color(btn, COLOR_0066FF, 0);
            lv_obj_scroll_to_view(btn, LV_ANIM_OFF);
            if (child_i > 3 && setting_layout && g_current_layout == LAYOUT_VER)
            {
                lv_obj_scroll_by(item_list, 0, -8, LV_ANIM_OFF);   // 对齐底部
            }
            lv_obj_set_style_text_color(btn, COLOR_FFFFFF, 0);
            // lv_group_focus_obj(item_bg);
        }else{
            lv_obj_set_style_bg_color(btn, COLOR_D9D9D9, 0);
            lv_obj_set_style_text_color(btn, COLOR_000000, 0);
        }

        /* 取出 button 里的 label（list_add_button 里最后一个子对象一般是 label） */
        lv_obj_t* item_text = lv_obj_get_child(btn, -1);
        if (menu_layout)
        {
            lv_obj_add_event_cb(btn, menu_child_click_cb, LV_EVENT_ALL, NULL);

            if (item_text) {
                lv_label_set_text(item_text, GetLanguageString(sub_option_ids[child_i]));
            }
        }else if (setting_layout)
        {
            lv_obj_add_event_cb(btn, child_click_cb, LV_EVENT_ALL, NULL);
            
            if (item_text) {
                update_single_item_value(row_i, item_text, child_i);
            }
        }
    }

    /** lvgl 模拟器的 LV_STATE_USER_x 效果不明显, 嵌入式开发板可尝试 **/
    // if (setting_layout && g_current_layout == LAYOUT_HOR)
    // {
    //     // >>>  挂样式
    //     lv_obj_add_style(item_list, &style_menu_slide_normal, 0);
    //     lv_obj_add_style(item_list, &style_menu_slide_from_left, LV_STATE_USER_2);

    //     // >>> 3) 动态设定起始偏移 = 屏幕宽度（右侧滑入用 +W，左侧滑入用 -W）
    //     lv_coord_t w = lv_display_get_horizontal_resolution(NULL);
    //     lv_obj_set_style_translate_x(item_list, -w, LV_STATE_USER_2);  // 从左往右滑入

    //     // >>> 4) 进入 USER_2，对象先跳到左侧屏幕外
    //     lv_obj_add_state(item_list, LV_STATE_USER_2);
    // }else{
    //     // >>>  挂样式
    //     lv_obj_add_style(item_list, &style_menu_slide_normal_y, 0);
    //     lv_obj_add_style(item_list, &style_menu_slide_normal_y, LV_STATE_USER_3);

    //     // >>> 3) 动态设定起始偏移 = 屏幕高度（下侧滑入用 +h，上侧滑入用 -h）
    //     lv_coord_t h = lv_display_get_vertical_resolution(NULL);
    //     lv_obj_set_style_translate_y(item_list, h, LV_STATE_USER_3);  // 从下往上滑入

    //     // >>> 4) 进入 USER_2，对象先跳到下侧屏幕外
    //     lv_obj_add_state(item_list, LV_STATE_USER_3);
    // }
    // //同一帧内 add_state + clear_state，LVGL 检测不到属性变化,强制刷一帧，让 LVGL 记录 TRANSP
    // lv_refr_now(NULL);     

    // // >>> 清除透明状态 → 触发淡入
    // if (setting_layout && g_current_layout == LAYOUT_HOR)
    // {
    //     lv_obj_clear_state(item_list, LV_STATE_USER_2);
    // }else{
    //     lv_obj_clear_state(item_list, LV_STATE_USER_3);
    // }

    lv_coord_t w = lv_display_get_horizontal_resolution(NULL);
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, item_list);
    lv_anim_set_values(&a, w, 0);
    lv_anim_set_time(&a, row_i == 3 && setting_layout ? (g_current_layout == LAYOUT_HOR ? 600 : 350) : 300);        //ms,电流设置行有较多子项，加长动画时间体现平移效果
    lv_anim_set_path_cb(&a, lv_anim_path_ease_out);
    lv_anim_set_exec_cb(&a, anim_x_cb);
    lv_anim_start(&a);


    return item_list;
}


//增加选中的父项行背景
static void update_setting_row_bg(int new_row){

    cancel_selected_setting_row();

    lv_obj_set_style_bg_opa(setting_mainlist[new_row].row_cont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(setting_mainlist[new_row].row_cont, COLOR_808080, 0);

    lv_obj_set_style_img_recolor(setting_mainlist[new_row].title_img, COLOR_FFFFFF, 0);
    lv_obj_set_style_text_color(setting_mainlist[new_row].title_label, COLOR_FFFFFF, 0);

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
        if(row_i == MAX_SETTING_ROW_COUNT - 1){
            printf("start task\n");
            // on_language_changed();

            // lv_obj_send_event(lv_obj_get_child(setting_mainlist[0].row_cont, 0), LV_EVENT_CLICKED,NULL);

            return;

        }

        lv_obj_t* clicked_list = setting_sub_container[row_i];

        printf("current_selected_taskrow_idx = %d, row_i = %d\n", current_selected_taskrow_idx, row_i);
        // ---------- 1. 点击的是当前展开的行 → 删除子列表 ----------
        if (current_selected_taskrow_idx == row_i && clicked_list) {
            printf(">>> delete current_list\n");
            lv_delete_layout(&setting_sub_container[current_selected_taskrow_idx]);
            cancel_selected_setting_row();
            return;
        }
      
        // ---------- 2. 点击的是新行 → 先删除旧行 ----------
        // 点击了新行 -> 先隐藏旧行
        if (current_selected_taskrow_idx > 0 && current_selected_taskrow_idx < MAX_SETTING_ROW_COUNT) {
            lv_obj_t* old_list = setting_sub_container[current_selected_taskrow_idx];
            if (old_list && lv_obj_is_valid(old_list)) {
                printf(">>> delete old_list\n");
                // 如果已经可见，滑出并隐藏
                cancel_selected_setting_row();
                lv_delete_layout(&setting_sub_container[current_selected_taskrow_idx]);
                return;
            }
        }


        // ---------- 3. 创建并展开新行 ----------
        // 获取当前行的子容器
        if (!clicked_list) {
            printf(">>> first build setting_item_list\n");
            setting_sub_container[row_i] = add_item_list(row_i);
            clicked_list = setting_sub_container[row_i];
        }

        printf(">>> update current_selected_taskrow_idx\n");

        // 更新背景并显示新行
        update_setting_row_bg(row_i);
        current_selected_taskrow_idx = row_i;

    }
}


static void back_event_cb(lv_event_t *e)
{
   lv_obj_t* obj = lv_event_get_target(e); 
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        printf("->>> setting page back -> bar page, current_batt_comp_idx = %d,  current_bar_idx = %d\n", current_selected_batt_comp_idx, current_selected_bar_idx);
        
        // printf("%d, %p\n", current_selected_taskrow_idx, setting_sub_container[current_selected_taskrow_idx] );
        
        //显示子列表时退出，需要删除子列表
        if (setting_sub_container[current_selected_taskrow_idx]){
            lv_delete_layout(&setting_sub_container[current_selected_taskrow_idx]);
        }
        lv_delete_layout(&setting_layout);

        if (!bar_layout)
        {
            current_selected_bar_idx = -1;
            bar_page_create();
        }
    }
}

/**
 * @brief 创建任务设置页面
 */
void setting_page_create(int new_bar_idx){
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
    lv_obj_set_size(setting_layout, lv_pct(100), lv_pct(100));
    lv_obj_add_style(setting_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(setting_layout, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(setting_layout, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(setting_layout, 2, 0);
    lv_obj_remove_flag(setting_layout, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(setting_layout, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(setting_layout, LV_ALIGN_CENTER, 0, 0);
    //设置变换支点到右上角，想让支点在左上角，就改成 (-bg_w/2, -bg_h/2)；想右下角就 (bg_w/2, bg_h/2)
    lv_obj_set_style_transform_pivot_x(setting_layout,  320, 0);    // 向右 → 右边缘 bg_w / 2
    lv_obj_set_style_transform_pivot_y(setting_layout,  -240, 0);   // 向上 → 上边缘 -bg_h / 2


    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    int idx_max = g_current_layout == LAYOUT_VER ? 4 : 2;

    //避免 bar_row 透出底层对象
    lv_obj_t* obj = lv_obj_create(setting_layout);
    lv_obj_set_size(obj, 
        g_current_layout == LAYOUT_VER ? 316 : 40, 
        g_current_layout == LAYOUT_VER ? 40 : 236);
        lv_obj_add_style(obj, &bg_style, 0);
    lv_obj_set_style_bg_color(obj, COLOR_000000, 0);
    lv_obj_align(obj, g_current_layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_RIGHT_MID, 0, 0);

    lv_obj_t* bar_row = lv_obj_create(obj);
    lv_obj_set_size(bar_row, 
        g_current_layout == LAYOUT_VER ? 316 : 8, 
        g_current_layout == LAYOUT_VER ? 8 : 236);
    lv_obj_align(bar_row, g_current_layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_RIGHT_MID, 0, 0);
    lv_obj_add_style(bar_row, &bg_style, 0);
    lv_obj_set_style_bg_color(bar_row, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(bar_row, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_column(bar_row, g_current_layout == LAYOUT_VER ? 2 : 0, 0);
    lv_obj_set_style_pad_row(bar_row, g_current_layout == LAYOUT_VER ? 0 : 10, 0);
    lv_obj_set_style_pad_top(bar_row, g_current_layout == LAYOUT_VER ? 0 : 15, 0);
    lv_obj_set_style_pad_bottom(bar_row, g_current_layout == LAYOUT_VER ? 0 : 15, 0);
    lv_obj_set_grid_dsc_array(bar_row, 
        g_current_layout == LAYOUT_VER ? four_grid_dsc : one_grid_dsc, 
        g_current_layout == LAYOUT_VER ? one_grid_dsc : two_grid_dsc);

        
    for(int batt_idx = 0; batt_idx < idx_max; batt_idx++){
        batt_t item;

        item.bg = lv_obj_create(bar_row);
        lv_obj_add_style(item.bg, &bg_style, 0);
        lv_obj_set_style_bg_color(item.bg, COLOR_CLEAR, 0);
        lv_obj_set_style_bg_opa(item.bg, LV_OPA_TRANSP, 0);
        // 设置 grid 单元格位置，例如 i=0 -> 第0行第0列，i=1 -> 第0行第1列，i=2 -> 第0行第0列，i=3 -> 第0行第1列
        lv_obj_set_grid_cell(item.bg, 
            LV_GRID_ALIGN_STRETCH,  g_current_layout == LAYOUT_VER ? batt_idx : 0, 1, 
            LV_GRID_ALIGN_STRETCH,  g_current_layout == LAYOUT_VER ? 0 : batt_idx, 1);

        //进度条容器
        item.bar_cont = lv_bar_create(item.bg);
        lv_obj_add_style(item.bar_cont, &bg_style, 0);
        lv_obj_set_size(item.bar_cont, 
            g_current_layout == LAYOUT_VER ? lv_pct(100) : lv_pct(100), 
            g_current_layout == LAYOUT_VER ? lv_pct(100) : lv_pct(100));
        lv_obj_set_style_pad_all(item.bar_cont, 2, 0);
        lv_obj_set_style_radius(item.bar_cont, 4, 0);
        lv_obj_align(item.bar_cont, g_current_layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_bg_color(item.bar_cont, COLOR_262626, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(item.bar_cont, LV_OPA_COVER, LV_PART_MAIN);

        //进度条
        item.bar = lv_bar_create(item.bar_cont);
        lv_obj_set_size(item.bar, 
            g_current_layout == LAYOUT_VER ? 72 : 4, 
            g_current_layout == LAYOUT_VER ? 4 : 94);
        lv_obj_align(item.bar, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_bg_color(item.bar, COLOR_262626, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(item.bar, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_bg_color(item.bar, COLOR_262626, LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(item.bar, LV_OPA_COVER, LV_PART_INDICATOR);
        lv_obj_remove_flag(item.bar, LV_OBJ_FLAG_CLICKABLE);

        //保存到数据结构体
        setting_bar_conts[g_current_layout][batt_idx] = item;

        //更新进度条
        update_stanby_bar_value(current_selected_batt_comp_idx, g_current_layout == LAYOUT_VER ? batt_idx : batt_idx+4);
    }

    lv_obj_t* content_bg = lv_obj_create(setting_layout);
    lv_obj_add_style(content_bg, &bg_style, 0);
    lv_obj_set_style_bg_color(content_bg, COLOR_FFFFFF, 0); 
    lv_obj_set_scrollbar_mode(content_bg, LV_SCROLLBAR_MODE_OFF); 
    lv_obj_set_style_radius(content_bg, 10, 0);
    lv_obj_set_style_clip_corner(content_bg, true, 0);
    lv_obj_set_style_pad_row(content_bg, 0, 0);
    if (g_current_layout == LAYOUT_VER) {
        lv_obj_set_size(content_bg, 316, 226);                       
        lv_obj_align(content_bg, LV_ALIGN_BOTTOM_MID, 0, 0);

    }else{
        lv_obj_set_size(content_bg, 306, 236);                 
        lv_obj_align(content_bg, LV_ALIGN_LEFT_MID, 0, 0);
    }


    setting_mainlist[0].row_cont = lv_obj_create(content_bg);
    lv_obj_add_style(setting_mainlist[0].row_cont, &bg_style, 0);
    lv_obj_set_size(setting_mainlist[0].row_cont, lv_pct(100), 37);
    lv_obj_set_style_bg_color(setting_mainlist[0].row_cont, COLOR_F2F2F2, 0);
    lv_obj_set_style_bg_opa(setting_mainlist[0].row_cont, LV_OPA_100, 0);
    lv_obj_align(setting_mainlist[0].row_cont, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_flex_flow(setting_mainlist[0].row_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(setting_mainlist[0].row_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_left(setting_mainlist[0].row_cont, 7, 0);                  //title_img 与父级间距
    lv_obj_set_style_pad_right(setting_mainlist[0].row_cont, 35, 0); 
    lv_obj_clear_flag(setting_mainlist[0].row_cont, LV_OBJ_FLAG_CLICK_FOCUSABLE);    // 清除焦点事件，绑定点击回调

    // 图像
    setting_mainlist[0].title_img = lv_image_create(setting_mainlist[0].row_cont);
    lv_obj_set_style_image_recolor_opa(setting_mainlist[0].title_img, LV_OPA_COVER, 0);
    lv_img_set_src(setting_mainlist[0].title_img, tasksetting_display_rows[0].main_img);
    lv_obj_set_style_img_recolor(setting_mainlist[0].title_img, COLOR_808080, 0);
    lv_obj_set_style_translate_x(setting_mainlist[0].title_img, -7, 0);
    // lv_image_set_inner_align(setting_mainlist[0].title_img, LV_IMAGE_ALIGN_LEFT_MID);   //调整内容层：实际的图像像素，它在对象内部的显示位置
    //扩大点击范围
    lv_obj_set_style_pad_left(setting_mainlist[0].title_img, 15, 0);
    lv_obj_set_style_pad_right(setting_mainlist[0].title_img, 15, 0);
    lv_obj_set_style_pad_top(setting_mainlist[0].title_img, 12, 0);
    lv_obj_set_style_pad_bottom(setting_mainlist[0].title_img, 12, 0);

    lv_obj_add_flag(setting_mainlist[0].title_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(setting_mainlist[0].title_img, back_event_cb, LV_EVENT_ALL, NULL);


    // 标题
    setting_mainlist[0].title_label = lv_label_create(setting_mainlist[0].row_cont);
    lv_obj_set_style_text_color(setting_mainlist[0].title_label, COLOR_333333, 0);
    lv_obj_set_style_text_font(setting_mainlist[0].title_label, &font_isdtyahei_18_2, 0);
    lv_obj_set_style_text_align(setting_mainlist[0].title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_right(setting_mainlist[0].title_label, 23, 0);
    lv_label_set_text(setting_mainlist[0].title_label, GetLanguageString(tasksetting_display_rows[0].main_text));
    lv_obj_set_flex_grow(setting_mainlist[0].title_label, 1);


    // 文本行容器（可滚动）: 父容器使用 Flex 布局支持滚动
    task_setting_text_cont = lv_obj_create(content_bg);
    lv_obj_add_style(task_setting_text_cont, &bg_style, 0);
    lv_obj_set_style_bg_color(task_setting_text_cont, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(task_setting_text_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_scrollbar_mode(task_setting_text_cont, LV_SCROLLBAR_MODE_OFF); 
    lv_obj_set_style_clip_corner(task_setting_text_cont, true, 0);
    lv_obj_set_style_pad_row(task_setting_text_cont, 0, 0);
    lv_obj_set_flex_flow(task_setting_text_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align( task_setting_text_cont,  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(task_setting_text_cont, LV_ALIGN_TOP_MID, 0, 37);
    lv_obj_set_size(task_setting_text_cont, lv_pct(100), g_current_layout == LAYOUT_VER ? 189 : 199);  //可视高度=实际高度 - 标题行高度37

    // 循环创建
    for (int i = 1; i < MAX_SETTING_ROW_COUNT; i++) {
        setting_mainlist[i].row_cont = lv_obj_create(task_setting_text_cont);
        lv_obj_set_size(setting_mainlist[i].row_cont, lv_pct(100), 41); 
        lv_obj_add_style(setting_mainlist[i].row_cont, &bg_style, 0);
        lv_obj_set_style_bg_color(setting_mainlist[i].row_cont, COLOR_CLEAR, 0);
        lv_obj_set_style_bg_opa(setting_mainlist[i].row_cont, LV_OPA_TRANSP, 0);
        lv_obj_set_flex_flow(setting_mainlist[i].row_cont, LV_FLEX_FLOW_ROW);
        lv_obj_set_flex_align(setting_mainlist[i].row_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
        lv_obj_set_style_pad_left(setting_mainlist[i].row_cont, 9, 0);
        lv_obj_set_style_pad_right(setting_mainlist[i].row_cont, 9, 0);
        lv_obj_set_style_pad_column(setting_mainlist[i].row_cont, 0, 0);

        lv_obj_clear_flag(setting_mainlist[i].row_cont, LV_OBJ_FLAG_CLICK_FOCUSABLE); //滚动时不触发焦点事件 LV_EVENT_FOCUSED
        lv_obj_set_user_data(setting_mainlist[i].row_cont, (void*)(intptr_t)i);//使用 intptr_t 保证可移植性
        lv_obj_add_event_cb(setting_mainlist[i].row_cont, row_click_event_cb, LV_EVENT_ALL, NULL);

        // --- 图像 ---
        setting_mainlist[i].title_img = lv_image_create(setting_mainlist[i].row_cont);
        lv_image_set_src(setting_mainlist[i].title_img, tasksetting_display_rows[i].main_img);
        lv_obj_set_style_image_recolor_opa(setting_mainlist[i].title_img, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(setting_mainlist[i].title_img, COLOR_4D4D4D, 0);
        lv_obj_set_style_margin_right(setting_mainlist[i].title_img, 15, 0);

        // --- 文本标签 1 ---
        setting_mainlist[i].title_label = lv_label_create(setting_mainlist[i].row_cont);
        lv_obj_set_style_text_color(setting_mainlist[i].title_label, COLOR_4D4D4D, 0);
        lv_label_set_text(setting_mainlist[i].title_label, GetLanguageString(tasksetting_display_rows[i].main_text));
        lv_obj_set_style_text_font(setting_mainlist[i].title_label, &font_isdtyahei_18_2, 0);
        lv_obj_set_flex_grow(setting_mainlist[i].title_label, 1); // 撑满剩余空间 → 右侧元素自动贴右


        if(i != MAX_SETTING_ROW_COUNT-1){
            // // --- 文本标签 2 ---
            setting_mainlist[i].value_label = lv_label_create(setting_mainlist[i].row_cont);
            lv_obj_set_style_text_color(setting_mainlist[i].value_label, COLOR_000000, 0);
            lv_label_set_text(setting_mainlist[i].value_label, "--");
            lv_obj_set_style_text_font(setting_mainlist[i].value_label, &font_isdtyahei_18_2, 0);
            lv_obj_set_style_margin_right(setting_mainlist[i].value_label, 9, 0);  

            lv_obj_t* right_arrow = lv_image_create(setting_mainlist[i].row_cont);
            lv_image_set_src(right_arrow, &img_right_arrow);
            lv_obj_set_style_image_recolor_opa(right_arrow, LV_OPA_COVER, 0);
            lv_obj_set_style_image_recolor(right_arrow, COLOR_000000, 0);
        }

    }

    refresh_setting_row_visibility();

    if (bar_layout)
    {
        // printf("delete bar_layout\n");
        lv_delete_layout(&bar_layout);
    } 

    //bar page -> info page, 未删除 bar_layout
    if (info_layout)
    {
        // printf("delete info_layout\n");
        lv_delete_layout(&info_layout);
    }

}




/**********************
 *   菜单设置
 **********************/

void on_language_changed(){

    // if (current_lang ==  GetCurrentLanguage()) return; // 语言没变，不处理

    SetCurrentLanguage(GetCurrentLanguage() == LANGUAGE_CN ? LANGUAGE_EN : LANGUAGE_CN);
    current_lang = GetCurrentLanguage();

    printf("current_lang= %d, GetCurrentLanguage()= %d\n", current_lang, GetCurrentLanguage());


    // // int battI = GET_BATT_INDEX(selected_batt_comp_idx, info_selected_arc_comp_idx);

    // // //更新子列表: 1, 2, 3, 5
    // // update_item_list_value(1);
    // // update_item_list_value(2);
    // // update_item_list_value(3);
    // // update_item_list_value(5);

    // for (int row = 0; row < MAX_SETTING_ROW_COUNT; row++) {
    //     //标题
    //     if (!lv_obj_has_flag(setting_rows[row].row_cont, LV_OBJ_FLAG_HIDDEN)){
    //         lv_label_set_text(setting_rows[row].title_label, GetLanguageString(setting_row_titles[row]));
    //     //    lv_label_set_text(setting_rows[row].test_label, GetLanguageString(setting_row_titles[row]));
    //     }

    //     // value 
    //     if (row > 0 && row < MAX_SETTING_ROW_COUNT - 1 ) {
    //         lv_label_set_text(setting_rows[row].test_label, GetLanguageString(setting_row_titles[row]));
    //          lv_label_set_text(setting_rows[row].value_label, GetLanguageString(setting_row_titles[row]));
    //         // int item_idx = g_batt_data[battI].setting_items[row - 1];
    //         // update_setting_row_value(row, item_idx);
    //     }
    // }
  
}

static void cancel_selected_menu_row(){

    // printf("-》》》 cancel_selected_menu_row, current_selected_menu_idx = %d\n", current_selected_menu_idx);

    if (current_selected_menu_idx == 0) return;

    lv_obj_set_style_bg_opa(menu_mainlist[current_selected_menu_idx].row_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_color(menu_mainlist[current_selected_menu_idx].row_cont, COLOR_CLEAR, 0);

    lv_obj_set_style_img_recolor(menu_mainlist[current_selected_menu_idx].title_img, COLOR_4D4D4D, 0);
    lv_obj_set_style_text_color(menu_mainlist[current_selected_menu_idx].title_label, COLOR_4D4D4D, 0);

}


/**
 * @brief  延时 100ms 再销毁子列表
 */
static void delete_callback(lv_timer_t* timer)
{

    lv_delete_layout(&menu_sub_container[current_selected_menu_idx]);
    // 清除父按钮的选中样式
    cancel_selected_menu_row();
}

static void menu_child_click_cb(lv_event_t *e)
{
    lv_obj_t* child_btn = lv_event_get_target(e);                   // 当前点击的子项行容器 row_cont 
    lv_obj_t* sub_container = lv_obj_get_parent(child_btn);         // 子容器 layout
    lv_event_code_t code = lv_event_get_code(e);


    if (code == LV_EVENT_CLICKED)
    {
        // 检查当前展开的父项行索引是否有效
        if (current_selected_menu_idx < 1 || current_selected_menu_idx >= MAX_MENU_ROW_COUNT) {
            printf("Invalid current_selected_taskrow_idx: %d\n", current_selected_menu_idx);
            return;
        }

        int current_selected_sub = menu_items_data[current_selected_menu_idx-1];            // 曾经选中的子项索引
        int clicked_sub_idx = (int)(intptr_t)lv_obj_get_user_data(child_btn);               // 当前点击的子项索引

        printf("-》》》 row(%d) child list olddata = %d, newdata = %d\n", 
            current_selected_menu_idx, current_selected_sub, clicked_sub_idx);

        // // ====== 1. 如果点击已选中子项 → 仅折叠（删除） ======
        if (current_selected_sub == clicked_sub_idx) {
            lv_delete_layout(&menu_sub_container[current_selected_menu_idx]);
            
            // 清除父按钮的选中样式
            cancel_selected_menu_row();
            return;
        }

        // ====== 2. 点击不同子项 → 切换选中 ======
        // 清除旧选中
        if (current_selected_sub >= 0) {
            lv_obj_t* last_obj = lv_obj_get_child(sub_container, current_selected_sub);
            if (last_obj && lv_obj_is_valid(last_obj)) {
                lv_obj_set_style_bg_opa(last_obj, LV_OPA_TRANSP, 0);
                lv_obj_set_style_text_color(lv_obj_get_child(last_obj, 0), COLOR_000000, 0);
                // printf(" cancel last_idx \n");
            }
        }


         // 设置新选中
        if (child_btn && lv_obj_is_valid(child_btn)) {
            lv_obj_set_style_bg_opa(child_btn, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(child_btn, COLOR_0066FF, 0);

            lv_obj_set_style_text_color(lv_obj_get_child(child_btn, 0), COLOR_FFFFFF, 0);
            // lv_obj_scroll_to_view(child_btn, LV_ANIM_OFF); //滚动定位
        }


        // 持久化到电池数据结构
        menu_items_data[current_selected_menu_idx - 1] = clicked_sub_idx;

        const int32_t* sub_option_ids = menu_display_rows[current_selected_menu_idx].sub_texts;
        lv_label_set_text(menu_mainlist[current_selected_menu_idx].value_label, GetLanguageString(sub_option_ids[clicked_sub_idx]));


        // ====== 3. 选择后自动折叠（删除子容器） ======
        // 注意：子容器索引与父按钮索引，其实数值上一致
        lv_refr_now(lv_display_get_default());      //手动强制刷新
        // vTaskDelay(100 / portTICK_PERIOD_MS);       //延时100ms

        //  GUI 开发中, LVGL 的非阻塞定时器事件
        lv_timer_t * timer = lv_timer_create(delete_callback, 100, NULL);
        lv_timer_set_repeat_count(timer, 1);                            // 重复次数为 1，执行完自动删除


    }

}


static void menu_back_event_cb(lv_event_t *e)
{
    lv_obj_t* obj = lv_event_get_target(e); 
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED)
    {
        printf("-》》》 menu page back, current_selected_menu_idx = %d \n", current_selected_menu_idx);

        //显示子列表时退出，需要删除子列表
        if (menu_sub_container[current_selected_menu_idx]){
            lv_delete_layout(&menu_sub_container[current_selected_menu_idx]);
        }
        lv_delete_layout(&menu_layout);

        if (!stanby_page_layout)
        {
            current_selected_batt_comp_idx = 0;
            stanby_page_create();
        }
    }

}



static void update_menu_row_bg(int new_row){

    cancel_selected_menu_row();

    lv_obj_set_style_bg_opa(menu_mainlist[new_row].row_cont, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(menu_mainlist[new_row].row_cont, COLOR_808080, 0);

    lv_obj_set_style_img_recolor(menu_mainlist[new_row].title_img, COLOR_FFFFFF, 0);
    lv_obj_set_style_text_color(menu_mainlist[new_row].title_label, COLOR_FFFFFF, 0);

}

static void menu_row_click_event_cb(lv_event_t *e)
{
    lv_obj_t* parent_btn = lv_event_get_target(e);
    task_settingrow_type_t row_i = (task_settingrow_type_t)(intptr_t)lv_obj_get_user_data(parent_btn);
    lv_event_code_t code = lv_event_get_code(e);

    if(code == LV_EVENT_CLICKED)
    {
        printf("-》》》 current_selected_menu_idx = %d, row_i = %d\n", current_selected_menu_idx, row_i);

        lv_obj_t* clicked_list = menu_sub_container[row_i];

        // ---------- 1. 点击的是当前展开的行 → 删除子列表 ----------
        if (current_selected_menu_idx == row_i && clicked_list) {
            lv_delete_layout(&menu_sub_container[current_selected_menu_idx]);
            // 清除父按钮的选中样式
            cancel_selected_menu_row();
            return;
        }

        // ---------- 2. 点击的是新行 → 先删除旧行 ----------
        if (current_selected_menu_idx > 0 && current_selected_menu_idx < MAX_MENU_ROW_COUNT) {
            lv_obj_t* old_list = menu_sub_container[current_selected_menu_idx];
            if (old_list && lv_obj_is_valid(old_list)) {
                // 如果已经可见，滑出并隐藏
                cancel_selected_menu_row();
                lv_delete_layout(&menu_sub_container[current_selected_menu_idx]);

                // printf("old_list is invalid,  return \n");
                return;
            }else{
                // printf("old_list is invalid\n");
            }
        }

        // ---------- 3. 创建并展开新行 ----------
                // 获取当前行的子容器
        if (!clicked_list) {
            // printf(">>> first build setting_item_list2\n");
            // create_menu_sublist(row_i);
            menu_sub_container[row_i] = add_item_list(row_i);
            clicked_list = menu_sub_container[row_i];
        }
        update_menu_row_bg(row_i);
        current_selected_menu_idx = row_i;
    }
}


void menu_page_create(void)
{
    if (menu_layout != NULL && lv_obj_is_valid(menu_layout)) {
        printf("menu_layout != NULL && lv_obj_is_valid\n");
        return;
    }

    if (menu_layout) {
        printf("menu_layout != NULL\n");
        lv_obj_del(menu_layout);
        menu_layout = NULL;
    }

    menu_layout = lv_obj_create(lv_scr_act());
    lv_obj_set_size(menu_layout, lv_pct(100), lv_pct(100));  
    lv_obj_add_style(menu_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(menu_layout, COLOR_FFFFFF, 0); 
    lv_obj_set_style_margin_all(menu_layout, 2, 0);
    lv_obj_set_style_radius(menu_layout, 10, 0);
    lv_obj_set_style_clip_corner(menu_layout, true, 0);
    lv_obj_remove_flag(menu_layout, LV_OBJ_FLAG_SCROLLABLE);
    lv_obj_set_scrollbar_mode(menu_layout, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(menu_layout, LV_ALIGN_CENTER, 0, 0);

    // >>>  挂样式 + 先进入"透明状态"
    lv_obj_add_style(menu_layout, &style_menu_fade_normal, 0);
    lv_obj_add_style(menu_layout, &style_menu_transparent, LV_STATE_USER_1);
    lv_obj_add_state(menu_layout, LV_STATE_USER_1);     /* 此刻 menu_layout 全透明 */

    
    menu_mainlist[0].row_cont = lv_obj_create(menu_layout);
    lv_obj_add_style(menu_mainlist[0].row_cont, &bg_style, 0);
    lv_obj_set_size(menu_mainlist[0].row_cont, lv_pct(100), 37);
    lv_obj_set_style_bg_color(menu_mainlist[0].row_cont, COLOR_F2F2F2, 0);
    lv_obj_set_style_bg_opa(menu_mainlist[0].row_cont, LV_OPA_100, 0);
    lv_obj_align(menu_mainlist[0].row_cont, LV_ALIGN_TOP_MID, 0, 0);
    lv_obj_set_flex_flow(menu_mainlist[0].row_cont, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(menu_mainlist[0].row_cont, LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_style_pad_left(menu_mainlist[0].row_cont, 7, 0); //title_img 与父级间距
    lv_obj_set_style_pad_right(menu_mainlist[0].row_cont, 35, 0); 
    lv_obj_clear_flag(menu_mainlist[0].row_cont, LV_OBJ_FLAG_CLICK_FOCUSABLE);    // 清除焦点事件，绑定点击回调

    // 图像
    menu_mainlist[0].title_img = lv_image_create(menu_mainlist[0].row_cont);
    lv_obj_set_style_image_recolor_opa(menu_mainlist[0].title_img, LV_OPA_COVER, 0);
    lv_img_set_src(menu_mainlist[0].title_img, menu_display_rows[0].main_img);
    lv_obj_set_style_img_recolor(menu_mainlist[0].title_img, COLOR_808080, 0);
    lv_obj_set_style_translate_x(menu_mainlist[0].title_img, -7, 0);
    // lv_image_set_inner_align(setting_mainlist[0].title_img, LV_IMAGE_ALIGN_LEFT_MID);   //调整内容层：实际的图像像素，它在对象内部的显示位置
    //扩大点击范围
    lv_obj_set_style_pad_left(menu_mainlist[0].title_img, 15, 0);
    lv_obj_set_style_pad_right(menu_mainlist[0].title_img, 15, 0);
    lv_obj_set_style_pad_top(menu_mainlist[0].title_img, 12, 0);
    lv_obj_set_style_pad_bottom(menu_mainlist[0].title_img, 12, 0);
    lv_obj_set_style_transform_pivot_x(menu_mainlist[0].title_img, lv_pct(50), 0);   // 注意：状态传 0，即默认
    lv_obj_set_style_transform_pivot_y(menu_mainlist[0].title_img, lv_pct(50), 0);
    // scale 只在 PRESSED 状态下放大，256 = 100%, 280 ≈ 120%
    lv_obj_set_style_transform_scale_x(menu_mainlist[0].title_img, 384, LV_STATE_PRESSED); 
    lv_obj_set_style_transform_scale_y(menu_mainlist[0].title_img, 384, LV_STATE_PRESSED);

    lv_obj_add_flag(menu_mainlist[0].title_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(menu_mainlist[0].title_img, menu_back_event_cb, LV_EVENT_ALL, NULL);


    // 标题
    menu_mainlist[0].title_label = lv_label_create(menu_mainlist[0].row_cont);
    lv_obj_set_flex_grow(menu_mainlist[0].title_label, 1);
    lv_obj_set_style_text_color(menu_mainlist[0].title_label, COLOR_333333, 0);
    lv_obj_set_style_text_font(menu_mainlist[0].title_label, &font_isdtyahei_18_2, 0);
    lv_obj_set_style_text_align(menu_mainlist[0].title_label, LV_TEXT_ALIGN_CENTER, 0);
    lv_obj_set_style_pad_right(menu_mainlist[0].title_label, 23, 0);
    lv_label_set_text(menu_mainlist[0].title_label, GetLanguageString(menu_display_rows[0].main_text));


    lv_obj_t* line = lv_obj_create(menu_layout);
    lv_obj_set_size(line, lv_pct(100), 1);
    lv_obj_add_style(line, &bg_style, 0);
    lv_obj_set_style_bg_color(line, COLOR_999999, 0);
    lv_obj_align(line, LV_ALIGN_TOP_MID, 0, 37);


    // 文本行容器（可滚动）: 父容器使用 Flex 布局支持滚动
    menu_text_cont = lv_obj_create(menu_layout);
    lv_obj_add_style(menu_text_cont, &bg_style, 0);
    lv_obj_set_style_bg_color(menu_text_cont, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(menu_text_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_scrollbar_mode(menu_text_cont, LV_SCROLLBAR_MODE_OFF); 
    lv_obj_set_style_clip_corner(menu_text_cont, true, 0);
    lv_obj_set_style_pad_row(menu_text_cont, 0, 0);
    lv_obj_set_flex_flow(menu_text_cont, LV_FLEX_FLOW_COLUMN);
    lv_obj_set_flex_align( menu_text_cont,  LV_FLEX_ALIGN_START, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_set_size(menu_text_cont, lv_pct(100), 203);   //可视高度=实际高度240 - 标题行高度37
    lv_obj_align(menu_text_cont, LV_ALIGN_TOP_MID, 0, 38);

    // 循环创建
    for (int row_i = 1; row_i < sizeof(menu_display_rows) / sizeof(menu_display_rows[0]); row_i++) {
        bool is_last = (row_i == MAX_SETTING_ROW_COUNT - 1);
        menu_mainlist[row_i] = create_setting_row(menu_text_cont, is_last);

        if (row_i == 1)
        {
            lv_obj_add_flag(menu_mainlist[row_i].line, LV_OBJ_FLAG_HIDDEN);
        }

        const int32_t* sub_option_ids = menu_display_rows[row_i].sub_texts;
        lv_label_set_text(menu_mainlist[row_i].value_label, GetLanguageString(sub_option_ids[menu_items_data[row_i-1]]));
        
        lv_obj_set_user_data(menu_mainlist[row_i].row_cont, (void *)(intptr_t)row_i);
        lv_obj_add_event_cb(menu_mainlist[row_i].row_cont, menu_row_click_event_cb, LV_EVENT_ALL, NULL);


        // 更新图像、标题
        lv_img_set_src(menu_mainlist[row_i].title_img, menu_display_rows[row_i].main_img);
        lv_label_set_text(menu_mainlist[row_i].title_label, GetLanguageString(menu_display_rows[row_i].main_text));
    }


    //同一帧内 add_state + clear_state，LVGL 检测不到属性变化,强制刷一帧，让 LVGL 记录 TRANSP
    lv_refr_now(NULL);     

    // >>> 清除透明状态 → 触发淡入
    lv_obj_clear_state(menu_layout, LV_STATE_USER_1);

}

/**********************
 *   信息页面
 **********************/


/**
* @brief 销毁图表
* @param batt_idx 电池索引
*/
static void destroy_info_chart(int batt_idx) {
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
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
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
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


void info_page_back_event_cb(lv_event_t* e) 
{
    lv_obj_t* cont = lv_event_get_target(e);
    int bar_idx = (int)(intptr_t)lv_obj_get_user_data(cont);
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;

    // printf("-》》》 batt_coms[%d] layout(0-纵向,1-横向)=%d\n", 
    //     current_selected_batt_comp_idx, g_current_layout);
    // printf("-》》》 bar_idx=%d, current_selected_bar_idx=%d\n", bar_idx, current_selected_bar_idx);

    printf("->>> bar(%d) info page back -> bar page, current_selected_bar_idx = %d\n", bar_idx, current_selected_bar_idx);

    int ui_bar_idx = g_current_layout == LAYOUT_VER ? current_selected_bar_idx : current_selected_bar_idx-4;
    chang_barcont_bar_border_color(g_current_layout, ui_bar_idx);
    lv_obj_del(info_layout);
    info_layout = NULL;
    current_selected_bar_idx = -1;

}
 
/**
 * @brief 创建信息页面
 */
static void info_page_create(int new_bar_idx){

    const lv_color_t info_imgs_color[5] = {
        COLOR_808080, COLOR_FF6600, COLOR_9E6EE8, COLOR_00C195, COLOR_3F85F5
    }; 
    static const lv_coord_t text_col_dsc[] = {42, 92, LV_GRID_FR(1), LV_GRID_TEMPLATE_LAST}; 

    if (info_layout != NULL && lv_obj_is_valid(info_layout)) {
        printf("info_layout != NULL && lv_obj_is_valid\n");
        return;
    }

    if (info_layout) {
        printf("info_layout != NULL\n");
        lv_obj_del(info_layout);
        info_layout = NULL;
    }

    info_layout = lv_obj_create(lv_scr_act()); 
    lv_obj_add_style(info_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(info_layout, COLOR_CLEAR, LV_PART_MAIN);
    lv_obj_set_style_bg_opa(info_layout, LV_OPA_TRANSP, 0);
    lv_obj_set_scrollbar_mode(info_layout, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_radius(info_layout, 10, 0);
    lv_obj_set_style_clip_corner(info_layout, true, 0);

    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    if (g_current_layout == LAYOUT_VER) {
        lv_obj_set_size(info_layout, 316, 226);  //实际高度216+ 圆角半径10
        lv_obj_align(info_layout, LV_ALIGN_BOTTOM_MID, 0, 10);

    }else{
        lv_obj_set_size(info_layout, 306, 236); //实际宽度296+圆角半径10
        lv_obj_set_style_pad_left(info_layout, 10, 0 );
        lv_obj_align(info_layout, LV_ALIGN_LEFT_MID, -10, 0);
    }

    // //清空子对象
    // lv_obj_clean(info_layout);

    lv_obj_t* title_row = lv_obj_create(info_layout);
    lv_obj_set_size(title_row, lv_pct(100), 37); 
    lv_obj_add_style(title_row, &bg_style, 0);
    lv_obj_set_style_bg_color(title_row, COLOR_F2F2F2, LV_PART_MAIN);
    lv_obj_align(title_row, 
        g_current_layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t *icon = lv_image_create(title_row);  
    lv_image_set_src(icon, info_imgs[0]);
    lv_obj_set_style_image_recolor_opa(icon, LV_OPA_COVER, 0);
    lv_obj_set_style_image_recolor(icon, info_imgs_color[0], 0);
    //扩大点击范围
    lv_obj_set_style_pad_left(icon, 15, 0);
    lv_obj_set_style_pad_right(icon, 15, 0);
    lv_obj_set_style_pad_top(icon, 12, 0);
    lv_obj_set_style_pad_bottom(icon, 12, 0);
    // lv_obj_set_style_transform_pivot_x(icon, lv_pct(50), 0);   // 注意：状态传 0，即默认
    // lv_obj_set_style_transform_pivot_y(icon, lv_pct(50), 0);
    // // scale 只在 PRESSED 状态下放大，256 = 100%, 280 ≈ 120%
    // lv_obj_set_style_transform_scale_x(icon, 384, LV_STATE_PRESSED); 
    // lv_obj_set_style_transform_scale_y(icon, 384, LV_STATE_PRESSED);

    lv_obj_add_flag(icon, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_set_user_data(icon, (void*)(intptr_t)new_bar_idx);
    lv_obj_add_event_cb(icon, info_page_back_event_cb, LV_EVENT_CLICKED, NULL);


    lv_obj_t *name_value = lv_obj_create(title_row);
    lv_obj_set_size(name_value, LV_SIZE_CONTENT, LV_SIZE_CONTENT);
    lv_obj_set_style_bg_opa(name_value, LV_OPA_0, 0);
    lv_obj_set_style_border_width(name_value, 0, 0);
    lv_obj_set_style_pad_all(name_value, 0, 0);
    lv_obj_set_style_pad_column(name_value, 2, 0);  // 两个标签之间的间距
    lv_obj_set_flex_flow(name_value, LV_FLEX_FLOW_ROW);
    lv_obj_set_flex_align(name_value, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER, LV_FLEX_ALIGN_CENTER);
    lv_obj_align(name_value, LV_ALIGN_CENTER, 0, 0);

    lv_obj_t *name = lv_label_create(name_value);
    lv_label_set_text(name, "--");
    lv_obj_set_style_text_font(name, &font_isdtyahei_18_2, 0);

    lv_obj_t *value = lv_label_create(name_value);
    lv_label_set_text(value, "(--)");
    lv_obj_set_style_text_font(value, &font_isdtyahei_18_2, 0);


    lv_obj_t *setting_btn = lv_btn_create(title_row);  
    lv_obj_remove_style_all(setting_btn);   // 清掉所有主题样式
    lv_obj_set_size(setting_btn, 25, 24);
    lv_obj_align(setting_btn, LV_ALIGN_RIGHT_MID, -7, 0);
    lv_obj_set_style_radius(setting_btn, 6, 0);
    lv_obj_set_style_bg_color(setting_btn, COLOR_FFFFFF, 0);
    lv_obj_set_style_bg_opa(setting_btn, LV_OPA_100, 0);
    lv_obj_set_style_border_color(setting_btn, COLOR_7F7F7F, 0);
    lv_obj_set_style_border_opa(setting_btn, LV_OPA_100, 0);
    lv_obj_set_style_border_width(setting_btn, 1, 0);
    lv_obj_set_style_transform_pivot_x(setting_btn, lv_pct(50), 0);   // 注意：状态传 0，即默认
    lv_obj_set_style_transform_pivot_y(setting_btn, lv_pct(50), 0);
    // scale 只在 PRESSED 状态下放大，256 = 100%, 280 ≈ 120%
    lv_obj_set_style_transform_scale_x(setting_btn, 384, LV_STATE_PRESSED); 
    lv_obj_set_style_transform_scale_y(setting_btn, 384, LV_STATE_PRESSED);

    lv_obj_set_user_data(setting_btn, (void*)(intptr_t)new_bar_idx);
    lv_obj_add_event_cb(setting_btn, task_setting_click_event_cb, LV_EVENT_CLICKED, NULL);

    lv_obj_t *setting_img = lv_image_create(setting_btn);  
    lv_image_set_src(setting_img, &img_info_setting);
    lv_obj_set_style_image_recolor_opa(setting_img, LV_OPA_COVER, 0);
    lv_obj_set_style_image_recolor(setting_img, COLOR_262626, 0);
    lv_obj_center(setting_img);


    // 保存到全局数组
    g_info_rows[g_current_layout][0].info_icon = icon;
    g_info_rows[g_current_layout][0].info_label_name = name;
    g_info_rows[g_current_layout][0].info_label_value = value;

    lv_obj_t* cont = lv_obj_create(info_layout); 
    lv_obj_set_size(cont, lv_pct(100), g_current_layout == LAYOUT_VER ? 179 : 199); //可视 = 实际高度 - 首行37
    lv_obj_add_style(cont, &bg_style, 0);
    lv_obj_align(cont, LV_ALIGN_TOP_MID, 0, 37); 
    lv_obj_set_style_bg_color(cont, COLOR_FFFFFF, 0); 
    lv_obj_set_style_border_color(cont, COLOR_CCCCCC, 0);
    lv_obj_set_style_border_opa(cont, LV_OPA_100, 0);
    lv_obj_set_style_border_width(cont, 1, 0);
    lv_obj_set_style_border_side(cont, LV_BORDER_SIDE_TOP, 0);
    lv_obj_set_scrollbar_mode(cont, LV_SCROLLBAR_MODE_OFF);
    lv_obj_set_style_pad_bottom(cont, 10, 0);   //内边距

    //创建 grid 布局： 3*4
    lv_obj_t* info_text_cont = lv_obj_create(cont); 
    lv_obj_set_size(info_text_cont, 251, 134); 
    lv_obj_add_style(info_text_cont, &bg_style, 0);
    lv_obj_align(info_text_cont, LV_ALIGN_TOP_LEFT, 15, 5);
    lv_obj_set_style_bg_color(info_text_cont, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(info_text_cont, LV_OPA_TRANSP, 0);
    lv_obj_set_grid_dsc_array(info_text_cont, text_col_dsc, four_grid_dsc); 


    for(int row_idx=0; row_idx<4;row_idx++){

        // 创建每行的三个控件（图标、名称、值）
        lv_obj_t *icon = lv_image_create(info_text_cont);  
        lv_obj_t *name = lv_label_create(info_text_cont);
        lv_obj_t *value = lv_label_create(info_text_cont);
        
        lv_image_set_src(icon, info_imgs[row_idx+1]);
        lv_obj_set_style_image_recolor_opa(icon, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(icon, info_imgs_color[row_idx+1], 0);

        lv_label_set_text(name, "--");
        lv_obj_set_style_text_font(name, &font_isdtyahei_18_2, 0);

        lv_label_set_text(value, "--");
        lv_obj_set_style_text_font(value, &font_isdtyahei_18_2, 0);

        // 设置它们在 Grid 中的位置（列索引 0,1,2）
        lv_obj_set_grid_cell(icon, LV_GRID_ALIGN_START, 0, 1, LV_GRID_ALIGN_CENTER, row_idx , 1);
        lv_obj_set_grid_cell(name, LV_GRID_ALIGN_START, 1, 1, LV_GRID_ALIGN_CENTER, row_idx, 1);
        lv_obj_set_grid_cell(value, LV_GRID_ALIGN_START, 2, 1, LV_GRID_ALIGN_CENTER, row_idx, 1);
        
        // 保存到全局数组
        g_info_rows[g_current_layout][row_idx+1].info_icon = icon;
        g_info_rows[g_current_layout][row_idx+1].info_label_name = name;
        g_info_rows[g_current_layout][row_idx+1].info_label_value = value;
    }


    //创建图表容器
    lv_obj_t* temporary_placeholder = lv_obj_create(cont);
    lv_obj_set_size(temporary_placeholder, 
        g_current_layout == LAYOUT_VER ? 290 : 268, 50);
    lv_obj_align(temporary_placeholder, LV_ALIGN_TOP_MID , 0, 154);
    lv_obj_add_style(temporary_placeholder, &bg_style, 0);
    lv_obj_set_style_bg_color(temporary_placeholder, COLOR_F2F2F2, 0);
    lv_obj_set_style_bg_opa(temporary_placeholder, LV_OPA_COVER, 0);
    lv_obj_set_style_border_width(temporary_placeholder, 1, 0);
    lv_obj_set_style_border_opa(temporary_placeholder, LV_OPA_COVER, 0);
    lv_obj_set_style_border_color(temporary_placeholder, COLOR_CCCCCC, 0);

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

        //更新文本
    update_bar_batt_type_text(new_bar_idx, true);
    update_bar_batt_volcur_text(new_bar_idx,  true);
    update_bar_batt_time_text(new_bar_idx, true); 
    update_batt_per_text(new_bar_idx);
    update_batt_mah_text(new_bar_idx);
    update_batt_ohms_text(new_bar_idx);
    update_batt_temp_text(new_bar_idx); 
    update_batt_wh_text(new_bar_idx);

}

/**********************
 *   进度条页面
 **********************/


 const lv_image_dsc_t* get_barbatt_workstatus_img(int32_t value){
    switch (value) {
        case ttCharge: return &img_bar_charge;
        case ttDischarge: return &img_bar_discharge;
        case ttStorage: return &img_bar_storage; 
        case ttCycle: return &img_bar_cycle;
        case ttAnalysis: return &img_bar_analyse; 
        case ttActivate: return &img_bar_activate; 
        case ttDestroy: return &img_bar_destory;
        default: return &img_bar_none; 
    }
}

void task_setting_click_event_cb(lv_event_t* e){
    lv_obj_t* cont = lv_event_get_target(e);
    int bar_idx = (int)(intptr_t)lv_obj_get_user_data(cont);
    lv_event_code_t code = lv_event_get_code(e);

    printf("->>> show task setting page, bar_idx = %d, current_selected_bar_idx = %d, %d\n", bar_idx, current_selected_bar_idx, code);

    if (code == LV_EVENT_CLICKED)
    {
        if (!setting_layout)
        {
            //bar page -> setting page, current_selected_bar_idx = -1,  bar_idx 表示实际设置的电池索引
            //info page 中途可能发生切换， -> setting page, current_selected_bar_idx 表示实际设置的电池索引
            current_selected_bar_idx = bar_idx;
            current_selected_taskrow_idx = 0;
            setting_page_create(current_selected_bar_idx);
        }

    }
}


/**
* @brief 进度条点击事件
*/
void bar_cont_click_event_cb(lv_event_t* e) 
{
    lv_event_code_t code = lv_event_get_code(e);
    if (code == LV_EVENT_CLICKED) {
        lv_obj_t* cont = lv_event_get_target(e);
        int bar_idx = (int)(intptr_t)lv_obj_get_user_data(cont);            //当前点击的进度条 ui 索引,纵向： 0-3；横向: 0-1
        layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;

        int actual_bar_idx = g_current_layout == LAYOUT_VER ? bar_idx : bar_idx+4;      //电池数据索引，,纵向： 0-3；横向: 4-5
        int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, actual_bar_idx);
        if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
        batt_data_t* data = &g_batt_data[batt_idx];                         //当前点击的进度条对应的电池数据

        // printf("->>> batt_coms[%d] layout(0-ver,1-hor)=%d\n", 
        //     current_selected_batt_comp_idx, g_current_layout);
        // printf("->>> click bar, actual_bar_idx = %d, current_selected_bar_idx = %d \n", actual_bar_idx, current_selected_bar_idx);

        //测试
        if (data->BatteryLoad == 0 || data->TaskState == tsStandby)
        {
            printf("->>> did not allow create info page, BatteryLoad = %d, TaskState = %d\n", data->BatteryLoad, data->TaskState);

            if (!info_layout && bar_layout)
            {
                
                if (bar_layout)
                {
                    lv_delete_layout(&bar_layout);
                } 
                
                if (!stanby_page_layout)
                {
                    current_selected_batt_comp_idx = 0;
                    stanby_page_create();
                }
            }

            return;
        }


        if (info_layout && current_selected_bar_idx == actual_bar_idx)
        {
            printf("->>> click double, info_layout != NULL, will delete info_layout\n");

            chang_barcont_bar_border_color(g_current_layout, bar_idx);

            // destroy_info_chart(batt_idx);

            lv_obj_del(info_layout);
            info_layout = NULL;
            current_selected_bar_idx = -1;

        }else if (info_layout && current_selected_bar_idx != actual_bar_idx){
            printf("->>> switch bar, info_layout != NULL, will translate arrow and update value\n");
            
            int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, current_selected_bar_idx);
            if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
            batt_data_t* old_data = &g_batt_data[batt_idx];
            int old_ui_idx = g_current_layout == LAYOUT_VER ? current_selected_bar_idx : current_selected_bar_idx-4;

            // 恢复默认边框
            lv_obj_set_style_border_color(bar_conts[g_current_layout][old_ui_idx].bar_cont, COLOR_404040, LV_PART_MAIN);
            lv_obj_set_style_bg_color(bar_conts[g_current_layout][old_ui_idx].bar, 
                    old_data->BatteryLoad == 0 ? COLOR_1A1A1A : COLOR_999999, 
                    LV_PART_INDICATOR);
            // 高亮边框
            lv_obj_set_style_border_color(bar_conts[g_current_layout][bar_idx].bar_cont, COLOR_FFFFFF, LV_PART_MAIN);
            lv_obj_set_style_bg_color(bar_conts[g_current_layout][bar_idx].bar, 
                    data->BatteryLoad == 0 ? COLOR_1A1A1A : get_batt_workstatus_color(data->work_status), 
                    LV_PART_INDICATOR);

            current_selected_bar_idx = actual_bar_idx;
            
            ////模拟器不执行嵌入式图表
            // destroy_info_chart(batt_idx);
            // manage_chart_timer(GET_BATT_INDEX(current_selected_batt_comp_idx, current_selected_bar_idx));

            //更新 infolayout 文本
            update_bar_batt_type_text(actual_bar_idx, true);
            update_bar_batt_volcur_text(actual_bar_idx,  true);
            update_bar_batt_time_text(actual_bar_idx, true); 
            update_batt_per_text(actual_bar_idx);
            update_batt_mah_text(actual_bar_idx);
            update_batt_ohms_text(actual_bar_idx);
            update_batt_temp_text(actual_bar_idx); 
            update_batt_wh_text(actual_bar_idx);
            //未装载电池，不显示设置页面按钮
            lv_obj_t* title_row = lv_obj_get_child(info_layout, 0);
            lv_obj_t *setting_btn = lv_obj_get_child(title_row, -1);
            if (data->BatteryLoad == 0)
            {
                lv_obj_add_flag(setting_btn, LV_OBJ_FLAG_HIDDEN);
            }else{
                lv_obj_remove_flag(setting_btn, LV_OBJ_FLAG_HIDDEN);
            }

        }else if (!info_layout){
            printf("->>> first create info_layout, bar_layout = %p\n", bar_layout);

            // lv_mem_monitor_t mon;
            // lv_mem_monitor(&mon);
            // printf("LVGL mem: total=%u free=%u max_free=%u used=%u frag=%u%%\n",
            //     mon.total_size, mon.free_size, mon.max_used,
            //     mon.used_cnt, mon.frag_pct);

            chang_barcont_bar_border_color(g_current_layout, bar_idx);
            info_page_create(actual_bar_idx);
                        //未装载电池，不显示设置页面按钮
            lv_obj_t* title_row = lv_obj_get_child(info_layout, 0);
            lv_obj_t *setting_btn = lv_obj_get_child(title_row, -1);
            if (data->BatteryLoad == 0)
            {
                lv_obj_add_flag(setting_btn, LV_OBJ_FLAG_HIDDEN);
            }else{
                lv_obj_remove_flag(setting_btn, LV_OBJ_FLAG_HIDDEN);
            }
            current_selected_bar_idx = actual_bar_idx;
        }

    }

}


/**
 * @brief 更新进度条状态图像
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 */
void update_bar_batt_img(int bar_idx)
{
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    batt_t b = bar_conts[g_current_layout][g_current_layout == LAYOUT_VER ? bar_idx : bar_idx-4]; 
    int newvalue = data->TaskState == tsStandby ? -1 : data->work_status;

    if (b.workstatus_img) 
    {
        lv_image_set_src(b.workstatus_img, data->BatteryLoad == 0 ? &img_bar_none : get_barbatt_workstatus_img(newvalue));
    }
}




/**
 * @brief 更新进度条页面的第 bar_idx 个电池状态颜色
 * @param batt_comp_idx 电池仓索引
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 * @param newvalue 工作状态
 */
static void update_bar_batt_workstatus(int bar_idx)
{
    int batt_idx = GET_BATT_INDEX(current_selected_batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    batt_t b = bar_conts[g_current_layout][g_current_layout == LAYOUT_VER ? bar_idx : bar_idx-4]; 
    int newvalue = data->TaskState == tsStandby ? -1 : data->work_status;

    // printf("-》》》 bar page batt(%d) workstatus = %d, BatteryLoad = %d, TaskState = %d\n", batt_idx, newvalue, data->BatteryLoad, data->TaskState);

    lv_obj_t* parent = lv_obj_get_parent(b.setting_img);
    if (data->BatteryLoad == 0)
    {
        lv_obj_add_flag(parent, LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_remove_flag(parent, LV_OBJ_FLAG_HIDDEN);
    }

    if (b.bar_cont)
    {
        lv_obj_set_style_bg_color(b.bar_cont, data->BatteryLoad == 0 ? COLOR_1A1A1A : COLOR_262626, LV_PART_MAIN);
        lv_obj_set_style_border_color(b.bar_cont, data->BatteryLoad == 0 ? COLOR_404040 : COLOR_999999, LV_PART_MAIN);
    }
    if (b.bar)
    {
        lv_obj_set_style_bg_color(b.bar, data->BatteryLoad == 0 ? COLOR_1A1A1A : COLOR_0D0D0D, LV_PART_MAIN);
        lv_obj_set_style_bg_color(b.bar, data->BatteryLoad == 0 ? COLOR_1A1A1A : get_batt_workstatus_color(newvalue), LV_PART_INDICATOR);
    }

    if (b.batt_body)
    {
        lv_obj_set_style_bg_color(b.batt_body, data->BatteryLoad == 0 ? COLOR_0D0D0D : COLOR_1A1A1A, LV_PART_MAIN);
        lv_obj_set_style_border_color(b.batt_body, data->BatteryLoad == 0 ? COLOR_404040 : COLOR_999999, LV_PART_MAIN);
    }

    if (b.workstatus_img) 
    {
        lv_obj_set_style_image_recolor(b.workstatus_img, data->BatteryLoad == 0 ? COLOR_000000 : get_batt_workstatus_color(newvalue), 0);
    }

    if (b.type_label)
    {
        lv_obj_set_style_text_color(b.type_label, data->BatteryLoad == 0 ? COLOR_4D4D4D : COLOR_FFFFFF, 0);
    }
    
    if (b.volcur_label)
    {
        lv_obj_set_style_text_color(b.volcur_label, data->BatteryLoad == 0 ? COLOR_4D4D4D : COLOR_FFFFFF, 0);
    }
    
    if (b.time_label)
    {
        lv_obj_set_style_text_color(b.time_label, data->BatteryLoad == 0 ? COLOR_4D4D4D : COLOR_FFFFFF, 0);
    }

}

/**
* @brief 创建进度条页面
*/
static void bar_page_create(){

    if (bar_layout != NULL && lv_obj_is_valid(bar_layout)) {
        printf("bar_layout != NULL && lv_obj_is_valid\n");
        return;
    }

    if (bar_layout) {
        printf("bar_layout != NULL\n");
        lv_obj_del(bar_layout);
        bar_layout = NULL;
    }


    layout_type_t g_current_layout = batt_coms[current_selected_batt_comp_idx].batt_cont_layout;
    int idx_max = g_current_layout == LAYOUT_VER ? 4 : 2;

    // printf("->>> create bar page , g_current_layout = %d ,current_selected_batt_comp_idx= %d, current_bar_idx = %d \n", g_current_layout, current_selected_batt_comp_idx, current_selected_bar_idx );


    bar_layout = lv_obj_create(lv_scr_act());
    lv_obj_set_size(bar_layout, lv_pct(100), lv_pct(100));
    lv_obj_align(bar_layout, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(bar_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(bar_layout, COLOR_000000, LV_PART_MAIN);
    lv_obj_set_style_pad_column(bar_layout, 0, 0);
    lv_obj_set_style_pad_row(bar_layout, g_current_layout == LAYOUT_VER ? 0 : 10, 0);
    lv_obj_set_style_pad_top(bar_layout, g_current_layout == LAYOUT_VER ? 0 : 15, 0);
    lv_obj_set_style_pad_bottom(bar_layout, g_current_layout == LAYOUT_VER ? 0 : 15, 0);
    lv_obj_set_grid_dsc_array(bar_layout, 
        g_current_layout == LAYOUT_VER ? four_grid_dsc : one_grid_dsc, 
        g_current_layout == LAYOUT_VER ? one_grid_dsc : two_grid_dsc);
    //设置变换支点到右上角，想让支点在左上角，就改成 (-bg_w/2, -bg_h/2)；想右下角就 (bg_w/2, bg_h/2)
    lv_obj_set_style_transform_pivot_x(bar_layout,  320, 0);    // 向右 → 右边缘 bg_w / 2
    lv_obj_set_style_transform_pivot_y(bar_layout,  120, 0);   // 向上 → 上边缘 -bg_h / 2


    // //清空子对象
    // lv_obj_clean(bar_layout);

    for(int batt_idx = 0; batt_idx < idx_max; batt_idx++){
        batt_t item;

        item.bg = lv_obj_create(bar_layout);
        lv_obj_add_style(item.bg, &bg_style, 0);
        lv_obj_set_style_bg_color(item.bg, COLOR_003D4D, 0);
        // lv_obj_set_style_bg_color(item.bg, COLOR_CLEAR, 0);
        // lv_obj_set_style_bg_opa(item.bg, LV_OPA_TRANSP, 0);
        lv_obj_set_style_pad_all(item.bg, 2, 0);
        // 设置 grid 单元格位置，例如 i=0 -> 第0行第0列，i=1 -> 第0行第1列，i=2 -> 第0行第0列，i=3 -> 第0行第1列
        lv_obj_set_grid_cell(item.bg, 
            LV_GRID_ALIGN_STRETCH,  g_current_layout == LAYOUT_VER ? batt_idx : 0, 1, 
            LV_GRID_ALIGN_STRETCH,  g_current_layout == LAYOUT_VER ? 0 : batt_idx, 1);

        //进度条容器
        item.bar_cont = lv_obj_create(item.bg);
        lv_obj_add_style(item.bar_cont, &bg_style, 0);
        lv_obj_set_size(item.bar_cont, 
            g_current_layout == LAYOUT_VER ? lv_pct(100) : 20, 
            g_current_layout == LAYOUT_VER ? 20 : lv_pct(100));     //lv_pct(100)=76 : lv_pct(100)=100
        lv_obj_align(item.bar_cont, g_current_layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_pad_all(item.bar_cont, 3, 0);
        lv_obj_set_style_radius(item.bar_cont, 10, 0);
        lv_obj_set_style_bg_color(item.bar_cont, COLOR_1A1A1A, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(item.bar_cont, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_border_width(item.bar_cont, 2, LV_PART_MAIN);
        lv_obj_set_style_border_color(item.bar_cont, COLOR_404040, LV_PART_MAIN);
        lv_obj_set_style_border_opa(item.bar_cont, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_transform_pivot_x(item.bar_cont, lv_pct(50), 0);   // 注意：状态传 0，即默认
        lv_obj_set_style_transform_pivot_y(item.bar_cont, lv_pct(50), 0);
        // scale 只在 PRESSED 状态下放大，256 = 100%
        lv_obj_set_style_transform_scale_x(item.bar_cont, g_current_layout == LAYOUT_VER ? 263 : 282, LV_STATE_PRESSED);   // 78 / (lv_pct(100)-pad_top-pad_bottom=76) *256
        lv_obj_set_style_transform_scale_y(item.bar_cont, g_current_layout == LAYOUT_VER ? 282 : 261, LV_STATE_PRESSED);   // 22 / 20 *256


        //进度条
        item.bar = lv_bar_create(item.bar_cont);
        lv_obj_set_size(item.bar, lv_pct(100), lv_pct(100));
        lv_obj_align(item.bar, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_bg_color(item.bar, COLOR_0D0D0D, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(item.bar, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_bg_color(item.bar, COLOR_999999, LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(item.bar, LV_OPA_COVER, LV_PART_INDICATOR);
        lv_obj_remove_flag(item.bar, LV_OBJ_FLAG_CLICKABLE);
            
        //电池身体
        item.batt_body = lv_obj_create(item.bg);
        lv_obj_add_style(item.batt_body, &bg_style, 0);
        lv_obj_set_size(item.batt_body, 
            g_current_layout == LAYOUT_VER ? lv_pct(100) : 294, 
            g_current_layout == LAYOUT_VER ? 214 : lv_pct(100));    //lv_pct(100)=76 : lv_pct(100)=100
        lv_obj_align(item.batt_body, g_current_layout == LAYOUT_VER ? LV_ALIGN_BOTTOM_MID : LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_set_style_radius(item.batt_body, 15, 0);
        lv_obj_set_style_bg_color(item.batt_body, COLOR_0D0D0D, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(item.batt_body, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_border_width(item.batt_body, 2, LV_PART_MAIN);
        lv_obj_set_style_border_color(item.batt_body, COLOR_404040, LV_PART_MAIN);
        lv_obj_set_style_border_opa(item.batt_body, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_pad_all(item.batt_body, 2, 0);
        lv_obj_set_style_transform_pivot_x(item.batt_body, lv_pct(50), 0);   // 注意：状态传 0，即默认
        lv_obj_set_style_transform_pivot_y(item.batt_body, lv_pct(50), 0);
        // scale 只在 PRESSED 状态下放大，256 = 100%
        lv_obj_set_style_transform_scale_x(item.batt_body, g_current_layout == LAYOUT_VER ? 263 : 258, LV_STATE_PRESSED);    // 78 / (lv_pct(100)-pad_top-pad_bottom=76) *256
        lv_obj_set_style_transform_scale_y(item.batt_body, g_current_layout == LAYOUT_VER ? 258 : 261, LV_STATE_PRESSED);    // 216 / 214 *256
        

        //电池索引图像
        item.batt_idx_img = lv_image_create(item.batt_body);
        lv_obj_set_size(item.batt_idx_img, g_current_layout == LAYOUT_VER ? lv_pct(100) : 56, 
                                            g_current_layout == LAYOUT_VER ? 16 : lv_pct(100));
        lv_image_set_src(item.batt_idx_img, batt_idx_imgs[g_current_layout == LAYOUT_VER ? batt_idx : batt_idx+4]);
        lv_obj_set_style_image_recolor_opa(item.batt_idx_img, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(item.batt_idx_img, COLOR_4D4D4D, 0);
        lv_obj_align(item.batt_idx_img, g_current_layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_RIGHT_MID,
            g_current_layout == LAYOUT_VER ? 0 : -17, 
            g_current_layout == LAYOUT_VER ? 8 : 0);

        //电池状态图像容器
        lv_obj_t *icon_bg = lv_obj_create(item.batt_body);
        lv_obj_set_size(icon_bg, 60, 60); 
        lv_obj_add_style(icon_bg, &bg_style, 0);
        lv_obj_align(icon_bg, 
            g_current_layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_LEFT_MID, 
            g_current_layout == LAYOUT_VER ? 0 : 139, 
            g_current_layout == LAYOUT_VER ? 39 : 0);
        lv_obj_set_style_bg_color(icon_bg, COLOR_CLEAR, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(icon_bg, LV_OPA_TRANSP, 0);
        lv_obj_remove_flag(icon_bg, LV_OBJ_FLAG_CLICKABLE); //icon_bg 以及它的任何子对象都不会响应点击事件,直接响应父级事件


        for (int i = 0; i < RIPPLE_NUM; i++) {
            item.ripples[i] = ripple_create(icon_bg, lv_color_hex(0x00A8FF));
            lv_obj_move_to_index(item.ripples[i], 0);      /* 放到最底层 */
        }


        item.workstatus_img = lv_image_create(icon_bg);
        lv_obj_set_size(item.workstatus_img, 30, 30);
        lv_image_set_src(item.workstatus_img, &img_small_none);
        lv_obj_set_style_image_recolor_opa(item.workstatus_img, LV_OPA_COVER, 0);
        lv_obj_center(item.workstatus_img);

        lv_obj_t* text_bg = lv_obj_create(item.batt_body);
        // lv_obj_set_size(text_bg, 54, 
        //     g_current_layout == LAYOUT_VER ? 50 : 60); 
        lv_obj_set_size(text_bg, g_current_layout == LAYOUT_VER ? lv_pct(100) : 70, g_current_layout == LAYOUT_VER ? 50 : 60);
        lv_obj_add_style(text_bg, &bg_style, 0);
        lv_obj_set_style_bg_color(text_bg, COLOR_CLEAR, 0);
        lv_obj_set_style_bg_opa(text_bg, LV_OPA_TRANSP, 0);
        // lv_obj_set_style_bg_color(text_bg, COLOR_0066FF, 0);
        lv_obj_align(text_bg, 
            g_current_layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_LEFT_MID, 
            g_current_layout == LAYOUT_VER ? 0 : 65, 
            g_current_layout == LAYOUT_VER ? 112 : 0);
        lv_obj_set_style_pad_row(text_bg, 0, 0);
        lv_obj_set_grid_dsc_array(text_bg, one_grid_dsc, three_grid_dsc);
        lv_obj_remove_flag(text_bg, LV_OBJ_FLAG_CLICKABLE); //text_bg 以及它的任何子对象都不会响应点击事件,直接响应父级事件

        // 创建 grid 布局: 1*3
        item.type_label = lv_label_create(text_bg);  
        item.volcur_label = lv_label_create(text_bg);
        item.time_label = lv_label_create(text_bg);
        
        lv_obj_set_style_text_color(item.type_label, COLOR_4D4D4D, 0);
        lv_label_set_text(item.type_label, "1.5VLi");
        lv_obj_set_style_text_font(item.type_label, &font_FUSION_PIXEL_PROPORTIONAL_12_2, 0); //font_FUSION_PIXEL_PROPORTIONAL_12_2

        lv_obj_set_style_text_color(item.volcur_label, COLOR_4D4D4D, 0);
        lv_label_set_text(item.volcur_label, "12V/10A");
        lv_obj_set_style_text_font(item.volcur_label, &font_FUSION_PIXEL_PROPORTIONAL_12_2, 0);

        lv_obj_set_style_text_color(item.time_label, COLOR_4D4D4D, 0);
        lv_label_set_text(item.time_label, "01:23:45");
        lv_obj_set_style_text_font(item.time_label, &font_FUSION_PIXEL_PROPORTIONAL_12_2, 0);

        // item.type_label = lv_img_create(text_bg);  
        // lv_image_set_src(item.type_label, &img_type_1_5vli);
        // lv_obj_set_style_image_recolor_opa(item.type_label, LV_OPA_COVER, 0);
        // lv_obj_set_style_image_recolor(item.type_label, COLOR_FFFFFF, 0);


        // // 图像字符串容器，替换原 label
        // item.volcur_label = lv_obj_create(text_bg);
        // lv_obj_remove_style_all(item.volcur_label);                 // 去掉默认背景/边框/pad
        // lv_obj_set_style_bg_opa(item.volcur_label, LV_OPA_TRANSP, 0);
        // lv_obj_set_style_pad_all(item.volcur_label, 0, 0);
        // lv_obj_set_style_pad_column(item.volcur_label, 2, 0);       // 字距，可按需调
        // lv_obj_set_flex_flow(item.volcur_label, LV_FLEX_FLOW_ROW);
        // lv_obj_set_flex_align(item.volcur_label,
        //                     LV_FLEX_ALIGN_CENTER,                 // 主轴：水平居中
        //                     LV_FLEX_ALIGN_CENTER,                 // 交叉轴：垂直居中
        //                     LV_FLEX_ALIGN_CENTER);
        // lv_obj_remove_flag(item.volcur_label, LV_OBJ_FLAG_SCROLLABLE);
        // lv_obj_remove_flag(item.volcur_label, LV_OBJ_FLAG_CLICKABLE);
        // lv_obj_set_width(item.volcur_label, LV_SIZE_CONTENT);
        // lv_obj_set_height(item.volcur_label, LV_SIZE_CONTENT);


        // item.time_label = lv_obj_create(text_bg);
        // lv_obj_remove_style_all(item.time_label);                 // 去掉默认背景/边框/pad
        // lv_obj_set_style_bg_opa(item.time_label, LV_OPA_TRANSP, 0);
        // lv_obj_set_style_pad_all(item.time_label, 0, 0);
        // lv_obj_set_style_pad_column(item.time_label, 2, 0);       // 字距，可按需调
        // lv_obj_set_flex_flow(item.time_label, LV_FLEX_FLOW_ROW);
        // lv_obj_set_flex_align(item.time_label,
        //                     LV_FLEX_ALIGN_CENTER,                 // 主轴：水平居中
        //                     LV_FLEX_ALIGN_CENTER,                 // 交叉轴：垂直居中
        //                     LV_FLEX_ALIGN_CENTER);
        // lv_obj_remove_flag(item.time_label, LV_OBJ_FLAG_SCROLLABLE);
        // lv_obj_remove_flag(item.time_label, LV_OBJ_FLAG_CLICKABLE);
        // lv_obj_set_width(item.time_label, LV_SIZE_CONTENT);
        // lv_obj_set_height(item.time_label, LV_SIZE_CONTENT);

        // // 初始文案
        // set_volcur_image_text(item.volcur_label, "1.0V/1.2A");
        // char buf2[32];
        // format_time(buf2, 3665);
        // set_volcur_image_text(item.time_label, "-");



        // 设置它们在 Grid 中的位置（行索引 0,1,2）
        lv_obj_set_grid_cell(item.type_label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 0, 1);
        lv_obj_set_grid_cell(item.volcur_label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 1, 1);
        lv_obj_set_grid_cell(item.time_label, LV_GRID_ALIGN_CENTER, 0, 1, LV_GRID_ALIGN_CENTER, 2, 1);
        

        lv_obj_t* setting_bg = lv_obj_create(item.batt_body);
        lv_obj_set_size(setting_bg, 
            g_current_layout == LAYOUT_VER ? 64 : 39, 
            g_current_layout == LAYOUT_VER ? 31 : 84); 
        lv_obj_add_style(setting_bg, &bg_style, 0);
        lv_obj_set_style_bg_color(setting_bg, COLOR_0D0D0D, LV_PART_MAIN);
        lv_obj_set_style_radius(setting_bg, g_current_layout == LAYOUT_VER ? 11 : 12, 0);
        lv_obj_align(setting_bg, 
            g_current_layout == LAYOUT_VER ? LV_ALIGN_BOTTOM_MID : LV_ALIGN_LEFT_MID, 
            g_current_layout == LAYOUT_VER ? 0 : 2,
            g_current_layout == LAYOUT_VER ? -2 : 0);
        lv_obj_add_flag(setting_bg, LV_OBJ_FLAG_HIDDEN);
        lv_obj_set_style_bg_color(setting_bg, COLOR_404040, LV_STATE_PRESSED);

        item.setting_img = lv_image_create(setting_bg);
        lv_image_set_src(item.setting_img, &img_bar_setting);
        lv_obj_set_style_image_recolor_opa(item.setting_img, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(item.setting_img, COLOR_CCCCCC, 0);
        lv_obj_set_style_image_recolor(item.setting_img, COLOR_FFFFFF, LV_STATE_PRESSED);
        lv_obj_center(item.setting_img);


        lv_obj_set_user_data(item.bar_cont, (void*)(intptr_t)batt_idx);
        lv_obj_add_event_cb(item.bar_cont, bar_cont_click_event_cb, LV_EVENT_CLICKED, NULL);
        lv_obj_set_user_data(item.batt_body, (void*)(intptr_t)batt_idx);
        lv_obj_add_event_cb(item.batt_body, bar_cont_click_event_cb, LV_EVENT_CLICKED, NULL);

        lv_obj_set_user_data(setting_bg, (void*)(intptr_t)(g_current_layout == LAYOUT_VER ? batt_idx : batt_idx+4));
        lv_obj_add_event_cb(setting_bg, task_setting_click_event_cb, LV_EVENT_CLICKED, NULL);


        //保存到数据结构体
        bar_conts[g_current_layout][batt_idx] = item;
    }

    //左滑事件增加遮罩,防止触发点击事件
    lv_obj_t* mask = lv_obj_create(bar_layout);
    lv_obj_set_size(mask, lv_pct(100), lv_pct(100));
    lv_obj_add_style(mask, &bg_style, 0);
    lv_obj_set_style_bg_color(mask, COLOR_0066FF, 0);
    // lv_obj_set_style_bg_color(mask, COLOR_CLEAR, 0);
    // lv_obj_set_style_bg_opa(mask, LV_OPA_TRANSP, 0);
    lv_obj_align(mask, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(mask, LV_OBJ_FLAG_HIDDEN);


    //更新电池、进度条、文本
    int bar_cnt = g_current_layout == LAYOUT_VER ? 4 : 6;
    int bar_init = g_current_layout == LAYOUT_VER ? 0 : 4;

    for (int bar_idx = bar_init; bar_idx < bar_cnt; bar_idx ++)
    {
        update_bar_batt_workstatus(bar_idx);
        update_bar_batt_img(bar_idx);
        update_stanby_bar_value(current_selected_batt_comp_idx, bar_idx);
        update_bar_batt_type_text(bar_idx, false);
        update_bar_batt_volcur_text(bar_idx, false);
        update_bar_batt_time_text(bar_idx, false); 

        bar_ripple_restart(g_current_layout, g_current_layout == LAYOUT_VER ? bar_idx : bar_idx-4);
    
    }


}

/**********************
 *   待机页面
 **********************/

void batt_components_init(batt_components_t *comp) {
    comp->bg = NULL;
    comp->light = NULL;
    comp->batt_cont = NULL;
    comp->batt_cont_layout = LAYOUT_VER;
    lv_memset(comp->batts, 0, sizeof(comp->batts));
}

/**
 * @brief 更新第 batt_comp_idx 个 电池仓内部的第 bar_idx 个电池状态
 * @param batt_comp_idx 电池仓索引
 * @param bar_idx 进度条索引, 0-3: 纵向电池，4-5：横向电池
 * @param newvalue 工作状态
 */
static void update_stanby_batt_workstatus(int batt_comp_idx, int bar_idx)
{
    int batt_idx = GET_BATT_INDEX(batt_comp_idx, bar_idx);
    if (batt_idx >= MAX_BATT_TOTAL_COUNT || stanby_page_layout == NULL) return;
    batt_data_t* data = &g_batt_data[batt_idx];
    batt_t b = batt_coms[batt_comp_idx].batts[bar_idx]; 
    int newvalue = data->TaskState == tsStandby ? -1 : data->work_status;
    layout_type_t g_current_layout = batt_coms[batt_comp_idx].batt_cont_layout;

    if ((bar_idx < MAX_VER_BATT_CNT && g_current_layout != LAYOUT_VER) || (bar_idx >= MAX_VER_BATT_CNT && g_current_layout != LAYOUT_HOR)) return;

    // printf("->>> stanby page batt(%d) workstatus = %d, BatteryLoad = %d, TaskState = %d\n", batt_idx, newvalue, data->BatteryLoad, data->TaskState);

    if (b.bar_cont)
    {
        lv_obj_set_style_bg_color(b.bar_cont, data->BatteryLoad == 0 ? COLOR_262626 : get_workstatus_bg_color(newvalue), LV_PART_MAIN);
        lv_obj_set_style_border_color(b.bar_cont, data->BatteryLoad == 0 ? COLOR_4D4D4D : get_workstatus_border_color(newvalue), LV_PART_MAIN);
    }
    if (b.bar)
    {
        lv_obj_set_style_bg_color(b.bar, data->BatteryLoad == 0 ? COLOR_262626 : get_batt_workstatus_color(newvalue), LV_PART_INDICATOR);
        lv_obj_set_style_bg_color(b.bar, data->BatteryLoad == 0 ? COLOR_262626 : get_bar_bg_color(newvalue), LV_PART_MAIN);
    }

    if (b.batt_body)
    {
        lv_obj_set_style_bg_color(b.batt_body, data->BatteryLoad == 0 ? COLOR_262626 : get_workstatus_bg_color(newvalue), LV_PART_MAIN);
        lv_obj_set_style_border_color(b.batt_body, data->BatteryLoad == 0 ? COLOR_4D4D4D : get_workstatus_border_color(newvalue), LV_PART_MAIN);
    }

    if (b.workstatus_img) 
    {
        lv_image_set_src(b.workstatus_img, data->BatteryLoad == 0 ? &img_small_none : get_stanbybatt_workstatus_img(newvalue));
        // if (data->value_percent >= 100)
        //     lv_image_set_src(b.workstatus_img, &img_small_done);
        // else    
        //     lv_image_set_src(b.workstatus_img, data->BatteryLoad == 0 ? &img_small_none : get_stanbybatt_workstatus_img(newvalue));

        lv_obj_set_style_image_recolor(b.workstatus_img, data->BatteryLoad == 0 ? COLOR_000000 : get_batt_workstatus_color(newvalue), 0);
    }

    if (b.batt_idx_img) {
        lv_obj_set_style_image_recolor(b.batt_idx_img, data->BatteryLoad == 0 ? COLOR_4D4D4D : get_workstatus_border_color(newvalue), 0);
    }

}

/**
 * @brief 更新电池仓内部的电池容器整体布局, 纵向或者横向
 * @param batt_com_idx 电池仓索引 
 * @param layout 电池容器布局
 */
static void update_batt_com_layout(int batt_com_idx)
{
    layout_type_t layout = batt_coms[batt_com_idx].batt_cont_layout;

    // printf("-》》》 current batt com[%d] layout = %d(0-ver)\n", batt_com_idx , layout);

    lv_obj_set_style_pad_column(batt_coms[batt_com_idx].batt_cont, layout == LAYOUT_VER ? 4 : 0, 0);
    lv_obj_set_style_pad_row(batt_coms[batt_com_idx].batt_cont, layout == LAYOUT_VER ? 0 : 4, 0);
    lv_obj_set_grid_dsc_array(batt_coms[batt_com_idx].batt_cont, 
        layout == LAYOUT_VER ? four_grid_dsc : one_grid_dsc, 
        layout == LAYOUT_VER ? one_grid_dsc : two_grid_dsc);

    //清空子对象
    lv_obj_clean(batt_coms[batt_com_idx].batt_cont);
    int idx_max = layout == LAYOUT_VER ? 4 : 2;

    for (int batt_idx = 0; batt_idx < idx_max; batt_idx++)
    {
        batt_t item;

        item.bg = lv_obj_create(batt_coms[batt_com_idx].batt_cont);
        lv_obj_add_style(item.bg, &bg_style, 0);
        lv_obj_set_style_bg_color(item.bg, COLOR_CLEAR, 0);
        lv_obj_set_style_bg_opa(item.bg, LV_OPA_TRANSP, 0);
        // 设置 grid 单元格位置，例如 i=0 -> 第0行第0列，i=1 -> 第0行第1列，i=2 -> 第0行第0列，i=3 -> 第0行第1列
        lv_obj_set_grid_cell(item.bg, 
            LV_GRID_ALIGN_STRETCH,  layout == LAYOUT_VER ? batt_idx : 0, 1, 
            LV_GRID_ALIGN_STRETCH,  layout == LAYOUT_VER ? 0 : batt_idx, 1);
        lv_obj_remove_flag(item.bg, LV_OBJ_FLAG_CLICKABLE);

        //进度条容器
        item.bar_cont = lv_bar_create(item.bg);
        lv_obj_add_style(item.bar_cont, &bg_style, 0);
        lv_obj_set_size(item.bar_cont, 
            layout == LAYOUT_VER ? lv_pct(100) : 13, 
            layout == LAYOUT_VER ? 13 : lv_pct(100));
        lv_obj_align(item.bar_cont, layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_RIGHT_MID, 0, 0);
        lv_obj_set_style_radius(item.bar_cont, 6, 0);
        lv_obj_set_style_pad_all(item.bar_cont, 2, 0);
        lv_obj_set_style_bg_color(item.bar_cont, COLOR_262626, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(item.bar_cont, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_border_width(item.bar_cont, 2, LV_PART_MAIN);
        lv_obj_set_style_border_color(item.bar_cont, COLOR_4D4D4D, LV_PART_MAIN);
        lv_obj_set_style_border_opa(item.bar_cont, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_remove_flag(item.bar_cont, LV_OBJ_FLAG_CLICKABLE);

        //进度条
        item.bar = lv_bar_create(item.bar_cont);
        lv_obj_set_size(item.bar, lv_pct(100), lv_pct(100));
        lv_obj_align(item.bar, LV_ALIGN_CENTER, 0, 0);
        lv_obj_set_style_bg_color(item.bar, COLOR_262626, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(item.bar, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_bg_color(item.bar, COLOR_262626, LV_PART_INDICATOR);
        lv_obj_set_style_bg_opa(item.bar, LV_OPA_COVER, LV_PART_INDICATOR);
        lv_obj_remove_flag(item.bar, LV_OBJ_FLAG_CLICKABLE);
            
        //电池身体
        item.batt_body = lv_obj_create(item.bg);
        lv_obj_add_style(item.batt_body, &bg_style, 0);
        lv_obj_set_size(item.batt_body, layout == LAYOUT_VER ? lv_pct(100) : 110, layout == LAYOUT_VER ? 67 : lv_pct(100));
        lv_obj_set_style_radius(item.batt_body, 5, 0);
        lv_obj_set_style_bg_color(item.batt_body, COLOR_262626, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(item.batt_body, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_set_style_border_width(item.batt_body, 2, LV_PART_MAIN);
        lv_obj_set_style_border_color(item.batt_body, COLOR_4D4D4D, LV_PART_MAIN);
        lv_obj_set_style_border_opa(item.batt_body, LV_OPA_COVER, LV_PART_MAIN);
        lv_obj_align(item.batt_body, layout == LAYOUT_VER ? LV_ALIGN_BOTTOM_MID : LV_ALIGN_LEFT_MID, 0, 0);
        lv_obj_remove_flag(item.batt_body, LV_OBJ_FLAG_CLICKABLE);

        //电池状态图像容器
        lv_obj_t *icon_bg = lv_obj_create(item.batt_body);
        lv_obj_set_size(icon_bg, 26, 30); 
        lv_obj_add_style(icon_bg, &bg_style, 0);
        lv_obj_align(icon_bg, layout == LAYOUT_VER ? LV_ALIGN_TOP_MID : LV_ALIGN_RIGHT_MID, layout == LAYOUT_VER ? 0 : -20, layout == LAYOUT_VER ? 10 : 0);
        lv_obj_set_style_bg_color(icon_bg, COLOR_CLEAR, LV_PART_MAIN);
        lv_obj_set_style_bg_opa(icon_bg, LV_OPA_TRANSP, 0);
        lv_obj_remove_flag(icon_bg, LV_OBJ_FLAG_CLICKABLE);

        item.workstatus_img = lv_image_create(icon_bg);
        lv_obj_set_size(item.workstatus_img, 26, 30);
        lv_image_set_src(item.workstatus_img, &img_small_none);
        lv_obj_set_style_image_recolor_opa(item.workstatus_img, LV_OPA_COVER, 0);
        lv_obj_center(item.workstatus_img);


        //电池索引图像
        item.batt_idx_img = lv_image_create(item.batt_body);
        lv_image_set_src(item.batt_idx_img, batt_idx_imgs[layout == LAYOUT_VER ? batt_idx : batt_idx+4]);
        lv_obj_set_style_image_recolor_opa(item.batt_idx_img, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(item.batt_idx_img, COLOR_4D4D4D, 0);
        lv_obj_align(item.batt_idx_img, layout == LAYOUT_VER ? LV_ALIGN_BOTTOM_MID : LV_ALIGN_CENTER, 
            layout == LAYOUT_VER ? 0 : -26, 
            layout == LAYOUT_VER ? -4 : 0);

        //保存到数据结构体
        batt_coms[batt_com_idx].batts[layout == LAYOUT_VER ? batt_idx : batt_idx+4] = item;
    }
}

/**
 * @brief 更新底部 wifi、phone 信号图标
 * @param icon_i 0-wifi、phone，1-wifi、phone
 * @param is_checked 使能状态
 */
static void update_bottom_sign_status(int icon_i, bool is_checked)
{
    lv_obj_t* icon = lv_obj_get_child(bottom_objs[0], icon_i);
    if (is_checked)
    {
        lv_obj_add_state(icon, LV_STATE_CHECKED);
    }else{
        lv_obj_remove_state(icon, LV_STATE_CHECKED);
    }

}


static void bottom_btn_event(lv_event_t* e) {
    lv_obj_t* current_obj = lv_event_get_target(e);
    int bottom_btn_idx = (int)(intptr_t)lv_obj_get_user_data(current_obj);
    lv_event_code_t code = lv_event_get_code(e);
    // printf("-》》》 bottom_btn_idx=%d\n", bottom_btn_idx);

    if (bottom_btn_idx == 3)
    {
        lv_delete_layout(&stanby_page_layout);

        current_selected_menu_idx = 0;
        menu_page_create();

    }else{
        lv_obj_t* img = lv_obj_get_child(current_obj, 0);
        lv_obj_t* label = lv_obj_get_child(current_obj, 1);

        if (lv_obj_has_state(img, LV_STATE_CHECKED) || (lv_obj_has_state(label, LV_STATE_CHECKED)))
        {
            lv_obj_remove_state(img, LV_STATE_CHECKED);
            lv_obj_remove_state(label, LV_STATE_CHECKED);

            lv_image_set_src(img, stanby_display_btns[bottom_btn_idx].main_img);
            lv_label_set_text(label, GetLanguageString(stanby_display_btns[bottom_btn_idx].main_text));
        }else{
            lv_obj_add_state(img, LV_STATE_CHECKED);
            lv_obj_add_state(label, LV_STATE_CHECKED);

            lv_image_set_src(img, stanby_display_btns[bottom_btn_idx].selected_img);
            lv_label_set_text(label, GetLanguageString(stanby_display_btns[bottom_btn_idx].selected_text));
        }
    }

}

static void show_bar_event(lv_event_t* e) {
    lv_obj_t* current_obj = lv_event_get_target(e);
    int batt_com_idx = (int)(intptr_t)lv_obj_get_user_data(current_obj);

    current_selected_batt_comp_idx = batt_com_idx;

    lv_delete_layout(&stanby_page_layout);
    if (!bar_layout)
    {
        current_selected_bar_idx = -1;
        bar_page_create();
    }

}

void stanby_page_create(void){

    if (stanby_page_layout != NULL && lv_obj_is_valid(stanby_page_layout)) {
        printf("stanby_page_layout != NULL && lv_obj_is_valid\n");
        return;
    }

    if (stanby_page_layout) {
        printf("stanby_page_layout != NULL\n");
        lv_obj_del(stanby_page_layout);
        stanby_page_layout = NULL;
    }


    // btn_indev_group_remove_all_obj();


    stanby_page_layout = lv_obj_create(lv_scr_act());
    lv_obj_set_size(stanby_page_layout, lv_pct(100), lv_pct(100)); 
    lv_obj_align(stanby_page_layout, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_obj_add_style(stanby_page_layout, &bg_style, 0);
    lv_obj_set_style_bg_color(stanby_page_layout, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(stanby_page_layout, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_all(stanby_page_layout, 2, 0);

    //创建 grid 布局： 2*2
    lv_obj_t* batt_com_bg = lv_obj_create(stanby_page_layout);
    lv_obj_set_size(batt_com_bg, lv_pct(100), current_batt_com_cnt == 2 ? 102 : 206); 
    lv_obj_align(batt_com_bg, LV_ALIGN_TOP_LEFT, 0, current_batt_com_cnt == 2 ? 69 : 0);
    lv_obj_add_style(batt_com_bg, &bg_style, 0);
    lv_obj_set_style_bg_color(batt_com_bg, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(batt_com_bg, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_column(batt_com_bg, 2, 0);
    lv_obj_set_style_pad_row(batt_com_bg, 2, 0);
    lv_obj_set_grid_dsc_array(batt_com_bg, two_grid_dsc, current_batt_com_cnt == 2 ? one_grid_dsc : two_grid_dsc);  
    

    for(int batt_com_idx=0; batt_com_idx < current_batt_com_cnt; batt_com_idx++){
        batt_coms[batt_com_idx].bg = lv_obj_create(batt_com_bg);
        lv_obj_add_style(batt_coms[batt_com_idx].bg, &bg_style, 0);
        lv_obj_set_style_bg_color(batt_coms[batt_com_idx].bg, COLOR_1A1A1A, 0);
        lv_obj_set_style_radius(batt_coms[batt_com_idx].bg, 5, 0);
        // 不需要再设置单独的大小，由 grid 拉伸填充
        // 设置 grid 单元格位置，例如 i=0 -> 第0行第0列，i=1 -> 第0行第1列，i=2 -> 第1行第0列，i=3 -> 第1行第1列
        lv_obj_set_grid_cell(batt_coms[batt_com_idx].bg, LV_GRID_ALIGN_STRETCH, batt_com_idx % 2, 1, LV_GRID_ALIGN_STRETCH, batt_com_idx / 2, 1);

        lv_obj_set_user_data(batt_coms[batt_com_idx].bg, (void*)(intptr_t)batt_com_idx);//使用 intptr_t 保证可移植性
        lv_obj_add_event_cb(batt_coms[batt_com_idx].bg, show_bar_event, LV_EVENT_CLICKED, NULL);


        batt_coms[batt_com_idx].light = lv_obj_create(batt_coms[batt_com_idx].bg);
        lv_obj_set_size(batt_coms[batt_com_idx].light, 4, 18);
        lv_obj_add_style(batt_coms[batt_com_idx].light, &bg_style, 0);
        lv_obj_set_style_bg_color(batt_coms[batt_com_idx].light, COLOR_4D4D4D, 0);
        lv_obj_set_style_radius(batt_coms[batt_com_idx].light, 9, 0);
        lv_obj_align(batt_coms[batt_com_idx].light, LV_ALIGN_LEFT_MID, 5, 0);
        lv_obj_remove_flag(batt_coms[batt_com_idx].light, LV_OBJ_FLAG_CLICKABLE);


        batt_coms[batt_com_idx].batt_cont = lv_obj_create(batt_coms[batt_com_idx].bg);
        lv_obj_set_size(batt_coms[batt_com_idx].batt_cont, 140, 92);
        lv_obj_add_style(batt_coms[batt_com_idx].batt_cont, &bg_style, 0);
        lv_obj_set_style_bg_color(batt_coms[batt_com_idx].batt_cont, COLOR_000000, 0);
        lv_obj_set_style_radius(batt_coms[batt_com_idx].batt_cont, 5, 0);
        lv_obj_align(batt_coms[batt_com_idx].batt_cont, LV_ALIGN_LEFT_MID, 14, 0);
        lv_obj_set_style_pad_all(batt_coms[batt_com_idx].batt_cont, 4, 0);
        lv_obj_remove_flag(batt_coms[batt_com_idx].batt_cont, LV_OBJ_FLAG_CLICKABLE);


        //更新电池仓内部的电池布局
        update_batt_com_layout(batt_com_idx);

    }

    bottom_objs[0] = lv_obj_create(stanby_page_layout);
    lv_obj_set_size(bottom_objs[0], 40, 28);
    lv_obj_align(bottom_objs[0], LV_ALIGN_BOTTOM_LEFT, 0, 0);
    lv_obj_add_style(bottom_objs[0], &bg_style, 0);
    lv_obj_set_style_bg_color(bottom_objs[0], COLOR_1A1A1A, 0);
    lv_obj_set_style_radius(bottom_objs[0], 5, 0);

    lv_obj_t *wifi_icon = lv_image_create(bottom_objs[0]);  
    lv_image_set_src(wifi_icon, &img_wifi);
    lv_obj_add_style(wifi_icon, &style_img_selected, LV_STATE_CHECKED);
    lv_obj_set_style_image_recolor_opa(wifi_icon, LV_OPA_COVER, 0);
    lv_obj_set_style_image_recolor(wifi_icon, COLOR_4D4D4D, 0);
    lv_obj_align(wifi_icon, LV_ALIGN_CENTER, -10, 0);

    lv_obj_t *mobile_connection_icon = lv_image_create(bottom_objs[0]);  
    lv_image_set_src(mobile_connection_icon, &img_mobile_con);
    lv_obj_add_style(mobile_connection_icon, &style_img_selected, LV_STATE_CHECKED);
    lv_obj_set_style_image_recolor_opa(mobile_connection_icon, LV_OPA_COVER, 0);
    lv_obj_set_style_image_recolor(mobile_connection_icon, COLOR_4D4D4D, 0);
    lv_obj_align(mobile_connection_icon, LV_ALIGN_CENTER, 10, 0);

    //创建 grid 布局： 4*1
    lv_obj_t* bottom_btn_layout = lv_obj_create(stanby_page_layout);
    lv_obj_set_size(bottom_btn_layout, 273, 28);
    lv_obj_add_style(bottom_btn_layout, &bg_style, 0);
    lv_obj_align(bottom_btn_layout, LV_ALIGN_BOTTOM_RIGHT, 0, 0);
    lv_obj_set_style_bg_color(bottom_btn_layout, COLOR_CLEAR, 0);
    lv_obj_set_style_bg_opa(bottom_btn_layout, LV_OPA_TRANSP, 0);
    lv_obj_set_style_pad_column(bottom_btn_layout, 3, 0);
    lv_obj_set_grid_dsc_array(bottom_btn_layout, four_grid_dsc, one_grid_dsc); 
    

    for(int bottom_btn_idx=0;bottom_btn_idx<4;bottom_btn_idx++){
        lv_obj_t* column_cont = lv_obj_create(bottom_btn_layout);
        lv_obj_add_style(column_cont, &bg_style, 0);
        lv_obj_set_style_bg_color(column_cont, COLOR_1A1A1A, 0); 
        lv_obj_set_style_radius(column_cont, 5, 0);
        if (bottom_btn_idx == 3)
        {
            lv_obj_add_style(column_cont, &style_child_btn_selected, LV_STATE_PRESSED);
        }

         // 设置 grid 单元格位置，例如 i=0 -> 第0行第0列，i=1 -> 第0行第1列，i=2 -> 第0行第0列，i=3 -> 第0行第1列
        lv_obj_set_grid_cell(column_cont, LV_GRID_ALIGN_STRETCH, bottom_btn_idx, 1, LV_GRID_ALIGN_STRETCH, 0, 1);

        lv_obj_t *icon = lv_image_create(column_cont);  
        lv_obj_add_style(icon, &style_img_selected, LV_STATE_CHECKED);
        lv_image_set_src(icon, stanby_display_btns[bottom_btn_idx].main_img);
        lv_obj_set_style_image_recolor_opa(icon, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(icon, bottom_btn_idx == 3 ? COLOR_FFFFFF : COLOR_4D4D4D, 0);
        lv_obj_align(icon, LV_ALIGN_LEFT_MID, 5, 0);

        lv_obj_t *name = lv_label_create(column_cont);
        lv_obj_add_style(name, &style_text_selected, LV_STATE_CHECKED);
        lv_obj_set_style_text_color(name, bottom_btn_idx == 3 ? COLOR_FFFFFF : COLOR_4D4D4D, 0);
        lv_label_set_text(name, GetLanguageString(stanby_display_btns[bottom_btn_idx].main_text));
        lv_obj_set_style_text_font(name, &font_FUSION_PIXEL_PROPORTIONAL_12_2, 0);
        lv_obj_align(name, LV_ALIGN_LEFT_MID, 28, 0);

        lv_obj_set_user_data(column_cont, (void*)(intptr_t)bottom_btn_idx);
        lv_obj_add_event_cb(column_cont, bottom_btn_event, LV_EVENT_CLICKED, NULL);

        bottom_objs[bottom_btn_idx+1] = column_cont;
    }

    //左滑事件增加遮罩,防止触发点击事件
    lv_obj_t* mask = lv_obj_create(stanby_page_layout);
    lv_obj_set_size(mask, lv_pct(100), lv_pct(100));
    lv_obj_add_style(mask, &bg_style, 0);
    lv_obj_set_style_bg_color(mask, COLOR_00B7E5, 0);
    // lv_obj_set_style_bg_color(mask, COLOR_CLEAR, 0);
    // lv_obj_set_style_bg_opa(mask, LV_OPA_TRANSP, 0);
    lv_obj_align(mask, LV_ALIGN_CENTER, 0, 0);
    lv_obj_add_flag(mask, LV_OBJ_FLAG_HIDDEN);

    //更新电池仓电池状态
    for (int i = 0; i < current_batt_com_cnt; i ++)
    {
        layout_type_t layout = batt_coms[i].batt_cont_layout;
        int init = layout == LAYOUT_VER ? 0 : 4;
        int max = layout == LAYOUT_VER ? 4 : 6;
        
        for (int j = init; j < max; j ++)
        {
            update_stanby_batt_workstatus(i, j);
            update_stanby_bar_value(i, j);
        }
    }

    // btn_indev_group_add_obj(stanby_page_layout);
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
    lv_style_set_bg_color(&style_row_selected, COLOR_808080);


    lv_style_init(&style_child_btn_selected);
    lv_style_set_bg_opa(&style_child_btn_selected, LV_OPA_COVER);
    lv_style_set_bg_color(&style_child_btn_selected, COLOR_0066FF);

    lv_style_init(&style_text_selected);
    lv_style_set_text_color(&style_text_selected, COLOR_FFFF00);

    lv_style_init(&style_img_selected);
    lv_style_set_image_recolor(&style_img_selected, COLOR_FFFF00);

    
    lv_style_init(&style_menu_img_selected);
    lv_style_set_image_recolor(&style_menu_img_selected, COLOR_FFFFFF);

    lv_style_init(&style_menu_text_selected);
    lv_style_set_text_color(&style_menu_text_selected, COLOR_FFFFFF);

    menu_fade_init_once();
    menu_slide_from_right_init_once();
    menu_slide_from_left_init_once();
    menu_slide_from_bottom_init_once();

}

void init_data()
{
    //初始菜单值
    static const int32_t init_value[5] = {0, 1, 0, 0, 0};
    lv_memcpy(menu_items_data, init_value, sizeof(menu_items_data));


    //初始化电池仓布局
    for(int batt_com_idx=0; batt_com_idx < current_batt_com_cnt; batt_com_idx++){ 
        batt_components_init(&batt_coms[batt_com_idx]);
        if (batt_com_idx == current_batt_com_cnt-1)
        {
            batt_components_t comp;
            comp.bg = NULL;
            comp.light = NULL;
            comp.batt_cont = NULL;
            comp.batt_cont_layout = LAYOUT_HOR;
            lv_memset(comp.batts, 0, sizeof(comp.batts));
            batt_coms[batt_com_idx] = comp;
        }
    }


    batt_data_t data0 = {
        .TaskState = 1,
        .BatteryLoad = 1,
        .work_status = 0,
        .batt_type = 0xff,
        .value_percent = 59,
        .value_mah = 999,       
        .value_Ohms = 56,
        .value_tempc = 45,
        .value_tempf = 100,
        .value_mWh = 999,
        .vol_mv = 4200,
        .cur_ma = 1010,
        .time_s = 3665, 
    };
    g_batt_data[0] = data0;

    batt_data_t data1 = {
        .TaskState = 0,
        .BatteryLoad = 1,
        .work_status = 0,
        .batt_type = 1,
        .value_percent = 50,
        .value_mah = 999,       
        .value_Ohms = 56,
        .value_tempc = 45,
        .value_tempf = 100,
        .value_mWh = 999,
        .vol_mv = 4200,
        .cur_ma = 1010,
        .time_s = 3665, 
    };
    g_batt_data[1] = data1;

    // batt_data_t data2 = {
    //     .TaskState = 1,
    //     .BatteryLoad = 1,
    //     .work_status = 3,
    //     .batt_type = 1,
    //     .value_percent = 110,
    //     .value_mah = 999,       
    //     .value_Ohms = 56,
    //     .value_tempc = 45,
    //     .value_tempf = 100,
    //     .value_mWh = 999,
    //     .vol_mv = 4200,
    //     .cur_ma = 1010,
    //     .time_s = 3665, 
    // };
    // g_batt_data[2] = data2;

    batt_data_t data10 = {
        .TaskState = 1,
        .BatteryLoad = 1,
        .work_status = 3,
        .batt_type = 1,
        .value_percent = 110,
        .value_mah = 999,       
        .value_Ohms = 56,
        .value_tempc = 45,
        .value_tempf = 100,
        .value_mWh = 999,
        .vol_mv = 4200,
        .cur_ma = 1010,
        .time_s = 3665, 
    };
    g_batt_data[10] = data10;

    // batt_data_t data11 = {
    //     .TaskState = 1,
    //     .BatteryLoad = 0,
    //     .work_status = 3,
    //     .batt_type = 1,
    //     .value_percent = 110,
    //     .value_mah = 999,       
    //     .value_Ohms = 56,
    //     .value_tempc = 45,
    //     .value_tempf = 100,
    //     .value_mWh = 999,
    //     .vol_mv = 4200,
    //     .cur_ma = 1010,
    //     .time_s = 3665, 
    // };
    // g_batt_data[11] = data11;

}

static lv_coord_t gesture_start_x = 0;      //滑动时 x 坐标
static lv_coord_t gesture_start_y = 0;      //滑动时 y 坐标
static bool gesture_has_start = false;      //判断滑动手势是否触发


static void hide_info_layout(void)
{
    lv_obj_t* title_row = lv_obj_get_child(info_layout, 0);
    lv_obj_send_event(lv_obj_get_child(title_row, 0), LV_EVENT_CLICKED, NULL);
}

static void start_bar_close_anim(void)
{
    if (!bar_layout || !lv_obj_is_valid(bar_layout)) {
        printf("!bar_layout || !lv_obj_is_valid(bar_layout)\n");
        bar_layout = NULL;
        return;
    }

    lv_delete_layout(&bar_layout);
    if (!stanby_page_layout)
    {
        current_selected_batt_comp_idx = 0;
        stanby_page_create();
    }

}

/** lvgl 模拟器的 LV_STATE_USER_x 效果不明显, 嵌入式开发板可尝试 **/
static void screen_gesture_test_cb(lv_event_t *e)
{
    // // printf("GESTURE e=%p lv_scr_act=%p info=%p bar=%p t=%lu\n",
    // //    e, lv_scr_act() ,info_layout, bar_layout, lv_tick_get());

    // lv_event_code_t code = lv_event_get_code(e);
    // lv_indev_t *indev = lv_indev_get_act();
    // if (!indev) return;
    // if (code != LV_EVENT_GESTURE) return;

    // lv_dir_t dir = lv_indev_get_gesture_dir(indev);
    // printf("gesture dir=%d \n", dir);

    // // if (dir == LV_DIR_RIGHT)
    // // {
    //     if (info_layout && lv_obj_is_valid(info_layout) ) {
    //         printf("close info_layout\n");
    //         hide_info_layout();

    //     } else if (bar_layout && lv_obj_is_valid(bar_layout)) {
    //         printf("close bar_layout\n");
    //         // //避免动画期间触发点击事件
    //         lv_obj_t* mask = lv_obj_get_child(bar_layout, -1);
    //         lv_obj_remove_flag(mask, LV_OBJ_FLAG_HIDDEN);

    //         lv_obj_t* mask1 = lv_obj_get_child(stanby_page_layout, -1);
    //         lv_obj_remove_flag(mask1, LV_OBJ_FLAG_HIDDEN);

    //         start_bar_close_anim();
    //     }
    // // }

    lv_event_code_t code = lv_event_get_code(e);
    lv_indev_t *indev = lv_indev_active();

    if (!indev) return;

    lv_point_t p;
    lv_indev_get_point(indev, &p);

    // if (code == LV_EVENT_PRESSED) {
    //     gesture_start_x = p.x;
    //     gesture_start_y = p.y;
    //     gesture_has_start = true;
    //     printf("gesture start: %d,%d\n", (int)p.x, (int)p.y);
    // } else if (code == LV_EVENT_GESTURE) {
        lv_dir_t dir = lv_indev_get_gesture_dir(indev);
        printf("gesture dir = %d\n", dir);

        // if (gesture_has_start) {
            lv_coord_t dx = p.x - gesture_start_x;
            lv_coord_t dy = p.y - gesture_start_y;
            printf("gesture dx=%d, dy=%d\n", (int)dx, (int)dy);

            if (info_layout && lv_obj_is_valid(info_layout) ) {
                printf("close info_layout\n");
                hide_info_layout();

            } else if (bar_layout && lv_obj_is_valid(bar_layout)) {
                printf("close bar_layout\n");
                //避免动画期间触发点击事件
                lv_obj_t* mask = lv_obj_get_child(bar_layout, -1);
                lv_obj_remove_flag(mask, LV_OBJ_FLAG_HIDDEN);

                lv_obj_t* mask1 = lv_obj_get_child(stanby_page_layout, -1);
                lv_obj_remove_flag(mask1, LV_OBJ_FLAG_HIDDEN);

                start_bar_close_anim();
            }
        // }
    // }
}

void page_create_v2(void){
    init_style();
    lv_obj_add_style(lv_scr_act(), &bg_style, LV_PART_MAIN);
    lv_obj_set_style_bg_color(lv_scr_act(), COLOR_000000, LV_PART_MAIN);
    lv_obj_remove_flag(lv_scr_act(), LV_OBJ_FLAG_SCROLLABLE);
    
    // printf("Screen W:%ld, H:%ld\n", lv_obj_get_width(lv_scr_act()), lv_obj_get_height(lv_scr_act()));

    // SetCurrentLanguage(LANGUAGE_CN);
    init_data();
    stanby_page_create();

    //屏幕左滑事件
    lv_obj_add_event_cb(lv_scr_act(), screen_gesture_test_cb, LV_EVENT_GESTURE,    NULL);


}
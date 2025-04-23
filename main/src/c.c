/**
 * @file c
 *
 */

/*********************
 *      INCLUDES
 *********************/
#include <stdio.h> // 添加此行以包含 printf 函数的声明
#include "lvgl/lvgl.h"
#include "lv_conf.h"
#include "lvgl/src/font/lv_font.h"// 包含中文字体头文件

#include "asserts/font/lv_font_sourcehan_sans_bhw_16.c"
#include "asserts/font/lv_font_sourcehan_sans_bhw_18.c"
#include "asserts/font/lv_font_sourcehan_sans_rhw_18.c"

 //添加图片文件
#include "asserts/img/img_bind.c"
#include "asserts/img/img_bind_confirm.c"
#include "asserts/img/img_bind_reject.c" 
#include "asserts/img/img_batt_close.c"
#include "asserts/img/img_turn.c"

#include "asserts/img/img_batt_none.c"
#include "asserts/img/img_batt_hightemp.c"
#include "asserts/img/img_batt_lowtemp.c"
#include "asserts/img/img_batt_bar_top.c"
#include "asserts/img/img_batt_bar_top_green.c"

#include "asserts/img/img_type_none.c"
#include "asserts/img/img_type_input.c"
#include "asserts/img/img_type_output.c"
#include "asserts/img/img_type_error.c"
#include "asserts/img/img_type_charge.c"
#include "asserts/img/img_type_discharge.c"
#include "asserts/img/img_type_average.c"
#include "asserts/img/img_type_gather.c"
#include "asserts/img/img_light.c"
#include "asserts/img/img_alert.c"

#include "asserts/img/img_num_none.c"
#include "asserts/img/img_num_0.c"
#include "asserts/img/img_num_1.c"
#include "asserts/img/img_num_2.c"
#include "asserts/img/img_num_3.c"
#include "asserts/img/img_num_4.c"
#include "asserts/img/img_num_5.c"
#include "asserts/img/img_num_6.c"
#include "asserts/img/img_num_7.c"
#include "asserts/img/img_num_8.c"
#include "asserts/img/img_num_9.c"

#include "asserts/img/img_sign_w.c"
#include "asserts/img/img_sign_per.c"

#include "asserts/img/img_port_batt_cv.c"
#include "asserts/img/img_port_batt_cap.c"
#include "asserts/img/img_port_batt_temp.c"
#include "asserts/img/img_port_batt_pwr.c"
#include "asserts/img/img_port_batt_cyc.c"
#include "asserts/img/img_port_num_1.c"
#include "asserts/img/img_port_num_2.c"
#include "asserts/img/img_port_num_3.c"
#include "asserts/img/img_port_num_4.c"
#include "asserts/img/img_port_batt_none.c"
#include "asserts/img/img_port_batt_0.c"

#include "asserts/img/gn/img_cycle.c"
#include "asserts/img/gn/img_signnum_2.c"

/*********************
 *      DEFINES
 *********************/
#define CLEAR lv_color_hex(0x00000000)
#define CONTENT_BG lv_color_hex(0x000000)
#define BIND_OFF lv_color_hex(0x666666)
#define BIND_ON lv_color_hex(0x55ddff)
#define BIND_REJECT lv_color_hex(0xff8080)
#define LINE_BG lv_color_hex(0x333333)
#define CHARGE_SIGN lv_color_hex(0Xb8f09a)
#define INPUT_SIGN lv_color_hex(0x00ff66)
#define ANI_ON lv_color_hex(0x999999)
#define BAR_BG lv_color_hex(0x595959)
#define BAR_GREEN lv_color_hex(0x2aff80)
#define BAR_YELLOW lv_color_hex(0xffcb2a)
#define BAR_RED lv_color_hex(0xff5555)
#define FUNCTION_OFF lv_color_hex(0x464d53)
#define LIST_BG lv_color_hex(0xcccccc)
#define LINE_WIDTH 2
#define BAR_COUNT 3
#define FUNCTION_COUNT 6

// 声明字体
LV_FONT_DECLARE(lv_font_sourcehan_sans_bhw_16);


// 声明图像描述符
LV_IMAGE_DECLARE(bind);
LV_IMAGE_DECLARE(bind_confirm);
LV_IMAGE_DECLARE(bind_reject);
LV_IMAGE_DECLARE(batt_close);
LV_IMAGE_DECLARE(turn);

LV_IMAGE_DECLARE(batt_none);
LV_IMAGE_DECLARE(batt_hightemp);
LV_IMAGE_DECLARE(batt_lowtemp);
LV_IMAGE_DECLARE(batt_bar_top);
LV_IMAGE_DECLARE(batt_bar_top_green);

LV_IMAGE_DECLARE(type_none);
LV_IMAGE_DECLARE(type_input);
LV_IMAGE_DECLARE(type_output);
LV_IMAGE_DECLARE(type_error);
LV_IMAGE_DECLARE(type_charge);
LV_IMAGE_DECLARE(type_average);
LV_IMAGE_DECLARE(type_discharge);
LV_IMAGE_DECLARE(type_gather);
LV_IMAGE_DECLARE(light);
LV_IMAGE_DECLARE(alert);

LV_IMAGE_DECLARE(num_none);
LV_IMAGE_DECLARE(num_0);
LV_IMAGE_DECLARE(num_1);
LV_IMAGE_DECLARE(num_2);
LV_IMAGE_DECLARE(num_3);
LV_IMAGE_DECLARE(num_4);
LV_IMAGE_DECLARE(num_5);
LV_IMAGE_DECLARE(num_6);
LV_IMAGE_DECLARE(num_7);
LV_IMAGE_DECLARE(num_8);
LV_IMAGE_DECLARE(num_9);

LV_IMAGE_DECLARE(sign_w);
LV_IMAGE_DECLARE(sign_per);

LV_IMAGE_DECLARE(port_batt_cv);
LV_IMAGE_DECLARE(port_batt_cap);
LV_IMAGE_DECLARE(port_batt_temp);
LV_IMAGE_DECLARE(port_batt_pwr);
LV_IMAGE_DECLARE(port_batt_cyc);
LV_IMAGE_DECLARE(port_num_1);
LV_IMAGE_DECLARE(port_num_2);
LV_IMAGE_DECLARE(port_num_3);
LV_IMAGE_DECLARE(port_num_4);
LV_IMAGE_DECLARE(port_batt_none);
LV_IMAGE_DECLARE(port_batt_0);

LV_IMAGE_DECLARE(cycle);
LV_IMAGE_DECLARE(signnum_2);

/**********************
 *      TYPEDEFS
 **********************/

typedef struct {
    int oval_bg_i;
    int timer_countdown; // 倒计时
} TimerData;

/**********************
 *  STATIC PROTOTYPES
 **********************/
static void init_style();
const lv_image_dsc_t * get_num_image(int index);
const lv_image_dsc_t * get_port_status_image(int index);
const lv_image_dsc_t * get_batt_bar_status_image(int index);
const lv_image_dsc_t * get_function_image(int index);

//绑定
static void bind_create();

//电池进度条
static void batt_bar_create();
static void update_bar_color(int bar_index, int value);
static void update_bar_per(int bar_index, int value);
static void update_bar_status(int bar_index);
static void bar_ani_add(int click_obj_i);

//端口
static void port_create(bool is_only_batt_bar);
static void update_port_status(int pow_i);
static void update_port_power(int pow_i, int data, int tens_digit_offset);

//功能
static void function_create();
static void update_selected_item();
static void select_item(lv_obj_t * obj);
static void deselect_item(lv_obj_t * obj);

//端口数据
static void add_port_bar();

//任务设置
static void task_page_create();
static void add_value_list(int list_item_count);
// static void add_title(lv_obj_t * parent, const char * fir_text, const char * sec_text);
static void add_tile(lv_obj_t * parent, bool is_task_on);
static void set_setting_btn_children_recolor(lv_obj_t *parent, lv_color_t color, lv_opa_t opa);

//系统设置
// static void setting_page_create();


/**********************
 *  STATIC VARIABLES
 **********************/
static lv_style_t bg_style;
static lv_style_t batt_style;
static lv_style_t bar_indic;
static lv_style_t list_item_style;

//绑定
static lv_timer_t * bind_timer;
static lv_obj_t * bind_ovals[10];
static int bind_timer_countdown = 10;

//端口状态
static lv_obj_t * typeCs_bg;
static int ports_status_data[BAR_COUNT] = {2, 1, 1}; 
static lv_obj_t * ports_status[BAR_COUNT];
const lv_image_dsc_t * port_images[] = {
    &type_none, &type_input, &type_output, &type_error
};

//端口功率
static int powers[BAR_COUNT] = {100, -2, 45};
static lv_obj_t * power_hundred_digit[BAR_COUNT];
static lv_obj_t * power_tens_digit[BAR_COUNT];
static lv_obj_t * power_singles_digit[BAR_COUNT];
static lv_obj_t * power_signs[BAR_COUNT];
// 将图像描述符存储在数组中
const lv_image_dsc_t * num_images[] = {
    &num_0, &num_1, &num_2, &num_3, &num_4,
    &num_5, &num_6, &num_7, &num_8, &num_9,
    &num_none
};

//端口-电池工作状态
static int batt_status_click_count = 0;
static lv_obj_t * batt_work_status;

//进度条
static bool is_only_batt_bar = true;
static int batt_bar_status_data[BAR_COUNT] = {4, 1, 2}; //0无电池, 1未装载, 2高温, 3低温, 4正常
static lv_obj_t * batt_bar_status[BAR_COUNT];
const lv_image_dsc_t * batt_bar_images[] = {
    NULL, &batt_none, &batt_hightemp, &batt_lowtemp
};
static lv_obj_t * bars_per_none[BAR_COUNT];
static lv_obj_t * bars_per_none1[BAR_COUNT];
static lv_timer_t * bar_ani_timers[BAR_COUNT];// 定义一个全局数组来保存定时器指针

static int timer_countdown = 8;
static bool timer_created[BAR_COUNT] = {false, false, false};// 定义标志位数组
static lv_obj_t * ovals_bg[BAR_COUNT];
static lv_obj_t * ovals[BAR_COUNT][3];
static lv_obj_t * bars[BAR_COUNT];
static lv_obj_t * bars_top[BAR_COUNT];
static lv_obj_t * bars_per[BAR_COUNT];
static lv_obj_t * bars_per_sign[BAR_COUNT];

//功能键
const lv_image_dsc_t * function_images[] = { //0充电, 1均电, 2输出, 3集电, 4灯光, 5警告
    &type_charge, &type_average, &type_discharge, &type_gather, &light, &alert
};
const char * function_strs[] = {
    "AVERAGE", "OUTPUT", "GATHER"
};
static lv_obj_t* function_page_1;
static lv_obj_t* function_page_2;
static lv_obj_t* function_img_items[FUNCTION_COUNT];
static lv_obj_t* batt_function_items[3];
static lv_obj_t* function_labels[FUNCTION_COUNT-3];
static lv_obj_t * selected_obj = NULL;  // 当前选中的对象
static int batt_status_index = -1; //0充电, 1均电, 2输出, 3集电
static int selected_index = -1; // 当前选中的索引,0均电, 1输出, 2集电
static bool is_recolored[] = {false, false};  // 标志变量，记录当前颜色状态

//端口电池进度
static float port_batt_data[BAR_COUNT][11] = {
    {1.15, 2.15, 3.15, 4.15, 4000, 42, 107, 80, 2.5, 999, 11},
    {4.15, 4.15, 4.15, 4.15, 4000, 42, 107, -80, -2.5, 999, 1},
    {-1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -2}  // 第三行初始化为 0
};
const lv_image_dsc_t * port_data_images[] = { //0 c/v, 5 cap, 6 temp, 7 pwr, 8 cyc
    &port_batt_cv, &port_num_1, &port_num_2, &port_num_3, &port_num_4, 
    &port_batt_cap, &port_batt_temp, &port_batt_pwr, &port_batt_cyc
};
static lv_obj_t * port_data_label[10] = {NULL}; // 初始化为NULL
static lv_obj_t * port_items[BAR_COUNT];
static lv_obj_t * port_bars[BAR_COUNT];


//任务设置
char * title = "任务设置";
char * task_title[9] = {"电池类型", "任务选择", "容量限制", "扩容充电", "电流设置", "结束条件", "充电电流", "放电电流", "循环次数"};
char * task_btn_value_init[9] = {"NiMh", "充电", "关闭", "关闭", "1.2A", "0.90V", "自动", "0.5A", "5"};
char * task_value_lists_lab[9] = {"自动", "NiMH", "NiCd", "NiZn", "Li-ion", "LiFePO4", "LiHv", "Eneloop", "1.5V Lilon"};   
int task_btn_i = -1 ;
static lv_obj_t * task_btns[9]; //设置按钮
static lv_obj_t * task_btn_value[9]; //设置值

static lv_obj_t * value_list = NULL;  // 设置值列表
static lv_obj_t **value_lists_btn = NULL;  // 讲设置值的列表按钮作为指针
static int value_lists_btn_count = 0;      // 记录当前长度

//系统设置
char * setting_title[9] = {"音量", "完成声音", "屏幕亮度", "容量限制", "扩容充电", "自动熄屏", "语言", "系统信息", "恢复出厂设定"};
char * setting_btn_value_init[7] = {"低", "单次", "高", "关闭", "关闭", "关闭", "简体中文"};
char * setting_value_lists_lab[4] = {"关", "低", "中", "高"};   
int setting_btn_i = -1 ;
bool is_task_on = true;
static lv_obj_t * setting_btns[9];
static lv_obj_t * setting_value_btns[4];
static lv_obj_t * setting_btn_value[7];

lv_obj_t * tile1;
lv_obj_t * tile2;


/********************** 
 *      MACROS
 **********************/

/**********************
 *   GLOBAL FUNCTIONS
 **********************/


void c(void)
{
    init_style();

    lv_obj_set_style_bg_color(lv_scr_act(), CONTENT_BG, LV_PART_MAIN);

    task_page_create();

    // add_port_bar();
    // function_create();

    //绑定页面
    // bind_create();
    // bind_ani_add();

}


/**********************
 *   STATIC FUNCTIONS
 **********************/

static void init_style(){
    lv_style_init(&bg_style);
    lv_style_set_bg_opa(&bg_style, LV_OPA_TRANSP);
    lv_style_set_border_opa(&bg_style, LV_OPA_TRANSP); //边框透明
    lv_style_set_border_width(&bg_style,0);
    lv_style_set_radius(&bg_style, 0);//圆角为0
    lv_style_set_pad_all(&bg_style, 0);//内边距为0
    lv_style_set_margin_all(&bg_style, 0);
    lv_style_set_flex_flow(&bg_style, LV_FLEX_FLOW_ROW);
    lv_style_set_flex_track_place(&bg_style, LV_FLEX_ALIGN_CENTER);
    lv_style_set_layout(&bg_style, LV_LAYOUT_FLEX); //设置灵活布局
    
    lv_style_init(&batt_style);
    lv_style_set_bg_opa(&batt_style, LV_OPA_COVER); // 确保背景完全不透明
    lv_style_set_bg_color(&batt_style,BAR_BG); 
    lv_style_set_border_opa(&batt_style, LV_OPA_TRANSP); //边框透明
    lv_style_set_border_width(&batt_style,0);
    lv_style_set_radius(&batt_style, 0);//圆角为0
    lv_style_set_pad_all(&batt_style, 0);//内边距为0
    lv_style_set_margin_all(&batt_style, 0);

    lv_style_init(&bar_indic);
    lv_style_set_bg_opa(&bar_indic, LV_OPA_COVER);
    lv_style_set_bg_color(&bar_indic, BAR_RED);
    lv_style_set_radius(&bar_indic, 0);

    lv_style_init(&list_item_style);
    lv_style_set_bg_opa(&list_item_style, LV_OPA_TRANSP); 
    lv_style_set_border_opa(&list_item_style, LV_OPA_COVER); //边框透明
    lv_style_set_border_width(&list_item_style,0);
    lv_style_set_radius(&list_item_style, 0);//圆角为0
    lv_style_set_pad_all(&list_item_style, 0);//内边距为0
    lv_style_set_margin_all(&list_item_style, 0);  

}

// 根据索引获取图像描述符, 返回值类型: const lv_image_dsc_t *
const lv_image_dsc_t * get_num_image(int index) {
    if (index >= 0 && index < sizeof(num_images) / sizeof(num_images[0])) { //不超出数组索引
        return num_images[index];
    }
    return NULL; // 索引无效时返回 NULL
}

const lv_image_dsc_t * get_port_status_image(int index) {
    if (index >= 0 && index < sizeof(port_images) / sizeof(port_images[0])) {
        return port_images[index];
    }
    return NULL; // 索引无效时返回 NULL
}

const lv_image_dsc_t * get_batt_bar_status_image(int index) {
    if (index >= 0 && index < sizeof(batt_bar_images) / sizeof(batt_bar_images[0])) {
        return batt_bar_images[index];
    }
    return NULL; // 索引无效时返回 NULL
}

const lv_image_dsc_t * get_function_image(int index) {
    if (index >= 0 && index < sizeof(function_images) / sizeof(function_images[0])) { //不超出数组索引
        return function_images[index];
    }
    return NULL; // 索引无效时返回 NULL
}

const lv_image_dsc_t * get_port_data_image(int index) {
    if (index >= 0 && index < sizeof(port_data_images) / sizeof(port_data_images[0])) { //不超出数组索引
        return port_data_images[index];
    }
    return NULL; // 索引无效时返回 NULL
}

//清屏
static void cleanup_bind_page(){
    /**
     *  先清屏 → 后置空,确保对象先被正确删除，避免悬空指针
     *  先置空 → 后清屏,可能导致短暂的内存泄漏或指针误用
     */
    lv_obj_clean(lv_scr_act()); //清屏 
    lv_memset(bind_ovals, 0, sizeof(bind_ovals));

}

static void cleanup_port_page(){

    lv_obj_clean(lv_scr_act()); //清屏

    //清屏后，重置所有指向已删除对象的指针，避免悬空指针
    typeCs_bg = NULL; 

    for (int i = 0; is_only_batt_bar ? i < 3 : i < 4; i++){
        if(is_only_batt_bar ? i==2 : i==3){
            batt_work_status = NULL;
        }

        if(i < BAR_COUNT){
            ovals_bg[i] = NULL;
            batt_bar_status[i] = NULL;
            bars_top[i] = NULL;
            bars[i] = NULL;
            bars_per_none[i] = NULL;
            bars_per_none1[i] = NULL;
            bars_per[i] = NULL;
            bars_per_sign[i] = NULL;
            if (bar_ani_timers[i]) {
                TimerData * timer_data = (TimerData *)lv_timer_get_user_data(bar_ani_timers[i]);// 获取定时器的用户数据
                if (timer_data) {
                    lv_free(timer_data); // 释放分配的内存
                }
                lv_timer_del(bar_ani_timers[i]); // 删除定时器
                bar_ani_timers[i] = NULL;        // 清除指针
                timer_created[i] = false; // 标记定时器未创建
            }

            if(!is_only_batt_bar){
                ports_status[i] = NULL;
                power_hundred_digit[i] = NULL;
                power_tens_digit[i] = NULL;
                power_singles_digit[i] = NULL;
                power_signs[i] = NULL;
            }
        }

    }

}

static void cleanup_function_page() {
    // 清屏
    lv_obj_clean(lv_scr_act());

    // 重置 function_page_1
    function_page_1 = NULL;
    function_page_2 = NULL;

    // 重置 function_img_items 数组
    lv_memset(batt_function_items, 0, sizeof(batt_function_items));
    lv_memset(function_labels, 0, sizeof(function_labels));
    lv_memset(function_img_items, 0, sizeof(function_img_items));

}

static void cleanup_port_data_page(){

    lv_obj_clean(lv_scr_act()); //清屏

    lv_memset(port_data_label, 0, sizeof(port_data_label));
    lv_memset(port_items, 0, sizeof(port_items));
    lv_memset(port_bars, 0, sizeof(port_bars));
    
}

static void cleanup_value_list() {
    if (value_lists_btn) {
        // 注意：不需要手动删除按钮，lv_obj_del(value_list) 会递归删除子对象
        lv_memset(value_lists_btn, 0, sizeof(value_lists_btn));
        value_lists_btn = NULL;
        value_lists_btn_count = 0;
    }

    if (value_list) {
        lv_obj_del(value_list);
        value_list = NULL;
    }

}

/**********************
 *   任务设置
 **********************/

//helper methods
static void set_setting_btn_children_recolor(lv_obj_t *parent, lv_color_t color, lv_opa_t opa) {
    uint32_t child_id = 0;
    lv_obj_t *child = lv_obj_get_child(parent, child_id);
    while (child != NULL) {
        if (lv_obj_check_type(child, &lv_image_class) || lv_obj_check_type(child, &lv_label_class)) {
            lv_obj_set_style_image_recolor_opa(child, opa, 0);
            lv_obj_set_style_image_recolor(child, color, 0);
            lv_obj_set_style_text_color(child, child_id == 2 ? ANI_ON : color, 0);
        }
        child = lv_obj_get_child(parent, ++child_id);
    }
}

// 事件
static void delayed_cleanup_cb(lv_timer_t * timer) {
    cleanup_value_list();

    lv_obj_set_style_bg_opa(is_task_on ? task_btns[task_btn_i] : setting_btns[setting_btn_i], LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_color(is_task_on ? task_btns[task_btn_i] : setting_btns[setting_btn_i], CLEAR, 0);
    set_setting_btn_children_recolor(is_task_on ? task_btns[task_btn_i] : setting_btns[setting_btn_i], lv_color_white(), LV_OPA_COVER);
    lv_timer_del(timer);       // 删除定时器（单次执行）
}

static void valuelist_item_event_handler(lv_event_t* e){
    if(lv_event_get_code(e) == LV_EVENT_CLICKED){
        lv_obj_t * obj = lv_event_get_target(e);
        int count = is_task_on ? sizeof(task_value_lists_lab)/sizeof(task_value_lists_lab[0]) : 
        sizeof(setting_value_lists_lab)/sizeof(setting_value_lists_lab[0]);
        int value_i = (int)(intptr_t)lv_obj_get_user_data(obj);
        for (int i = 0; i < count; i++){
            if (value_lists_btn[i]) {
                lv_obj_set_style_bg_opa(value_lists_btn[i], value_i == i ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
                lv_obj_set_style_bg_color(value_lists_btn[i], value_i == i ? BAR_GREEN : CLEAR, 0);
            }
        }

        lv_label_set_text(is_task_on ? task_btn_value[task_btn_i] : setting_btn_value[setting_btn_i], 
        is_task_on ? task_value_lists_lab[value_i] : setting_value_lists_lab[value_i]);

        // LV_LOG_USER("is_task_on: %d ", is_task_on); 
        // LV_LOG_USER("count: %d ", count); 
        // LV_LOG_USER("value_i: %d ", value_i); 

        lv_timer_create(delayed_cleanup_cb, 500, NULL);  // 500ms 后触发

    }
}

static void titlelist_item_event_handler(lv_event_t* e){
    if(lv_event_get_code(e) == LV_EVENT_CLICKED){
        lv_obj_t * obj = lv_event_get_target(e);
        int count = is_task_on ? sizeof(task_btns)/sizeof(task_btns[0]) : sizeof(setting_btns)/sizeof(setting_btns[0]);
        lv_obj_t * btns[count];
        int value_count = is_task_on ? sizeof(task_value_lists_lab)/sizeof(task_value_lists_lab[0]) : 
                        sizeof(setting_value_lists_lab)/sizeof(setting_value_lists_lab[0]);;

        int btn_i = (int)(intptr_t)lv_obj_get_user_data(obj);

        if(is_task_on){
            task_btn_i = (int)(intptr_t)lv_obj_get_user_data(obj);
        }else{
            setting_btn_i = (int)(intptr_t)lv_obj_get_user_data(obj);  
        }

        for (int i = 0; i < count; i++){
            if(is_task_on){
                btns[i] = task_btns[i];
            }else{
                btns[i] = setting_btns[i];
            }

            lv_obj_set_style_bg_opa(btns[i], btn_i == i ? LV_OPA_COVER : LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_color(btns[i], btn_i == i ? ANI_ON : CLEAR, 0);
            set_setting_btn_children_recolor(btns[i], btn_i == i ? lv_color_black() : lv_color_white(), LV_OPA_COVER);
        }

        add_value_list(value_count);

        // LV_LOG_USER("is_task_on: %d ", is_task_on); //1代表true , 0代表false
        // LV_LOG_USER("btn_i: %d ", btn_i); 
        // LV_LOG_USER("value_count: %d ", value_count); 

    }
}

static void tileview_scroll_event_handler(lv_event_t *e) {
    lv_obj_t *tv = lv_event_get_target(e);
    lv_obj_t *current_tile = lv_tileview_get_tile_act(tv);
    int tile_id = (intptr_t)lv_obj_get_user_data(current_tile);

    switch (tile_id) {
        case 1: 
        is_task_on = true;
        cleanup_value_list();
        if(setting_btn_i != -1){
            lv_obj_set_style_bg_opa(setting_btns[setting_btn_i], LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_color(setting_btns[setting_btn_i], CLEAR, 0);
            set_setting_btn_children_recolor(setting_btns[setting_btn_i], lv_color_white(), LV_OPA_COVER);
        }        
        if(task_btn_i != -1){
            lv_obj_set_style_bg_opa(task_btns[task_btn_i], LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_color(task_btns[task_btn_i], CLEAR, 0);
            set_setting_btn_children_recolor(task_btns[task_btn_i], lv_color_white(), LV_OPA_COVER);
        }
        LV_LOG_USER("Switched to tile1"); 
        break;
        case 2: 
        is_task_on = false;
        cleanup_value_list();
        if(task_btn_i != -1){
            lv_obj_set_style_bg_opa(task_btns[task_btn_i], LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_color(task_btns[task_btn_i], CLEAR, 0);
            set_setting_btn_children_recolor(task_btns[task_btn_i], lv_color_white(), LV_OPA_COVER);
        }
        if(setting_btn_i != -1){
            lv_obj_set_style_bg_opa(setting_btns[setting_btn_i], LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_color(setting_btns[setting_btn_i], CLEAR, 0);
            set_setting_btn_children_recolor(setting_btns[setting_btn_i], lv_color_white(), LV_OPA_COVER);
        }
        LV_LOG_USER("Switched to tile2"); break;
    }
}


//ui
static void add_value_list(int list_item_count){

    cleanup_value_list();  // 清理旧对象

    // 动态分配按钮数组
    value_lists_btn = (lv_obj_t **)lv_malloc(list_item_count * sizeof(lv_obj_t *));
    value_lists_btn_count = list_item_count;

    int pct_value = (14 * lv_obj_get_height(lv_scr_act())) / 100;
    value_list = lv_list_create(lv_scr_act());
    lv_obj_remove_style_all(value_list); 
    lv_obj_set_size(value_list, lv_pct(50), lv_pct(77));
    lv_obj_add_style(value_list, &list_item_style, 0);
    lv_obj_set_style_bg_opa(value_list, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(value_list, LIST_BG, 0);
    lv_obj_set_scrollbar_mode(value_list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(value_list, LV_ALIGN_BOTTOM_RIGHT, 0, -pct_value);

    for(int i = 0; i < list_item_count; i++) {
        value_lists_btn[i] = lv_button_create(value_list);
        lv_obj_remove_style_all(value_lists_btn[i]); 
        lv_obj_set_size(value_lists_btn[i], lv_pct(85), 40);
        lv_obj_add_style(value_lists_btn[i], &list_item_style, 0);  
        lv_obj_set_style_border_side(value_lists_btn[i], LV_BORDER_SIDE_BOTTOM, 0);
        lv_obj_set_style_border_width(value_lists_btn[i], i == list_item_count-1 ? 0 : 1, 0);
        lv_obj_set_style_border_color(value_lists_btn[i], lv_color_black(), 0); 
        lv_obj_align(value_lists_btn[i], LV_ALIGN_TOP_MID, 0, i * 40); // 40 是按钮高度

        lv_obj_set_user_data(value_lists_btn[i], (void *)(intptr_t)i);
        lv_obj_add_event_cb(value_lists_btn[i], valuelist_item_event_handler, LV_EVENT_CLICKED, NULL);

        lv_obj_t * value_lab = lv_label_create(value_lists_btn[i]);
        lv_obj_set_style_text_font(value_lab, (lv_font_t *)&lv_font_sourcehan_sans_rhw_18, 0); 
        lv_label_set_text(value_lab, is_task_on ? task_value_lists_lab[i] : setting_value_lists_lab[i]);
        lv_obj_center(value_lab);
        
    }  
} 


lv_obj_t * add_img_title(lv_obj_t * parent, bool is_top, const lv_image_dsc_t * img_src,  char * fir_text,  char * sec_text){

    int w = lv_obj_get_width(parent);
    int h = 9 * lv_obj_get_height(parent) / 100;
    lv_obj_t * title_bg = lv_obj_create(parent);
    lv_obj_set_size(title_bg, lv_pct(100), h);
    lv_obj_add_style(title_bg, &batt_style, 0);
    lv_obj_set_style_bg_color(title_bg, CLEAR, 0);

    lv_obj_t * title_sign_bg = lv_obj_create(title_bg);
    lv_obj_set_size(title_sign_bg, lv_pct(12), lv_pct(100));
    lv_obj_add_style(title_sign_bg, &batt_style, 0);
    lv_obj_set_style_bg_color(title_sign_bg, is_top ? lv_color_white() : CLEAR, 0);
    lv_obj_set_align(title_sign_bg, LV_ALIGN_LEFT_MID);

    lv_obj_t * img = lv_img_create(title_sign_bg);
    lv_img_set_src(img, img_src);
    lv_obj_set_style_image_recolor_opa(img, LV_OPA_COVER, 0);
    lv_obj_set_style_image_recolor(img, ANI_ON, 0);
    lv_obj_center(img);

    lv_obj_t * title = lv_label_create(title_bg);
    lv_obj_set_style_text_color(title, lv_color_white(), 0);
    lv_label_set_text(title, fir_text);
    lv_obj_set_style_text_font(title, is_top ? &lv_font_sourcehan_sans_rhw_18 : &lv_font_montserrat_16, 0);
    lv_obj_align_to(title, title_sign_bg, LV_ALIGN_OUT_RIGHT_MID, 10, 0);


    lv_obj_t * title_time = lv_label_create(title_bg);
    lv_obj_set_style_text_color(title_time, lv_color_white(), 0);
    lv_label_set_text(title_time, sec_text);
    lv_obj_set_style_text_font(title_time, is_top ? &lv_font_sourcehan_sans_rhw_18 : &lv_font_montserrat_16, 0);
    if(is_top){
        lv_obj_align(title_time, LV_ALIGN_RIGHT_MID, -5, 0);
    }else{
        lv_obj_align(title_time, LV_ALIGN_LEFT_MID, w/2, 0);
    }

   return  title_bg;
}

static void add_tile(lv_obj_t * parent, bool is_task_on){

    title = is_task_on ? "任务设置" : "系统设置";
    lv_obj_t * item = add_img_title(parent, true, &cycle, title, "00:00:00");
    lv_obj_align(item, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t * title_line = lv_obj_create(item);
    lv_obj_add_style(title_line, &list_item_style, 0);
    lv_obj_set_size(title_line, lv_pct(87), 3);
    lv_obj_set_style_bg_opa(title_line, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(title_line, lv_color_white(), 0);
    lv_obj_set_align(title_line, LV_ALIGN_BOTTOM_RIGHT);

    int pct_value = (14 * lv_obj_get_height(parent)) / 100;
    lv_obj_t * list = lv_list_create(parent);
    lv_obj_set_size(list, lv_pct(100), lv_pct(77));
    lv_obj_add_style(list, &list_item_style, 0);
    lv_obj_set_scrollbar_mode(list, LV_SCROLLBAR_MODE_OFF);
    lv_obj_align(list, LV_ALIGN_BOTTOM_MID, 0, -pct_value);
    
    int count = is_task_on ? sizeof(task_btns)/sizeof(task_btns[0]) : sizeof(setting_btns)/sizeof(setting_btns[0]);
    lv_obj_t * btns[count];

    int value_count = is_task_on ? sizeof(task_btn_value_init)/sizeof(task_btn_value_init[0]) : 
                        sizeof(setting_btn_value_init)/sizeof(setting_btn_value_init[0]);;
    lv_obj_t * values[value_count];                   

    for(int i = 0; i < count; i++) {
        if(is_task_on){
            task_btns[i] = lv_button_create(list);
            btns[i] = task_btns[i]; //复制元素
        }else{
            setting_btns[i] = lv_button_create(list);
            btns[i] = setting_btns[i]; //复制元素
        }
        lv_obj_remove_style_all(btns[i]);  // 移除默认样式
        lv_obj_set_size(btns[i], lv_pct(100), 40);
        lv_obj_add_style(btns[i], &list_item_style, 0);  
        lv_obj_set_style_border_side(btns[i], LV_BORDER_SIDE_BOTTOM, 0);
        lv_obj_set_style_border_width(btns[i], i == 8 ? 0 : 1, 0);
        lv_obj_set_style_border_color(btns[i], LINE_BG, 0);
        lv_obj_align(btns[i], LV_ALIGN_RIGHT_MID, 0, 0);

        lv_obj_set_user_data(btns[i], (void *)(intptr_t)i);
        lv_obj_add_event_cb(btns[i], titlelist_item_event_handler, LV_EVENT_CLICKED, NULL);

        lv_obj_t * title_img = lv_image_create(btns[i]);
        // lv_image_set_src(title_img, LV_SYMBOL_SETTINGS); //LV_SYMBOL_SETTINGS 属于字体符号（属于文本）
        // lv_obj_set_style_text_color(title_img, lv_color_white(), 0);
        lv_image_set_src(title_img, &cycle);
        lv_obj_set_style_image_recolor_opa(title_img, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(title_img, lv_color_white(), 0);
        lv_obj_align(title_img, LV_ALIGN_LEFT_MID, 4, 0);
        
        lv_obj_t * title_lab = lv_label_create(btns[i]);
        lv_obj_set_style_text_font(title_lab, (lv_font_t *)&lv_font_sourcehan_sans_rhw_18, 0); // 设置字体
        lv_label_set_text(title_lab, is_task_on ? task_title[i] : setting_title[i]);
        lv_obj_set_style_text_color(title_lab, lv_color_white(), 0);
        lv_obj_align_to(title_lab, title_img, LV_ALIGN_OUT_RIGHT_MID, 6, 0);

        if(i<value_count){
            if(is_task_on){
                task_btn_value[i] = lv_label_create(btns[i]);
                values[i] = task_btn_value[i];
            }else{
                setting_btn_value[i] = lv_label_create(btns[i]);
                values[i] = setting_btn_value[i];
            }
            lv_obj_set_style_text_font(values[i], (lv_font_t *)&lv_font_sourcehan_sans_rhw_18, 0); 
            lv_obj_set_style_text_color(values[i], ANI_ON, 0);
            lv_label_set_text(values[i], is_task_on ? task_btn_value_init[i] : setting_btn_value_init[i]);
            lv_obj_align(values[i], LV_ALIGN_RIGHT_MID, -4, 0);  
        }
    }

    lv_obj_t * back_btn = lv_btn_create(parent);
    lv_obj_set_size(back_btn, lv_pct(50), lv_pct(13));
    lv_obj_set_style_bg_color(back_btn, lv_color_white(), 0);
    lv_obj_align(back_btn, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    lv_obj_t * back_label = lv_label_create(back_btn);
    lv_label_set_text(back_label, LV_SYMBOL_LEFT );//"返回"
    lv_obj_set_style_text_color(back_label, lv_color_black(), 0);
    // lv_obj_set_style_text_font(back_label, (lv_font_t *)&lv_font_sourcehan_sans_rhw_18, 0);
    lv_obj_center(back_label);

    lv_obj_t * play_btn = lv_btn_create(parent);
    lv_obj_set_size(play_btn, lv_pct(50), lv_pct(13));
    lv_obj_set_style_bg_color(play_btn, lv_color_white(), 0);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_RIGHT, 0, 0);

    lv_obj_t * play_label = lv_label_create(play_btn);
    lv_obj_set_style_text_color(play_label, lv_color_black(), 0);
    // lv_obj_set_style_text_font(play_label, (lv_font_t *)&lv_font_sourcehan_sans_rhw_18, 0);
    lv_label_set_text(play_label, LV_SYMBOL_PLAY);//"重新开始"
    lv_obj_center(play_label); 

}

static void add_tile3(lv_obj_t * parent){
    char * labs[9] = {"镍氢", "00:00:00", "100%", "9999mAh", "4.20V", "1.01A", "56mΩ", "45°C, 100°F", "9999Wh"};
    int h = 9 * lv_obj_get_height(parent) / 100;

    for(int i=0; i<5; i++){
        lv_obj_t * item;
        item = add_img_title(parent, i == 0 , &signnum_2, labs[i*2], i == 4 ? "" : labs[i*2+1]);
        lv_obj_align(item, LV_ALIGN_TOP_MID, 0, i*h);

        if(i!=4){
            lv_obj_t * title_line = lv_obj_create(item);
            lv_obj_add_style(title_line, &list_item_style, 0);
            lv_obj_set_size(title_line, lv_pct(87), i == 0 ? 3 : 1);
            lv_obj_set_style_bg_opa(title_line, LV_OPA_COVER, 0);
            lv_obj_set_style_bg_color(title_line, i == 0 ? lv_color_white() : LINE_BG, 0);
            lv_obj_set_align(title_line, LV_ALIGN_BOTTOM_RIGHT);
        }

    }

    lv_obj_t * tage = lv_obj_create(parent);
    lv_obj_set_size(tage, lv_pct(100), lv_pct(35));
    lv_obj_add_style(tage, &list_item_style, 0);
    lv_obj_set_style_bg_opa(tage, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tage, LINE_BG, 0);
    lv_obj_align(tage, LV_ALIGN_TOP_MID, 0, 5*h+5);

    lv_obj_t * tage_title = lv_obj_create(tage);
    lv_obj_set_size(tage_title, lv_pct(88), lv_pct(18));
    lv_obj_add_style(tage_title, &list_item_style, 0);
    lv_obj_set_style_bg_opa(tage_title, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tage_title, ANI_ON, 0);
    lv_obj_align(tage_title, LV_ALIGN_TOP_RIGHT, 0, 0);

    lv_obj_t * title_label = lv_label_create(tage_title);
    lv_label_set_text(title_label, "UOLTAGE" );
    lv_obj_align(title_label, LV_ALIGN_LEFT_MID, 10, 0);

    lv_obj_t * tage_left = lv_obj_create(tage);
    lv_obj_set_size(tage_left, lv_pct(12), lv_pct(50));
    lv_obj_add_style(tage_left, &list_item_style, 0);
    lv_obj_set_style_bg_opa(tage_left, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tage_left, CLEAR, 0);
    lv_obj_align(tage_left, LV_ALIGN_TOP_LEFT, 0, 0);

    lv_obj_t * tage_left_bottom = lv_obj_create(tage);
    lv_obj_set_size(tage_left_bottom, lv_pct(12), lv_pct(50));
    lv_obj_add_style(tage_left_bottom, &list_item_style, 0);
    lv_obj_set_style_bg_opa(tage_left_bottom, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(tage_left_bottom, CLEAR, 0);
    lv_obj_align(tage_left_bottom, LV_ALIGN_BOTTOM_LEFT, 0, 0);

    for(int i=0; i<2;i++){
        lv_obj_t * tage_img_1 = lv_img_create(i==0 ? tage_left : tage_left_bottom);
        lv_obj_set_style_image_recolor_opa(tage_img_1, LV_OPA_COVER, 0);
        lv_obj_set_style_image_recolor(tage_img_1, ANI_ON, 0);
        lv_image_set_src(tage_img_1, &num_none);
        lv_obj_align(tage_img_1, LV_ALIGN_CENTER, 0, 0);
    }

    
    lv_obj_t * area = lv_obj_create(tage);
    lv_obj_set_size(area, lv_pct(51), lv_pct(43));
    lv_obj_add_style(area, &list_item_style, 0);
    lv_obj_set_style_bg_opa(area, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(area, lv_color_hex(0xff99c2), 0);
    lv_obj_align_to(area, tage_left_bottom, LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
    
    lv_obj_t * area_title = lv_obj_create(tage);
    lv_obj_set_size(area_title, lv_pct(51), 4);
    lv_obj_add_style(area_title, &list_item_style, 0);
    lv_obj_set_style_bg_opa(area_title, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(area_title, lv_color_hex(0xff5599), 0);
    lv_obj_set_style_line_rounded(area_title, true, 0);
    lv_obj_set_style_radius(area_title, 2 , 0);
    lv_obj_align_to(area_title, area, LV_ALIGN_OUT_TOP_MID, 0, 2);


    lv_obj_t * play_btn = lv_btn_create(parent);
    lv_obj_set_size(play_btn, lv_pct(100), lv_pct(13));
    lv_obj_add_style(play_btn, &list_item_style, 0);
    lv_obj_set_style_bg_opa(play_btn, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(play_btn, BAR_GREEN, 0);
    lv_obj_align(play_btn, LV_ALIGN_BOTTOM_MID, 0, 0);

    lv_obj_t * left_label = lv_label_create(play_btn);
    lv_label_set_text(left_label, LV_SYMBOL_UP);
    lv_obj_set_style_text_color(left_label, lv_color_black(), 0);
    lv_obj_align(left_label, LV_ALIGN_LEFT_MID, 4, 0);

    lv_obj_t * center_label = lv_label_create(play_btn);
    lv_label_set_text(center_label, "完成");
    lv_obj_set_style_text_color(center_label, lv_color_black(), 0);
    lv_obj_set_style_text_font(center_label, (lv_font_t *)&lv_font_sourcehan_sans_rhw_18, 0);
    lv_obj_center(center_label);
    
    lv_obj_t * right_img = lv_img_create(play_btn);
    lv_obj_set_style_image_recolor_opa(right_img, LV_OPA_COVER, 0);
    lv_obj_set_style_image_recolor(right_img, lv_color_black(), 0);
    lv_img_set_src(right_img, &cycle);
    lv_obj_align(right_img, LV_ALIGN_RIGHT_MID, -4, 0);
}

/**
 * Create a 1x2 tile view and allow scrolling only in an "-" shape.
 * Demonstrate scroll chaining with a long list that
 * scrolls the tile view when it can't be scrolled further.
 */
static void task_page_create(){

    lv_obj_t * tv = lv_tileview_create(lv_scr_act());
    lv_obj_add_style(tv, &list_item_style, 0);
    lv_obj_set_scrollbar_mode(tv, LV_SCROLLBAR_MODE_OFF);

    // tile1 = lv_tileview_add_tile(tv, 0, 0, (lv_dir_t)(LV_DIR_RIGHT | LV_DIR_BOTTOM)); // Tile1: 只能向右滑到 Tile2
    // tile2 = lv_tileview_add_tile(tv, 1, 0, (lv_dir_t)(LV_DIR_LEFT | LV_DIR_RIGHT));
    // add_tile(tile1, true);
    // add_tile(tile2, false);

    // lv_obj_set_user_data(tile1, (void *)1);  // tile1 的 ID=1
    // lv_obj_set_user_data(tile2, (void *)2);  // tile2 的 ID=2
    // lv_obj_add_event_cb(tv, tileview_scroll_event_handler, LV_EVENT_VALUE_CHANGED, NULL);


    // lv_obj_t * tile3 = lv_tileview_add_tile(tv, 1, 0, (lv_dir_t)(LV_DIR_RIGHT | LV_DIR_TOP));
    // add_tile3(tile3);

    lv_obj_t * tile4 = lv_tileview_add_tile(tv, 0, 0, (lv_dir_t)(LV_DIR_RIGHT | LV_DIR_TOP));


    static int32_t col_dsc[] = {58, 58, 58, 58, LV_GRID_TEMPLATE_LAST};
    static int32_t row_dsc[] = {48, 48, 48, 48, LV_GRID_TEMPLATE_LAST};

    lv_obj_t * grid = lv_obj_create(tile4);
    lv_obj_set_style_grid_column_dsc_array(grid, col_dsc, 0);
    lv_obj_set_style_grid_row_dsc_array(grid, row_dsc, 0);
    lv_obj_set_size(grid, lv_pct(100), 98);
    lv_obj_set_layout(grid, LV_LAYOUT_GRID);
    lv_obj_align(grid, LV_ALIGN_TOP_MID, 0, 0);

    lv_obj_t * label;
    lv_obj_t * obj;
    lv_obj_t * img;
    lv_obj_t * bar;

    uint8_t i;
    for(i = 0; i < 8; i++) {
        uint8_t col = i % 4;
        uint8_t row = i / 4;

        obj = lv_button_create(grid);
        lv_obj_add_style(obj, &list_item_style, 0);
        /*Stretch the cell horizontally and vertically too
         *Set span to 1 to make the cell 1 column/row sized*/
        lv_obj_set_grid_cell(obj, LV_GRID_ALIGN_STRETCH, col, 1,
                             LV_GRID_ALIGN_STRETCH, row, 1);

        label = lv_label_create(obj);
        lv_label_set_text_fmt(label, "c%d, r%d", col, row);
        lv_obj_center(label);

        img = lv_img_create(obj);
        lv_img_set_src(img, &signnum_2);
        lv_obj_align(img, LV_ALIGN_TOP_LEFT, 0, 0);

        bar = lv_bar_create(obj);
        lv_obj_add_style(bar, &list_item_style, 0);
        
        lv_bar_set_value(bar, 50, 0);



    }



}

/**********************
 *   端口数据
 **********************/

//helper methods
// 设置所有子对象的 image_recolor 属性
static void set_children_image_recolor(lv_obj_t *parent, lv_color_t color, lv_opa_t opa) {
    uint32_t child_id = 0;
    lv_obj_t *child = lv_obj_get_child(parent, child_id);
    while (child != NULL) {
        if (lv_obj_check_type(child, &lv_image_class) || lv_obj_check_type(child, &lv_bar_class)) {
            lv_obj_set_style_image_recolor_opa(child, opa, 0);
            lv_obj_set_style_image_recolor(child, color, 0);
            lv_obj_set_style_bg_color(child, color, LV_PART_INDICATOR);
        }
        child = lv_obj_get_child(parent, ++child_id);
    }
}


static void update_port_batt_per(int i){

    update_port_power(i, port_batt_data[i][10], port_batt_data[i][10] < 0 ? -1 : -2);
    lv_image_set_src(ports_status[i], port_batt_data[i][10] < 0 ? &port_batt_none : &port_batt_0);

    if(port_batt_data[i][10] >= 100){
        lv_obj_align(power_tens_digit[i], LV_ALIGN_BOTTOM_MID, -2, 0);
        lv_obj_align_to(power_hundred_digit[i], power_tens_digit[i], LV_ALIGN_OUT_LEFT_BOTTOM, 0, 0);
        lv_obj_align_to(power_singles_digit[i], power_tens_digit[i], LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
        lv_obj_align_to(power_signs[i], power_singles_digit[i], LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0); 
    }
    
    if(port_batt_data[i][10] >= 0){
        lv_obj_clear_flag(port_items[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(port_items[i], LV_OBJ_FLAG_CLICKABLE);
        lv_bar_set_value(port_bars[i], port_batt_data[i][10] >= 100 ? 100 : port_batt_data[i][10], LV_ANIM_OFF); 
    }else{
        lv_obj_add_flag(port_bars[i], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(port_items[i], LV_OBJ_FLAG_CLICKABLE);
        lv_bar_set_value(port_bars[i], 0, LV_ANIM_OFF);
    }

}

static void update_port_label(int portI){
    if(port_batt_data[portI][10] >= 0){
        for(int j=0;j<10;j++){
            char temp[20];
            const char * uint = "";
            switch (j)
            {
            case 0:
            case 1:
            case 2:
            case 3:
                uint = "V";
                break;
            case 4:
                uint = "mAh";
                break;     
            case 5:
                uint = "°C"; // 使用ASCII兼容表示
                lv_obj_set_style_text_font(port_data_label[j], &lv_font_montserrat_18, 0);
                break;   
            case 6:
                uint = "°F"; // 使用ASCII兼容表示
                lv_obj_set_style_text_font(port_data_label[j], &lv_font_montserrat_18, 0);
                break; 
            case 7:
                uint = "W";
                break; 
            case 8:
                uint = "A";
                break;                 
            default:
                break;
            }

            if(j < 4) {  // 需要2位小数的值
                lv_snprintf(temp, sizeof(temp), "%.2f%s", port_batt_data[portI][j], uint);
            } else if(j == 8){  // 整数值
                lv_snprintf(temp, sizeof(temp), "%.1f%s", port_batt_data[portI][j], uint);
            }else{
                lv_snprintf(temp, sizeof(temp), "%.0f%s", port_batt_data[portI][j], uint);
            }
            // printf("Label %d: Formatted text: %s\n", j, temp);  // 检查格式化后的字符串
            if(port_data_label[j]) {
                lv_label_set_text(port_data_label[j], temp);
            }
        }
    }
}


// 事件
static void port_batt_click_event_cb(lv_event_t* e){
    lv_event_code_t code = lv_event_get_code(e); // 获取事件类型
    if (code == LV_EVENT_CLICKED) {
        lv_obj_t * obj = lv_event_get_target(e);
        int i = (int)(intptr_t)lv_obj_get_user_data(obj); // 获取用户数据
        // LV_LOG_USER("batt index: %d", i);

        for (int portI = 0; portI < BAR_COUNT; portI++){
            set_children_image_recolor(port_items[portI], i == portI ? lv_color_white() : LINE_BG, LV_OPA_COVER);
        }
        update_port_label(i);
    }
}


//ui
static void add_port_bar(){

    lv_obj_t * ports_bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ports_bg, lv_pct(98), lv_pct(25));
    lv_obj_add_style(ports_bg, &bg_style, 0); //bg_style 已设置横向排列, 且子对象 y 轴居中
    lv_obj_set_style_flex_main_place(ports_bg, LV_FLEX_ALIGN_SPACE_AROUND, 0); //横向上的内容首或尾间距为子对象间的一半
    lv_obj_align(ports_bg, LV_ALIGN_BOTTOM_MID, 0, 0);//约束布局

    for (int i = 0; i < BAR_COUNT; i++){
        port_items[i] = lv_obj_create(ports_bg);
        lv_obj_set_size(port_items[i], LV_PCT(27), LV_PCT(72));
        lv_obj_add_style(port_items[i], &batt_style,0);
        lv_obj_set_style_bg_opa(port_items[i], LV_OPA_TRANSP, 0); // 确保背景完全透明
        lv_obj_set_style_bg_color(port_items[i], CLEAR, 0);

        // 设置用户数据,将 int 类型的值转换为 intptr_t 类型，然后再将 intptr_t 类型的值转换为 void* 指针类型。
        lv_obj_set_user_data(port_items[i], (void*)(intptr_t)i);
        lv_obj_add_event_cb(port_items[i], port_batt_click_event_cb, LV_EVENT_ALL, NULL);

        //设置端口状态
        ports_status[i] = lv_image_create(port_items[i]);
        lv_image_set_src(ports_status[i], &port_batt_0);
        lv_obj_align(ports_status[i], LV_ALIGN_TOP_MID, 0, 0);

        port_bars[i] = lv_bar_create(port_items[i]);
        lv_obj_set_size(port_bars[i], 25, 8);
        lv_obj_add_style(port_bars[i], &batt_style, 0);
        lv_obj_add_style(port_bars[i], &bar_indic, LV_PART_INDICATOR);
        
        lv_obj_set_style_bg_opa(port_bars[i], LV_OPA_TRANSP, 0);
        lv_obj_set_style_bg_color(port_bars[i], CLEAR, 0);
        lv_obj_set_style_bg_color(port_bars[i], i == 0 ? lv_color_white() : LINE_BG, LV_PART_INDICATOR);
        lv_bar_set_value(port_bars[i], 0, LV_ANIM_OFF);
        lv_obj_align_to(port_bars[i], ports_status[i], LV_ALIGN_TOP_MID, -2, 2);


        //设置端口功率
        power_hundred_digit[i] = lv_image_create(port_items[i]);
        power_tens_digit[i] = lv_image_create(port_items[i]);
        power_singles_digit[i] = lv_image_create(port_items[i]); 
        power_signs[i] = lv_image_create(port_items[i]);
        lv_image_set_src(power_hundred_digit[i],  get_num_image(1));
        lv_image_set_src(power_signs[i], &sign_per);        

        update_port_batt_per(i);

        set_children_image_recolor(port_items[i], i == 0 ? lv_color_white() : LINE_BG, LV_OPA_COVER);
    }

    lv_obj_t * line = lv_obj_create(lv_scr_act());
    lv_obj_set_size(line, lv_pct(100), 1);
    lv_obj_set_style_bg_opa(line, LV_OPA_COVER, 0);
    lv_obj_set_style_bg_color(line, LINE_BG, 0);
    lv_obj_set_style_border_width(line, 0, 0); // 确保无边框
    lv_obj_align_to(line, ports_bg, LV_ALIGN_OUT_TOP_MID, 0, 0);

    lv_obj_t * data_bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(data_bg, lv_pct(91), lv_pct(74));
    lv_obj_add_style(data_bg, &batt_style, 0);
    lv_obj_set_style_bg_opa(data_bg, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_color(data_bg, CLEAR, 0);
    lv_obj_align(data_bg, LV_ALIGN_TOP_MID, 0, lv_pct(3));//约束布局

    lv_obj_t * obj[6];
    for(int i = 0; i < 6; i++){
        lv_obj_t * img_title;

        obj[i] = lv_obj_create(data_bg);
        lv_obj_set_size(obj[i], lv_pct(100), lv_pct(15));
        lv_obj_add_style(obj[i], &batt_style, 0);
        lv_obj_set_style_bg_opa(obj[i], LV_OPA_TRANSP, 0);
        lv_obj_set_style_bg_color(obj[i], CLEAR, 0);

        if(i == 0){
            lv_obj_align(obj[0], LV_ALIGN_TOP_MID, 0, 0);
        }else{
            lv_obj_align_to(obj[i], obj[i-1] ,LV_ALIGN_OUT_BOTTOM_MID, 0, i == 1 ? 0 : 2);
        }

        if(i!=1){
            img_title = lv_img_create(obj[i]);
            lv_image_set_src(img_title, get_port_data_image(i == 0 ? i : i+3));
            lv_obj_align(img_title, LV_ALIGN_LEFT_MID, 0, 0);
        }

        switch (i)
        {
        case 0:
        case 1:{ //0 -> 0,1; 1 -> 2,3
            for(int j=0;j<2;j++){
                lv_obj_t * img_num = lv_img_create(obj[i]);
                lv_image_set_src(img_num, get_port_data_image(i == 0 ? j+1 : j+3));
                lv_obj_align(img_num, LV_ALIGN_LEFT_MID, 62+82*j, 0); 

                int index = i == 0 ? 0 : 2;
                port_data_label[index + j] = lv_label_create(obj[i]);
                lv_obj_set_style_text_color(port_data_label[index + j], lv_color_white(), 0);
                lv_obj_set_style_text_font(port_data_label[index + j], (lv_font_t *)&lv_font_sourcehan_sans_bhw_18, 0);
                lv_label_set_text(port_data_label[index + j], "4.15V");    
                lv_obj_align_to(port_data_label[index + j], img_num, LV_ALIGN_OUT_RIGHT_MID , 5, 0); 
                      
            }
            break;
        }
        case 2:
        case 5:{//2 -> 4; 5 -> 9
            int index = i == 2 ? 4 : 9;
            port_data_label[index] = lv_label_create(obj[i]);
            lv_obj_set_style_text_color(port_data_label[index], lv_color_white(), 0);
            lv_obj_set_style_text_font(port_data_label[index], (lv_font_t *)&lv_font_sourcehan_sans_bhw_18, 0);
            lv_label_set_text(port_data_label[index], "4000mAh");    
            lv_obj_align(port_data_label[index], LV_ALIGN_LEFT_MID, 62, 0); 
            break;
        }
        case 3:
        case 4:{//3 -> 5,6; 4 -> 7,8
            int index = i == 3 ? 5 : 7;
            for(int j=0;j<2;j++){
                port_data_label[index + j] = lv_label_create(obj[i]);
                lv_obj_set_style_text_color(port_data_label[index+j], lv_color_white(), 0);
                lv_obj_set_style_text_font(port_data_label[index+j], (lv_font_t *)&lv_font_sourcehan_sans_bhw_18, 0);
                lv_label_set_text(port_data_label[index+j], j == 0 ? "-80W" : "-2.5A");    
                lv_obj_align(port_data_label[index+j], LV_ALIGN_LEFT_MID, 62+62*j, 0); 
            }
            break;
        }
        default:
        break;
        }
    }
    
    update_port_label(0); //初始数据 

}


/**********************
 *   功能键
 **********************/

// 事件
static void deselect_timer_cb(lv_timer_t * timer) {
    lv_obj_t * obj = lv_timer_get_user_data(timer);
    deselect_item(obj);  // 执行取消选中操作
    selected_obj = NULL;
    lv_timer_del(timer);  // 删除定时器
    
}

static void turn_event_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e); // 获取事件类型
    if (code == LV_EVENT_CLICKED) {

        selected_index = (selected_index + 1) % 5; 
        LV_LOG_USER("turn select index %d ", selected_index);

        if (selected_index < 3){
            if(selected_index == 0 ){
                // 获取屏幕宽度
                lv_coord_t screen_width = lv_obj_get_width(lv_scr_act());

                // 设置 page2 移出屏幕
                lv_obj_set_x(function_page_2, screen_width);

                // 设置 page1 移入屏幕
                lv_obj_set_x(function_page_1, 0);
            }

            //选中当前对象
            if (selected_obj != batt_function_items[selected_index]) {
                selected_obj = batt_function_items[selected_index];
                select_item(selected_obj);

                lv_timer_t * timer2 = lv_timer_create(deselect_timer_cb, 500, selected_obj);
                lv_timer_set_repeat_count(timer2, 1);  // 只执行一次
            } else {
                selected_obj = NULL;  // 取消选中
            }
        }else{
            if(selected_index == 3 ){
                // 获取屏幕宽度
                lv_coord_t screen_width = lv_obj_get_width(lv_scr_act());

                // 设置 page1 移出屏幕
                lv_obj_set_x(function_page_1, -screen_width);

                // 设置 page2 移入屏幕
                lv_obj_set_x(function_page_2, 0);
            }
            // 选中当前对象
            if (selected_obj != function_img_items[selected_index]) {
                selected_obj = function_img_items[selected_index];
                select_item(selected_obj);

                // 启动定时器，2 秒后执行 deselect_item
                lv_timer_t * timer = lv_timer_create(deselect_timer_cb, 500, selected_obj);
                lv_timer_set_repeat_count(timer, 1);  // 只执行一次
            } else {
                selected_obj = NULL;  // 取消选中
            }           
        }

    }

}

static void function_item_event_cb(lv_event_t* e) {
    lv_event_code_t code = lv_event_get_code(e); // 获取事件类型
    if (code == LV_EVENT_CLICKED) {
        if(selected_index >= 0 && selected_index < sizeof(function_images)/ sizeof(function_images[0])){
            if(selected_index < 3){
                batt_status_index = selected_index+1;
            }else{
                // batt_status_index = 0;
            }
            update_selected_item();
        }else {
            LV_LOG_ERROR("Invalid selected_index: %d", selected_index);
        }
    }else if(code == LV_EVENT_LONG_PRESSED){

        cleanup_function_page();

        LV_LOG_USER("selected_index: %d", selected_index);
        LV_LOG_USER("batt_status_index: %d", batt_status_index);

        is_only_batt_bar = false;
        port_create(false);
        batt_bar_create();

        const lv_image_dsc_t * img_status = get_function_image(batt_status_index);
        lv_image_set_src(batt_work_status, img_status ? img_status : &type_charge);
        batt_status_click_count = batt_status_index;
        selected_index = -1;
    }

}


// helper methods
static void anim_set_zoom(void * obj, int32_t value) {
    lv_obj_set_style_transform_zoom(obj, value, 0);
}

// 定义缩放动画
static void zoom_anim(lv_obj_t * obj, int32_t start, int32_t end) {
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, obj);
    lv_anim_set_values(&a, start, end);  // 设置缩放范围
    lv_anim_set_time(&a, 200);           // 动画时间 200ms
    lv_anim_set_exec_cb(&a, anim_set_zoom);
    lv_anim_start(&a);
}

// 选中效果
static void select_item(lv_obj_t * obj) {
    zoom_anim(obj, 256, 282);  // 从 100% 放大到 120%
}

// 取消选中效果
static void deselect_item(lv_obj_t * obj) {
    zoom_anim(obj, 282, 256);  // 从 120% 恢复到 100%, 256 表示 100% 缩放
}

// 更新选中状态
static void update_selected_item() {
    LV_LOG_USER("selected_index: %d", selected_index);
    LV_LOG_USER("batt status index: %d", batt_status_index);

    if(selected_index >= 0 && selected_index < sizeof(function_labels)/ sizeof(function_labels[0])){
        for (uint8_t i = 0; i < sizeof(function_labels)/ sizeof(function_labels[0]); i++) {
            lv_obj_set_style_text_color(function_labels[i], (i == batt_status_index-1) ? lv_color_white() : FUNCTION_OFF, 0);
            lv_obj_set_style_image_recolor(function_img_items[i], (i == batt_status_index-1) ? BAR_GREEN : FUNCTION_OFF, 0);
        } 
    }else{
        is_recolored[selected_index-3] = !is_recolored[selected_index-3];
        lv_obj_set_style_image_recolor(function_img_items[selected_index],is_recolored[selected_index-3] ? BAR_GREEN : FUNCTION_OFF, 0);
    }

}


//ui
//功能键页面
static void function_create(){
    lv_obj_t *confirm_image = lv_image_create(lv_scr_act());
    lv_image_set_src(confirm_image, &bind_confirm);
    lv_obj_align(confirm_image, LV_ALIGN_TOP_RIGHT, -10, 30);
    lv_obj_add_flag(confirm_image, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(confirm_image, function_item_event_cb , LV_EVENT_ALL, NULL);

    lv_obj_t *turn_image = lv_image_create(lv_scr_act());
    lv_image_set_src(turn_image, &turn);
    lv_obj_align(turn_image, LV_ALIGN_BOTTOM_RIGHT, -10, -30);
    lv_obj_add_flag(turn_image, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(turn_image, turn_event_cb , LV_EVENT_CLICKED, NULL);   

    // 创建滑动视图
    lv_obj_t* tabview = lv_obj_create(lv_scr_act());
    lv_obj_set_size(tabview, lv_pct(100)-8, lv_pct(30));
    lv_obj_add_style(tabview, &batt_style, 0);
    lv_obj_set_style_bg_opa(tabview, LV_OPA_TRANSP, LV_PART_SCROLLBAR);
    lv_obj_set_style_width(tabview, 0, LV_PART_SCROLLBAR);
    lv_obj_set_style_bg_color(tabview, CLEAR, 0);
    lv_obj_align(tabview, LV_ALIGN_CENTER, 0, 0);

    // 创建容器并添加到页面
    function_page_1 = lv_obj_create(tabview);
    lv_obj_set_size(function_page_1, lv_pct(100), lv_pct(100));
    lv_obj_add_style(function_page_1, &bg_style, 0);
    lv_obj_set_style_flex_main_place(function_page_1, LV_FLEX_ALIGN_SPACE_EVENLY, 0);
    lv_obj_align(function_page_1, LV_ALIGN_LEFT_MID, 0, 0);

    function_page_2 = lv_obj_create(tabview);
    lv_obj_set_size(function_page_2, lv_pct(100), lv_pct(100));
    lv_obj_add_style(function_page_2, &batt_style, 0);
    lv_obj_set_style_bg_opa(function_page_2, LV_OPA_TRANSP, 0);
    lv_obj_set_style_bg_color(function_page_2, CLEAR, 0);
    lv_obj_align_to(function_page_2, function_page_1, LV_ALIGN_OUT_RIGHT_MID, 0, 0);

    //创建对象并添加到页面
    for (uint32_t i = 0; i < sizeof(function_images)/ sizeof(function_images[0]); i++) {
        if(i < 3){
            batt_function_items[i] = lv_obj_create(function_page_1);
            lv_obj_set_size(batt_function_items[i], lv_pct(30), lv_pct(100));
            lv_obj_add_style(batt_function_items[i], &batt_style, 0);
            lv_obj_set_style_bg_opa(batt_function_items[i], LV_OPA_TRANSP, 0);
            lv_obj_set_style_bg_color(batt_function_items[i], CLEAR, 0);

            // 创建图片
            function_img_items[i] = lv_img_create(batt_function_items[i]);
            lv_img_set_src(function_img_items[i], get_function_image(i+1));
            lv_obj_set_style_image_recolor(function_img_items[i], FUNCTION_OFF, 0);
            lv_obj_set_style_image_recolor_opa(function_img_items[i], LV_OPA_COVER, 0);
            lv_obj_add_flag(function_img_items[i], LV_OBJ_FLAG_OVERFLOW_VISIBLE); // 设置溢出可见
            lv_obj_align(function_img_items[i], LV_ALIGN_TOP_MID, 0, 0);

            // 创建标签
            function_labels[i] = lv_label_create(batt_function_items[i]);
            lv_obj_set_style_text_font(function_labels[i], &lv_font_sourcehan_sans_bhw_16, 0);
            lv_label_set_text(function_labels[i], function_strs[i]);
            lv_obj_set_style_text_color(function_labels[i], FUNCTION_OFF, 0);
            lv_obj_align(function_labels[i], LV_ALIGN_BOTTOM_MID, 0, 0);
        }else{
            // 创建图片
            function_img_items[i] = lv_img_create(function_page_2);
            lv_img_set_src(function_img_items[i], get_function_image(i+1));
            lv_obj_set_style_image_recolor(function_img_items[i], is_recolored[i-3] ? BAR_GREEN : FUNCTION_OFF, 0);
            lv_obj_set_style_image_recolor_opa(function_img_items[i], LV_OPA_COVER, 0);
            lv_obj_align(function_img_items[i], LV_ALIGN_CENTER, (i==3) ? -30 : 45, 0);
        }
    }

}

/**********************
 *   绑定
 **********************/

// helper methods
static void bind_timer_del(){
    if (bind_timer) {
        LV_LOG_USER("Delete bind_timer");
        //若提前删除定时器, 已分配的 timer_data 需要释放，避免内存泄漏问题
        TimerData * timer_data = (TimerData *)lv_timer_get_user_data(bind_timer);// 获取定时器的用户数据
        if (timer_data) {
            lv_free(timer_data); // 释放分配的内存
        }
        lv_timer_del(bind_timer); // 删除定时器
        bind_timer = NULL; // 将 bind_timer 设置为 NULL,避免重复删除或使用无效指针
    } else {
        LV_LOG_USER("bind_timer is already NULL or deleted");
    }
}

// 事件
static void bind_confirm_event_cb(lv_event_t * e){
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        bind_timer_del();
        cleanup_bind_page(); 
        port_create(true);
        batt_bar_create();
    }
}

static void bind_reject_event_cb(lv_event_t * e){
    bind_timer_del();
    cleanup_bind_page();  
    add_port_bar();
}

static void bind_ani_event_cb(lv_timer_t * timer){
    TimerData * timer_data = (TimerData *)lv_timer_get_user_data(timer); // 获取定时器的用户数据
    if (!timer_data) return; // 确保数据有效
    if(timer_data->timer_countdown > 0){
        int i = bind_timer_countdown - timer_data->timer_countdown;
        lv_obj_set_style_bg_color(bind_ovals[i], BIND_ON, 0);
        // LV_LOG_USER("bind_ovals- %d", i);
        timer_data->timer_countdown--; // 减少倒计时
    } else {
        lv_timer_del(timer); // 删除定时器
        bind_timer = NULL; // 将 bind_timer 设置为 NUL,确保全局变量不再指向一个已经被删除的定时器
        lv_free(timer_data); // 释放分配的内存
    }
}

static void bind_ani_add(){
    // 动态分配 TimerData 结构体内存
    TimerData * oval_data = (TimerData *)lv_malloc(sizeof(TimerData)); 
    if (!oval_data) {
        LV_LOG_USER("Failed to allocate memory for TimerData");
        return;
    }
    oval_data->timer_countdown = bind_timer_countdown; // 初始化倒计时
    // 创建指示点动画定时器并传递用户数据 oval_data,每隔1000ms触发一次
    bind_timer = lv_timer_create(bind_ani_event_cb, 1000, oval_data);

}

//ui
//绑定页面
static void bind_create(){
    lv_obj_t *confirm_img = lv_image_create(lv_scr_act());
    lv_image_set_src(confirm_img, &bind_confirm);
    lv_obj_align(confirm_img, LV_ALIGN_TOP_RIGHT, -10, 30);
    lv_obj_add_flag(confirm_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(confirm_img, bind_confirm_event_cb , LV_EVENT_CLICKED, NULL);

    lv_obj_t *confirm_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(confirm_label, (lv_font_t *)&lv_font_sourcehan_sans_bhw_16, 0);
    lv_obj_set_style_text_color(confirm_label, INPUT_SIGN, 0);
    lv_label_set_text(confirm_label, "CONFIRM");
    lv_obj_align_to(confirm_label, confirm_img, LV_ALIGN_OUT_LEFT_MID, -10, 0);

    lv_obj_t *bind_img = lv_image_create(lv_scr_act());
    lv_image_set_src(bind_img, &bind);
    lv_obj_center(bind_img);

    lv_obj_t * ani = lv_obj_create(lv_scr_act());
    lv_obj_set_size(ani, 70, 5);
    lv_obj_add_style(ani, &bg_style, 0);
    lv_obj_set_style_pad_gap(ani, 2, 0);
    lv_obj_align_to(ani, bind_img, LV_ALIGN_OUT_BOTTOM_MID, 5, 12);

    for(int i=0; i<sizeof(bind_ovals)/sizeof(bind_ovals[0]); i++){
        bind_ovals[i] = lv_obj_create(ani);
        lv_obj_set_size(bind_ovals[i], 4, 4);
        lv_obj_add_style(bind_ovals[i], &batt_style, 0);
        lv_obj_set_style_radius(bind_ovals[i], LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(bind_ovals[i], BIND_OFF, 0);
    }

    lv_obj_t *binding_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(binding_label, (lv_font_t *)&lv_font_sourcehan_sans_bhw_16, 0);
    lv_obj_set_style_text_color(binding_label, lv_color_white(), 0);
    lv_label_set_text(binding_label, "BINDING");
    lv_obj_align_to(binding_label, ani, LV_ALIGN_OUT_BOTTOM_MID, -5, 5);

    lv_obj_t *reject_img = lv_image_create(lv_scr_act());
    lv_image_set_src(reject_img, &bind_reject);
    lv_obj_align(reject_img, LV_ALIGN_BOTTOM_RIGHT, -10, -30);
    lv_obj_add_flag(reject_img, LV_OBJ_FLAG_CLICKABLE);
    lv_obj_add_event_cb(reject_img, bind_reject_event_cb , LV_EVENT_CLICKED, NULL);

    lv_obj_t *reject_label = lv_label_create(lv_scr_act());
    lv_obj_set_style_text_font(reject_label, (lv_font_t *)&lv_font_sourcehan_sans_bhw_16, 0);
    lv_obj_set_style_text_color(reject_label, BIND_REJECT, 0);
    lv_label_set_text(reject_label, "REJECT");
    lv_obj_align_to(reject_label, reject_img, LV_ALIGN_OUT_LEFT_MID, -10, 0);
}

/**********************
 *   端口
 **********************/

// 事件
static void batt_status_changed_event_cb(lv_event_t * e){
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t * obj = lv_event_get_target(e);
        int click_obj_i = (int)(intptr_t)lv_obj_get_user_data(obj); // 获取用户数据

        if(is_only_batt_bar && click_obj_i == 2){
            // lv_obj_clean(typeCs_bg); //保留 typeCs_bg 本身,删除其子对象以及子对象的子对象
            cleanup_port_page();
 
            LV_LOG_USER("pointer: %p", batt_work_status);
            LV_LOG_USER("pointer: %p",  batt_bar_status[1]);
            LV_LOG_USER("pointer: %p",  bars_top[1]);
         
            is_only_batt_bar = false;
            port_create(false);
            batt_bar_create();

        }else{
            if(click_obj_i == 3){
                LV_LOG_USER("batt_status_index: %d", batt_status_index);

                cleanup_port_page();
                function_create();
                update_selected_item();

            }else if(click_obj_i == 2){
                batt_status_click_count++;// 增加点击计数
                ports_status_data[2] = batt_status_click_count % 4;
                if(ports_status_data[2] == 0){
                    powers[0] = -1;
                    update_port_power(2, powers[2], 0);
                }else{
                    powers[0] = batt_status_click_count;
                    update_port_status(2);
                }

                powers[1] = batt_status_click_count*2;
                update_port_power(1, powers[1], 0);

                powers[0] -= batt_status_click_count*10;
                update_port_power(0, powers[0], 0);
                
            }else if(click_obj_i == 1){
                batt_status_click_count++;// 增加点击计数
                
                const lv_image_dsc_t * img_status = get_function_image(batt_status_click_count % 4);
                lv_image_set_src(batt_work_status, img_status);

                //传递更改后的电池工作状态到功能键页面
                batt_status_index = batt_status_click_count % 4; 
                selected_index = batt_status_index-1;
            }
        }
    }
}

//helper methods
static void update_port_power(int pow_i, int data, int tens_digit_offset){

    int ten = data / 10 < 10 ? data/10 : data/10%10 ;
    int single = data % 10;
    const lv_image_dsc_t * ten_image = get_num_image(ten); // 获取十位数的图像描述符
    const lv_image_dsc_t * single_image = get_num_image(single); // 获取个位数的图像描述符

    LV_LOG_USER("pow_i(%d): %d, %d, %d", pow_i , data , ten, single);

    if(data / 100 != 1  && data >= 0 && ten == 0 && single >= 0){
        lv_obj_add_flag(power_tens_digit[pow_i], LV_OBJ_FLAG_HIDDEN);
        lv_image_set_src(power_singles_digit[pow_i],  (single_image) ? single_image : get_num_image(10));
        lv_obj_align(power_singles_digit[pow_i], LV_ALIGN_BOTTOM_MID, 0, 0);
    }else{
        lv_obj_clear_flag(power_tens_digit[pow_i], LV_OBJ_FLAG_HIDDEN); 
        lv_image_set_src(power_tens_digit[pow_i], (ten_image && data >= 0) ? ten_image : get_num_image(10));
        lv_image_set_src(power_singles_digit[pow_i],  (single_image && data >= 0) ? single_image : get_num_image(10));
        lv_obj_update_layout(power_tens_digit[pow_i]); //更改 img 后强制更新布局
    
        int power_ten_digit_width = lv_obj_get_width(power_tens_digit[pow_i]);// 获取power_ten_digit的宽度

        lv_obj_align(power_tens_digit[pow_i], LV_ALIGN_BOTTOM_MID, -(power_ten_digit_width / 2)+tens_digit_offset, 0);
        lv_obj_align_to(power_singles_digit[pow_i], power_tens_digit[pow_i], LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);
    }

    if(data / 100 == 1){
        lv_obj_clear_flag(power_hundred_digit[pow_i], LV_OBJ_FLAG_HIDDEN); 
        lv_obj_align_to(power_hundred_digit[pow_i], power_tens_digit[pow_i], LV_ALIGN_OUT_LEFT_BOTTOM, 0, 0);
    }else{
        lv_obj_add_flag(power_hundred_digit[pow_i], LV_OBJ_FLAG_HIDDEN);
    }

    //符号
    if(data >= 0){
        lv_obj_clear_flag(power_signs[pow_i], LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_add_flag(power_signs[pow_i], LV_OBJ_FLAG_HIDDEN);
    }
    lv_obj_align_to(power_signs[pow_i], power_singles_digit[pow_i], LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0); 

}

static void update_port_status(int pow_i){
    const lv_image_dsc_t * img_status = get_port_status_image(ports_status_data[pow_i]);// 获取图像描述符
    lv_image_set_src(ports_status[pow_i], (img_status) ? img_status : get_port_status_image(0));// 使用图像描述符
}

//ui
static void port_create(bool is_only_batt_bar){
    if(!typeCs_bg){
        typeCs_bg = lv_obj_create(lv_scr_act());
    }
    lv_obj_set_size(typeCs_bg, lv_pct(100), lv_pct(27));
    lv_obj_add_style(typeCs_bg, &bg_style, 0);
    lv_obj_set_style_flex_main_place(typeCs_bg, is_only_batt_bar ? LV_FLEX_ALIGN_SPACE_AROUND : LV_FLEX_ALIGN_CENTER, 0); //主轴上的内容居中对齐
    if(!is_only_batt_bar){
        lv_obj_set_style_border_side(typeCs_bg, LV_BORDER_SIDE_BOTTOM, 0); // 只绘制底部边框
        lv_obj_set_style_border_width(typeCs_bg, LINE_WIDTH, 0);// 边框宽度
        lv_obj_set_style_border_color(typeCs_bg, LINE_BG, 0);// 底部边框颜色 
        lv_obj_set_style_border_opa(typeCs_bg,  LV_OPA_COVER, 0);// 确保边框不透明
    }
    lv_obj_align(typeCs_bg, LV_ALIGN_TOP_MID, 0, 0);//约束布局

    for (int i = 0; is_only_batt_bar ? i < 3 : i < 4; i++){
        lv_obj_t * obj = lv_obj_create(typeCs_bg);
        lv_obj_set_size(obj, is_only_batt_bar ?  LV_PCT(27) : LV_PCT(22), LV_PCT(85));
        lv_obj_add_style(obj, &batt_style,0);
        lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0); // 确保背景完全透明
        lv_obj_set_style_bg_color(obj, CLEAR, 0);

        // 设置用户数据,将 int 类型的值转换为 intptr_t 类型，然后再将 intptr_t 类型的值转换为 void* 指针类型。
        lv_obj_set_user_data(obj, (void*)(intptr_t)i);
        lv_obj_add_event_cb(obj, batt_status_changed_event_cb, LV_EVENT_ALL, NULL);

        if(is_only_batt_bar ? i==2 : i==3){
            //设置电池工作状态
            batt_work_status = lv_image_create(obj);
            lv_image_set_src(batt_work_status, is_only_batt_bar ? &batt_close : &type_charge);
            lv_obj_align(batt_work_status, LV_ALIGN_CENTER, 0, is_only_batt_bar ? 10 : 0);
        }

        if(i < BAR_COUNT){
            if(!is_only_batt_bar){
                //设置端口状态
                ports_status[i] = lv_image_create(obj);
                lv_obj_align(ports_status[i], LV_ALIGN_TOP_MID, 0, 0);
                update_port_status(i);

                //设置端口功率
                power_hundred_digit[i] = lv_image_create(obj);
                lv_image_set_src(power_hundred_digit[i],  get_num_image(1));

                power_tens_digit[i] = lv_image_create(obj);
                power_singles_digit[i] = lv_image_create(obj);
                power_signs[i] = lv_image_create(obj);
                lv_image_set_src(power_signs[i], &sign_w);
                update_port_power(i, powers[i], 0);
            }
        }
    }
}

/**********************
 *   进度条
 **********************/

// 事件
static void ani_timer_event_cb(lv_timer_t * timer) {
    TimerData * timer_data = (TimerData *)lv_timer_get_user_data(timer); // 获取定时器的用户数据
    if (!timer_data) return; // 确保数据有效

    int oval_bg_i = timer_data->oval_bg_i; // 获取用户数据
    timer_data->timer_countdown--; // 减少倒计时 
    
    if(timer_data->timer_countdown >= 0){
        if(ovals_bg[oval_bg_i] ){
            lv_obj_clear_flag(ovals_bg[oval_bg_i], LV_OBJ_FLAG_HIDDEN);
        }
        lv_obj_set_style_bg_color(ovals[oval_bg_i][0],LINE_BG, 0);
        lv_obj_set_style_bg_color(ovals[oval_bg_i][1],LINE_BG, 0);
        lv_obj_set_style_bg_color(ovals[oval_bg_i][2],LINE_BG, 0);

        int i = (timer_countdown - timer_data->timer_countdown) % 4;
        // LV_LOG_USER("ovals- %d : %d , %d", oval_bg_i, timer_data->timer_countdown, i);
        if (i >= 0 && i <= 2){
            lv_obj_set_style_bg_color(ovals[oval_bg_i][i],ANI_ON, 0);
        }
    } else {
        if(ovals_bg[oval_bg_i] ){
            lv_obj_add_flag(ovals_bg[oval_bg_i], LV_OBJ_FLAG_HIDDEN);// 隐藏指示点动画
        }
        lv_timer_del(timer); // 删除定时器
        lv_free(timer_data); // 释放分配的内存
        timer_created[oval_bg_i] = false;
        bar_ani_timers[oval_bg_i] = NULL; // 清除指针

    }
}

static void bar_value_changed_event_cb(lv_event_t * e)
{
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t * obj = lv_event_get_target(e);
        int click_obj_i = (int)(intptr_t)lv_obj_get_user_data(obj); // 获取用户数据
        // LV_LOG_USER("bar_value_changed_event_cb, click_obj_index: %d ", click_obj_i);

        if (click_obj_i >= 0 && click_obj_i < BAR_COUNT) {

            if(batt_bar_status_data[click_obj_i] > 1){
                //增进度条
                int value = lv_bar_get_value(bars[click_obj_i]) + 10;
                lv_bar_set_value(bars[click_obj_i], value, LV_ANIM_ON);
                update_bar_per(click_obj_i, value);

                if(!is_only_batt_bar){
                    bar_ani_add(click_obj_i);
                }
            }
        }
    }
}

static void bar_value_reduced_event_cb(lv_event_t * e){
    if(lv_event_get_code(e) == LV_EVENT_CLICKED) {
        lv_obj_t * obj = lv_event_get_target(e);
        int click_obj_i = (int)(intptr_t)lv_obj_get_user_data(obj); // 获取用户数据
        if (click_obj_i >= 0 && click_obj_i < BAR_COUNT) {

            int value = lv_bar_get_value(bars[click_obj_i]) - 10;
            lv_bar_set_value(bars[click_obj_i], value, LV_ANIM_ON);
            update_bar_per(click_obj_i, value);

            if(click_obj_i == 2){
                //改变电池进度条状态
                batt_status_click_count++;
                batt_bar_status_data[click_obj_i] = batt_status_click_count;
                if(batt_status_click_count > 4){
                    batt_status_click_count = 0;
                }
                LV_LOG_USER(" batt_status_click_count: %d ", batt_status_click_count);
                update_bar_status(click_obj_i);
            }
        }
    }
}


//helper methods
static void bar_ani_add(int click_obj_i){
    // 检查是否已经创建了定时器
    if (timer_created[click_obj_i]) {
        LV_LOG_USER("Timer already created for index %d", click_obj_i);
        return;
    }

    // 动态分配 TimerData 结构体内存
    TimerData * oval_data = (TimerData *)lv_malloc(sizeof(TimerData)); 
    if (!oval_data) {
        LV_LOG_USER("Failed to allocate memory for TimerData");
        return;
    }
    oval_data->oval_bg_i = click_obj_i;
    oval_data->timer_countdown = timer_countdown; // 初始化倒计时

    // 创建指示点动画定时器并传递用户数据 oval_data,每隔1000ms触发一次
    bar_ani_timers[click_obj_i] = lv_timer_create(ani_timer_event_cb, 1000, oval_data);
    timer_created[click_obj_i] = true; // 标记定时器已创建

}

static void update_bar_status(int bar_index){
    int index = batt_bar_status_data[bar_index]; 
    const lv_image_dsc_t * img_status = get_batt_bar_status_image(index);// 获取图像描述符
    lv_image_set_src(batt_bar_status[bar_index], (img_status) ? img_status : batt_bar_images[1]);

    if(index < 2){//无电池或者未装载
        lv_obj_clear_flag(batt_bar_status[bar_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(bars_per_none[bar_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(bars_per_none1[bar_index], LV_OBJ_FLAG_HIDDEN);

        lv_obj_add_flag(bars_top[bar_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(bars[bar_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(bars_per[bar_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(bars_per_sign[bar_index], LV_OBJ_FLAG_HIDDEN);
    }else{
        lv_obj_clear_flag(bars_per[bar_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_clear_flag(bars_per_sign[bar_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(bars_per_none[bar_index], LV_OBJ_FLAG_HIDDEN);
        lv_obj_add_flag(bars_per_none1[bar_index], LV_OBJ_FLAG_HIDDEN); 
        if(index < 4){ 
            lv_obj_clear_flag(batt_bar_status[bar_index], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(bars_top[bar_index], LV_OBJ_FLAG_HIDDEN);
            lv_obj_add_flag(bars[bar_index], LV_OBJ_FLAG_HIDDEN);            
        }else{
            lv_obj_add_flag(batt_bar_status[bar_index], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(bars_top[bar_index], LV_OBJ_FLAG_HIDDEN);
            lv_obj_clear_flag(bars[bar_index], LV_OBJ_FLAG_HIDDEN);
        }
    }
}

static void update_bar_per(int bar_index, int value){
    if(value >= 0 && value <= 100){
        //将无符号 32 位整数格式化为字符串并设置为标签的文本
        lv_label_set_text_fmt(bars_per[bar_index], "%"LV_PRIu32"", value); 
        // 重新对齐 label
        lv_obj_align_to(bars_per_sign[bar_index], bars_per[bar_index], LV_ALIGN_OUT_RIGHT_BOTTOM, 2, -2);

        update_bar_color(bar_index, value);
    }
}

static void update_bar_color(int bar_index, int value){
    if(value >= 0 && value <= 100){
        if (value > 10 && value < 80) {
            lv_obj_set_style_bg_color(bars[bar_index], BAR_YELLOW, LV_PART_INDICATOR);
        } else if (value >= 80 && value <= 100) {
            lv_obj_set_style_bg_color(bars[bar_index], BAR_GREEN, LV_PART_INDICATOR);
        } else {
            lv_obj_set_style_bg_color(bars[bar_index], BAR_RED, LV_PART_INDICATOR);
        }
        lv_obj_set_style_image_recolor_opa(bars_top[bar_index], LV_OPA_COVER, 0);       // 设置不透明度为完全覆盖
        lv_obj_set_style_image_recolor(bars_top[bar_index], (value == 100) ? BAR_GREEN : BAR_BG, 0); // 设置重新着色为红色
    }
}


//ui
static void batt_bar_horizontal_oval_draw(int bar_i, lv_obj_t * parent){
    for(int i=0; i<3; i++){
        ovals[bar_i][i] = lv_obj_create(parent);
        lv_obj_set_size(ovals[bar_i][i], LV_PCT(100), lv_pct(24));
        lv_obj_add_style(ovals[bar_i][i], &batt_style, 0);
        lv_obj_set_style_radius(ovals[bar_i][i], 12, 0);
        lv_obj_set_style_bg_color(ovals[bar_i][i], LINE_BG, 0);
    }
}

static void batt_bar_dot_draw(int bar_i, lv_obj_t * parent){
    for(int i=0; i<3; i++){
        ovals[bar_i][i] = lv_obj_create(parent);
        lv_obj_set_size(ovals[bar_i][i], LV_PCT(58), LV_PCT(24));
        lv_obj_add_style(ovals[bar_i][i], &batt_style, 0);
        lv_obj_set_style_radius(ovals[bar_i][i], LV_RADIUS_CIRCLE, 0);
        lv_obj_set_style_bg_color(ovals[bar_i][i], LINE_BG, 0);
    }
}

static void batt_bar_create(){

    lv_obj_t * batteries_capacity_bg = lv_obj_create(lv_scr_act());
    lv_obj_set_size(batteries_capacity_bg, lv_pct(100), lv_pct(73));
    lv_obj_add_style(batteries_capacity_bg, &bg_style, 0);
    lv_obj_set_style_flex_main_place(batteries_capacity_bg, LV_FLEX_ALIGN_SPACE_AROUND, 0);
    //lv_obj_align_to 只会在调用时计算一次对齐位置，不会动态更新
    lv_obj_align_to(batteries_capacity_bg, typeCs_bg, LV_ALIGN_OUT_BOTTOM_MID, 0, 0); 

    for (int i = 0; i < BAR_COUNT; i++){
        lv_obj_t *obj = lv_obj_create(batteries_capacity_bg);
        lv_obj_set_size(obj, LV_PCT(27), LV_PCT(93));
        lv_obj_add_style(obj, &batt_style,0);
        lv_obj_set_style_bg_opa(obj, LV_OPA_TRANSP, 0); // 确保背景完全透明
        lv_obj_set_style_bg_color(obj, CLEAR, 0);
        lv_obj_set_user_data(obj, (void*)(intptr_t)i);
        lv_obj_add_event_cb(obj, bar_value_reduced_event_cb, LV_EVENT_ALL, NULL); 

        //指示点
        ovals_bg[i] = lv_obj_create(obj); 
        lv_obj_set_size(ovals_bg[i], LV_PCT(17), LV_PCT(13)); 
        lv_obj_add_style(ovals_bg[i], &batt_style,0);
        lv_obj_set_layout(ovals_bg[i], LV_LAYOUT_FLEX);
        lv_obj_set_flex_flow(ovals_bg[i], LV_FLEX_FLOW_COLUMN);
        lv_obj_set_style_flex_main_place(ovals_bg[i], LV_FLEX_ALIGN_SPACE_BETWEEN, 0); 
        lv_obj_set_style_flex_cross_place(ovals_bg[i], LV_FLEX_ALIGN_CENTER, 0);
        lv_obj_set_style_bg_opa(ovals_bg[i], LV_OPA_TRANSP, 0); // 确保背景完全透明
        lv_obj_set_style_bg_color(ovals_bg[i], CLEAR, 0);
        //增加指示点 ui
        if(i==2){
            lv_obj_set_style_flex_track_place(ovals_bg[2], LV_FLEX_ALIGN_CENTER, 0);   
            batt_bar_dot_draw(i, ovals_bg[i]);
        }else{
            batt_bar_horizontal_oval_draw(i, ovals_bg[i]);  
        }
        lv_obj_set_align(ovals_bg[i], LV_ALIGN_TOP_MID);
        lv_obj_add_flag(ovals_bg[i], LV_OBJ_FLAG_HIDDEN);

        //电池
        lv_obj_t *obj_child = lv_obj_create(obj);//电池头
        lv_obj_set_size(obj_child, LV_PCT(78), LV_PCT(66));
        lv_obj_add_style(obj_child, &batt_style,0);
        lv_obj_set_style_bg_opa(obj_child, LV_OPA_TRANSP, 0); // 确保背景完全透明
        lv_obj_set_style_bg_color(obj_child, CLEAR, 0);
        lv_obj_center(obj_child);

        batt_bar_status[i] = lv_image_create(obj_child);
        lv_obj_set_size(batt_bar_status[i], LV_PCT(100), LV_PCT(100));
        lv_image_set_src(batt_bar_status[i], batt_bar_images[1]);
        lv_obj_center(batt_bar_status[i]);

        bars_top[i] = lv_image_create(obj_child);
        lv_image_set_src(bars_top[i],  &batt_bar_top);
        lv_obj_align(bars_top[i], LV_ALIGN_TOP_MID, 0, 1); 

        bars[i] = lv_bar_create(obj_child);//电池进度条
        lv_obj_set_size(bars[i], LV_PCT(100), LV_PCT(95));
        lv_obj_add_style(bars[i], &bar_indic, LV_PART_INDICATOR);
        lv_obj_add_style(bars[i], &batt_style, 0);
        lv_bar_set_value(bars[i], 0, LV_ANIM_OFF);
        lv_obj_align(bars[i], LV_ALIGN_BOTTOM_MID,0,0);
        lv_obj_set_user_data(bars[i], (void*)(intptr_t)i);
        lv_obj_add_event_cb(bars[i], bar_value_changed_event_cb, LV_EVENT_ALL, NULL);  

        //电量百分比
        bars_per_none[i] = lv_image_create(obj);
        lv_image_set_src(bars_per_none[i],  get_num_image(10));
        lv_obj_update_layout(bars_per_none[i]); //强制更新布局
        int num_none = lv_obj_get_width(bars_per_none[i]);// 获取power_ten_digit的宽度
        lv_obj_align(bars_per_none[i], LV_ALIGN_BOTTOM_MID, -(num_none / 2), 0);

        bars_per_none1[i] = lv_image_create(obj);
        lv_image_set_src(bars_per_none1[i],  get_num_image(10)); 
        lv_obj_align_to(bars_per_none1[i], bars_per_none[i], LV_ALIGN_OUT_RIGHT_BOTTOM, 0, 0);


        bars_per[i] = lv_label_create(obj);
        lv_obj_set_style_text_color(bars_per[i], lv_color_white(), 0);
        lv_obj_set_style_text_font(bars_per[i], (lv_font_t *)&lv_font_montserrat_20, 0);
        // %"LV_PRIu32",指在插入一个 uint32_t 类型的整数值
        lv_label_set_text_fmt(bars_per[i], "%"LV_PRIu32"", 0); 
        lv_obj_align(bars_per[i], LV_ALIGN_BOTTOM_MID, 0, 0);   

        bars_per_sign[i] = lv_label_create(obj);
        lv_obj_set_style_text_color(bars_per_sign[i], lv_color_white(), 0);
        lv_obj_set_style_text_font(bars_per_sign[i], (lv_font_t *)&lv_font_montserrat_10, 0);
        lv_label_set_text_fmt(bars_per_sign[i], "%%");
        lv_obj_align_to(bars_per_sign[i], bars_per[i], LV_ALIGN_OUT_RIGHT_BOTTOM, 2, -2);

        update_bar_status(i);

    }
}



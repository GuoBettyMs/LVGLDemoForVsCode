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
#include "chart_process.h"           //嵌入式图表


// typedef struct {
//     uint8_t slave_device_count; //识别到内部电池的工作仓数量
// }ui_data_t;

void page_create_v2(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*PAGE_MAIN_H*/
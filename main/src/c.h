#ifndef C_H
#define C_H

#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"
#include "assets/img/image_defines.h"


void c(void);
void mem_monitor();
void detailed_mem_monitor();  // 声明函数

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*C_H*/
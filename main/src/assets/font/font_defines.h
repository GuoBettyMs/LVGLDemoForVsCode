/**
 * @file font_defines.h
 *
 */


/**
 * 防止头文件重复包含,避免重复定义导致的编译错误
 * #ifndef FONT_DEFINES_H : 如果宏 IMAGE_DEFINES_H 没有定义过，就继续执行下面的代码
 * #define IMAGE_DEFINES_H：定义这个宏，表示这个头文件已经被包含过了
 * 结尾需配对: #endif
 */
#ifndef FONT_DEFINES_H 
#define FONT_DEFINES_H

/**
 * 支持 C++ 调用 C 的代码,确保 C++ 编译器正确链接 C 的函数和变量
 * #ifdef __cplusplus：如果当前是 C++ 编译器（如 g++），则执行下面的 extern "C"
 * extern "C"：告诉 C++ 编译器，这部分代码是用 C 编写的，不要对函数名做 C++ 风格的名称修饰（mangling）
 * 结尾需配对: #ifdef __cplusplus
            }
            #endif
 * 
 */
#ifdef __cplusplus
extern "C" {
#endif

#include "lvgl/lvgl.h"

// 声明字体
LV_FONT_DECLARE(font_04B_20_8);	
LV_FONT_DECLARE(font_isdtyahei_20);	
LV_FONT_DECLARE(font_isdtyahei_18);
LV_FONT_DECLARE(font_bahnschrift_18);	
LV_FONT_DECLARE(font_bahnschrift_20);	


#ifdef __cplusplus
}
#endif

#endif /*FONT_DEFINES_H*/
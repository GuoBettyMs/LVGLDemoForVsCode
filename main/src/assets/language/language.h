/**
 * @file language.h
 * 
 */

#ifndef LANGUAGE_H
#define LANGUAGE_H 

#ifdef __cplusplus
 extern "C" {
#endif


#include <stdint.h>
#include <stdbool.h>
#include "language_config.h"

typedef enum
{
    #define STR_ID
    #include "StrDefines.h"
    STR_MAX // 自动成为最大值
    #undef STR_ID
}StringID;

typedef enum {
    #if LANG_CHINAC_USE
    LANGUAGE_CN,  // 中文ID
    #endif

    #if LANG_ENGLISH_USE
    LANGUAGE_EN,  // 英文ID
    #endif

	LANGUAGE_FCN,
	// LANGUAGE_JPN,
	// LANGUAGE_GER,
	// LANGUAGE_FRE,
    // LANGUAGE_SPA,
	// LANGUAGE_ITA,
	LANGUAGE_MAX,
}Language_TypeDef;

 // 声明嵌套字符串表
extern const char **StringTable[];


/**
 * 获取当前语言
 */
Language_TypeDef getCurrentLanguage(void);

/**
 * 获取当前语言的字符串
 */
const char* getLocalString(StringID id);


/**
 * 设置当前语言
 */
void setCurrentLanguage(Language_TypeDef lang);


#ifdef __cplusplus
 }/*extern "C"*/
#endif
 
#endif  /*LANGUAGE_H*/

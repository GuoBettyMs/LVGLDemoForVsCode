/**
 * @file language.c
 * @brief 多语言文件
 */


#include <stddef.h>  // 定义 NULL 的最标准头文件
#include <stdio.h> // 添加此行以包含 printf 函数的声明
#include "language.h"

Language_TypeDef currentLanguage = LANGUAGE_EN;

#if LANG_ENGLISH_USE
#undef stringTableEn
static const char *stringTableEn[] = {
    #define STR_EN
    #include "StrDefines.h"
    #undef STR_EN
};
#endif

#if LANG_CHINAC_USE
#undef stringTableCn
static const char *stringTableCn[] = {
    #define STR_CN
    #include "StrDefines.h"
    #undef STR_CN
};
#endif

//顺序、长度需与 Language_TypeDef 一致
const char **stringTable[] =
{
    #if LANG_CHINAC_USE
    stringTableCn,
    #endif

    #if LANG_ENGLISH_USE
    stringTableEn,
    #endif

    NULL,// stringTableFCn,
	// stringTableJPN,
	// stringTableGER,
	// stringTableFRE,
    // stringTableSPA,
	// stringTableITA,
};	

Language_TypeDef getCurrentLanguage(void) {
    return currentLanguage;
}

void setCurrentLanguage(Language_TypeDef lang) {

    if (lang >= 0 && lang < LANGUAGE_MAX){
        currentLanguage = lang;
    }else{
        printf("ERROR: Invalid language %d\n", lang);
        // lang = LANGUAGE_EN; // 默认回退英文
    }
    
}


const char* getLocalString(StringID id) {
    // 参数校验
    if (id < 0 || id >= STR_MAX) {
        printf("ERROR: Invalid string ID %d\n", id);
        return "INVALID ID";
    }

    Language_TypeDef lang = getCurrentLanguage();
    printf("Debug: id=%d, lang=%d\n", id, lang);

    // 语言表校验
    if (lang < 0 || lang >= LANGUAGE_MAX) {
        printf("ERROR: Invalid language %d\n", lang);
        return "LANG_UNAVAILABLE";
    }

    const char** currentTable = stringTable[lang];
    if (!currentTable) {
        printf("ERROR: Language table %d not loaded\n", lang);
        return "TABLE MISSING";
    }

    // 字符串存在性检查
    if (!currentTable[id]) {
        printf("WARNING: No text for ID %d in lang %d\n", id, lang);
        return "TEXT MISSING";
    }

    return currentTable[id];
}



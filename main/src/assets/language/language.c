/**
 * @file language.c
 * @brief 多语言文件
 */


#include <stddef.h>  // 定义 NULL 的最标准头文件
#include <stdio.h> // 添加此行以包含 printf 函数的声明
#include "language.h"


#ifndef NULL
#define NULL 0
#endif

#define StringTableCn                NULL



static const char *StringTableEn[] = {
    #define STR_EN
    #include "str_define.h"
};
//
//简体中文
//
#ifdef LAN_CHINAC_EN
#undef StringTableCn
static const char *StringTableCn[] = {
    #define STR_CN
    #include "str_define.h"
};
#endif


const char **StringTable[] =
{
    StringTableEn,
	StringTableCn,
};	



uint8_t ucLaguageIndex = LANGUAGE_EN;

// 设置当前语言
void SetCurrentLanguage(uint8_t lang) {
    if (lang < LANGUAGE_NUMBERS) {
        ucLaguageIndex = lang;
        VerifyLanguageSet();
    }
}

const uint8_t GetCurrentLanguage() {
    return ucLaguageIndex;
}



const char * GetLanguageString(unsigned int StringId)
{
    const char *pMsg = "";

    if(ucLaguageIndex>= LANGUAGE_NUMBERS)
    {
        ucLaguageIndex = (uint32_t)LANGUAGE_EN;
    }

    if(ucLaguageIndex < LANGUAGE_NUMBERS)
    {
        if(NULL != StringTable[LANGUAGE_ENABLE_FLG_EN])
        {
            //valild ID
            if(StringId < sizeof(StringTableEn) / sizeof(const char *))
            {
                pMsg = StringTable[ucLaguageIndex][StringId];
            }
        }
    }
    return(pMsg);
}

// 获取系统支持的语言标志
uint32_t GetSystemLanguageSupportsFLg(void)
{
    uint32_t LanFlg;

    LanFlg = LANGUAGE_ENABLE_FLG_EN;
	
#ifdef   LAN_CHINAC_EN           //寰疯
    LanFlg |= LANGUAGE_ENABLE_FLG_CN;
#endif	
    return(LanFlg);
}

// 验证并确保语言设置有效
void VerifyLanguageSet(void)
{
    uint32_t i;
    uint32_t EnableLanguageIndex;
    int32_t CurLanguageIndex;

    EnableLanguageIndex = 0;
    CurLanguageIndex = -1;

    (void)i;
    (void)EnableLanguageIndex;
    (void)CurLanguageIndex;
//    for(i=0;i < LANGUAGE_NUMBERS;i++)
//    {
//        if(g_LVGUIPara.LanguageEnableFlg & (1 << i))
//        {
//            EnableLanguageIndex = i;
//
//            if(i == g_LVGUIPara.LaguageIndex)
//            {
//                CurLanguageIndex = i;
//            }
//        }
//    }

    if(-1 == CurLanguageIndex)
    {
//        if(g_LVGUIPara.LanguageEnableFlg & LANGUAGE_ENABLE_FLG_EN)
//        {
//            g_LVGUIPara.LaguageIndex = LANGUAGE_EN;
//        }
//        else
//        {
//            g_LVGUIPara.LaguageIndex = EnableLanguageIndex;
//        }
    }
}


// 获取支持的语言数量
uint32_t GetCurLanguageNumbers(void)
{
    uint32_t i;
    uint32_t Numbers;

    Numbers = 0;
    (void)i;
//    for(i=0;i < LANGUAGE_NUMBERS;i++)
//    {
//        if(g_LVGUIPara.LanguageEnableFlg & (1 << i))
//        {
//            Numbers++;
//        }
//    }

    return(Numbers);
}


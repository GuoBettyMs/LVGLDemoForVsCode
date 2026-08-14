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
//#define   LAN_LATIN_ONLY


#define   LAN_CHINAC_EN             //简体中�?
#define   LAN_ENGLISH_EN            //英文
#define	  LANGUAGE_NUMBERS      2

enum LanguageFlg_t
{
    LANGUAGE_ENABLE_FLG_EN = (1 << 0),
    LANGUAGE_ENABLE_FLG_CN = (1 << 1),

};

enum Language_TypeDef
{
	LANGUAGE_EN = 0,
	LANGUAGE_CN,

};

typedef enum
{
    #define STR_ID
    #include "str_define.h"
}LanguageString_t;


extern const char **StringTable[];
const char * GetLanguageString(unsigned int StringId);

#ifdef __cplusplus
 extern "C" {
#endif

void VerifyLanguageSet(void);
uint32_t GetSystemLanguageSupportsFLg(void);
bool SetLanguageSupports(uint32_t LanguageFlg);
uint32_t GetLanguageSupportsNumbers(void);
uint32_t GetCurLanguageNumbers(void);
uint32_t GetLanguageSupports(void);
	 
/**
* 获取当前语言
*/	 
const uint8_t GetCurrentLanguage();
	 
/**
* 设置当前语言
*/	 
void SetCurrentLanguage(uint8_t lang); 

#ifdef __cplusplus
 }
#endif
 
#endif



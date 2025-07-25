/**
 * @file StrDefines.h
 * 字符串定义
 */

 
#ifdef __cplusplus
 extern "C" {
#endif

#ifdef STR_ID
    #undef LANGUAGE_STR_DEF
    #define LANGUAGE_STR_DEF(id, strCN,strEn)   id
#endif

#ifdef STR_EN
    #undef LANGUAGE_STR_DEF
    #define LANGUAGE_STR_DEF(id, strCN, strEn)   strEn
#endif

#ifdef STR_CN
    #undef LANGUAGE_STR_DEF
    #define LANGUAGE_STR_DEF(id, strCN, strEn)   strCN
#endif

LANGUAGE_STR_DEF(STR_NIMH,	"镍氢", "NiMH"),
LANGUAGE_STR_DEF(STR_NICD,	"镍镉", "NiCd"),
LANGUAGE_STR_DEF(STR_NIZN,	"镍锌", "NiZn"),
LANGUAGE_STR_DEF(STR_LIION,	"锂离子", "Li-ion"),
LANGUAGE_STR_DEF(STR_LIFE,	"磷酸铁锂", "LiFe"),
LANGUAGE_STR_DEF(STR_LIHV,	"高压锂离子", "LiHv"),
LANGUAGE_STR_DEF(STR_ENELOOP,	"松下镍氢", "Eneloop"),
LANGUAGE_STR_DEF(STR_LIION_1,	"1.5V 锂电池", "1.5V Li-ion"),

LANGUAGE_STR_DEF(STR_TASK,	"任务设置", "TASK"),
LANGUAGE_STR_DEF(STR_SETTING, "系统设置", "SETTING"),
LANGUAGE_STR_DEF(STR_BACK,	"返回", "BACK"),
LANGUAGE_STR_DEF(STR_START,	"开始", "START"),

LANGUAGE_STR_DEF(STR_VOLUME, "音量", "Volume"),
LANGUAGE_STR_DEF(STR_FINISH_BEEP,	"完成声音", "Finish beep"),
LANGUAGE_STR_DEF(STR_BRIGHTNESS,	"屏幕亮度", "Brightness"),
LANGUAGE_STR_DEF(STR_TSDK_MRMORY,	"任务记忆", "Task memory"),
LANGUAGE_STR_DEF(STR_AUTO_SCREEN_OFF,	"自动熄屏", "Auto screen off"),
LANGUAGE_STR_DEF(STR_LANGUAGE,	"语言", "Language"),
LANGUAGE_STR_DEF(STR_SYSINFO,	"系统信息", "System Info"),
LANGUAGE_STR_DEF(STR_RESTORE,	"恢复出厂设置", "Restore"),

LANGUAGE_STR_DEF(STR_BATTERY,	"电池类型", "Chemistry"),
LANGUAGE_STR_DEF(STR_TASK_SELECTION, "任务选择", "Task"),
LANGUAGE_STR_DEF(STR_CAPACITY_LIMIT,	"容量限制", "Capacity limit"),
LANGUAGE_STR_DEF(STR_ACTIVATE_CHARGING,	"扩容充电", "Activate charging"),
LANGUAGE_STR_DEF(STR_CURRENT,	"电流设置", "Current"),
LANGUAGE_STR_DEF(STR_CONDITION,	"结束条件", "Condition"),
LANGUAGE_STR_DEF(STR_CHARGE_CURRENT,	"充电电流", "Charge current"),
LANGUAGE_STR_DEF(STR_DISCHARGE_CURRENT,	"放电电流", "Discharge current"),
LANGUAGE_STR_DEF(STR_CYCLES,	"循环次数", "Cycles"),


LANGUAGE_STR_DEF(STR_OFF,	"关闭", "Off"),
LANGUAGE_STR_DEF(STR_ON,	"开启", "On"),
LANGUAGE_STR_DEF(STR_OFF_VOLUME,	"关", "Off"),
LANGUAGE_STR_DEF(STR_LOW,	"低", "Low"),
LANGUAGE_STR_DEF(STR_MID,	"中", "Middle"),
LANGUAGE_STR_DEF(STR_HIG,	"高", "High"),
LANGUAGE_STR_DEF(STR_AUTO,	"自动", "Auto"),

LANGUAGE_STR_DEF(STR_SINGLE,	"单次", "Single"),
LANGUAGE_STR_DEF(STR_REPEAT,	"重复", "Repeat"),

LANGUAGE_STR_DEF(STR_OVERALL, "全局", "Overall"),
LANGUAGE_STR_DEF(STR_MONO, "单通道", "Mono"),

LANGUAGE_STR_DEF(STR_CHARGE, "充电", "Charge"),
LANGUAGE_STR_DEF(STR_ANALYSE, "分析", "Analyse"),
LANGUAGE_STR_DEF(STR_ACTIVATE, "激活", "Activate"),
LANGUAGE_STR_DEF(STR_STORAGE, "存储", "Storage"),
LANGUAGE_STR_DEF(STR_CYCLE, "循环", "Cycle"),
LANGUAGE_STR_DEF(STR_DESTROY, "销毁", "Destroy"),
LANGUAGE_STR_DEF(STR_DISCHARGE, "放电", "Discharge"),

LANGUAGE_STR_DEF(STR_STATE_CHARGE, "充电中", "Charge"),
LANGUAGE_STR_DEF(STR_STATE_ANALYSE, "分析中", "Analyse"),
LANGUAGE_STR_DEF(STR_STATE_ACTIVATE, "激活中", "Activate"),
LANGUAGE_STR_DEF(STR_STATE_STORAGE, "存储中 ", "Storage"),
LANGUAGE_STR_DEF(STR_STATE_CYCLE, "循环中 ", "Cycle"),
LANGUAGE_STR_DEF(STR_STATE_DESTROY, "销毁中", "Destroy"),
LANGUAGE_STR_DEF(STR_STATE_DISCHARGE, "放电中", "Discharge"),
LANGUAGE_STR_DEF(STR_FINISH, "完成", "Finish"),

LANGUAGE_STR_DEF(STR_ENGLISH, "英文", "English"),
LANGUAGE_STR_DEF(STR_CHINESE, "中文", "Chinese"),

LANGUAGE_STR_DEF(STR_NULL, " ", " "),


#undef LANGUAGE_STR_DEF
// #undef STR_EN
// #undef STR_CN
// #undef STR_ID
// #undef STR_FCN
// #undef STR_JPN
// #undef STR_GER
// #undef STR_FRE
// #undef STR_SPA
// #undef STR_ITA
// #undef STR_RUS
// #undef STR_KOR
// #undef STR_ARA
// #undef STR_THA
// #undef STR_POR
// #undef STR_HEB

#ifdef __cplusplus
 }/*extern "C"*/
#endif




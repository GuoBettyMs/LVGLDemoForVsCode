#ifndef C_H
#define C_H

#ifdef __cplusplus
extern "C" {
#endif

/*********************
 *      INCLUDES
 *********************/

#include "lvgl/lvgl.h"
#include "assets/img/image_defines.h"

#include "components/custom_chart/lv_chart_package.h"

#include "type_def.h"
#include "components/custom_chart/custom_chart.h"

/*********************
 *      DEFINES
 *********************/

/**********************
 *      TYPEDEFS
 **********************/

typedef enum {
	ID_DESK_DEFAULT,
	ID_DESK_WORK,
	ID_DESK_VIEW,
	ID_DESK_EXPERT_MODE,
	ID_DESK_NUMS,
}PGDeskID_t;

typedef enum
{
	PAGE_NONE,
	PAGE_LOGO,
	PAGE_DEBUG,
	PAGE_IDLE_WARNING,
	PAGE_ABOUT,
	PAGE_DATA_INPUT,
	PAGE_FSHARE,
	PAGE_SELF_TEST,
	PAGE_DESK,
	PAGE_VER_DESK,
	PAGE_VIEW,
	PAGE_INPUT_SN,
	PAGE_ADJUST,
	PAGE_SELECT,
	PAGE_MSG_BOX,
	PAGE_CONFIRM,
	PAGE_CONFIRM_ERR,
	PAGE_CONFIRM_WARN,
	PAGE_CONFIRM_WARN_EXT,
	PAGE_CONFIRM_TIP,
	PAGE_HERR,
	PAGE_AUTOCHECK,
	PAGE_UPGRADE,
	PAGE_RECORD,
	PAGE_SLAVEUPDATE,
	PAGE_CONNECTION,
	PAGE_CONFIRM_TEST,
	PAGE_TEST,
	PAGE_PRETASK,
	PAGE_DEFAULT_DESK,
	PAGE_AGING_TEST,
	PAGE_SCREEN_PROTECTION,
}PageIndex_t;
typedef enum
{
	tsStandby, // 待机
	tsActivateChging, // 充电激活
	tsCurrentClimbChging, // 充电电流爬升
	tsConstCurChging, // 充电恒流
	tsConstVolChging, // 充电恒压
	tsTrickleChging, // 充电完成
	tsCurrentClimbDischging, // 放电电流爬升
	tsConstCurDischging, // 放电恒流
	tsConstVolDischging, // 放电恒压
    tsDischargDone, // 放电完成
}TaskState_t;

typedef enum {
	acsIdle,
	acsCheck,
	acsStart,
	acsWaitingCharge,
	acsFinish,
	acsBeIntrrpt,
	acsWaiting,
}eAutoCharing;

typedef struct _auto_charging_t {
	eAutoCharing	state;
	uint8_t  channel;
	uint8_t  beepCnt;
	uint32_t time_stamp;
	uint32_t time_end;
	// uint8_t aePrevChargerState[POWER_CHANNEL_ALL];
}AutoCharging_t;

typedef union
{
	struct
	{
		INT16U OutputOverCurrent         : 1 ;  // 输出过流
		INT16U OutputOverVoltage         : 1 ;  // 输出过压
		INT16U InputOverVoltage          : 1 ;  // 输入过压
		INT16U InputLowVoltage           : 1 ;  // 输入欠压
		INT16U InputVoltageUnstable      : 1 ;  // 输入电压不稳
		INT16U BatteryOverVoltage        : 1 ;  // 电池过压
		INT16U BatteryReverse            : 1 ;  // 电池接反
		INT16U OverTemperature           : 1 ;  // 温度过高
		INT16U LowTemperature            : 1 ;  // 温度过低
		INT16U TimeOut                   : 1 ;  // 充电超时
		INT16U ConnectionBroken          : 1 ;  // 连接状态破坏
		INT16U NoBlnPortCharge			 : 1 ;  // 不支持平衡口充电
		INT16U CellsNode				 : 1 ;  // 电池节点连接错误
		INT16U NoBattery 				 : 1 ;  // 输出未连电池
		INT16U CapacityLimit             : 1 ;  // 容量限制
		INT16U Unknown                   : 1 ;  // 未知错误
	};
	INT16U Errors;
}eBatteryManagerError;

typedef struct _channel_data_t {
	eBatteryManagerError  BatteryManagerError;
	INT8U 	TaskState;
	INT8U	WorkState;
	INT8U   ChargerState;
	INT8U 	WorkTask;
	INT8S   Tmp;				      	  // 温度
	INT8S   DscTmp;
	INT8U   BatType;
	INT8U   Percent;
	INT8U   TotalCount;
	INT8U   RemainCount;
	INT16U  InputVoltagemV;				  // 输入电压
	INT16U  InputCurrentmA;				  // 输入电流
	INT16U  OutputVoltagemV;				  // 输出电压
	INT16U  OutputVoltagemV_chart;				  // 输出电压
	INT16S  OutputCurrentmA;				  // 输出电流
	INT32S  OutputPowermW;				  // 输出功率
	INT32U 	InputPowermW;				  // 输入功率
	INT32U  Impedence;
	INT32U  RunTime;
	INT32U  ChargeCapacity;
	INT32U  DischargeCapacity;
}ChannelShowData_t;

typedef enum 
{
    POWER_CHANNEL_1 = 0,
    POWER_CHANNEL_2,
    POWER_CHANNEL_3,
    POWER_CHANNEL_4,
    POWER_CHANNEL_5,
    POWER_CHANNEL_6,
	POWER_CHANNEL_ALL,
}ePowerChannel;

typedef struct _pg_dsc_t {
	uint8_t  direction;
	uint8_t  sub_id;
	uint8_t  cur_channel;
	uint8_t  start_channel;
	uint8_t  channel_max;
	uint32_t ticks;

	struct {
		lv_point_t pos;
	}disp_dsc;

	struct {
		lv_coord_t cont_width;
		lv_coord_t cont_height;
	}info_dsc;
}PageDsc_t;

typedef struct _pg_desk_data_t {
	INT8U WorkState;
	union {
		struct {
			uint8_t WorkState;
			uint8_t ChartDisplaychannel;
			uint8_t WorkDisplayChannel;
			uint8_t BatteryType;
		}m_workInfo;
	};
	bool abSelected[6];
	uint8_t aucPercent[POWER_CHANNEL_ALL];
	ChannelShowData_t	tUIData[POWER_CHANNEL_ALL];
}PGDeskData_t;

typedef enum _work_state_t{
	wsBattUnload,
	wsBattLoad,
	wsWorking,
	wsWorkDone,
	wsWarn,
	wsError,
}WorkState_t;

typedef enum
{
    csBattUnload,   // 未装电池
    csAdatarLimit,  // 适配器受限
    csBattLoad,     // 已装电池
    csBatteryRev,   // 电池反接
    csCharging,     // 充电中
    csChargeDone,   // 充电完成
    csDscharging,   // 放电中
    csDschargeDone, // 放电完成
    csStoraging,    // 存储中
    csStorageDone,  // 存储完成
    csCycling,      // 循环中
    csCycleDone,    // 循环完成
    csAnalysising,  // 分析中
    csAnalysisDone, // 分析完成
    csActivating,   // 激活中
    csActivateDone, // 激活完成
    csDestroying,   // 销毁中
    csDestroyDone,  // 销毁完成
    csTaskStop,     // 任务终止
}eChargerState;

/**********************
 *  FUNCTION PROTOTYPES
 **********************/

void c(void);
void mem_monitor();
void detailed_mem_monitor();  // 声明函数

void create_1(void);

#ifdef __cplusplus
} /* extern "C" */
#endif

#endif /*C_H*/
/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#ifndef USER_COMMUNICATION_H
#define USER_COMMUNICATION_H

#ifdef __cplusplus
extern "C" {
#endif

//---------------------------------------------------------------------------
#define SLAVE_DEVICE_COUNT          6
#define	FRAME_HEADER_FLG            0xAA
#define	FRAME_DATA_LEN              255
#define HOST_ADDRESS                0x01      // 上位机地址
#define BIND_ADDRESS                0x00      // 绑定地址
#define UNBIND_ADDRESS              0x0F      // 解绑地址
#define MAX_TRY_COUNT               3         // 最大重试次数

#define CH543_ADDRESS               0x02      // Ch543地址

#define PORT_STATE_IDLE		        0
#define	PORT_STATE_SINK		        1
#define	PORT_STATE_SOURCE	        2
//---------------------------------------------------------------------------
typedef enum 
{
    POWER_CHANNEL_1 = 0,
    POWER_CHANNEL_2,
    POWER_CHANNEL_3,
    POWER_CHANNEL_4,
    POWER_CHANNEL_5,
    POWER_CHANNEL_6,
	POWER_CHANNEL_ALL,
}ePowerChannel_t;

typedef enum
{
	rsWaitSync,
	rsWaitAddres, 
	rsWaitLength,    
	rsWaitData,
	rsWaitChkSum,
}eCommunication_t;

typedef enum
{
	bsNormal,
	bsBinding, 
	bsUnBinding,
}eBindStatus_t;

typedef enum
{
	tsStandby, // 待机
	tsActivateChging, // 充电激活
	tsCurrentClimbChging, // 充电电流爬升
	tsConstCurChging, // 充电恒流
	tsConstVolChging, // 充电恒压
	tsBalanceChging, // 充电平衡
	tsTrickleChging, // 充电涓流
	tsCurrentClimbDischging, // 放电电流爬升
	tsConstCurDischging, // 放电恒流
	tsConstVolDischging, // 放电恒压
    tsDischargDone, // 放电完成
    tsTaskStateNum,
}eTaskState_t;

typedef enum
{
    btAuto = 0,
	btLiHv,
	btLiIon,
	btLiFe,
	btNiZn,
	btNiMHCd,
	btLi5V,
	btBattTypeNum,
}eBatteryType_t;

typedef enum
{
    ttCharge,
    ttDischarge,
    ttStorage,
    ttCycle,
    ttAnalysis,
    ttActivate,
    ttDestroy,
    ttTaskTypeNum,
}eTaskType_t;

typedef enum
{
    tcIdle,
    tcStart,
    tcStop,
    tcCheck,
    tcExeOk,
    tcError,
}eTaskCommand_t;

// typedef union
// {
// 	struct
// 	{
// 		uint32_t BatteryReverse            : 1 ;  // 电池接反
// 		uint32_t BatteryType               : 1;   //电池类型错误
//         uint32_t BatteryLowTemp            : 1;   //电池温度过低
//         uint32_t BatteryHighTemp           : 1;   //电池温度过高
//         uint32_t BoardHighTemp             : 1;   //系统温度过高
// 		uint32_t TimeOut                   : 1 ;  // 充电超时
// 		uint32_t ConnectionBroken          : 1 ;  // 连接状态破坏
// 		uint32_t BatteryOverVoltage        : 1 ;  // 电池过压
// 		uint32_t BatteryLowVoltage         : 1 ;  // 电池欠压
// 		uint32_t CellsNode                 : 1 ;  // 电池节点连接错误
// 		uint32_t NoBattery                 : 1 ;  // 输出未连电池
// 		uint32_t CapacityLimit             : 1 ;  // 容量限制
// 		uint32_t Unknown                   : 1 ;  // 未知错误
// 	} ErrorBits;
// 	uint32_t Errors;
// }TBatteryManagerError_t;

// typedef struct
// {
// 	uint8_t TxAdr;
// 	uint8_t RxAdr;
//     uint8_t TxSize;
//     uint8_t RxSize;
// 	uint8_t TxCnt;
//     uint8_t RxCnt;
// 	uint8_t Buf[FRAME_DATA_LEN];
//     uint8_t SyncByteCnt;
//     uint8_t Verify;
//     uint8_t SlaveIdx;
//     eBindStatus_t eBindStatus;
//     eCommunication_t eRxStatus;
// }PackageInfo_t;

// typedef struct
// {
// 	uint16_t ChargeCurrent;
// 	uint16_t ChargeVoltage;
// 	uint16_t DischargeCurrent;
// 	uint16_t DischargeVoltage;
// 	eBatteryType_t BatteryType;
// 	eTaskType_t Task;
//     eTaskCommand_t TaskCommand;
// }TaskContent_t;

// typedef struct 
// {
//     int8_t BatteryTemperature;  // 电池温度
//     uint8_t bBatteryLoad;       // 电池是否加载
//     uint8_t TaskPercent;        // 任务进度
//     int16_t OutputVoltage;      // 输出电压
// 	int16_t OutputCurrent;      // 输出电流
//     uint32_t TaskCapacity;      // 任务容量
//    	uint32_t TaskEnergy;        // 任务能量
//     uint32_t TaskConsumingTime; // 任务消耗时间
//     uint32_t Impedance100uOhm;  // 100uΩ阻抗
//     eBatteryType_t BatteryType; // 电池类型
//     eTaskState_t TaskState;     // 任务状态
//     TBatteryManagerError_t BatteryManagerError; // 电池管理错误
//     TaskContent_t TaskContent;  // 任务内容
// } ChannelData_t;

// typedef struct 
// {
//     uint8_t is_active;    // 设备是否活跃
//     uint8_t device_id;    // 设备ID
//     uint8_t try_cnt;      // 尝试次数
//     uint8_t cmd;          // 命令字
//     uint8_t prio_cmd;     // 优先命令字
//     uint8_t DeviceID[8];  // 设备ID
//     uint8_t HwVersion[4]; // 硬件版本
//     uint8_t BtVersion[4]; // Boot版本
//     uint8_t SwVersion[4]; // 软件版本
//     uint8_t channel;      // 通道号
//     ChannelData_t channel_data[POWER_CHANNEL_ALL];
// } SlaveDevice_t;

// typedef struct 
// {
//     uint8_t is_app;       // 是否在APP
//     uint8_t PortState;
//     uint8_t BtVersion[2]; // Boot版本
//     uint8_t SwVersion[2]; // 软件版本
//     uint16_t SnkVoltage_mV;
//     uint16_t SnkCurrent_mA;
//     uint16_t SrcVoltage_mV;
//     uint16_t SrcCurrent_mA;
//     uint16_t Vbus_mV;
//     uint16_t Ibus_mA;
//     uint16_t PDPower_w;
// } Ch543RxInfo_t;

// typedef struct 
// {
//     uint8_t SetDeviceParas;
//     uint8_t SetPwmVal;
//     uint8_t AppUpdate;
//     uint8_t PDPower_w;
//     uint16_t PwmVal;
// } Ch543TxInfo_t;

// void host_comm_task(void *pvParameters);
// void uart_comm_task(void *pvParameters);

// void host_ch543_comm_task(void *pvParameters);

// void start_device_discovery(void); // 开始寻机
// bool is_device_discovery_complete(void); // 判断寻机是否完成
// uint8_t get_slave_device_count(void); // 获取从机数量
// SlaveDevice_t *get_slave_device(uint8_t slave_idx); // 获取从机设备指针
// ChannelData_t *get_channel_data(uint8_t slave_idx, uint8_t channel); // 获取通道数据指针
// void set_slave_work_task(uint8_t slave_idx, uint8_t channel, TaskContent_t task_content); // 设置从机工作任务

// void Ch543_Set_PDPower_w(uint8_t w);
// uint8_t Ch543_Get_PDPower(void);
// void Ch543_Set_PwmVal(uint16_t val);

#ifdef __cplusplus
}
#endif

#endif /* USER_COMMUNICATION_H */

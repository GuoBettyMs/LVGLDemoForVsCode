/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#include <stdio.h>
// #include "esp_log.h"
// #include "esp_err.h"
// #include "freertos/FreeRTOS.h"
// #include "freertos/task.h"
// #include "freertos/portmacro.h"
// #include "driver/gpio.h"
// #include "driver/uart.h"
// #include "driver/uhci.h"
// #include "User_communication.h"
// //---------------------------------------------------------------------------
// #define RS485_UART_PORT  UART_NUM_1
// #define RS485_TX_PIN     9
// #define RS485_RX_PIN     10
// #define RS485_DE_PIN     46
// #define UART1_BAUD_RATE  115200
// #define UART1_BUF_SIZE   1024
// #define UART1_QUEUE_SIZE 20

// #define UART2_NUM        UART_NUM_2
// #define UART2_TX_PIN     47
// #define UART2_RX_PIN     48
// #define UART2_BAUD_RATE  115200
// #define UART2_BUF_SIZE   1024
// #define UART2_QUEUE_SIZE 20
// // 事件位掩码
// #define COMM_EVENT_SEND_BIT      (1 << 0) // 发送事件
// #define COMM_EVENT_RECEIVE_BIT   (1 << 1) // 接收事件
// #define COMM_EVENT_TIMEOUT_BIT   (1 << 2) // 超时事件
// //---------------------------------------------------------------------------
// static const char *TAG = "UART_HOST";
// static QueueHandle_t uart_event_queue = NULL;
// static EventGroupHandle_t comm_event_group = NULL;
// static PackageInfo_t PackageInfo;

// static const char *TAG_Ch543 = "UART_Ch543";
// static QueueHandle_t uart_event_queue_Ch543 = NULL;
// // static uhci_controller_handle_t uhci_handle_Ch543 = NULL;
// static EventGroupHandle_t comm_event_group_Ch543 = NULL;
// static PackageInfo_t PackageInfo_Ch543;
// Ch543RxInfo_t Ch543RxInfo;
// Ch543TxInfo_t Ch543TxInfo;

// void Ch543_host_recv_process(void);

// static SlaveDevice_t SlaveDevice[SLAVE_DEVICE_COUNT] = 
// { 
//     { .device_id = 2, .is_active = 0, .try_cnt = 0, .cmd = 0, .prio_cmd = 0xFF },
//     { .device_id = 3, .is_active = 0, .try_cnt = 0, .cmd = 0, .prio_cmd = 0xFF },
//     { .device_id = 4, .is_active = 0, .try_cnt = 0, .cmd = 0, .prio_cmd = 0xFF },
//     { .device_id = 5, .is_active = 0, .try_cnt = 0, .cmd = 0, .prio_cmd = 0xFF },
//     { .device_id = 6, .is_active = 0, .try_cnt = 0, .cmd = 0, .prio_cmd = 0xFF },
//     { .device_id = 7, .is_active = 0, .try_cnt = 0, .cmd = 0, .prio_cmd = 0xFF },
// };
// //---------------------------------------------------------------------------
// static bool RxDataProcess(PackageInfo_t *pPackageInfo, uint8_t rxData)
// {
// 	if (rxData == FRAME_HEADER_FLG)
// 	{
// 		pPackageInfo->SyncByteCnt++;
// 		if (pPackageInfo->SyncByteCnt & 0x01) return false;
// 	}
// 	else
// 	{
// 		if (pPackageInfo->SyncByteCnt & 0x01)
// 		{
// 			pPackageInfo->eRxStatus = rsWaitAddres;
// 		}
// 		pPackageInfo->SyncByteCnt = 0;
// 	}

// 	switch (pPackageInfo->eRxStatus)
// 	{
// 		case rsWaitSync: break;

// 		case rsWaitAddres:
// 		{
//             uint8_t TxAdr = (rxData >> 4)  & 0x0f;
// 			uint8_t RxAdr = rxData  & 0x0f;

// 			if (RxAdr == pPackageInfo->TxAdr)
// 			{
//                 pPackageInfo->Verify = rxData;
// 			    pPackageInfo->eRxStatus = rsWaitLength;
// 			}
// 			else
//             {
//                 pPackageInfo->eRxStatus = rsWaitSync; 
//             }
// 		}
// 		break;

// 		case rsWaitLength:
// 		{
// 			if (rxData < 1 || rxData > FRAME_DATA_LEN)
// 			{
// 				pPackageInfo->eRxStatus = rsWaitSync; 
// 				break; 
// 			}
// 			pPackageInfo->RxCnt = 0;
// 			pPackageInfo->RxSize = rxData;
// 			pPackageInfo->Verify += rxData;
// 			pPackageInfo->eRxStatus = rsWaitData;
// 		}
// 		break;

// 		case rsWaitData:
// 		{
// 			pPackageInfo->Buf[pPackageInfo->RxCnt++] = rxData;
// 			pPackageInfo->Verify += rxData;
// 			if (pPackageInfo->RxCnt >= pPackageInfo->RxSize)
// 			{
// 				pPackageInfo->eRxStatus = rsWaitChkSum;
// 			}
// 		}
// 		break;

// 		case rsWaitChkSum:
// 		{
// 			if (rxData == pPackageInfo->Verify)
// 			{
//                 pPackageInfo->eRxStatus = rsWaitSync;
// 				return true;
// 			} 
// 			else
// 			{
// 				pPackageInfo->eRxStatus = rsWaitSync;
// 			}
// 		}
// 		break;

// 		default:
// 		{
// 			pPackageInfo->eRxStatus = rsWaitSync;
// 		}
// 		break;
// 	}

//     return false;
// }
// //---------------------------------------------------------------------------
// static uint8_t *_PushData(uint8_t Data, uint8_t *pOutBuf)
// {
// 	*pOutBuf++ = Data;
	
// 	if (FRAME_HEADER_FLG == Data)
// 	{
// 		*pOutBuf++ = FRAME_HEADER_FLG;
// 	}
	
// 	return pOutBuf;
// }
// //---------------------------------------------------------------------------
// static int GenTxPackage(const PackageInfo_t *pPackageInfo, uint8_t * const pOutBuf)
// {
// 	uint8_t Data;
// 	uint8_t *pDataBuf = pOutBuf;
// 	uint8_t VerifySum = 0;

// 	*pDataBuf++ = FRAME_HEADER_FLG;

//     Data = ((pPackageInfo->TxAdr & 0x0f) << 4) | (pPackageInfo->RxAdr & 0x0f);
// 	pDataBuf = _PushData(Data, pDataBuf);
// 	VerifySum += Data;

// 	Data = pPackageInfo->TxSize;
// 	pDataBuf = _PushData(Data, pDataBuf);
// 	VerifySum += Data;

// 	for (uint8_t i = 0; i < pPackageInfo->TxSize; i++)
// 	{
// 		Data = pPackageInfo->Buf[i];
// 		pDataBuf = _PushData(Data, pDataBuf);
// 		VerifySum += Data;
// 	}

// 	pDataBuf = _PushData(VerifySum, pDataBuf);

// 	return (pDataBuf - pOutBuf);
// }
// //---------------------------------------------------------------------------
// static void uart1_dma_init(void) 
// {
//     uart_config_t uart_config = 
//     {
//         .baud_rate = UART1_BAUD_RATE,
//         .data_bits = UART_DATA_8_BITS,
//         .parity = UART_PARITY_DISABLE,
//         .stop_bits = UART_STOP_BITS_1,
//         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
//         .source_clk = UART_SCLK_DEFAULT,
//     };

//     ESP_ERROR_CHECK(uart_param_config(RS485_UART_PORT, &uart_config));
//     ESP_ERROR_CHECK(uart_set_pin(RS485_UART_PORT, RS485_TX_PIN, RS485_RX_PIN, RS485_DE_PIN, UART_PIN_NO_CHANGE));
//     ESP_ERROR_CHECK(uart_driver_install(RS485_UART_PORT, UART1_BUF_SIZE, UART1_BUF_SIZE, UART1_QUEUE_SIZE, &uart_event_queue, 0));
//     ESP_ERROR_CHECK(uart_set_mode(RS485_UART_PORT, UART_MODE_RS485_HALF_DUPLEX));
// }
// //---------------------------------------------------------------------------
// // 状态机状态定义
// typedef enum 
// {
//     COMM_STATE_SEND,          // 发送状态
//     COMM_STATE_RECEIVE        // 接收状态
// } comm_state_t;

// // 通讯任务函数
// void uart_comm_task(void *pvParameters)
// {
//     // 初始化UART和UHCI
//     uart1_dma_init();

//     uint8_t tx_data[UART1_BUF_SIZE];
//     uint8_t rx_data[UART1_BUF_SIZE];
//     comm_state_t state = COMM_STATE_SEND; // 初始状态为发送

//     while (1) 
//     {
//         switch (state) 
//         {
//             case COMM_STATE_SEND:
//             {
//                 // 阻塞等待发送事件
//                 EventBits_t bits = xEventGroupWaitBits(
//                     comm_event_group,        // 事件组
//                     COMM_EVENT_SEND_BIT,     // 等待的事件位
//                     pdTRUE,                  // 清除事件位
//                     pdFALSE,                 // 不需要等待所有事件位
//                     portMAX_DELAY            // 无限等待
//                 );
                
//                 if (bits & COMM_EVENT_SEND_BIT) 
//                 {
//                     // 生成发送数据
//                     int tx_len = GenTxPackage(&PackageInfo, tx_data);
//                     size_t  remain = 0;
//                     // 发送数据
//                     uart_write_bytes(RS485_UART_PORT, tx_data, tx_len);
                    
//                     // ESP_LOGI(TAG, "Send %d bytes:", tx_len);
//                     // for (int i = 0; i < tx_len; i++) ESP_LOGI(TAG, " %02x", tx_data[i]);
//                     // 发送完成，转换到接收状态
//                     state = COMM_STATE_RECEIVE;
//                 }
//             }
//             break;

//             case COMM_STATE_RECEIVE:
//             {
//                 uart_event_t event;
                
//                 if (xQueueReceive(uart_event_queue, &event, pdMS_TO_TICKS(1000))) 
//                 {
//                     switch (event.type) 
//                     {
//                         case UART_DATA:
//                         {
//                             int rx_len = uart_read_bytes(RS485_UART_PORT, rx_data, event.size, portMAX_DELAY); 

//                             // ESP_LOGI(TAG, "Receive %d bytes:", rx_len);
//                             // for (int i = 0; i < rx_len; i++) ESP_LOGI(TAG, " %02x", rx_data[i]);

//                             for (int i = 0; i < rx_len; i++) 
//                             {
//                                 if (RxDataProcess(&PackageInfo, rx_data[i]))
//                                 {
//                                     // 数据处理完成，设置接收事件
//                                     xEventGroupSetBits(comm_event_group, COMM_EVENT_RECEIVE_BIT);
//                                     state = COMM_STATE_SEND;
//                                 }
//                             }
//                         }
//                         break;

//                         case UART_FIFO_OVF:
//                         case UART_BUFFER_FULL:
//                         {
//                             uart_flush_input(RS485_UART_PORT);
//                             ESP_LOGI(TAG, "UART FIFO overflow or buffer full");
//                         }
//                         break;

//                         default:
//                         {
//                             ESP_LOGI(TAG, "Unhandled event type: %d", event.type);
//                         }
//                         break;  
//                     }
//                 } 
//                 else 
//                 {
//                     ESP_LOGW(TAG, "Receive event timeout");
//                     // 超时，转换到发送状态并设置超时事件
//                     xEventGroupSetBits(comm_event_group, COMM_EVENT_TIMEOUT_BIT);
//                     state = COMM_STATE_SEND;
//                 }
//             }
//             break;

//             default:
//             {
//                 ESP_LOGE(TAG, "Unknown state: %d", state);
//                 state = COMM_STATE_SEND; // 回到发送状态
//             }
//             break;
//         }
//     }
// }
// //---------------------------------------------------------------------------
// static void TestCmd(PackageInfo_t *pi, SlaveDevice_t *slave)
// {    
// 	uint8_t i = 0;

// 	pi->Buf[i++] = 0x00;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static void AddressBindCmd(PackageInfo_t *pi, SlaveDevice_t *slave)
// {    
// 	uint8_t i = 0;

// 	pi->Buf[i++] = 0x02;
//     pi->Buf[i++] = slave->device_id;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static void AddressUnbindCmd(PackageInfo_t *pi, SlaveDevice_t *slave)
// {    
// 	uint8_t i = 0;

// 	pi->Buf[i++] = 0x04;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static void QueryDeviceInfoCmd(PackageInfo_t *pi, SlaveDevice_t *slave)
// {    
// 	uint8_t i = 0;

// 	pi->Buf[i++] = 0xE0;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static void CheckWorkStateCmd(PackageInfo_t *pi, SlaveDevice_t *slave)
// {
//     uint8_t i = 0;

// 	pi->Buf[i++] = 0xE6;
//     pi->Buf[i++] = slave->channel;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static void SetWorkTaskCmd(PackageInfo_t *pi, SlaveDevice_t *slave)
// {
//     uint8_t i = 0;

// 	pi->Buf[i++] = 0xEA;
//     pi->Buf[i++] = slave->channel;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.Task;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.BatteryType;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.ChargeVoltage;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.ChargeVoltage >> 8;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.ChargeCurrent;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.ChargeCurrent >> 8;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.DischargeVoltage;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.DischargeVoltage >> 8;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.DischargeCurrent;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.DischargeCurrent >> 8;
//     pi->Buf[i++] = slave->channel_data[slave->channel].TaskContent.TaskCommand;
    
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static uint8_t host_send_process(SlaveDevice_t *slave)
// {
//     uint8_t cmd;
//     PackageInfo_t *pi = &PackageInfo;
    
//     if (pi->eBindStatus == bsBinding)
//     {
//         cmd = 0x02;
//         pi->TxAdr = HOST_ADDRESS;
//         pi->RxAdr = BIND_ADDRESS;
//     }
//     else if (pi->eBindStatus == bsUnBinding)
//     {
//         cmd = 0x04;
//         pi->TxAdr = HOST_ADDRESS;
//         pi->RxAdr = UNBIND_ADDRESS;
//     }
//     else
//     {
//         if (slave->prio_cmd != 0xFF) // 优先发送主动命令
//         {
//             cmd = slave->prio_cmd;
//         }
//         else
//         {
//             cmd = slave->cmd;
//         }

//         pi->TxAdr = HOST_ADDRESS;
//         pi->RxAdr = slave->device_id;
//     } 

//     switch (cmd)
//     {
//         case 0x00: TestCmd(pi, slave); break;
//         case 0x02: AddressBindCmd(pi, slave); break;
//         case 0x04: AddressUnbindCmd(pi, slave); break;
//         case 0xE0: QueryDeviceInfoCmd(pi, slave); break;
//         case 0xE6: CheckWorkStateCmd(pi, slave); break;
//         case 0xEA: SetWorkTaskCmd(pi, slave); break;
//         default: pi->TxSize = 0; break;
//     }

//     return pi->TxSize;
// }
// //---------------------------------------------------------------------------
// static void TestAck(PackageInfo_t *pi, SlaveDevice_t *slave)
// {    

// }
// //---------------------------------------------------------------------------
// static void AddressBindAck(PackageInfo_t *pi, SlaveDevice_t *slave)
// {
// 	uint8_t adr = pi->Buf[1];

//     if (adr == slave->device_id) // 绑定成功
//     {
//         slave->is_active = 1;
//     }
// }
// //---------------------------------------------------------------------------
// static void AddressUnbindAck(PackageInfo_t *pi, SlaveDevice_t *slave)
// {    
// 	uint8_t result = pi->Buf[1];

//     if (result == 0x00) // 解绑成功
//     {
//         slave->is_active = 0;
//     }
// }
// //---------------------------------------------------------------------------
// static void QueryDeviceInfoAck(PackageInfo_t *pi, SlaveDevice_t *slave)
// {    
// 	for (uint8_t i = 0; i < 8; i++) slave->DeviceID[i] = pi->Buf[i + 1];
//     for (uint8_t i = 0; i < 4; i++) slave->HwVersion[i] = pi->Buf[i + 9];
//     for (uint8_t i = 0; i < 4; i++) slave->BtVersion[i] = pi->Buf[i + 13];
//     for (uint8_t i = 0; i < 4; i++) slave->SwVersion[i] = pi->Buf[i + 17];
// }
// //---------------------------------------------------------------------------
// static void CheckWorkStateAck(PackageInfo_t *pi, SlaveDevice_t *slave)
// {
//     uint8_t channel = pi->Buf[1];

//     if (channel >= POWER_CHANNEL_ALL) return;

// 	ChannelData_t *pChannelData = &slave->channel_data[channel];

//     pChannelData->bBatteryLoad = pi->Buf[2];
//     pChannelData->TaskState = pi->Buf[3];
//     pChannelData->Impedance100uOhm = pi->Buf[4] | (pi->Buf[5] << 8);
//     pChannelData->TaskCapacity = pi->Buf[6] | (pi->Buf[7] << 8) | (pi->Buf[8] << 16) | (pi->Buf[9] << 24);
//     pChannelData->TaskEnergy = pi->Buf[10] | (pi->Buf[11] << 8) | (pi->Buf[12] << 16) | (pi->Buf[13] << 24);
//     pChannelData->TaskConsumingTime = pi->Buf[14] | (pi->Buf[15] << 8) | (pi->Buf[16] << 16) | (pi->Buf[17] << 24);
//     pChannelData->BatteryType = pi->Buf[18];
//     pChannelData->OutputVoltage = pi->Buf[19] | (pi->Buf[20] << 8);
//     pChannelData->OutputCurrent = pi->Buf[21] | (pi->Buf[22] << 8);
//     pChannelData->BatteryTemperature = pi->Buf[23];
//     pChannelData->TaskPercent = pi->Buf[24];
//     pChannelData->BatteryManagerError.Errors = (pi->Buf[25] | (pi->Buf[26] << 8) | (pi->Buf[27] << 16) | (pi->Buf[28] << 24));

//     // uint8_t ch = PackageInfo.SlaveIdx*POWER_CHANNEL_ALL+channel;
//     // printf("ch%d :%d %d %ld\n",ch,pChannelData->bBatteryLoad,pChannelData->TaskState,pChannelData->BatteryManagerError.Errors);

//     if (channel == slave->channel)
//     {
//         if (++slave->channel >= POWER_CHANNEL_ALL) slave->channel = 0;
//     }
// }
// //---------------------------------------------------------------------------
// static void SetWorkTaskCmdAck(PackageInfo_t *pi, SlaveDevice_t *slave)
// {
//     uint8_t channel = pi->Buf[1];
//     uint8_t result = pi->Buf[2];
// }
// //---------------------------------------------------------------------------
// void host_recv_process(SlaveDevice_t *slave)
// {
//     PackageInfo_t *pi = &PackageInfo;
//     uint8_t ack = pi->Buf[0];

//     switch (ack)
//     {
//         case 0x01: TestAck(pi, slave); break;
//         case 0x03: AddressBindAck(pi, slave); break;
//         case 0x05: AddressUnbindAck(pi, slave); break;
//         case 0xE1: QueryDeviceInfoAck(pi, slave); break;
//         case 0xE7: CheckWorkStateAck(pi, slave); break;
//         case 0xEB: SetWorkTaskCmdAck(pi, slave); break;
//         default: break;
//     }

//     if (ack == 0x01) slave->cmd = 0xE0;
//     else if (ack == 0xE1) slave->cmd = 0xE6;
//     else if (ack == slave->prio_cmd + 1) slave->prio_cmd = 0xFF;
// }
// //---------------------------------------------------------------------------
// // 开启寻机操作
// void start_device_discovery(void)
// {
//     PackageInfo_t *pPackageInfo = &PackageInfo;

//     pPackageInfo->eBindStatus = bsUnBinding; 
    
//     // 重置所有从机状态
//     for (int i = 0; i < SLAVE_DEVICE_COUNT; i++)
//     {
//         SlaveDevice[i].is_active = 0;
//         SlaveDevice[i].try_cnt = 0;
//         SlaveDevice[i].cmd = 0;
//         SlaveDevice[i].prio_cmd = 0xFF;
//     }
    
//     ESP_LOGI(TAG, "Start device discovery...");
// }

// // 检查寻机是否完成
// bool is_device_discovery_complete(void)
// {
//     PackageInfo_t *pPackageInfo = &PackageInfo;
    
//     return (pPackageInfo->eBindStatus == bsNormal);
// }

// // 获取当前寻机到的从机数量
// uint8_t get_slave_device_count(void)
// {
//     uint8_t count = 0;

//     for (int i = 0; i < SLAVE_DEVICE_COUNT; i++)
//     {
//         if (SlaveDevice[i].is_active) count++;
//     }

//     return count;
// }

// // 获取从机设备指针
// SlaveDevice_t *get_slave_device(uint8_t slave_idx)
// {
//     if (!SlaveDevice[slave_idx].is_active) return NULL;
    
//     return &SlaveDevice[slave_idx];
// }

// // 获取通道数据指针
// ChannelData_t *get_channel_data(uint8_t slave_idx, uint8_t channel)
// {
//     if (!SlaveDevice[slave_idx].is_active) return NULL;
//     if (channel >= POWER_CHANNEL_ALL) return NULL;

//     return &SlaveDevice[slave_idx].channel_data[channel];
// }

// // 设置从机工作任务
// void set_slave_work_task(uint8_t slave_idx, uint8_t channel, TaskContent_t task_content)
// {
//     SlaveDevice_t *slave = &SlaveDevice[slave_idx];

//     if (slave->is_active)
//     {
//         slave->channel = channel;
//         slave->channel_data[channel].TaskContent = task_content;
//         slave->prio_cmd = 0xEA;
//     }
// }
// //---------------------------------------------------------------------------
// // 主机通讯管理任务
// void host_comm_task(void *pvParameters)
// {
//     start_device_discovery(); // 开始寻机

//     comm_event_group = xEventGroupCreate(); // 创建事件组

//     xTaskCreate(uart_comm_task, "uart_comm_task", 4096, NULL, 5, NULL);
    
//     while (1) 
//     {
//         PackageInfo_t *pPackageInfo = &PackageInfo;
//         SlaveDevice_t *slave = &SlaveDevice[pPackageInfo->SlaveIdx];

//         if ((pPackageInfo->eBindStatus == bsBinding || pPackageInfo->eBindStatus == bsUnBinding || slave->is_active) && host_send_process(slave))
//         {
//             // 设置发送事件，触发发送
//             xEventGroupSetBits(comm_event_group, COMM_EVENT_SEND_BIT);
            
//             // 等待接收事件
//             EventBits_t bits = xEventGroupWaitBits(
//                 comm_event_group,        // 事件组
//                 COMM_EVENT_RECEIVE_BIT | COMM_EVENT_TIMEOUT_BIT,  // 等待的事件位
//                 pdTRUE,                  // 清除事件位
//                 pdFALSE,                 // 不需要等待所有事件位
//                 portMAX_DELAY            // 不超时
//             );
            
//             if (bits & COMM_EVENT_RECEIVE_BIT) 
//             {
//                 host_recv_process(slave);
//                 slave->try_cnt = 0;
//                 if (++pPackageInfo->SlaveIdx >= SLAVE_DEVICE_COUNT) pPackageInfo->SlaveIdx = 0;
//             } 
//             else 
//             {
//                 ESP_LOGW(TAG, "Receive timeout for device %d", pPackageInfo->SlaveIdx);
//                 // 处理超时情况
//                 if (++slave->try_cnt >= MAX_TRY_COUNT)
//                 {
//                     slave->try_cnt = 0;

//                     if (pPackageInfo->eBindStatus == bsUnBinding)
//                     {
//                         pPackageInfo->eBindStatus = bsBinding;
//                     }
//                     else if (pPackageInfo->eBindStatus == bsBinding)
//                     {
//                         pPackageInfo->eBindStatus = bsNormal;
//                         pPackageInfo->SlaveIdx = 0;
//                     }
//                     else
//                     {
//                         if (++pPackageInfo->SlaveIdx >= SLAVE_DEVICE_COUNT) pPackageInfo->SlaveIdx = 0;
//                     }
//                 }
//             }
//         }
//         else
//         {
//             slave->try_cnt = 0;
//             if (++pPackageInfo->SlaveIdx >= SLAVE_DEVICE_COUNT) pPackageInfo->SlaveIdx = 0;
//         }
//         // 完整执行一轮后延时一段时间，避免任务过于频繁执行
//         if (pPackageInfo->SlaveIdx == 0) vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }
// //---------------------------------------------------------------------------
// static void uart2_dma_init(void) 
// {
//     uart_config_t uart_config = 
//     {
//         .baud_rate = UART2_BAUD_RATE,
//         .data_bits = UART_DATA_8_BITS,
//         .parity = UART_PARITY_DISABLE,
//         .stop_bits = UART_STOP_BITS_1,
//         .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
//         .source_clk = UART_SCLK_DEFAULT,
//     };

//     ESP_ERROR_CHECK(uart_param_config(UART2_NUM, &uart_config));
//     ESP_ERROR_CHECK(uart_set_pin(UART2_NUM, UART2_TX_PIN, UART2_RX_PIN, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE));
//     ESP_ERROR_CHECK(uart_driver_install(UART2_NUM, UART2_BUF_SIZE, UART2_BUF_SIZE, UART2_QUEUE_SIZE, &uart_event_queue_Ch543, 0));
                                        
//     // uhci_controller_config_t uhci_config = 
//     // {
//     //     .uart_port = UART2_NUM,
//     //     .tx_trans_queue_depth = 10,
//     //     .max_receive_internal_mem = 10 * 1024,
//     //     .max_transmit_size = 10 * 1024,
//     //     .dma_burst_size = 32,
//     //     .rx_eof_flags = { .idle_eof = 1, }
//     // };

//     // ESP_ERROR_CHECK(uhci_new_controller(&uhci_config, &uhci_handle_Ch543));
// }
// //---------------------------------------------------------------------------

// // 通讯任务函数
// void uart2_comm_tx_task(void *pvParameters)
// {
//     // 初始化UART和UHCI
//     uart2_dma_init();

//     uint8_t tx_data[UART2_BUF_SIZE];

//     while (1) 
//     {
//         // 阻塞等待发送事件
//         EventBits_t bits = xEventGroupWaitBits(
//             comm_event_group_Ch543,  // 事件组
//             COMM_EVENT_SEND_BIT | COMM_EVENT_RECEIVE_BIT,     // 等待的事件位
//             pdTRUE,                  // 清除事件位
//             pdFALSE,                 // 不需要等待所有事件位
//             portMAX_DELAY            // 无限等待
//         );
        
//         if (bits & COMM_EVENT_SEND_BIT) 
//         {
//             // 生成发送数据
//             int tx_len = GenTxPackage(&PackageInfo_Ch543, tx_data);
        
//             // 发送数据
//             uart_write_bytes(UART2_NUM, tx_data, tx_len);
            
//             // ESP_LOGI(TAG_Ch543, "Send %d bytes:", tx_len);
//             // for (int i = 0; i < tx_len; i++) ESP_LOGI(TAG_Ch543, " %02x", tx_data[i]);
//         }
//         if (bits & COMM_EVENT_RECEIVE_BIT) 
//         {
//             Ch543_host_recv_process();
//         } 
//     }
// }

// void uart2_comm_rx_task(void *pvParameters)
// {
//     uint8_t rx_data[UART2_BUF_SIZE];

//     while (1) 
//     {
//         uart_event_t event;
        
//         if (xQueueReceive(uart_event_queue_Ch543, &event, pdMS_TO_TICKS(1000))) 
//         {
//             switch (event.type) 
//             {
//                 case UART_DATA:
//                 {
//                     int rx_len = uart_read_bytes(UART2_NUM, rx_data, event.size, portMAX_DELAY); 

//                     // ESP_LOGI(TAG_Ch543, "Receive %d bytes:", rx_len);
//                     // for (int i = 0; i < rx_len; i++) ESP_LOGI(TAG_Ch543, " %02x", rx_data[i]);

//                     for (int i = 0; i < rx_len; i++) 
//                     {
//                         if (RxDataProcess(&PackageInfo_Ch543, rx_data[i]))
//                         {
//                             // 数据处理完成，设置接收事件
//                             xEventGroupSetBits(comm_event_group_Ch543, COMM_EVENT_RECEIVE_BIT);
//                         }
//                     }
//                 }
//                 break;

//                 case UART_FIFO_OVF:
//                 case UART_BUFFER_FULL:
//                 {
//                     uart_flush_input(UART2_NUM);
//                     ESP_LOGI(TAG_Ch543, "UART FIFO overflow or buffer full");
//                 }
//                 break;

//                 default:
//                 {
//                     ESP_LOGI(TAG_Ch543, "Unhandled event type: %d", event.type);
//                 }
//                 break;  
//             }
//         } 
//     }
// }
// //---------------------------------------------------------------------------
// static void Ch543_TestCmd(PackageInfo_t *pi)
// {    
// 	uint8_t i = 0;

// 	pi->Buf[i++] = 0x00;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static void Ch543_SetDeviceParasCmd(PackageInfo_t *pi)
// {    
// 	uint8_t i = 0;
    
// 	pi->Buf[i++] = 0xB0;
//     pi->Buf[i++] = Ch543TxInfo.PDPower_w;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static void Ch543_SetPwmCmd(PackageInfo_t *pi)
// {    
// 	uint8_t i = 0;
//     ESP_LOGI(TAG_Ch543, "Set Pwm: %d", Ch543TxInfo.PwmVal);
// 	pi->Buf[i++] = 0xB4;
//     pi->Buf[i++] = Ch543TxInfo.PwmVal;
//     pi->Buf[i++] = Ch543TxInfo.PwmVal >> 8;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static void Ch543_AppUpdateCmd(PackageInfo_t *pi)
// {    
// 	uint8_t i = 0;

// 	pi->Buf[i++] = 0xF0;
//     pi->Buf[i++] = 0xAC;
//     pi->TxSize = i;
// }
// //---------------------------------------------------------------------------
// static uint8_t Ch543_host_send_process(void)
// {
//     uint8_t cmd = 0xff;
//     PackageInfo_t *pi = &PackageInfo_Ch543;
    
//     if (Ch543RxInfo.is_app == 0)
//     {
//         cmd = 0x00;
//     }
//     else if(Ch543TxInfo.AppUpdate)
//     {
//         Ch543TxInfo.AppUpdate = 0;
//         cmd = 0xF0;
//     }
//     else if(Ch543TxInfo.SetDeviceParas)
//     {
//         Ch543TxInfo.SetDeviceParas = 0;
//         cmd = 0xB0;
//     }
//     else if(Ch543TxInfo.SetPwmVal)
//     {
//         Ch543TxInfo.SetPwmVal = 0;
//         cmd = 0xB4;
//     }
//     pi->TxAdr = HOST_ADDRESS;
//     pi->RxAdr = CH543_ADDRESS;

//     switch (cmd)
//     {
//         case 0x00: Ch543_TestCmd(pi); break;
//         case 0xB0: Ch543_SetDeviceParasCmd(pi); break;
//         case 0xB4: Ch543_SetPwmCmd(pi); break;
//         case 0xF0: Ch543_AppUpdateCmd(pi); break;
//         default: pi->TxSize = 0; break;
//     }

//     return pi->TxSize;
// }
// //---------------------------------------------------------------------------
// static void Ch543_TestAck(PackageInfo_t *pi)
// {    
//     uint8_t result = pi->Buf[1];
//     if(result == 0x00)
//     {
//         Ch543RxInfo.is_app = 1;
//         Ch543RxInfo.SwVersion[0] = pi->Buf[2];
//         Ch543RxInfo.SwVersion[1] = pi->Buf[3];
//     }
//     else
//     {
//         Ch543RxInfo.is_app = 0;
//         Ch543RxInfo.BtVersion[0] = pi->Buf[2];
//         Ch543RxInfo.BtVersion[1] = pi->Buf[3];
//     }
// }
// //---------------------------------------------------------------------------
// static void Ch543_SetDeviceParasAck(PackageInfo_t *pi)
// {
	
// }
// //---------------------------------------------------------------------------
// static void Ch543_AppUpdateAck(PackageInfo_t *pi)
// {    
	
// }
// //---------------------------------------------------------------------------
// static void Ch543_WorkStateAck(PackageInfo_t *pi)
// {    
// 	Ch543RxInfo.PortState = pi->Buf[1];
//     // ESP_LOGI(TAG_Ch543, "WorkState: %d",Ch543RxInfo.PortState);
//     if(Ch543RxInfo.PortState == PORT_STATE_IDLE)
//     {
//        Ch543RxInfo.PDPower_w =  pi->Buf[2] | (pi->Buf[3] << 8);
//     //    ESP_LOGI(TAG_Ch543, "IDLE: %dW", Ch543RxInfo.PDPower_w); 
//     }
//     else if(Ch543RxInfo.PortState == PORT_STATE_SINK)
//     {
//        Ch543RxInfo.SnkVoltage_mV =  pi->Buf[2] | (pi->Buf[3] << 8);
//        Ch543RxInfo.SnkCurrent_mA =  pi->Buf[4] | (pi->Buf[5] << 8);
//     //    ESP_LOGI(TAG_Ch543, "SINK: %dmV %dmA",  Ch543RxInfo.SnkVoltage_mV,Ch543RxInfo.SnkCurrent_mA); 
//     }
//     else
//     {
//         Ch543RxInfo.SrcVoltage_mV =  pi->Buf[2] | (pi->Buf[3] << 8);
//         Ch543RxInfo.SrcCurrent_mA =  pi->Buf[4] | (pi->Buf[5] << 8);
//         // ESP_LOGI(TAG_Ch543, "SOURCE: %dmV %dmA",  Ch543RxInfo.SrcVoltage_mV,Ch543RxInfo.SrcCurrent_mA); 
//     }
//     Ch543RxInfo.Vbus_mV =  pi->Buf[6] | (pi->Buf[7] << 8);
//     Ch543RxInfo.Ibus_mA =  pi->Buf[8] | (pi->Buf[9] << 8);

//     // ESP_LOGI(TAG_Ch543, "Bus: %dmV %dmA",   Ch543RxInfo.Vbus_mV,Ch543RxInfo.Ibus_mA); 
  
// }
// //---------------------------------------------------------------------------
// void Ch543_host_recv_process(void)
// {
//     PackageInfo_t *pi = &PackageInfo_Ch543;
//     uint8_t ack = pi->Buf[0];

//     switch (ack)
//     {
//         case 0x01: Ch543_TestAck(pi); break;
//         case 0xB1: Ch543_SetDeviceParasAck(pi); break;
//         case 0xB3: Ch543_WorkStateAck(pi); break;
//         case 0xF1: Ch543_AppUpdateAck(pi); break;
//         default: break;
//     }
// }
// //---------------------------------------------------------------------------
// void Ch543_Set_PDPower_w(uint8_t w)
// {
//     Ch543TxInfo.PDPower_w = w;
//     Ch543TxInfo.SetDeviceParas = 1;
// }
// //---------------------------------------------------------------------------
// uint8_t Ch543_Get_PDPower(void)
// {
//     return Ch543RxInfo.PDPower_w;
// }
// //---------------------------------------------------------------------------
// void Ch543_Set_PwmVal(uint16_t val)
// {
//     Ch543TxInfo.PwmVal = val;
//     Ch543TxInfo.SetPwmVal = 1;
// }
// //---------------------------------------------------------------------------
// // 主机通讯管理任务
// void host_ch543_comm_task(void *pvParameters)
// {
//     Ch543RxInfo.is_app = 0;
//     comm_event_group_Ch543 = xEventGroupCreate(); // 创建事件组

//     xTaskCreate(uart2_comm_tx_task, "uart2_comm_tx_task", 4096, NULL, 5, NULL);
//     xTaskCreate(uart2_comm_rx_task, "uart2_comm_rx_task", 4096, NULL, 5, NULL);

//     while (1) 
//     {
//         if (Ch543_host_send_process())
//         {
//             // 设置发送事件，触发发送
//             xEventGroupSetBits(comm_event_group_Ch543, COMM_EVENT_SEND_BIT);
            
//             // 等待接收事件
//             // EventBits_t bits = xEventGroupWaitBits(
//             //     comm_event_group_Ch543,        // 事件组
//             //     COMM_EVENT_RECEIVE_BIT | COMM_EVENT_TIMEOUT_BIT,  // 等待的事件位
//             //     pdTRUE,                  // 清除事件位
//             //     pdFALSE,                 // 不需要等待所有事件位
//             //     portMAX_DELAY            // 不超时
//             // );
            
//             // if (bits & COMM_EVENT_RECEIVE_BIT) 
//             // {
//             //     Ch543_host_recv_process();
//             // } 
//             // else 
//             // {
//             //     ESP_LOGW(TAG_Ch543, "Receive timeout for device");
//             //     // 处理超时情况
                
//             // }
//         }
//         // else
//         // {
            
//         // }

//         // // 嵌入式时需调用: 延时一段时间，避免任务过于频繁执行 
//         // vTaskDelay(100 / portTICK_PERIOD_MS);
//     }
// }
// //---------------------------------------------------------------------------
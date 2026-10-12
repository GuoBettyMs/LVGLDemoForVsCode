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

// //---------------------------------------------------------------------------
// #define BUTTON_GPIO 14
// #define BUTTON_FILTER_CNT   3       // 连续 3 次相同才确认（约 30ms 消抖）
// //---------------------------------------------------------------------------
// static const char *TAG = "UART_GPIO";

// static uint8_t key_state;

// void gpio_init(void)
// {
//     gpio_config_t io_conf = {
//         .pin_bit_mask = (1ULL << BUTTON_GPIO),  // 指定 GPIO 14
//         .mode = GPIO_MODE_INPUT,                 // 输入模式
//         .pull_up_en = GPIO_PULLUP_ENABLE,        // 使能内部上拉（关键）
//         .pull_down_en = GPIO_PULLDOWN_DISABLE,   // 禁用下拉
//         .intr_type = GPIO_INTR_DISABLE,          
//     };
//     gpio_config(&io_conf);

// }

// uint8_t get_key_state(void)
// {
//     uint8_t curr_key_state = key_state;
//     key_state = false;
//     return curr_key_state;
// }

// void gpio_task(void *pvParameters)
// {
//     gpio_init();
//     bool stable_state = true;      // 稳定状态：true=未按下(高)，false=按下(低)
//     bool last_raw = true;          // 上一次原始电平
//     uint8_t same_cnt = 0;          // 连续相同计数

//     while (1) {
//         // 读取原始电平：低电平表示按下
//         bool raw = gpio_get_level(BUTTON_GPIO) ? true : false;

//         if (raw == last_raw) {
//             // 电平未变，累加计数
//             if (same_cnt < BUTTON_FILTER_CNT) {
//                 same_cnt++;
//             }
//         } else {
//             // 电平变化，重新计数
//             last_raw = raw;
//             same_cnt = 0;
//         }

//         // 连续 N 次相同，且与稳定状态不同 → 确认状态翻转
//         if (same_cnt >= BUTTON_FILTER_CNT && raw != stable_state) {
//             stable_state = raw;
//             same_cnt = 0;

//             if(!stable_state)
//                 key_state = true;   // 低电平 = 按下
//         }

//         vTaskDelay(10 / portTICK_PERIOD_MS);
//     }
// }


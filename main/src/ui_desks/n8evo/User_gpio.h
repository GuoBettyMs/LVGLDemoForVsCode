/*
 * SPDX-FileCopyrightText: 2010-2022 Espressif Systems (Shanghai) CO LTD
 *
 * SPDX-License-Identifier: CC0-1.0
 */

#ifndef USER_GPIO_H
#define USER_GPIO_H

#ifdef __cplusplus
extern "C" {
#endif

void gpio_task(void *pvParameters);
uint8_t get_key_state(void);

#ifdef __cplusplus
}
#endif

#endif /* USER_COMMUNICATION_H */

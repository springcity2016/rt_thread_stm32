/*
 * Copyright (c) 2006-2018, RT-Thread Development Team
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Change Logs:
 * Date           Author       Notes
 * 2018-11-06     SummerGift   first version
 */

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>
#include "user_btn.h"

#define LED1_PIN    GET_PIN(C, 7)           // LED1
#define LED4_PIN    GET_PIN(A, 8)           // LED2
#define LED5_PIN    GET_PIN(A, 9)           // LED3
#define LED6_PIN    GET_PIN(A, 10)          // LED4





int main(void)
{
    int count = 1;
    /* set LED0 pin mode to output */
    // rt_pin_mode(LED1_PIN, PIN_MODE_OUTPUT);
    // rt_pin_mode(LED4_PIN, PIN_MODE_OUTPUT);
    // rt_pin_mode(LED5_PIN, PIN_MODE_OUTPUT);
    // rt_pin_mode(LED6_PIN, PIN_MODE_OUTPUT);
    user_btn_cfg_init();

    while (count++)
    {
        // rt_kprintf(" set pin3 high level! %d\n", __LINE__);
        // rt_pin_write(LED1_PIN, PIN_HIGH);
        // rt_pin_write(LED4_PIN, PIN_HIGH);
        // rt_pin_write(LED5_PIN, PIN_HIGH);
        // rt_pin_write(LED6_PIN, PIN_HIGH);
        // rt_thread_mdelay(1000);
        // rt_kprintf(" set pin3 low level! %d\n", __LINE__);
        // rt_pin_write(LED1_PIN, PIN_LOW);
        // rt_pin_write(LED4_PIN, PIN_LOW);
        // rt_pin_write(LED5_PIN, PIN_LOW);
        // rt_pin_write(LED6_PIN, PIN_LOW);
        rt_thread_mdelay(500);
    }

    return RT_EOK;
}

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
    rt_err_t _ret = RT_EOK;
    time_t now;

    user_btn_cfg_init();

    _ret = set_date(2019, 12, 16);
    rt_kprintf(" set date: 2019-12-16! ret: %d, %d \n", _ret, __LINE__);

    _ret = set_time(23, 36, 0);
    rt_kprintf(" set time: 23-36-00! ret: %d, %d \n", _ret, __LINE__);

    while (count++)
    {
        if ((count % 10) ==  0) {
            now = time(RT_NULL);
            rt_kprintf("%s \n", ctime(&now));
        }
        rt_thread_mdelay(1000);
    }

    return RT_EOK;
}

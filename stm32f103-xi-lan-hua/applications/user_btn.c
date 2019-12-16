
#define __USER_BTN_C_

#include "multi_button.h"

#include <rtthread.h>
#include <rtdevice.h>
#include <board.h>


// #define KEY1_BTN                GET_PIN(A, 13)      // K1
// #define KEY2_BTN                GET_PIN(D, 6)       // K2
// #define KEY3_BTN                GET_PIN(E, 0)       // K3
// #define KEY4_BTN                GET_PIN(E, 0)       // K3

#define KEY1_BTN                GET_PIN(E, 11)      // K1
#define KEY2_BTN                GET_PIN(E, 12)       // K2
#define KEY3_BTN                GET_PIN(E, 13)       // K3
#define KEY4_BTN                GET_PIN(E, 14)       // K4

struct Button key1_btn;
struct Button key2_btn;
struct Button key3_btn;
struct Button key4_btn;
static void key1_btn_callback(void *params);
static void key2_btn_callback(void *params);
static void key3_btn_callback(void *params);
static void key4_btn_callback(void *params);

static rt_uint8_t read_key1_pindata(void);
static rt_uint8_t read_key2_pindata(void);
static rt_uint8_t read_key3_pindata(void);
static rt_uint8_t read_key4_pindata(void);
static void gpio_irq_callback(void *args);

// 定时器管理函数
static rt_timer_t btn_timer;
static void btn_timer_callback(void *params);
static void start_btn_timer(unsigned char force);
static void stop_btn_timer(void);

#define LOOSE_BTN_DEF_TIME      (500 / TICKS_INTERVAL)
typedef struct _btn_timer_t
{
    unsigned char bfwork;

    unsigned char bf_key1st;
    unsigned char bf_key2nd;
    unsigned char bf_key3rd;
    unsigned char bf_key4th;

    unsigned int delay_cnt;

}btn_timer_t;

static btn_timer_t mst_btn_timer;

void user_btn_cfg_init(void)
{
    memset(&mst_btn_timer, 0, sizeof(btn_timer_t));

    rt_pin_mode(KEY1_BTN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(KEY2_BTN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(KEY3_BTN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_mode(KEY4_BTN, PIN_MODE_INPUT_PULLDOWN);
    rt_pin_attach_irq(KEY1_BTN, PIN_IRQ_MODE_RISING, gpio_irq_callback, (void *)KEY1_BTN);
    rt_pin_attach_irq(KEY2_BTN, PIN_IRQ_MODE_RISING, gpio_irq_callback, (void *)KEY2_BTN);
    rt_pin_attach_irq(KEY3_BTN, PIN_IRQ_MODE_RISING, gpio_irq_callback, (void *)KEY3_BTN);
    rt_pin_attach_irq(KEY4_BTN, PIN_IRQ_MODE_RISING, gpio_irq_callback, (void *)KEY4_BTN);
    rt_pin_irq_enable(KEY1_BTN, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY2_BTN, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY3_BTN, PIN_IRQ_ENABLE);
    rt_pin_irq_enable(KEY4_BTN, PIN_IRQ_ENABLE);

    button_init(&key1_btn, read_key1_pindata, 1);
    button_attach(&key1_btn, PRESS_DOWN, key1_btn_callback);
    button_attach(&key1_btn, PRESS_UP, key1_btn_callback);
    button_attach(&key1_btn, PRESS_REPEAT, key1_btn_callback);
    button_attach(&key1_btn, SINGLE_CLICK, key1_btn_callback);
    button_attach(&key1_btn, DOUBLE_CLICK, key1_btn_callback);
    button_attach(&key1_btn, LONG_RRESS_START, key1_btn_callback);
    button_attach(&key1_btn, LONG_PRESS_HOLD, key1_btn_callback);
    button_start(&key1_btn);

    button_init(&key2_btn, read_key2_pindata, 1);
    button_attach(&key2_btn, PRESS_DOWN, key2_btn_callback);
    button_attach(&key2_btn, PRESS_UP, key2_btn_callback);
    button_attach(&key2_btn, PRESS_REPEAT, key2_btn_callback);
    button_attach(&key2_btn, SINGLE_CLICK, key2_btn_callback);
    button_attach(&key2_btn, DOUBLE_CLICK, key2_btn_callback);
    button_attach(&key2_btn, LONG_RRESS_START, key2_btn_callback);
    button_attach(&key2_btn, LONG_PRESS_HOLD, key2_btn_callback);
    button_start(&key2_btn);

    button_init(&key3_btn, read_key3_pindata, 1);
    button_attach(&key3_btn, PRESS_DOWN, key3_btn_callback);
    button_attach(&key3_btn, PRESS_UP, key3_btn_callback);
    button_attach(&key3_btn, PRESS_REPEAT, key3_btn_callback);
    button_attach(&key3_btn, SINGLE_CLICK, key3_btn_callback);
    button_attach(&key3_btn, DOUBLE_CLICK, key3_btn_callback);
    button_attach(&key3_btn, LONG_RRESS_START, key3_btn_callback);
    button_attach(&key3_btn, LONG_PRESS_HOLD, key3_btn_callback);
    button_start(&key3_btn);

    button_init(&key4_btn, read_key4_pindata, 1);
    button_attach(&key4_btn, PRESS_DOWN, key4_btn_callback);
    button_attach(&key4_btn, PRESS_UP, key4_btn_callback);
    button_attach(&key4_btn, PRESS_REPEAT, key4_btn_callback);
    button_attach(&key4_btn, SINGLE_CLICK, key4_btn_callback);
    button_attach(&key4_btn, DOUBLE_CLICK, key4_btn_callback);
    button_attach(&key4_btn, LONG_RRESS_START, key4_btn_callback);
    button_attach(&key4_btn, LONG_PRESS_HOLD, key4_btn_callback);
    button_start(&key4_btn);

    btn_timer = rt_timer_create("btn_timer", btn_timer_callback, NULL, TICKS_INTERVAL, RT_TIMER_FLAG_PERIODIC);
    if (btn_timer == NULL) {
        rt_kprintf(" btn timer is NULL! %d \n ", __LINE__);
    }

    // start_btn_timer(1);
}

static void key1_btn_callback(void *params)
{
    rt_uint8_t key_evt;
    key_evt = get_button_event((struct Button *)params);
    switch (key_evt)
    {
    case PRESS_DOWN:
        rt_kprintf(" key1, press down! %d \n", __LINE__);
        break;
	case PRESS_UP:
        rt_kprintf(" key1, press up! %d \n", __LINE__);
        mst_btn_timer.bf_key1st = 0;
        mst_btn_timer.delay_cnt = 0;
        break;
	case PRESS_REPEAT:
        rt_kprintf(" key1, press repeat! %d \n", __LINE__);
        break;
	case SINGLE_CLICK:
        rt_kprintf(" key1, single click! %d \n", __LINE__);
        break;
	case DOUBLE_CLICK:
        rt_kprintf(" key1, double click! %d \n", __LINE__);
        break;
	case LONG_RRESS_START:
        rt_kprintf(" key1, long press start! %d \n", __LINE__);
        break;
	case LONG_PRESS_HOLD:
        rt_kprintf(" key1, long press hold! %d \n", __LINE__);
        break;
    default:
        break;
    }

}

static void key2_btn_callback(void *params)
{
    rt_uint8_t key_evt;
    key_evt = get_button_event((struct Button *)params);
    switch (key_evt)
    {
    case PRESS_DOWN:
        rt_kprintf(" key2, press down! %d \n", __LINE__);
        break;
	case PRESS_UP:
        rt_kprintf(" key2, press up! %d \n", __LINE__);
        mst_btn_timer.bf_key2nd = 0;
        mst_btn_timer.delay_cnt = 0;
        break;
	case PRESS_REPEAT:
        rt_kprintf(" key2, press repeat! %d \n", __LINE__);
        break;
	case SINGLE_CLICK:
        rt_kprintf(" key2, single click! %d \n", __LINE__);
        break;
	case DOUBLE_CLICK:
        rt_kprintf(" key2, double click! %d \n", __LINE__);
        break;
	case LONG_RRESS_START:
        rt_kprintf(" key2, long press start! %d \n", __LINE__);
        break;
	case LONG_PRESS_HOLD:
        rt_kprintf(" key2, long press hold! %d \n", __LINE__);
        break;
    default:
        break;
    }

}

static void key3_btn_callback(void *params)
{
    rt_uint8_t key_evt;
    key_evt = get_button_event((struct Button *)params);
    switch (key_evt)
    {
    case PRESS_DOWN:
        rt_kprintf(" key3, press down! %d \n", __LINE__);
        break;
	case PRESS_UP:
        rt_kprintf(" key3, press up! %d \n", __LINE__);
        mst_btn_timer.bf_key3rd = 0;
        mst_btn_timer.delay_cnt = 0;
        break;
	case PRESS_REPEAT:
        rt_kprintf(" key3, press repeat! %d \n", __LINE__);
        break;
	case SINGLE_CLICK:
        rt_kprintf(" key3, single click! %d \n", __LINE__);
        break;
	case DOUBLE_CLICK:
        rt_kprintf(" key3, double click! %d \n", __LINE__);
        break;
	case LONG_RRESS_START:
        rt_kprintf(" key3, long press start! %d \n", __LINE__);
        break;
	case LONG_PRESS_HOLD:
        rt_kprintf(" key3, long press hold! %d \n", __LINE__);
        break;
    default:
        break;
    }

}

static void key4_btn_callback(void *params)
{
    rt_uint8_t key_evt;
    key_evt = get_button_event((struct Button *)params);
    switch (key_evt)
    {
    case PRESS_DOWN:
        rt_kprintf(" key4, press down! %d \n", __LINE__);
        break;
	case PRESS_UP:
        rt_kprintf(" key4, press up! %d \n", __LINE__);
        mst_btn_timer.bf_key4th = 0;
        mst_btn_timer.delay_cnt = 0;
        break;
	case PRESS_REPEAT:
        rt_kprintf(" key4, press repeat! %d \n", __LINE__);
        break;
	case SINGLE_CLICK:
        rt_kprintf(" key4, single click! %d \n", __LINE__);
        break;
	case DOUBLE_CLICK:
        rt_kprintf(" key4, double click! %d \n", __LINE__);
        break;
	case LONG_RRESS_START:
        rt_kprintf(" key4, long press start! %d \n", __LINE__);
        break;
	case LONG_PRESS_HOLD:
        rt_kprintf(" key4, long press hold! %d \n", __LINE__);
        break;
    default:
        break;
    }

}

static rt_uint8_t read_key1_pindata(void)
{
    return rt_pin_read(KEY1_BTN);
}
static rt_uint8_t read_key2_pindata(void)
{
    return rt_pin_read(KEY2_BTN);
}
static rt_uint8_t read_key3_pindata(void)
{
    return rt_pin_read(KEY3_BTN);
}
static rt_uint8_t read_key4_pindata(void)
{
    return rt_pin_read(KEY4_BTN);
}

static void gpio_irq_callback(void *args)
{
    unsigned int data = (unsigned int)args;
    if (data == KEY1_BTN) {
        rt_kprintf(" key1 btn callback! %d \n", __LINE__);
        mst_btn_timer.bf_key1st = 1;
    } else if (data == KEY2_BTN) {
        rt_kprintf(" key2 btn callback! %d \n", __LINE__);
        mst_btn_timer.bf_key2nd = 1;
    } else if (data == KEY3_BTN) {
        rt_kprintf(" key3 btn callback! %d \n", __LINE__);
        mst_btn_timer.bf_key3rd = 1;
    } else if (data == KEY4_BTN) {
        rt_kprintf(" key4 btn callback! %d \n", __LINE__);
        mst_btn_timer.bf_key4th = 1;
    }

     // 启动定时器
    if (mst_btn_timer.bfwork == 0) {
        mst_btn_timer.bfwork = 1;
        start_btn_timer(1);
    }
}
static void btn_timer_callback(void *params)
{
    button_ticks();
     // 关闭定时器
    if ((mst_btn_timer.bfwork == 1) 
     && (mst_btn_timer.bf_key1st == 0)
     && (mst_btn_timer.bf_key2nd == 0)
     && (mst_btn_timer.bf_key3rd == 0)
     && (mst_btn_timer.bf_key4th == 0)) {
        if (mst_btn_timer.delay_cnt++ >= LOOSE_BTN_DEF_TIME) {
            mst_btn_timer.delay_cnt = 0;
            mst_btn_timer.bfwork = 0;
            stop_btn_timer();
        }
    }
}
static void start_btn_timer(unsigned char force)
{
    if (btn_timer != NULL) {
        rt_timer_stop(btn_timer);
        // rt_timer_delete(btn_timer);
    }
    if (force) {
        rt_kprintf(" start timer! %d \n", __LINE__);
        if (btn_timer != NULL) {
            rt_timer_start(btn_timer);
        }
    }
}
static void stop_btn_timer(void)
{
    if (btn_timer != NULL) {
        rt_kprintf(" stop timer! %d \n", __LINE__);
        rt_timer_stop(btn_timer);
        // rt_timer_delete(btn_timer);
    }    
}

#undef __USER_BTN_C_



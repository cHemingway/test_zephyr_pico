#include <zephyr.h>
#include <devicetree.h>
#include <drivers/led.h>

#include <logging/log.h>
LOG_MODULE_REGISTER(main);

/* 1000 msec = 1 sec */
#define ON_TIME_MS      100
#define SLEEP_TIME_MS   400

#define LED_NODE DT_ALIAS(led0)
#define LED_PARENT_CONTROLLER DT_PARENT(LED_NODE)

#if DT_NODE_HAS_STATUS(LED_PARENT_CONTROLLER, okay)
const struct device *led_dev = DEVICE_DT_GET(LED_PARENT_CONTROLLER);
#else
#error "Node is disabled"
#endif

void led_timer(struct k_timer *timer_id);

K_TIMER_DEFINE(my_timer, led_timer, NULL);

/* Timer state, stores if the LED is on */
bool m_led_is_on = false;

/* LED timer */
void led_timer(struct k_timer *timer_id) {
    bool *led_status = k_timer_user_data_get(timer_id);
    if (*led_status) {
        led_on(led_dev,0);
        k_timer_start(&my_timer, K_MSEC(ON_TIME_MS), K_NO_WAIT);
    } else {
        led_off(led_dev,0);
        k_timer_start(&my_timer, K_MSEC(SLEEP_TIME_MS), K_NO_WAIT);
    }
    *led_status =! *led_status;
}


void main(void)
{
	int ret;

	if (!device_is_ready(led_dev)) {
        LOG_ERR("LED is not ready");
		return;
	}

    /* Method 1, try and use led_blink directly. Not supported for GPIO yet */
    ret = led_blink(led_dev, 0, ON_TIME_MS, SLEEP_TIME_MS);
    if (ret<0) {
        LOG_ERR("led_blink failed, got (%d)",ret);
    } else {
        while(1) {k_yield();}
    }

    /* Method 2, use timer delay, start a single shot timer */
    k_timer_user_data_set(&my_timer, &m_led_is_on); /* Pass in LED status as user data */
    k_timer_start(&my_timer, K_NO_WAIT, K_NO_WAIT);
    while(1) {k_yield();}

    /* Method 3, simply use a delay loop in this thread */
    while(1) {
        led_on(led_dev, 0);
        k_msleep(ON_TIME_MS);
        led_off(led_dev, 0);
        k_msleep(SLEEP_TIME_MS);
    }

}

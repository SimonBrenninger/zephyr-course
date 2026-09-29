#include "zephyr/device.h"
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/gpio.h>
#include <zephyr/drivers/sensor.h>

#include "our_driver/our_driver.h"

#define SLEEP_TIME_MS		CONFIG_APP_HEARTBEAT_PERIOD_MS

/* The devicetree node identifier for the "led0" alias. */
#define LED_NODE		DT_ALIAS(app_led)
#define OUR_SENSOR_NODE		DT_ALIAS(our_sensor)

static const struct gpio_dt_spec led = GPIO_DT_SPEC_GET(LED_NODE, gpios);
static const struct device *our_sensor = DEVICE_DT_GET(OUR_SENSOR_NODE);

LOG_MODULE_REGISTER(main, LOG_LEVEL_INF);

int main(void)
{
    bool led_state = true;

    if (!gpio_is_ready_dt(&led)) return 0;

    if (gpio_pin_configure_dt(&led, GPIO_OUTPUT_ACTIVE) < 0) return 0;

    while (1)
    {
        if (gpio_pin_toggle_dt(&led) < 0) return 0;

        led_state = !led_state;
        // LOG_INF("LED state: %s", led_state ? "ON" : "OFF");
        k_msleep(SLEEP_TIME_MS);
    }
    return 0;
}

#include "zephyr/kernel.h"
#include <sys/errno.h>
#include <zephyr/device.h>
#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zephyr/drivers/gpio.h>


#define DT_DRV_COMPAT	our_driver


// immutable settings
struct our_driver_config {
	struct gpio_dt_spec gpio;
};

// mutable data (driver state)
struct our_driver_data {
	uint8_t calib;
};


static inline int our_driver_set_led(const struct gpio_dt_spec *led)
{
	return gpio_pin_set_dt(led, 1);
}


static inline int our_driver_reset_led(const struct gpio_dt_spec *led)
{
	return gpio_pin_set_dt(led, 0);
}


static int our_driver_init(const struct device *dev)
{
	int ret;
	const struct our_driver_config *cfg = dev->config;
	const struct gpio_dt_spec gpio = cfg->gpio;

	if (!gpio_is_ready_dt(&gpio)) {
		return -ENODEV;
	}

	ret = gpio_pin_configure_dt(&gpio, GPIO_OUTPUT);
	if (ret != 0) {
		return ret;
	}
	return 0;
}


static int our_driver_sample_fetch(const struct device *dev, enum sensor_channel chan)
{
	const struct our_driver_config *cfg = dev->config;
	const struct gpio_dt_spec gpio = cfg->gpio;
	return our_driver_set_led(&gpio);
}


static int our_driver_channel_get(const struct device *dev, enum sensor_channel chan, struct sensor_value *val)
{
	val->val1 = 1;
	val->val2 = 50000;
	const struct our_driver_config *cfg = dev->config;
	const struct gpio_dt_spec gpio = cfg->gpio;
	return our_driver_reset_led(&gpio);
}


int our_driver_set_calibration(const struct device *dev, uint8_t calibration)
{
	struct our_driver_data *data = dev->data;
	data->calib = calibration;
	return 0;
}


static DEVICE_API(sensor, our_driver_api) = {
	.sample_fetch = our_driver_sample_fetch,
	.channel_get = our_driver_channel_get,
};


#define OUR_DRIVER_DEFINE(inst) 				\
	static const struct our_driver_config cfg_##inst = {	\
		.gpio = GPIO_DT_SPEC_GET(			\
			DT_INST(inst, our_driver), gpios),	\
	};							\
	static struct our_driver_data data_##inst = {		\
		.calib = 0,					\
	};							\
	DEVICE_DT_INST_DEFINE(inst,				\
		our_driver_init,				\
		NULL,						\
		&data_##inst,					\
		&cfg_##inst,					\
		POST_KERNEL,					\
		CONFIG_SENSOR_INIT_PRIORITY,			\
		&our_driver_api)


DT_INST_FOREACH_STATUS_OKAY(OUR_DRIVER_DEFINE)

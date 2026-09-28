#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>


int our_driver_set_calibration(const struct device *dev, uint32_t calibration);
int our_driver_sample_fetch(const struct device *dev, enum sensor_channel chan);

#ifdef __cplusplus
}
#endif

#endif // OUR_DRIVER_H

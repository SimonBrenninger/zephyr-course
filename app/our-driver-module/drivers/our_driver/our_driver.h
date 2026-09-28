#ifndef OUR_DRIVER_H
#define OUR_DRIVER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <zephyr/device.h>
#include <zephyr/drivers/sensor.h>


int our_driver_set_calibration(const struct device *dev, uint32_t calibration);

#ifdef __cplusplus
}
#endif

#endif // OUR_DRIVER_H

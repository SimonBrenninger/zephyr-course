#include "zephyr/device.h"
#include "zephyr/toolchain.h"
#include <stdlib.h>
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>
#include "our_driver.h"

#define OUR_SENSOR_NODE		DT_ALIAS(our_sensor)

static const struct device *our_sensor = DEVICE_DT_GET(OUR_SENSOR_NODE);
static const char* sensor_name = DEVICE_DT_NAME(OUR_SENSOR_NODE);


static int sensor_fetch_handler(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	shell_info(sh, "Fetching Sensor..");
	sensor_sample_fetch(our_sensor);
	shell_info(sh, "Fetching complete");
	return 0;
}


static int sensor_read_handler(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	struct sensor_value val;
	shell_info(sh, "Reading Sensor..");
	sensor_channel_get(our_sensor, SENSOR_CHAN_ALL, &val);
	shell_info(sh, "Reading complete: %d.%06d", val.val1, val.val2);
	return 0;
}


static int sensor_info_handler(const struct shell *sh, size_t argc, char **argv)
{
	ARG_UNUSED(argc);
	ARG_UNUSED(argv);

	char* sensor_status = device_is_ready(our_sensor) ? "okay" : "disabled";
	shell_info(sh, "Sensor Info: \n  Name: %s\n  Status: %s", sensor_name, sensor_status);
	return 0;
}


static int sensor_set_handler(const struct shell *sh, size_t argc, char **argv)
{
	int ret, calib_arg = 0;
	uint8_t calib;
	if (argc != 2) {
		shell_error(sh, "Invalid argument count: %d", argc);
		return -1;
	}
	calib_arg = atoi(argv[1]);
	if (calib_arg < 0 || calib_arg > 255)
	{
		shell_error(sh, "Calibration value (%d) out of range (0-255)", calib_arg);
		return -1;
	}

	calib = calib_arg & 0xFF;
	shell_info(sh, "Setting Sensor calibration to %" PRIu8, calib);
	if ((ret = our_driver_set_calibration(our_sensor, calib)) != 0)
	{
		shell_error(sh, "Calibration update failed (err: %d)", ret);
		return -1;
	}
	shell_info(sh, "Calibration update complete");
	return 0;
}

SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmd,
		SHELL_CMD(fetch, NULL, "Fetch sensor", sensor_fetch_handler),
		SHELL_CMD(read, NULL, "Read sensor", sensor_read_handler),
		SHELL_CMD(info, NULL, "Get sensor info", sensor_info_handler),
		SHELL_CMD_ARG(set, NULL, "Set sensor calibration <0-255>", sensor_set_handler, 2, 0),
		SHELL_SUBCMD_SET_END
);


SHELL_CMD_REGISTER(sensor, &sensor_subcmd, "Sensor shell command", NULL);

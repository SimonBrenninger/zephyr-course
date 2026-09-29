#include "zephyr/device.h"
#include <zephyr/shell/shell.h>
#include <zephyr/drivers/sensor.h>

#define OUR_SENSOR_NODE		DT_ALIAS(our_sensor)

static const struct device *our_sensor = DEVICE_DT_GET(OUR_SENSOR_NODE);
static const char* sensor_name = DEVICE_DT_NAME(OUR_SENSOR_NODE);


static int sensor_fetch_handler(const struct shell *sh, size_t argc, char **argv)
{
	shell_print(sh, "Fetching Sensor..");
	sensor_sample_fetch(our_sensor);
	shell_print(sh, "Fetching complete");
	return 0;
}


static int sensor_read_handler(const struct shell *sh, size_t argc, char **argv)
{
	struct sensor_value val;
	shell_print(sh, "Reading Sensor..");
	sensor_channel_get(our_sensor, SENSOR_CHAN_ALL, &val);
	shell_print(sh, "Reading complete: %d.%06d", val.val1, val.val2);
	return 0;
}


static int sensor_info_handler(const struct shell *sh, size_t argc, char **argv)
{
	char* sensor_status = device_is_ready(our_sensor) ? "okay" : "disabled";
	shell_print(sh, "Sensor Info: \n  Name: %s\n  Status: %s", sensor_name, sensor_status);
	return 0;
}


SHELL_STATIC_SUBCMD_SET_CREATE(sensor_subcmd,
		SHELL_CMD(fetch, NULL, "Fetch sensor", sensor_fetch_handler),
		SHELL_CMD(read, NULL, "Read sensor", sensor_read_handler),
		SHELL_CMD(info, NULL, "Get sensor info", sensor_info_handler),
		SHELL_SUBCMD_SET_END
);


SHELL_CMD_REGISTER(sensor, &sensor_subcmd, "Sensor shell command", NULL);

#include <zephyr/kernel.h>


void board_late_init_hook(void)
{
	printk("Board Initialized\n");
}

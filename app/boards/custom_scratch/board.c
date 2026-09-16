#include <zephyr/init.h>
#include <zephyr/kernel.h>


static int custom_scratch_init_hook(void)
{
	printk("Board Initialized\n");
	return 0;
}


SYS_INIT(custom_scratch_init_hook, POST_KERNEL, 0);

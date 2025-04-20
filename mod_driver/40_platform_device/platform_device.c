#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>

static struct resource my_device_resource[] = {
    [0] = {
        .start = 0x1610401A,
        .end = 0x1610401A,
        .flags = IORESOURCE_MEM,
    },
    [1] = {
        .start = 13,
        .end = 13,
        .flags = IORESOURCE_IRQ,
    },
};

void mydevice_release(struct device *dev){
    printk("This is mydevice_release!\n");
}

struct platform_device platform_device_test = {
    .name = "mydevice",
    .id = -1,
    .resource = my_device_resource,
    .num_resources = ARRAY_SIZE(my_device_resource),
    .dev = {
        .release = mydevice_release,
    },
};

static int platform_device_init(void)
{
    platform_device_register(&platform_device_test);
    printk("platform_device_init!\n");
    return 0;
}

static void platform_device_exit(void)
{
    platform_device_unregister(&platform_device_test);
    printk("platform_device_exit!\n");
}

module_init(platform_device_init);
module_exit(platform_device_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fashi");
MODULE_VERSION("V1.0");

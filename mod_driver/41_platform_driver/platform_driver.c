#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>

int mydriver_probe(struct platform_device *dev){
    printk("This is mydriver_probe\n");
    return 0;
}

int mydriver_remove(struct platform_device *dev){
    printk("This is mydriver_remove\n");
    return 0;
}

const struct platform_device_id mydriver_id_table = {
    .name = "mydevice",
};

struct platform_driver platform_driver_test = {
    .probe = mydriver_probe,
    .remove = mydriver_remove,
    .driver = {
        .name = "mydevice",
        .owner = THIS_MODULE,
    },
    .id_table = &mydriver_id_table,
};

static int platform_driver_init(void)
{
    printk("platform_driver_init!\n");
    platform_driver_register(&platform_driver_test);
    return 0;
}

static void platform_driver_exit(void)
{
    printk("platform_driver_exit!\n");
    platform_driver_unregister(&platform_driver_test);
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fashi");
MODULE_VERSION("V1.0");
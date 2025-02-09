#include <linux/module.h>
#include <linux/init.h>

static int helloworld_init(void)
{
    printk(KERN_EMERG "helloworld!\n");
    return 0;
}

static void helloworld_exit(void)
{
    printk("helloworld bye!\n");
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fashi");
MODULE_VERSION("V1.0");
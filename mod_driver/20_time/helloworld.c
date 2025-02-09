#include <linux/module.h>
#include <linux/init.h>
#include <linux/time.h>

static void timer_function(struct timer_list *t)
{
    printk("This is timer_function\n");
}

DEFINE_TIMER(test_timer, timer_function);

static int helloworld_init(void)
{
    printk("helloworld!\n");
    test_timer.expires=jiffies_64 + msecs_to_jiffies(5000);
    add_timer(&test_timer);
    return 0;
}

static void helloworld_exit(void)
{
    printk("helloworld bye!\n");
    del_timer(&test_timer);
}

module_init(helloworld_init);
module_exit(helloworld_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fashi");
MODULE_VERSION("V1.0");
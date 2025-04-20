#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <asm-generic/io.h>
#include <linux/ioctl.h>
#include <linux/time.h>
#include <linux/atomic.h>
#include <asm-generic/atomic.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>
#include <linux/of.h>

#define OFF_CMD _IO('C', 0)        //关灯
#define ON_CMD _IOW('C', 1, int)   //开灯 1.常亮  2.闪
#define BUFSIZE 1024

struct device_s_led{
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_s_led;
    struct class *class_s_led;
    struct device *device_s_led;
    char kbuf[32];
    unsigned int *vir_gpio_dr;
    int led_val;
};

struct device_node *s_led_device_node;
struct property *s_led_device_node_reg;
int s_led_device_node_reg_size;
int s_led_device_node_reg_num;
u32 s_led_device_node_reg_values[2];

struct device_s_led s_led;
static spinlock_t spinlock;
static int flag = 1;

static void timer_function(struct timer_list *t);
DEFINE_TIMER(timer, timer_function);

static void s_led_ctl(int val){
    if(val == 0){
        *(s_led.vir_gpio_dr) &= ~(1 << 3);
    }else{
        *(s_led.vir_gpio_dr) |= (1 << 3);
    }
    s_led.led_val = val;
    return;
}

static void timer_function(struct timer_list *t)
{
    //printk("add_timer 03\n");
    mod_timer(&timer, jiffies_64 + msecs_to_jiffies(1000));
    if (s_led.led_val == 0)
    {
        s_led_ctl(1);
    } else {
        s_led_ctl(0);
    }
}

static int s_led_open(struct inode *inode, struct file *file){
    spin_lock(&spinlock);
    if (flag!=1) {
        spin_unlock(&spinlock);
        return -EBUSY;
    }
    flag = 0;
    spin_unlock(&spinlock);

    file->private_data=&s_led;
    printk("this is s_led open\n");
    return 0;
}

static int s_led_release(struct inode *inode, struct file *file){
    spin_lock(&spinlock);
    flag = 1;
    spin_unlock(&spinlock);
    return 0;
}

static long s_led_ioctl (struct file *file, unsigned int cmd, unsigned long arg){
    struct device_test *test_dev=(struct device_test *)file->private_data;
    switch (cmd) 
    {
        case OFF_CMD:
             s_led_ctl(1);
             del_timer(&timer);
             break;
        case ON_CMD:
             s_led_ctl(0);
             if(arg > 1)
             {
                //printk("add_timer 01\n");
                 timer.expires = jiffies_64 + msecs_to_jiffies(1000);
                 add_timer(&timer);
                 //printk("add_timer 02\n");
             } else {
                del_timer(&timer);
             }
             break;
        default:
        break;
    }
    return 0;
}

static struct file_operations s_led_ops = {
    .owner = THIS_MODULE,
    .open = s_led_open,
    .release = s_led_release,
    .unlocked_ioctl = s_led_ioctl
};

int s_led_probe(struct platform_device *dev){
    printk("s_led:This is s_led_probe\n");

    /*
    s_led_device_node = of_find_node_by_name(NULL,"s_led");
    s_led_device_node_parent = of_get_parent(s_led_device_node);
    s_led_device_node = of_get_next_child(s_led_device_node_parent, NULL);
    s_led_device_node = of_find_compatible_node(NULL, NULL, "s_led_dev");

    of_property_read_u32_index(s_led_device_node, "reg", 0, &s_led_device_node_reg_value);
    of_property_read_string(s_led_device_node, "reg", &s_led_device_node_compatible_value);
    */

    s_led_device_node = of_find_node_by_path("/soc/s_led");
    printk("s_led:s-led device_node name is %s\n", s_led_device_node->name);

    s_led_device_node_reg = of_find_property(s_led_device_node, "reg", &s_led_device_node_reg_size);
    printk("s_led:s-led s_led node reg name is %s\n", s_led_device_node_reg->name);

    s_led_device_node_reg_num = of_property_count_elems_of_size(s_led_device_node, "reg", 4);
    printk("s_led:s-led s_led node reg num is %d\n",s_led_device_node_reg_num);

    of_property_read_variable_u32_array(s_led_device_node, "reg", s_led_device_node_reg_values, 1, 2);
    printk("s_led:s-led s_led node reg u32value[0] is 0x%x\n",s_led_device_node_reg_values[0]);
    printk("s_led:s-led s_led node reg u32value[1] is 0x%x\n",s_led_device_node_reg_values[1]);

    int ret;
    ret = alloc_chrdev_region(&s_led.dev_num,0,1,"s_led");
    if (ret < 0) {
        goto err_char_region;
    }
    printk("s_led:alloc_chrdev_region is ok\n");

    s_led.major = MAJOR(s_led.dev_num);
    s_led.minor = MINOR(s_led.dev_num);
    s_led.cdev_s_led.owner=THIS_MODULE;
    cdev_init(&s_led.cdev_s_led,&s_led_ops);

    ret = cdev_add(&s_led.cdev_s_led, s_led.dev_num, 1);
    if (ret < 0) {
        goto err_cdev_add;
    }
    printk("s_led:cdev_add is ok\n");

    s_led.class_s_led=class_create("s_led");
    if (IS_ERR(s_led.class_s_led)) {
        ret = PTR_ERR(s_led.class_s_led);
        goto err_class_create;
    }
    printk("s_led:class_create is ok\n");

    s_led.device_s_led=device_create(s_led.class_s_led,NULL,s_led.dev_num,NULL,"s_led");
    if (IS_ERR(s_led.device_s_led)) {
        ret = PTR_ERR(s_led.device_s_led);
        goto err_device_create;
    }
    printk("s_led:device_create is ok\n");


    s_led.vir_gpio_dr=ioremap(s_led_device_node_reg_values[1], 1);
    if (IS_ERR(s_led.vir_gpio_dr)) {
        ret = PTR_ERR(s_led.vir_gpio_dr);
        goto err_ioremap;
    }
    printk("s_led:ioremap is ok\n");

    s_led.led_val = 0;

    return 0;

    err_ioremap:
    iounmap(s_led.vir_gpio_dr);
    device_destroy(s_led.class_s_led,s_led.dev_num);

    err_device_create:
    class_destroy(s_led.class_s_led);

    err_class_create:
    cdev_del(&s_led.cdev_s_led);

    err_cdev_add:
    unregister_chrdev_region(s_led.dev_num,1);

    err_char_region:
    return ret;
}

int s_led_remove(struct platform_device *dev){
    printk("s_led:This is s_led_remove\n");
    return 0;
}

const struct of_device_id of_match_table_id[]={
    {.compatible = "s_led_dev"},
    {}
};

const struct platform_device_id id_table = {
    .name = "s_led_dev",
};

struct platform_driver s_led_platform_driver = {
    .probe = s_led_probe,
    .remove = s_led_remove,
    .driver = {
        .name = "s_led_dev",
        .owner = THIS_MODULE,
        .of_match_table=of_match_table_id,
    },
    .id_table = &id_table,
};

static int mod_init(void)
{
    printk("s_led:mod_init is ok\n");
    platform_driver_register(&s_led_platform_driver);
    return 0;
}

static void mod_exit(void)
{
    printk("s_led:mod_exit is ok\n");
    platform_driver_unregister(&s_led_platform_driver);

    del_timer(&timer);

    unregister_chrdev_region(s_led.dev_num,1);
    cdev_del(&s_led.cdev_s_led);
    device_destroy(s_led.class_s_led,s_led.dev_num);
    class_destroy(s_led.class_s_led);
    iounmap(s_led.vir_gpio_dr);
}

//驱动加载（必须有）:当加载驱动时，会被内核自动执行。
module_init(mod_init);
//驱动卸载（必须有）：当卸载驱动时，会被内核自动执行。
module_exit(mod_exit);

//许可证声明（必须有）：可以接收的有“GPL”、“GPL v2”、"GPL and additional rights"、"Dual BSD/GPL"、"Dual MIT/GPL"、"Dual MPL/GPL"
MODULE_LICENSE("GPL v2");

//作者和版本信息（可选）
MODULE_AUTHOR("LJT");
MODULE_VERSION("V1.0");
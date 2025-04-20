#include <linux/module.h>
#include <linux/init.h>
#include <linux/platform_device.h>
#include <linux/mod_devicetable.h>

#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <asm-generic/io.h>

struct resource *myresources;

struct device_s_led{
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_s_led;
    struct class *class_s_led;
    struct device *device_s_led;
    char kbuf[32];
    unsigned int *vir_gpio_dr;
};

struct device_s_led s_led;

static int s_led_open(struct inode *inode, struct file *file){
    file->private_data=&s_led;
    printk("this is s_led open\n");
    return 0;
}

static ssize_t s_led_read(struct file *file, char __user *buf, size_t size, loff_t *off){
    struct device_s_led *dev_s_led = (struct device_s_led *)file->private_data;
    if(copy_to_user(buf,dev_s_led->kbuf,strlen(dev_s_led->kbuf)) != 0){
        printk("copy to user err\n");
    return -1;
    }
    return 0;
}

static ssize_t s_led_write(struct file *file, const char __user *buf, size_t size, loff_t *off){
    struct device_s_led *dev_s_led = (struct device_s_led *)file->private_data;
    if(copy_from_user(dev_s_led->kbuf,buf,size) != 0){
        printk("copy from user err\n");
    return -1;
    }

    if(dev_s_led->kbuf[0] == 1){
        *(s_led.vir_gpio_dr) |= (1 << 3);
        printk("dev_s_led->kbuf[0] is %d\n", dev_s_led->kbuf[0]);
    }else if(dev_s_led->kbuf[0] == 0){
        *(s_led.vir_gpio_dr) &= ~(1 << 3);
        printk("dev_s_led->kbuf[0] is %d\n", dev_s_led->kbuf[0]);
    }
    return 0;
}

static int s_led_release(struct inode *inode, struct file *file){
    return 0;
}

static struct file_operations s_led_ops = {
    .owner = THIS_MODULE,
    .open = s_led_open,
    .read = s_led_read,
    .write = s_led_write,
    .release = s_led_release
};

int mydriver_probe(struct platform_device *dev){
    printk("This is mydriver_probe\n");

    printk("IRQ IS %lld\n", dev->resource[1].start);

    myresources = platform_get_resource(dev, IORESOURCE_IRQ, 0);
    printk("IRQ IS %0lld\n", myresources->start);

    myresources = platform_get_resource(dev, IORESOURCE_MEM, 0);
    printk("MEM IS %0llx\n", myresources->start);

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


    s_led.vir_gpio_dr=ioremap(myresources->start, 1);
    if (IS_ERR(s_led.vir_gpio_dr)) {
        ret = PTR_ERR(s_led.vir_gpio_dr);
        goto err_ioremap;
    }
    printk("s_led:device_create is ok\n");


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
    unregister_chrdev_region(s_led.dev_num,1);
    cdev_del(&s_led.cdev_s_led);
    device_destroy(s_led.class_s_led,s_led.dev_num);
    class_destroy(s_led.class_s_led);
    iounmap(s_led.vir_gpio_dr);
    printk("platform_driver_exit!\n");
    platform_driver_unregister(&platform_driver_test);
}

module_init(platform_driver_init);
module_exit(platform_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fashi");
MODULE_VERSION("V1.0");
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/fs.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/uaccess.h>
#include <linux/errno.h>
#include <linux/time.h>

#define TIME_CLOSE_CMD _IO('L', 0)
#define TIME_OPEN_CMD _IO('L', 1)
#define TIME_SETTIME_CMD _IOW('L', 2, int)

struct device_test{
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *class;
    struct device *device;
    char kbuf[32];
    int counter;
};

struct device_test dev1;

static void timer_function(struct timer_list *t);

DEFINE_TIMER(test_timer, timer_function);

static void timer_function(struct timer_list *t)
{
    mod_timer(&test_timer, jiffies_64 + msecs_to_jiffies(dev1.counter));
    printk("This is timer_function\n");
}

static int cdev_test_open(struct inode *inode, struct file *file){
    file->private_data=&dev1;
    printk("this is cdev_test_open\n");
    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off){
    struct device_test *test_dev=(struct device_test *)file->private_data;
    return 0;
}

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off){
    struct device_test *test_dev=(struct device_test *)file->private_data;
    if (copy_from_user (test_dev->kbuf,buf,size) != 0){
        printk("copy_from_user error\n");
        return -1;
    } 
    printk("test_dev->kbuf is %s\n", test_dev->kbuf);
    
    return 0;
}

static int cdev_test_release(struct inode *inode, struct file *file){
    del_timer(&test_timer);
    return 0;
}

static long cdev_test_ioctl (struct file *file, unsigned int cmd, unsigned long arg){
    struct device_test *test_dev=(struct device_test *)file->private_data;
    switch (cmd) 
    {
        case TIME_CLOSE_CMD:
             del_timer(&test_timer);
             break;
        case TIME_OPEN_CMD:
             add_timer(&test_timer);
             break;
        case TIME_SETTIME_CMD:
             test_dev->counter=arg;
             test_timer.expires = jiffies_64 + msecs_to_jiffies(test_dev->counter=arg);
             break;
        default:
        break;
    }
    return 0;
}

static struct file_operations cdev_test_ops={
    .owner=THIS_MODULE,
    .open=cdev_test_open,
    .read=cdev_test_read,
    .write=cdev_test_write,
    .release=cdev_test_release,
    .unlocked_ioctl=cdev_test_ioctl
};

static int mod_init(void)
{
    int ret;
    ret=alloc_chrdev_region(&dev1.dev_num, 0, 1, "alloc_name");
    if (ret < 0){
        goto err_chrdev;
    }
    printk("alloc_chrdev_region is ok\n");

    dev1.major=MAJOR(dev1.dev_num);
    dev1.minor=MINOR(dev1.dev_num);
    printk("major is %d\n", dev1.major);
    printk("minor is %d\n", dev1.minor);
    
    dev1.cdev_test.owner=THIS_MODULE;
    cdev_init(&dev1.cdev_test, &cdev_test_ops);
    ret = cdev_add(&dev1.cdev_test, dev1.dev_num, 1);
    if (ret < 0){
        goto err_chr_add;
    }

    dev1.class=class_create("test");
    if(IS_ERR(dev1.class)){
        ret=PTR_ERR(dev1.class);
        goto err_class_create;
    }

    dev1.device=device_create(dev1.class, NULL, dev1.dev_num, NULL, "test");
    if(IS_ERR(dev1.device)){
        ret=PTR_ERR(dev1.device);
        goto err_device_create;
    }

    return 0;

err_device_create:
    class_destroy(dev1.class);

err_class_create:
    cdev_del(&dev1.cdev_test);

err_chr_add:
    unregister_chrdev_region(dev1.dev_num,1);

err_chrdev:
    return ret;
}

static void mod_exit(void)
{
    unregister_chrdev_region(dev1.dev_num,1);
    cdev_del(&dev1.cdev_test);

    device_destroy(dev1.class,dev1.dev_num);
    class_destroy(dev1.class);
    del_timer(&test_timer);
    printk("bye bye");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fashi");
MODULE_VERSION("V1.0");
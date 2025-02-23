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

#define OFF_CMD _IO('C', 0)        //关灯
#define ON_CMD _IOW('C', 1, int)   //开灯 1.常亮  2.闪

#define GPIO_DR 0x1610401A

#define BUFSIZE 1024
static char mem[BUFSIZE] = {0};

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

static ssize_t s_led_read(struct file *file, char __user *buf, size_t size, loff_t *off){
    struct device_test *test_dev=(struct device_test *)file->private_data;
    loff_t p = *off;
    size_t count = size;
    if (p > BUFSIZE){
        return 0;
    }
    if (count > BUFSIZE - p){
        count = BUFSIZE - p;
    }

    if (copy_to_user(buf,mem+p, count) != 0){
        printk("copy_to_user error \n");
        return -1;
    }
    int i;
    for(i=0; i < strlen(mem); i++){
        printk("buf[%d] is %c\n",i, mem[i]);
    }
    printk("mem is %s, p is %lu, count is %d\n", mem+p, p, count);
    *off += count;
    return count;
}

static ssize_t s_led_write(struct file *file, const char __user *buf, size_t size, loff_t *off){
    struct device_test *test_dev=(struct device_test *)file->private_data;
    loff_t p = *off;
    size_t count = size;

    if (p > BUFSIZE){
        return 0;
    }
    if (count > BUFSIZE - p){
        count = BUFSIZE - p;
    }

    if (copy_from_user(mem+p, buf, count) != 0){
        printk("copy_from_user error \n");
        return -1;
    }
    printk("mem is %s, p is %lu\n", mem+p, p);
    *off += count;
    return count;
}

static loff_t cdev_test_llseek(struct file *file, loff_t offset, int whence){
    loff_t new_offset;
    switch (whence)
    {
        case SEEK_SET:
          if (offset < 0) {
              return -EINVAL;
              break;
          }
          if (offset > BUFSIZE) {
              return -EINVAL;
              break;
          }
          new_offset = offset;
        break;
        case SEEK_CUR:
          if ((file->f_pos+offset) > BUFSIZE) {
              return -EINVAL;
              break;
          }
          if ((file->f_pos+offset) < 0) {
              return -EINVAL;
              break;
          }
          new_offset = file->f_pos + offset;
        break;
        case SEEK_END:
          if ((file->f_pos+offset) < 0) {
              return -EINVAL;
              break;
           }
          new_offset = BUFSIZE + offset;
        break;
        default:
        return -EINVAL;
    }
    file->f_pos = new_offset;
    return new_offset;
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
    .read = s_led_read,
    .write = s_led_write,
    .llseek=cdev_test_llseek,
    .release = s_led_release,
    .unlocked_ioctl = s_led_ioctl
};

static int mod_init(void)
{
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


    s_led.vir_gpio_dr=ioremap(GPIO_DR, 1);
    if (IS_ERR(s_led.vir_gpio_dr)) {
        ret = PTR_ERR(s_led.vir_gpio_dr);
        goto err_ioremap;
    }
    printk("s_led:device_create is ok\n");

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

static void mod_exit(void)
{
    del_timer(&timer);

    unregister_chrdev_region(s_led.dev_num,1);
    cdev_del(&s_led.cdev_s_led);
    device_destroy(s_led.class_s_led,s_led.dev_num);
    class_destroy(s_led.class_s_led);
    iounmap(s_led.vir_gpio_dr);

    printk("mod exit is ok\n");
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
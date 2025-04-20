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

#define CMD_TEST0 _IO('L', 0)
#define CMD_TEST1 _IOW('L', 1, int)
#define CMD_TEST2 _IOR('L', 2, int)

#define BUFSIZE 1024

static char mem[BUFSIZE] = {0};

struct device_test{
    dev_t dev_num;
    int major;
    int minor;
    struct cdev cdev_test;
    struct class *class;
    struct device *device;
    char kbuf[32];
};

struct args
{
    int a;
    int b;
    int c;
};

struct device_test dev1;

static int cdev_test_open(struct inode *inode, struct file *file){
    file->private_data=&dev1;
    printk("this is cdev_test_open\n");

    return 0;
}

static ssize_t cdev_test_read(struct file *file, char __user *buf, size_t size, loff_t *off){
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

static ssize_t cdev_test_write(struct file *file, const char __user *buf, size_t size, loff_t *off){
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

static int cdev_test_release(struct inode *inode, struct file *file){
    return 0;
}

static long cdev_test_ioctl (struct file *file, unsigned int cmd, unsigned long arg){
    int val = 1;
    struct args test;

    switch (cmd) 
    {
        case CMD_TEST0:
        printk("This is CMD_TEST0\n");
        break;
        case CMD_TEST1:
        printk("This is CMD_TEST1\n");
        if (copy_from_user(&test, (int *)arg, sizeof(test))!=0)
        {
            printk("This is CMD_TEST1 copy_from_user err\n");
            return -1;
        }
        printk("arg is %d\n", test.a);
        printk("arg is %d\n", test.b);
        printk("arg is %d\n", test.c);
        break;
        case CMD_TEST2:
        printk("This is CMD_TEST2 and val=%d\n", val);
        if(copy_to_user((int *)arg, &val, sizeof(val))!=0)
        {
            printk("This is CMD_TEST2 copy_to_user err\n");
            return -1;
        }
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
    .llseek=cdev_test_llseek,
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

    printk("bye bye");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fashi");
MODULE_VERSION("V1.0");
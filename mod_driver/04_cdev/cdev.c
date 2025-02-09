#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>

#include <linux/fs.h>
#include <linux/kdev_t.h>

#include <linux/cdev.h>

dev_t dev_num;
struct cdev cdev_test;

struct file_operations cdev_test_ops={
    .owner=THIS_MODULE
};

static int mod_init(void)
{
    int ret;
    ret=alloc_chrdev_region(&dev_num, 0, 1, "alloc_name");
    if (ret < 0){
        printk("alloc_chrdev_region is error\n");
    }
    printk("alloc_chrdev_region is ok\n");
    
    cdev_test.owner=THIS_MODULE;
    cdev_init(&cdev_test, &cdev_test_ops);
    cdev_add(&cdev_test, dev_num, 1);
    return 0;
}

static void mod_exit(void)
{
    unregister_chrdev_region(dev_num,1);
    cdev_del(&cdev_test);
    printk("bye bye");
}

module_init(mod_init);
module_exit(mod_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("fashi");
MODULE_VERSION("V1.0");
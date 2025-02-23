#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_MITIGATION_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0xf72b337f, "cdev_del" },
	{ 0x93e2c454, "device_destroy" },
	{ 0xe6c64fd2, "class_destroy" },
	{ 0x92997ed8, "_printk" },
	{ 0x4dfa8d4b, "mutex_lock" },
	{ 0x3213f038, "mutex_unlock" },
	{ 0xcefb0c9f, "__mutex_init" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x9cca1661, "cdev_init" },
	{ 0xdfb40505, "cdev_add" },
	{ 0xcd5ec888, "class_create" },
	{ 0xaa17a720, "device_create" },
	{ 0xd9caf6e1, "__copy_user" },
	{ 0xfb578fc5, "memset" },
	{ 0x81c6c860, "cpu_data" },
	{ 0x98cf60b3, "strlen" },
	{ 0x2f2590d3, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "E3DE585660C8805EF84F10C");

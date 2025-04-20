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
	{ 0xba8fbd64, "_raw_spin_lock" },
	{ 0xb5b54b34, "_raw_spin_unlock" },
	{ 0x92997ed8, "_printk" },
	{ 0x41297e80, "of_find_node_opts_by_path" },
	{ 0xc9de5868, "of_find_property" },
	{ 0xc04b3d21, "of_property_count_elems_of_size" },
	{ 0xa14bdf5a, "of_property_read_variable_u32_array" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x9cca1661, "cdev_init" },
	{ 0xdfb40505, "cdev_add" },
	{ 0xcd5ec888, "class_create" },
	{ 0xaa17a720, "device_create" },
	{ 0x40863ba1, "ioremap_prot" },
	{ 0xedc03953, "iounmap" },
	{ 0x93e2c454, "device_destroy" },
	{ 0xe6c64fd2, "class_destroy" },
	{ 0xf72b337f, "cdev_del" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x24854c69, "__platform_driver_register" },
	{ 0xeedb39fe, "platform_driver_unregister" },
	{ 0xdc0e4855, "timer_delete" },
	{ 0x6a8831e0, "jiffies_64" },
	{ 0xc38c83b8, "mod_timer" },
	{ 0x24d273d1, "add_timer" },
	{ 0x2f2590d3, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "D54AE2D2C4E266E1863BF78");

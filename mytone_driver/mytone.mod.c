#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

BUILD_SALT;

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

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif

static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0x3726c6aa, "module_layout" },
	{ 0xe32d5ecf, "param_ops_int" },
	{ 0x6091b333, "unregister_chrdev_region" },
	{ 0x37a0cba, "kfree" },
	{ 0x5abf8d2b, "class_destroy" },
	{ 0x8781d48, "device_create" },
	{ 0xe346f67a, "__mutex_init" },
	{ 0x83c50091, "cdev_add" },
	{ 0x2f31c9f4, "cdev_init" },
	{ 0x2f129604, "gpiod_direction_output_raw" },
	{ 0x47229b5c, "gpio_request" },
	{ 0xbf451cca, "__class_create" },
	{ 0x2d6fcc06, "__kmalloc" },
	{ 0xe3ec2f2b, "alloc_chrdev_region" },
	{ 0x67ea780, "mutex_unlock" },
	{ 0x8e865d3c, "arm_delay_ops" },
	{ 0x5f754e5a, "memset" },
	{ 0xae353d77, "arm_copy_from_user" },
	{ 0x3375510b, "mutex_lock_interruptible" },
	{ 0xfe990052, "gpio_free" },
	{ 0x77abe509, "cdev_del" },
	{ 0x98171fcf, "device_destroy" },
	{ 0x6eb0fe6d, "gpiod_set_raw_value" },
	{ 0xe62ca738, "gpio_to_desc" },
	{ 0xefd6cf06, "__aeabi_unwind_cpp_pr0" },
	{ 0xc5850110, "printk" },
};

MODULE_INFO(depends, "");


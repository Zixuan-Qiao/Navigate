#ifndef MYTONE_HEADER
#define MYTONE_HEADER

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/mutex.h>

#define DEBUG
#ifdef DEBUG
#	define PDEBUG(format, args...) printk(KERN_ERR "Mytone: " format, ## args)
#else 
#	define PDEBUG(format, args...)
#endif

#define MAX_NUM 5
#define WRITE_LEN 2

static int ports[MAX_NUM] = {48, 49, 60, 115, 117};
static char* labels[MAX_NUM] = {"P9_15", "P9_23", "P9_12", "P9_25", "P9_27"};
static int num = 1;
module_param(num, int, 0644);
MODULE_PARM_DESC(num, "Number of piezos");

struct piezo_dev {
	char name[6];	// to generate devices under their class
	int gpio_port;
	dev_t dev_num;
	struct mutex lock;
	struct cdev c_dev;
};

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Providing user interface to multiple piezos");

#endif

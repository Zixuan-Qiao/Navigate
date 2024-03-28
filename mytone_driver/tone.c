#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/gpio.h>

#define NUM 48

static dev_t dev_id;
static struct cdev *mydev;
static struct class *mydev_class;
char buff[3];

ssize_t mydev_write(struct file *file, const char __user *data, size_t size, loff_t *loff) {
	// semaphore for each device
	printk("mytone write\n");
	__copy_from_user(buff, data, 2);
	buff[2] = '\0';
	int delay = (int) buff[0];
	int cycle = (int) buff[1];
	
	int i;
	for(i = 0; i < cycle; i++) {
		gpio_set_value(NUM, 0);
		mdelay(delay);
		gpio_set_value(NUM, 1);
		mdelay(delay);
		gpio_set_value(NUM, 0);
		mdelay(delay);
		gpio_set_value(NUM, 1);
		mdelay(delay);
	}
		
	return 0;
}

int mydev_open(struct inode *inode, struct file *file) {
	printk("mytone open\n");
	return 0;
} 

static struct file_operations mydev_fops = {
	.owner = THIS_MODULE, 
	.open = mydev_open,
	.write = mydev_write,
};

static int __init mydev_init(void) {
	if(!gpio_is_valid(NUM)) {
		printk("Invalid GPIO number %d. \n", NUM);
		return -ENODEV;
	}
	
	gpio_request(NUM, "P9_15");

	gpio_direction_output(NUM, 0);

	alloc_chrdev_region(&dev_id, 1, 1, "mytone");
	mydev = cdev_alloc();
	cdev_init(mydev, &mydev_fops);
	cdev_add(mydev, dev_id, 1);
	printk("major: %d; minor: %d\n", MAJOR(dev_id), MINOR(dev_id));
	
	mydev_class = class_create(THIS_MODULE, "mytone");
	device_create(mydev_class, NULL, dev_id, NULL, "mytone");
	
	return 0;
}

static void __exit mydev_exit(void) {
	device_destroy(mydev_class, dev_id);
	class_destroy(mydev_class);
	
	cdev_del(mydev);
	kfree(mydev);
	unregister_chrdev_region(dev_id, 1);
	
	gpio_set_value(NUM, 0);
	gpio_free(NUM);
	printk("mytone unloaded\n");
}

module_init(mydev_init);
module_exit(mydev_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Trying to sound a piezo");

#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/device.h>
#include <linux/delay.h>
#include <linux/gpio.h>
#include <linux/mutex.h>

#define MAX 5

static int ports[MAX] = {48, 49, 60, 115, 117};
static char* labels[MAX] = {"P9_15", "P9_23", "P9_12", "P9_25", "P9_27"};
static int num = MAX;
module_param(num, int, 0644);
MODULE_PARM_DESC(num, "Number of ports");

struct piezo_dev {
	char name[6];	// to generate sysfs user interface
	int gpio_port;
	dev_t dev_num;
	char buff[3];
	struct mutex lock;
	struct cdev c_dev;
};

static dev_t dev_id;
unsigned int piezo_major;
unsigned int piezo_minor;
struct class *dev_class;

static struct piezo_dev *piezo_devices;

ssize_t mytone_write(struct file *filp, const char __user *data, size_t size, loff_t *loff) {
	printk("mytone write\n");
	struct piezo_dev *my_piezo = filp->private_data;
	
	if(mutex_lock_interruptible(&(my_piezo->lock)))
		return -ERESTARTSYS;
	
	copy_from_user(my_piezo->buff, data, 2);
	my_piezo->buff[2] = '\0';
	int delay = (int) my_piezo->buff[0];
	int cycle = (int) my_piezo->buff[1];
	
	printk("%s Delay: %d, cycle: %d, gpio port: %d. \n", my_piezo->name, delay, cycle, my_piezo->gpio_port);
	
	int i;
	for(i = 0; i < cycle; i++) {
		gpio_set_value(my_piezo->gpio_port, 0);
		mdelay(delay);
		gpio_set_value(my_piezo->gpio_port, 1);
		mdelay(delay);
		gpio_set_value(my_piezo->gpio_port, 0);
		mdelay(delay);
		gpio_set_value(my_piezo->gpio_port, 1);
		mdelay(delay);
	}
	mutex_unlock(&(my_piezo->lock));
		
	return 0;
}

int mytone_open(struct inode *inode, struct file *filp) {
	printk("mytone open\n");
	struct piezo_dev *my_piezo;
	my_piezo = container_of(inode->i_cdev, struct piezo_dev, c_dev);
	filp->private_data = my_piezo;
	return 0;
} 

static struct file_operations mytone_fops = {
	.owner = THIS_MODULE, 
	.open = mytone_open,
	.write = mytone_write,
};

static inline void release_gpio(int num) {
	int i;
	for(i = 0; i < num; i++) {
		gpio_free(ports[i]);
	}
}

static inline void release_cdev(int num) {
	int i;
	for(i = 0; i < num; i++) {
		device_destroy(dev_class, piezo_devices[i].dev_num);
		
		cdev_del(&piezo_devices[i].c_dev);
	}
}

static int __init mytone_init(void) {	
	int result = alloc_chrdev_region(&dev_id, 0, num, "mytone");
	if(result < 0) {
		printk("Failed when requesting device number. \n");
		return result;
	}
	piezo_major = MAJOR(dev_id);
	piezo_minor = MINOR(dev_id);
	
	piezo_devices = (struct piezo_dev *)kmalloc(num * sizeof(struct piezo_dev), __GFP_NORETRY);
	if(!piezo_devices) {
		result = -ENOMEM;
		goto km_fail;
	}
	
	dev_class = class_create(THIS_MODULE, "piezo");
	if(IS_ERR(dev_class)) {
		goto class_fail;
	}
	
	int i;
	for(i = 0; i < num; i++) {
		if(!gpio_is_valid(ports[i])) {
			printk("Invalid GPIO number %d. \n", ports[i]);
			goto gpio_fail;
		}
	}
	
	for(i = 0; i < num; i++) {
		if(gpio_request(ports[i], labels[i]) < 0) {
			printk("Failed when requesting %s. \n", labels[i]);
			release_gpio(i);
			goto gpio_fail;
		}
	}
	
	for(i = 0; i < num; i++) {
		gpio_direction_output(ports[i], 0);
	}
	
	for(i = 0; i < num; i++) {
		piezo_devices[i].gpio_port = ports[i];
	
		piezo_devices[i].name[0] = 't';
		piezo_devices[i].name[1] = 'o';
		piezo_devices[i].name[2] = 'n';
		piezo_devices[i].name[3] = 'e';
		piezo_devices[i].name[4] = (char) (i + 48);
		piezo_devices[i].name[5] = '\0';
		
		piezo_devices[i].dev_num = MKDEV(piezo_major, (piezo_minor + i));
		printk("Piezo number %d, MKDEV. \n", i);
		
		cdev_init(&(piezo_devices[i].c_dev), &mytone_fops);
		printk("Piezo number %d, cdev init. \n", i);
		
		piezo_devices[i].c_dev.owner = THIS_MODULE;
		
		int error = cdev_add(&(piezo_devices[i].c_dev), piezo_devices[i].dev_num, 1);
		printk("Piezo number %d, cdev add. \n", i);
		if(error) {
			release_cdev(i);
			goto cdev_fail;
		}
		
		printk("Piezo number %d, cdev added. \n", i);
		
		mutex_init(&(piezo_devices[i].lock));
		
		device_create(dev_class, NULL, piezo_devices[i].dev_num, NULL, piezo_devices[i].name);
		printk("Piezo number %d, device created. \n", i);
	}
	
	return 0;

cdev_fail:
	release_gpio(num);

gpio_fail:
	class_destroy(dev_class);

class_fail:
	kfree(piezo_devices);
	
km_fail:
	unregister_chrdev_region(dev_id, num);
	
	return result;
}

static void __exit mytone_exit(void) {
	release_cdev(num);

	release_gpio(num);

	class_destroy(dev_class);

	kfree(piezo_devices);
	
	unregister_chrdev_region(dev_id, num);
	
	printk("mytone unloaded\n");
}

module_init(mytone_init);
module_exit(mytone_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Manage multiple piezos");

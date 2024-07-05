#include "mytone.h"

ssize_t mytone_write(struct file *filp, const char __user *data, size_t size, loff_t *loff) {
	int delay, cycle, i;
	char buff[WRITE_LEN + 1];
	struct piezo_dev *my_piezo;
	
	if(size != WRITE_LEN * sizeof(char)) {
		PDEBUG("Incorrect argument format (requires a len %d char array). \n", WRITE_LEN);
		return 0;
	}
	
	if(copy_from_user(buff, data, WRITE_LEN)) {
		PDEBUG("Copying failed. \n");
		return 0;
	}
	
	my_piezo = filp->private_data;
	
	if(mutex_lock_interruptible(&(my_piezo->lock))) {
		PDEBUG("Cannot perform mutex locking, restart system. \n");
		return 0;
	}
	
	buff[WRITE_LEN] = '\0';
	delay = (int) buff[0];
	cycle = (int) buff[1];
	
	PDEBUG("Received data: %s delay: %d, cycle: %d, gpio port: %d. \n", my_piezo->name, delay, cycle, my_piezo->gpio_port);
	
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
		
	return sizeof(char) * WRITE_LEN;
}

int mytone_open(struct inode *inode, struct file *filp) {
	struct piezo_dev *my_piezo;

	PDEBUG("Device file opened. \n");
	
	my_piezo = container_of(inode->i_cdev, struct piezo_dev, c_dev);
	if(!my_piezo) {
		PDEBUG("Failed when retrieving data. \n");
		return -EFAULT;
	}
	
	filp->private_data = my_piezo;
	
	return 0;
} 

static struct file_operations mytone_fops = {
	.owner = THIS_MODULE, 
	.open = mytone_open,
	.write = mytone_write,
};

static dev_t dev_id;
unsigned int piezo_major;
unsigned int piezo_minor;
struct class *dev_class;
static struct piezo_dev *piezo_devices;

static inline void mytone_release_gpio(int num) {
	int i;
	
	for(i = 0; i < num; i++) {
		gpio_free(ports[i]);
	}
}

static inline void mytone_release_cdev(int num) {
	int i;
	
	for(i = 0; i < num; i++) {		
		cdev_del(&piezo_devices[i].c_dev);
	}
}

static inline void mytone_destroy_device(int num) {
	int i;
	
	for(i = 0; i < num; i++) {
		device_destroy(dev_class, piezo_devices[i].dev_num);
	}
}

static int __init mytone_init(void) {
	int result, i;
	struct device *dev_res;
	
	if(num > MAX_NUM) {
		num = MAX_NUM;
		PDEBUG("Piezo number reduced, maximum is %d. \n", MAX_NUM);
	
	}
	
	result = alloc_chrdev_region(&dev_id, 0, num, "mytone");
	if(result < 0) {
		PDEBUG("Failed when requesting device number. \n");
		return result;
	}
	piezo_major = MAJOR(dev_id);
	piezo_minor = MINOR(dev_id);
	
	piezo_devices = (struct piezo_dev *)kzalloc(num * sizeof(struct piezo_dev), GFP_KERNEL);
	if(!piezo_devices) {
		result = -ENOMEM;
		goto km_fail;
	}
	
	dev_class = class_create(THIS_MODULE, "piezo");
	result = (int)PTR_ERR_OR_ZERO(dev_class);
	if(result) {
		PDEBUG("Failed when creating device class. \n");
		goto class_fail;
	}
	
	for(i = 0; i < num; i++) {
		if(!gpio_is_valid(ports[i])) {
			PDEBUG("Invalid GPIO number %d. \n", ports[i]);
			result = -ENOTTY;
			goto gpio_fail;
		}
	}
	
	for(i = 0; i < num; i++) {
		result = gpio_request(ports[i], labels[i]);
		if(result < 0) {
			printk("Failed when requesting %s. \n", labels[i]);
			mytone_release_gpio(i);
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
		
		cdev_init(&(piezo_devices[i].c_dev), &mytone_fops);
		
		piezo_devices[i].c_dev.owner = THIS_MODULE;
		
		result = cdev_add(&(piezo_devices[i].c_dev), piezo_devices[i].dev_num, 1);
		if(result < 0) {
			PDEBUG("Piezo number %d, failed when adding cdev. \n", i);
			mytone_release_cdev(i);
			mytone_destroy_device(i);
			goto cdev_fail;
		}	
		
		dev_res= device_create(dev_class, NULL, piezo_devices[i].dev_num, NULL, piezo_devices[i].name);
		result = (int)PTR_ERR_OR_ZERO(dev_res);
		if(result) {
			PDEBUG("Piezo number %d, failed when creating device file. \n", i);
			mytone_release_cdev(i + 1);
			mytone_destroy_device(i);
			goto cdev_fail;
		}
		
		mutex_init(&(piezo_devices[i].lock));
	}
	
	return 0;

cdev_fail:
	mytone_release_gpio(num);

gpio_fail:
	class_destroy(dev_class);

class_fail:
	kfree(piezo_devices);
	
km_fail:
	unregister_chrdev_region(dev_id, num);
	
	return result;
}

static void __exit mytone_exit(void) {
	mytone_destroy_device(num);

	mytone_release_cdev(num);

	mytone_release_gpio(num);

	class_destroy(dev_class);

	kfree(piezo_devices);
	
	unregister_chrdev_region(dev_id, num);
	
	PDEBUG("Driver unloaded. \n");
}

module_init(mytone_init);
module_exit(mytone_exit);

/* SPDX-License-Identifier: GPL-3.0 */
#include <linux/dev_printk.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/kdev_t.h>
#include <linux/export.h>
#include <linux/fs.h>
#include <linux/printk.h>
#include <linux/seq_file.h>
#include <linux/types.h>
#include <linux/sysfs.h>
#include <linux/kobject.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/device/class.h>

MODULE_DESCRIPTION("/dev/no device driver module");
MODULE_AUTHOR("Celina Kalus <hello@celinakalus.de>");
MODULE_LICENSE("GPL");

#define NO_CHRDEV_NAME "respectfully decline device"

static int no_open(struct inode *, struct file *);
static int no_show(struct seq_file *, void *);

static int major;
static dev_t dev_no_devt;

static const char *no_list[] = {
		"Not in this lifetime.",
		"Hard pass.",
		"Negative, Ghost Rider.",
		"I’ll take a rain check.",
		"Nah, I’m good.",
		"Not gonna happen.",
		"Thanks, but no thanks.",
		"I’m out.",
		"That’s a no from me, dawg.",
		"Can’t help you there.",
		"Over my dead Wi-Fi signal.",
		"Nope-tastic.",
		"I’ve gotta take a pass on that.",
		"I’ll have to decline your generous offer.",
		"As if!",
		"Yeah, no.",
		"In your dreams.",
		"That’s a no for now.",
		"I don’t think so, Tim.",
		"Denied!",
	};

static const struct file_operations no_fops = {
	.owner = THIS_MODULE,
	.open = no_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

/* attribute for device class of /dev/no */
static struct attribute *class_dev_no_attrs[] = {
	NULL,
};
ATTRIBUTE_GROUPS(class_dev_no);

/* /dev/no device class */
static struct class class_dev_no = {
	.name = "dev_no",
	.class_groups = class_dev_no_groups,
};

/* pointer to the kobj corresponding to /dev/no */
struct device *dev_no_device;

static int no_open(struct inode *inode, struct file *file)
{
	return single_open(file, no_show, NULL);
}

static int no_show(struct seq_file *file,  void *data)
{
	static int list_index = 0;

	(void)data;
	seq_printf(file, "%s\n", no_list[list_index]);
	list_index = (list_index + 1) % ARRAY_SIZE(no_list);
	return 0;
}

static __init int my_init(void)
{
	int retval = 0;

	/* register the character device */
	retval = register_chrdev(0, NO_CHRDEV_NAME, &no_fops);

	if (retval <= 0) {
		pr_err("Failed to register char device: %i\n", retval);

		return retval;
	}

	major = retval;
	dev_no_devt = MKDEV(major, 0);

	/*
	 * create a device class. if this fails, it's okay to return early
	 * without unregistering the char device; the user will just have to
	 * create the device file /dev/no themselves.
	 */
	retval = class_register(&class_dev_no);

	if (retval < 0) {
		pr_err("Could not register device class: %i\n", retval);

		return 0;
	}

	dev_no_device = device_create(&class_dev_no, NULL, dev_no_devt, NULL, "no");

	if (PTR_ERR_OR_ZERO(dev_no_device)) {
		pr_err("Unable to create device /dev/no: %li\n", PTR_ERR(dev_no_device));

		return 0;
	}

	dev_info(dev_no_device, "Registered device /dev/no with major %i.\n", major);

	return 0;
}
module_init(my_init);

static __exit void my_exit(void)
{
	device_destroy(&class_dev_no, dev_no_devt);
	class_unregister(&class_dev_no);
	unregister_chrdev(major, NO_CHRDEV_NAME);
}
module_exit(my_exit);

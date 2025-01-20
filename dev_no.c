/* SPDX-License-Identifier: GPL-3.0 */
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

MODULE_DESCRIPTION("/dev/no device driver module");
MODULE_AUTHOR("Celina Kalus <hello@celinakalus.de>");
MODULE_LICENSE("GPL");

#define NO_CHRDEV_NAME "respectfully decline device"

static int no_open(struct inode *, struct file *);
static int no_show(struct seq_file *, void *);

static int major;

static const struct file_operations no_fops = {
	.owner = THIS_MODULE,
	.open = no_open,
	.read = seq_read,
	.llseek = seq_lseek,
	.release = single_release,
};

static int no_open(struct inode *inode, struct file *file)
{
	return single_open(file, no_show, NULL);
}

static int no_show(struct seq_file *file,  void *data)
{
	(void)data;
	seq_puts(file, "no.\n");
	return 0;
}

static __init int my_init(void)
{
	int retval = 0;

	retval = register_chrdev(0, NO_CHRDEV_NAME, &no_fops);

	if (retval <= 0) {
		pr_err("Failed to register char device: %i\n", retval);

		return retval;
	}

	major = retval;

	pr_info("Registered char device with major %i.\n", major);

	return 0;
}
module_init(my_init);

static __exit void my_exit(void)
{
	unregister_chrdev(major, NO_CHRDEV_NAME);
}
module_exit(my_exit);

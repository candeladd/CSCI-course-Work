#include<linux/init.h>
#include<linux/module.h>

#include<linux/fs.h>
#include<asm/uaccess.h>
#define BUFFER_SIZE 1024

static char device_buffer[BUFFER_SIZE];
static int opnd = 0;
static int clsd = 0;
static int len = 0;
ssize_t simple_char_driver_read (struct file *pfile, char __user *buffer, size_t length, loff_t *offset)
{
	int uncopy = copy_to_user(*buffer, device_buffer+*offset, length);
	printk(KERN_EMERG "%s\n", buffer);
	*offset += length - uncopy;
/* *buffer is the userspace bufr to where you are writing the data you want to be read from the device file*/
	/*  length is the length of the userspace buffer*/
	/*  current position of the opened file*/
	/* copy_to_user function. source is device_buffer (the buffer defined at the start of the code) and destination is the userspace buffer *buffer */
	return length - uncopy;
}



ssize_t simple_char_driver_write (struct file *pfile, const char __user *buffer, size_t length, loff_t *offset)
{
	int uncopy = copy_from_user(device_buffer+*offset+len, buffer, length);
	len+=*offset;
	/* *buffer is the userspace buffer where you are writing the data you want to be written in the device file*/
	/*  length is the length of the userspace buffer*/
	/*  current position of the opened file*/
	/* copy_from_user function. destination is device_buffer (the buffer defined at the start of the code) and source is the userspace buffer *buffer */
	return length - uncopy;
}


int simple_char_driver_open (struct inode *pinode, struct file *pfile)
{
	printk(KERN_ALERT "Device has been opened\n");
	printk(KERN_ALERT "This device has been opened %d times\n", opnd);
	opnd++;
	/* print to the log file that the device is opened and also print the number of times this device has been opened until now*/
	return 0;
}


int simple_char_driver_close (struct inode *pinode, struct file *pfile)
{
        printk(KERN_ALERT "Device has been closed\n");
        printk(KERN_ALERT "This device has been closed %d times\n", clsd);
	clsd++;
	/* print to the log file that the device is closed and also print the number of times this device has been closed until now*/
	return 0;
}

struct file_operations simple_char_driver_file_operations = {

	.owner   = THIS_MODULE,
	.open = simple_char_driver_open,
	.read = simple_char_driver_read,
	.write = simple_char_driver_write,
	.release = simple_char_driver_close,

	/* add the function pointers to point to the corresponding file operations. look at the file fs.h in the linux souce code*/
};

static int simple_char_driver_init(void)
{
	printk(KERN_ALERT "Inside %s function\n",__FUNCTION__);
	register_chrdev(150, "simple_char_driver", &simple_char_driver_file_operations);
	return 0;
	
	/* print to the log file that the init function is called.*/
	/* register the device */
}

static int simple_char_driver_exit(void)
{
	printk(KERN_ALERT "Inside %s function\n",__FUNCTION__);
        unregister_chrdev(150, "simple_char_driver");
        return 0;
	/* print to the log file that the exit function is called.*/
	/* unregister  the device using the register_chrdev() function. */
}

/* add module_init and module_exit to point to the corresponding init and exit function*/

module_init(simple_char_driver_init);
module_exit(simple_char_driver_exit);

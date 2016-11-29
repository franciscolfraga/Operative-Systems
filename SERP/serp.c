/*                                                     
 * $Id: hello.c,v 1.5 2004/10/26 03:32:21 corbet Exp $ 
*/
 //version 6.2
#include "serial_reg.h"                                                 
#include <linux/init.h>
#include <linux/module.h>
#include <linux/types.h>
#include <linux/ioport.h>
#include <linux/fs.h>
#include <asm/io.h> 
#include <linux/cdev.h>
#include <asm/uaccess.h>
#include <linux/slab.h>
#include <linux/sched.h>
#include <linux/fs.h> 
#include <linux/jiffies.h>
MODULE_LICENSE("Dual BSD/GPL");
#define ECHO_MAJOR 0   /* dynamic major by default */
#define ECHO_DEVS 4
#define start 0x3f8
#define len 0x008
unsigned char lcr = 0x3f8+UART_LCR;
unsigned char lsr =  0x3f8 + UART_LSR;
  /* echo0 through echo3 */
//declaration of variables
static dev_t dev;
int major,result;
int echo_major=ECHO_MAJOR;     /* echo.c */
int echo_devs=ECHO_DEVS;
module_param(echo_major, int, 0);
module_param(echo_devs, int, 0);

//declaration of structs

struct echo_dev { 
struct cdev* cdp; // struct cdev for this echo device 
int cnt; // number of characters written to device 
};
static struct echo_dev dnr[4];//dnr "device nr.", might support several devices, say 2 or 4
//declaration of functions
int open(struct inode *inodep, struct file *filep);
int close(struct inode *inodep, struct file *filep);
ssize_t write(struct file *filep, const char __user *buff, size_t count, loff_t *offp);
ssize_t read(struct file *filep, char __user *buff, size_t count, loff_t *offp);
extern int nonseekable_open(struct inode * inode, struct file * filp);
ssize_t read(struct file *filep, char __user *buff, size_t count, loff_t *offp);
ssize_t write(struct file *filep, const char __user *buff, size_t count, loff_t *offp);
unsigned long copy_to_user(void __user *to,const void *from,unsigned long count);
unsigned long copy_from_user(void *to, const void __user *from, unsigned long count);

struct file_operations echo_fops = {
	.owner = THIS_MODULE,
	.open = open,
	.write = write,
	.read = read,
	.release = close,
	.llseek =  no_llseek,
};
static int hello_init(void)
{
	request_region(start,len,"serp");
	lcr=UART_LCR_DLAB; 
	outb(lcr, start + UART_LCR); // coloca DLAB a 1 para mudar a bitrate
	
	outb(UART_DIV_1200, start +UART_DLL); // BITRATE A 1200 BPS
	outb(0x0, start +UART_DLM);

	lcr &= ~UART_LCR_DLAB;
	outb(lcr, start + UART_LCR); // DLAB a 0
	
	lcr=UART_LCR_WLEN8 | UART_LCR_PARITY | UART_LCR_EPAR | UART_LCR_STOP; //PAR - 2STOP BITS - palavra de 8BITS
	int c,err;
	outb(lcr,start + UART_LCR);
	outb(0,start+UART_IER); //interrupçoes a 0
	result = alloc_chrdev_region(&dev, 0, echo_devs, "serp");
	echo_major = MAJOR(dev);
	printk(KERN_ALERT "Major number is: %d\n",echo_major);
	for(c=0;c<4;c++){
	dnr[c].cdp = kmalloc(sizeof(struct cdev), GFP_KERNEL); 
	cdev_init(dnr[c].cdp, &echo_fops);
	dnr[c].cdp->ops = &echo_fops; 
	dnr[c].cdp->owner = THIS_MODULE;
	err=cdev_add (dnr[c].cdp, dev, 1);
}
	return 0;

}

static void hello_exit(void)
{
	int c;
	for(c=0;c<4;c++){
	cdev_del(dnr[c].cdp);
}
	unregister_chrdev_region(dev, echo_devs);
	release_region(start, len);
 	printk (KERN_ALERT "MYCOM1 freed successfully\n");
}


int open (struct inode *inodep, struct file *filep)
{
	/*use filep->private_data to point to the device data */
	static struct echo_dev* cdevs;
	cdevs = container_of(&inodep->i_cdev, struct echo_dev, cdp);
	nonseekable_open(inodep, filep);
	filep->private_data = cdevs;
	printk(KERN_ALERT "Open initialized\n");
	return 0;          /* success */
}
int close(struct inode *inodep, struct file *filep){
	return 0;
}
ssize_t write(struct file *filep, const char __user *buff, size_t count, loff_t *offp){
	static struct echo_dev* cdevs;
	cdevs = filep->private_data;
	char* charbuffer=kmalloc(sizeof(char)*count,GFP_KERNEL);
	if (copy_from_user(charbuffer,buff, count)!=0) {
		printk(KERN_ALERT "ERROR\n");
		return -1;
	}
	int c;
	for(c=0;c<count;c++){
			while((inb(lsr) & UART_LSR_THRE)==0){
				set_current_state(TASK_INTERRUPTIBLE);
				schedule_timeout (4);
			}
			outb(charbuffer[c],start+UART_RX);
			cdevs->cnt++;
		}
	/* update the size */
	kfree(charbuffer);
	return count;
}



ssize_t read(struct file *filep, char __user *buff, size_t count, loff_t *offp){
	static struct echo_dev* cdevs;
	cdevs = filep->private_data;
	char* charbuffer=kmalloc(sizeof(char)*(count),GFP_KERNEL);
	if(inb(lsr) & (UART_LSR_OE || UART_LSR_PE || UART_LSR_FE)==0){
		printk(KERN_ALERT "ERROR\n");
		return -EIO;
	}
	while((inb(lsr) & UART_LSR_DR )==0){
			set_current_state(TASK_INTERRUPTIBLE);
			schedule_timeout (4);
	}
	*charbuffer=inb(start+UART_TX);
	copy_to_user(buff,charbuffer, count);
	kfree(charbuffer);
	return 1;
}
module_init(hello_init);
module_exit(hello_exit);

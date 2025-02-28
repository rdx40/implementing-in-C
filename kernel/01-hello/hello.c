#include <linux/init.h> //for module init & cleanup
#include <linux/module.h> //used for including the module into the kernel
#include <linux/kernel.h> 

MODULE_LICENSE("GPL");  //code must include GPL if any GNU Public Licensed Code is included
MODULE_AUTHOR("Omar"); 
MODULE_DESCRIPTION("a simple hello");


static int __init hello_init(void){
	printk(KERN_INFO "Hello\n");
	return 0;
}

static void __exit hello_exit(void){
	printk(KERN_INFO "Goodbye\n");
}

module_init(hello_init);
module_exit(hello_exit);


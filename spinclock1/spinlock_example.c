#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/delay.h>
#include <linux/kthread.h>
#include <linux/sched.h>
#include <linux/semaphore.h>
#include <linux/spinlock.h> 

MODULE_LICENSE("GPL");
MODULE_AUTHOR("tommy"); 
MODULE_DESCRIPTION("tm plays with kthreads within test module."); 
MODULE_VERSION("0.2"); 
#define TM_DMESG_PREFIX "tm spinlock: "

static struct task_struct *thread1,*thread2;
static spinlock_t my_lock;
static int my_global_resource = 1000;
static int mparam_int1=1,new_value=1000;
static char* mparam_str1;



static int func1(void *__para)
{
    int lock ;
    
    printk(KERN_INFO "%s Thread func1 started", TM_DMESG_PREFIX);
    printk(KERN_INFO "%s my_global_resource %d by the enter of th func1.",TM_DMESG_PREFIX,my_global_resource);

    //spin_lock(&my_lock);

    while(!kthread_should_stop()) {
        
        lock = spin_trylock(&my_lock);
        
        //global resource write
        if(!lock) {
            printk(KERN_INFO "%s Unable to take lock in func1",TM_DMESG_PREFIX);
        } else {
            printk(KERN_INFO "%s Lock acquired in func1",TM_DMESG_PREFIX);
            my_global_resource+=2;
            spin_unlock(&my_lock);     
            printk(KERN_INFO "%s Lock released in func1",TM_DMESG_PREFIX);
        }
        schedule();
        set_current_state(TASK_INTERRUPTIBLE);
        msleep(1000);
    }
    
    //global resourece read
    printk(KERN_INFO "%s my_global_resource %d by the exit of th func1.",TM_DMESG_PREFIX,my_global_resource);

    return 0;
}

static int  func2(void *__para) {
    int lock;
    
    printk(KERN_INFO "%s Thread func2 started",TM_DMESG_PREFIX);
    printk(KERN_INFO "%s my_global_resource %d by the enter of th func2.",TM_DMESG_PREFIX,my_global_resource);

    while(!kthread_should_stop()) {

        lock = spin_trylock(&my_lock);
        
        if(!lock) {
            printk(KERN_INFO "%s Unable to take lock in func2",TM_DMESG_PREFIX);
        } else {
            printk(KERN_INFO "%s Lock acquired in func2",TM_DMESG_PREFIX);

            //write to global resource 
            my_global_resource-=10; 
            spin_unlock(&my_lock);
            printk(KERN_INFO "%s Lock released in func2",TM_DMESG_PREFIX);
        }
        schedule();
        set_current_state(TASK_INTERRUPTIBLE);

        msleep(1100);
    }

    //global resourece read
    printk(KERN_INFO "%s my_global_resource %d by the exit of th func2.",TM_DMESG_PREFIX,my_global_resource);

    return 0;
}



static int __init start_init(void)
{
    char name1[11]="tm thread1";
    char name2[11]="tm thread2";

    printk(KERN_INFO "%s module initialized. params values mparam_int1=%d mparam_str1=%s",TM_DMESG_PREFIX,mparam_int1,mparam_str1);
    spin_lock_init(&my_lock);
    printk(KERN_INFO "%s my_loc initialized",TM_DMESG_PREFIX);

    thread1 = kthread_create(func1,NULL,name1);
    if(thread1) {
        wake_up_process(thread1);
    }

    thread2 = kthread_create(func2,NULL,name2);
    if(thread2) {
        wake_up_process(thread2);
    }

    printk(KERN_INFO "%s start_init() done",TM_DMESG_PREFIX);   
    return 0;
}



static void __exit end_exit(void)
{
    int ret1,ret2;
 
    ret1 = kthread_stop(thread1);
    if(!ret1)
        printk(KERN_INFO "%s Thread stopped via kthread_stop(thread1)",TM_DMESG_PREFIX);

    ret2 = kthread_stop(thread2);
    if(!ret2)
        printk(KERN_INFO "%s Thread stopped via kthread_stop(thread2)",TM_DMESG_PREFIX);


    //czy tutaj powinienem cos zrobic z &my_lock
    //spin_lock_init(&my_lock);
    printk(KERN_INFO "%s params values mparam_int1=%d mparam_str1=%s at end_exit()",TM_DMESG_PREFIX,mparam_int1,mparam_str1);
    printk(KERN_INFO "%s end_exit() done.",TM_DMESG_PREFIX);
}

int tm_param_set_int(const char *val, const struct kernel_param *kp)
{
    //setter  is called after #echo 100 > /sys/modules/spinlock_example/parameters/mparam_int1
    //having custom callback handler for just setting the param makes no sense 
    //lets try to do more :-)

    int res = param_set_int(val, kp);
    if (res==0) {
        printk(KERN_INFO "%s tm_param_set_int called to set mparam_int1 to new value %d",TM_DMESG_PREFIX,new_value);

        if (new_value > 10 && new_value < 1000){ //my testing hack to reject value gr 1000
            spin_lock(&my_lock);
            printk(KERN_INFO "%s taking lock to set mparam_int1 and reset my_global_resource to 10000",TM_DMESG_PREFIX);
            mparam_int1 = new_value;
            //write to global resource 
            my_global_resource=10000; 
            spin_unlock(&my_lock);
            printk(KERN_INFO "%s mparam_int1=%d by exit of tm_param_set_int",TM_DMESG_PREFIX,mparam_int1);
            return 0;
        }        
        else {
            //I dont like to set value le 10 or gr 1000 
            printk(KERN_INFO "%s no changes made, mparam_int1=%d by exit of tm_param_set_int",TM_DMESG_PREFIX,mparam_int1);
            //check this cmd to trigger this path
            //sudo echo 8888 >  /sys/module/spinlock_example/parameters/mparam_int1 
            //echo: write error: Operation not permitted
        }
    }
    //cant set or value le 10 gr 1000
    return -1;
}

int tm_param_get_int(char *val, const struct kernel_param *kp)
{    
    //getter is called after #cat /sys/modules/spinlock_example/parameters/mparam_int1
    printk(KERN_INFO "%s tm_param_get_int called to returm mparam_int1 and its value is %d",TM_DMESG_PREFIX,mparam_int1);
        
    spin_lock(&my_lock);
    //read global resource 
    //aka critical operation
    //but also dumps to dmesg my_global_resource
    printk(KERN_INFO "%s taking the lock here and reading my_global_resource=%d ",TM_DMESG_PREFIX,my_global_resource); 
    spin_unlock(&my_lock);

   return param_get_int(val, kp);
}

const struct kernel_param_ops tm_param_ops =
{
    .set = &tm_param_set_int, //my custom callback handler with lock
    .get = &tm_param_get_int, //my custom callback handler with lock - now works 
    //.get = &param_get_int, //std getter
};

//module_param_cb this is the way to register my custom getter and setter via tm_param_ops for module param mparam_int1
module_param_cb(mparam_int1,&tm_param_ops,&new_value,S_IRUGO|S_IWUSR );

//default built-in getter and setter for mparam_int1 are not used as I wanted to try on my own :-)
//module_param(mparam_int1, int, S_IRUSR|S_IWUSR);

//and default built-in getter and setter for mparam_str1 is to be used - open: custom getter and setter for the param type char[] 
module_param(mparam_str1, charp, S_IRUSR|S_IWUSR);
module_init(start_init);
module_exit(end_exit);

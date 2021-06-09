

#include <linux/module.h>
#include <net/sock.h>
#include <linux/netlink.h>
#include <linux/skbuff.h>
// #include <linux/list.h>

#define NETLINK_USER 31
#define LIST_HEAD(name) struct list_head name = LIST_HEAD_INIT(name) 
struct sock *nl_sk = NULL;
struct node {
     int pid ;
     struct list_head mylist ;
} ;
LIST_HEAD(subscriber_list);

static void hello_nl_recv_msg(struct sk_buff *skb) {
struct nlmsghdr *nlh;
int pid;
struct sk_buff *skb_out;
int msg_size;
char *msg="Registered";
char *sent_val;
int sent_val_size;
int res;
printk(KERN_INFO "Entering: %s\n", __FUNCTION__);

msg_size=strlen(msg);
nlh=(struct nlmsghdr*)skb->data;
printk(KERN_INFO "Netlink received msg payload:%s\n",(char*)nlmsg_data(nlh));
// msg = (char*)nlmsg_data(nlh);
sent_val = (char*)nlmsg_data(nlh);
sent_val_size = strlen(sent_val);


        printk(KERN_INFO "Comparing %s and 0 equaling %d\n", sent_val, strcmp(sent_val, "0"));
    if (strcmp(sent_val, "0")==0){
        pid = nlh->nlmsg_pid; /*pid of sending process */
        skb_out = nlmsg_new(msg_size,0);
        if(!skb_out)
        {
            printk(KERN_ERR "Failed to allocate new skb\n");
            return;

        } 
        nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,msg_size,0);  
        NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
        printk(KERN_INFO "%d Added to list\n", pid);
        struct node *new = kmalloc(sizeof(*new), GFP_KERNEL);
        new->pid = pid;
        list_add(&new->mylist, &subscriber_list);
        strncpy(nlmsg_data(nlh),msg,msg_size);
        res=nlmsg_unicast(nl_sk,skb_out,pid);
        if(res<0)
            printk(KERN_INFO "Error while sending bak to user\n");
    }
    else {
        struct node *datastructureptr = NULL;
        list_for_each_entry ( datastructureptr , & subscriber_list, mylist ) 
            { 
            printk ("Sending to   %d\n" , datastructureptr->pid ); 
            pid = nlh->nlmsg_pid; /*pid of sending process */
            skb_out = nlmsg_new(sent_val_size,0);
            if(!skb_out)
            {
                printk(KERN_ERR "Failed to allocate new skb\n");
                return;

            }
            nlh=nlmsg_put(skb_out,0,0,NLMSG_DONE,sent_val_size,0);  
            NETLINK_CB(skb_out).dst_group = 0; /* not in mcast group */
            printk(KERN_INFO "Redirecting to subscriber\n");
            strncpy(nlmsg_data(nlh),sent_val,sent_val_size);
            res=nlmsg_unicast(nl_sk,skb_out,datastructureptr->pid);
            if(res<0)
                printk(KERN_INFO "Error while sending bak to user\n");
        }
    }
 printk(KERN_INFO "To the End child\n");
}

static int __init hello_init(void) {

printk("Entering: %s\n",__FUNCTION__);
//This is for 3.6 kernels and above.
struct netlink_kernel_cfg cfg = {
    .input = hello_nl_recv_msg,
};

nl_sk = netlink_kernel_create(&init_net, NETLINK_USER, &cfg);
if(!nl_sk)
{

    printk(KERN_ALERT "Error creating socket.\n");
    return -10;

}

return 0;
}

static void __exit hello_exit(void) {

printk(KERN_INFO "exiting hello module\n");
netlink_kernel_release(nl_sk);
}

module_init(hello_init); module_exit(hello_exit);

MODULE_LICENSE("GPL");
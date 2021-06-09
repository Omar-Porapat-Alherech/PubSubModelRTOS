//
// Created by pora on 4/12/20.
//
#include <linux/netlink.h>
#include <sys/socket.h>
#include <unistd.h>
#include <pthread.h>
#define NETLINK_USER 31
#define MAX_PAYLOAD 1024 /* maximum payload size*/

#ifndef PROJECT4_IOPUBSUB_H
#define PROJECT4_IOPUBSUB_H


struct IOPubSub {
	struct sockaddr_nl src_addr, dest_addr;
	struct nlmsghdr *nlh;
	struct iovec iov;
	int sock_fd;
	struct msghdr msg;
	char * first_byte;
};

#endif //PROJECT4_IOPUBSUB_H

//
//if(((char*)NLMSG_DATA(proc_1->nlh))[0] =='0'){
//send_payload(proc_1);
//while(1){
//printf("Waiting for message from kernel\n");
//recvmsg(proc_1->sock_fd, &(proc_1->msg), 0);
//printf("1  Received message payload: %s\n", NLMSG_DATA(proc_1->nlh));
//}
//}
//else {
//while(1){
//set_payload(proc_1);
//send_payload(proc_1);
//}
//}

//if (child_1 == 0){
//child_2 = fork();
//if (child_2 == 0){
//child_3 = fork();
//if(child_3 == 0){
//
//}
//else {
////Parent 3
//}
//}
//else {
////Parent 2
//}
//}
//else {
////Parent 1
//}

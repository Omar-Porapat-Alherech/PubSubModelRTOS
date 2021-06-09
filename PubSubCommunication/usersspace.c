//
// Created by pora on 4/12/20.
//
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include<sys/wait.h>
#include<unistd.h>
#include <stdbool.h>
#include "IOPubSub.h"


struct IOPubSub * initialize(){
	struct IOPubSub * new_process = malloc(sizeof(struct sockaddr_nl) + sizeof(struct nlmsghdr) + sizeof(struct iovec)
	                                       + sizeof(int) + sizeof(struct msghdr) + sizeof(char) + (sizeof( char * )*MAX_PAYLOAD-1));
	new_process->nlh = NULL;
	new_process->sock_fd = socket(PF_NETLINK, SOCK_RAW, NETLINK_USER);
	if (new_process->sock_fd < 0)
		printf("Error Initializing\n");

	memset(&(new_process->src_addr), 0, sizeof(new_process->src_addr));
	new_process->src_addr.nl_family = AF_NETLINK;
	new_process->src_addr.nl_pid = getpid(); /* self pid */
	new_process->src_addr.nl_groups = 0;

	bind(new_process->sock_fd, (struct sockaddr *)&(new_process->src_addr), sizeof(new_process->src_addr));

	memset(&(new_process->dest_addr), 0, sizeof(new_process->dest_addr));
	new_process->dest_addr.nl_family = AF_NETLINK;
	new_process->dest_addr.nl_pid = 0; /* For Linux Kernel */
	new_process->dest_addr.nl_groups = 0; /* unicast */

	new_process->nlh = (struct nlmsghdr *)malloc(NLMSG_SPACE(MAX_PAYLOAD));
	memset(new_process->nlh, 0, NLMSG_SPACE(MAX_PAYLOAD));
	new_process->nlh->nlmsg_len = NLMSG_SPACE(MAX_PAYLOAD);
	new_process->nlh->nlmsg_pid = getpid();
	new_process->nlh->nlmsg_flags = 0;

	new_process->iov.iov_base = (void *)new_process->nlh;
	new_process->iov.iov_len = new_process->nlh->nlmsg_len;
	new_process->msg.msg_name = (void *)&(new_process->dest_addr);
	new_process->msg.msg_namelen = sizeof(new_process->dest_addr);
	new_process->msg.msg_iov = &(new_process->iov);
	new_process->msg.msg_iovlen = 1;

};

void set_payload(struct IOPubSub * obj){
	char payload[MAX_PAYLOAD];
	fputs("Publisher: Please enter payload\n", stdout);
	fgets(payload, MAX_PAYLOAD, stdin);
	strtok(((char*)NLMSG_DATA(obj->nlh)), "\n");
	strcpy(NLMSG_DATA(obj->nlh), payload);
}

int set_first_byte(struct IOPubSub * obj){
	int flush;
	char payload[MAX_PAYLOAD];
	fputs("Please enter 0 for a Subscriber, and 1 for a Publisher\n", stdout);
	fgets(payload, MAX_PAYLOAD, stdin);
	return atoi(payload);
}

//void rec_payload(){
//	recvmsg(sock_fd, &msg, 0);
//	printf("Received message payload: %s\n", NLMSG_DATA(nlh));
//	close(sock_fd);
//}

void send_payload(struct IOPubSub * obj){
	printf("Sending message to kernel\n");
	sendmsg(obj->sock_fd, &(obj->msg), 0);
}

int main (){
	int val;
	int flush;
	struct IOPubSub * proc_1 = initialize();
	val = set_first_byte(proc_1);
	
	if(val==0){
		printf("Subscriber\n");
		printf("1  Setting pid as %d\n", proc_1->nlh->nlmsg_pid );
		strcpy(NLMSG_DATA(proc_1->nlh), "0");
		send_payload(proc_1);
		while(1){
			recvmsg(proc_1->sock_fd, &(proc_1->msg), 0);
			printf("Received message payload: %s\n", NLMSG_DATA(proc_1->nlh));
		}
	}
	if (val==1){
		printf("Publisher\n");
		while(1){
			set_payload(proc_1);
			send_payload(proc_1);
		}
	}

	// if(((char*)NLMSG_DATA(proc_1->nlh))[0]=='0'){
	// 	printf("1  Setting pid as %d\n", proc_1->nlh->nlmsg_pid );
	// 	send_payload(proc_1);
	// 	while(1){
	// 		recvmsg(proc_1->sock_fd, &(proc_1->msg), 0);
	// 		printf("Received message payload: %s\n", NLMSG_DATA(proc_1->nlh));
	// 	}
	// }
	// else {
	// 	while(1){
	// 		set_payload(proc_1);
	// 		send_payload(proc_1);
	// 	}
	// }
}



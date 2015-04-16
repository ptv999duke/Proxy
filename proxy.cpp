#include "proxy.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

Proxy::Proxy(int port, int cache_size) {
	myPort = port;
	myCacheSize = cache_size;
}


int Proxy::server()
{
	int sock,new_s;
	struct sockaddr_in server_addr;
	pthread_t thread;
	int recv_len;


	/* build address data structure */
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_addr.s_addr =INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(myPort);

	/* setup passive open */
	if ((sock = socket(PF_INET, SOCK_STREAM/* use tcp */, 0)) < 0) {
		perror("Create socket error:");
		return 1;
	}

	if ((bind(sock,(struct sockaddr *)&server_addr,sizeof(server_addr)))<0){
		perror("Bind socket error:");
		return 1;
	}

	listen(sock, 32767);


	while (1){
		if ((new_s = accept(sock, (struct sockaddr *)&server_addr, (socklen_t *) &recv_len)) < 0) {
					perror("Accept error:");
					exit(1);
				}
				if (pthread_create(&thread, NULL, newSock, (void *) &new_s)<0){
					perror("Could not create thread\n");
					return 1;
				}

			}
	return 0;
}

void * Proxy::newSock(void *new_socket) {

	// int recv_len;
	// char buffer[MAX_MSG_LENGTH],msg[MAX_MSG_LENGTH*3];
	// int new_s = *(int *) new_socket;

	// printf("Running a new thread now\n");

	// while (recv_len = recv(new_s, buffer, MAX_MSG_LENGTH, 0)){
	// 	if (recv_len<0){
	// 		perror("Receive Error:");
	// 		exit(1);
	// 	}
	// 	strncat(msg, buffer, MAX_MSG_LENGTH);
	// 	strncat(msg, buffer, MAX_MSG_LENGTH);
	// 	strncat(msg, buffer, MAX_MSG_LENGTH);

	// 	if ((send(new_s, msg, MAX_MSG_LENGTH*3, 0)) < 0) {
	// 		perror("Send error:");
	// 		exit(1);
	// 	}
	// 	memset(msg,0,sizeof(msg));
	// 	memset(buffer,0,sizeof(buffer));
	// }
	// close(new_s);
	pthread_exit(NULL);

}
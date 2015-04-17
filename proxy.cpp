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
				if (pthread_create(&thread, NULL, Proxy::newSock, (void *) &new_s)<0){
					perror("Could not create thread\n");
					return 1;
				}

			}
	return 0;
}

 void * Proxy::newSock(void *new_socket) {

	int MAX_GET_REQUEST_LENGTH = 10000;
	int MAX_RESPONSE_LENGTH = 10000;
	char buffer[MAX_GET_REQUEST_LENGTH],msg[MAX_RESPONSE_LENGTH];
	int clientSock = *(int *) new_socket;
	int recv_len;

	printf("Running a new thread now\n");
	int isPersistent = true;
	int serverSock;
	do {
		while (recv_len = recv(clientSock, buffer, MAX_GET_REQUEST_LENGTH, 0)){
			cout<< "in while"<<endl;
			cout << recv_len <<endl;
			//cout << buffer <<endl;
			if (recv_len > 0){
				if(strcmp(buffer, "GET") == 32 ){
					char* serverIP="199.27.79.73"; //Extract from http header
					//Check if it's GET and if it's in cache. If not :
					struct sockaddr_in server_addr;
					char serverResponse[MAX_RESPONSE_LENGTH];
					if ((serverSock = socket(AF_INET, SOCK_STREAM/* use tcp */, 0)) < 0) {
						perror("Create server socket error:");
						close(clientSock);
						pthread_exit(NULL);
						return NULL;
					}
					cout <<"after serverSock"<<endl;
					server_addr.sin_addr.s_addr = inet_addr(serverIP);
					server_addr.sin_family = AF_INET;
					server_addr.sin_port = htons(80);

					cout<<"before connect"<<endl;
					if (connect(serverSock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
						close(clientSock);
						pthread_exit(NULL);
					}
					cout<<"before send"<<endl;

					if (send(serverSock, buffer, MAX_GET_REQUEST_LENGTH, 0) < 0) {
						perror("Send to server error:");
						close(clientSock);
						close(serverSock);
						pthread_exit(NULL);
					}
					cout<<"before read"<<endl;

					recv_len = read(serverSock, msg, MAX_RESPONSE_LENGTH);
					cout<<"after read"<<endl;

					cout << msg <<endl;
					if ((send(clientSock, msg, recv_len, 0)) < 0) {
					perror("Send error:");
					pthread_exit(NULL);
					}

					cout << "after send"<<endl;
				}

				//store in cache
			}

		}
	} while(isPersistent);
	
	close(serverSock);
	close(clientSock);
	pthread_exit(NULL);

}



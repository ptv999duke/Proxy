#include "proxy.h"
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <iostream>
#include <string>
#include <netdb.h>
#include <sstream>

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
		// cout<<"start while"<<endl;
		while (recv_len = recv(clientSock, buffer, MAX_GET_REQUEST_LENGTH, 0)){
			cout<< "in while"<<endl;
			cout << recv_len <<endl;
			

			if (recv_len > 0){
				cout << buffer <<endl;
				string str(buffer);
				size_t host_start= str.find("Host");
				size_t url_end=str.find("\n",host_start+1);
				string host=str.substr(host_start+6,url_end-host_start-7);
				cout <<"host starts at:"<<host_start<<endl;
				cout<<"url ends at:" << url_end << endl;
				cout << host<<endl;
				// cout << firstline.at(url_end-13) <<endl;
				// cout << firstline.at(url_end-2) <<endl;
				// if (firstline.at(url_end-2)=='/')
				// 	url = firstline.substr(11,url_end-13);
				// else
				// 	url = firstline.substr(11,url_end-12);
				// cout << url <<endl;
				if(strcmp(buffer, "GET") == 32)
				{
					char* serverIP="199.27.79.73"; //Extract from http header
					//Check if it's GET and if it's in cache. If not :
					// struct sockaddr_in server_addr;
					char serverResponse[MAX_RESPONSE_LENGTH];
					// if ((serverSock = socket(AF_INET, SOCK_STREAM use tcp , 0)) < 0) {
					// 	perror("Create server socket error:");
					// 	close(clientSock);
					// 	pthread_exit(NULL);
					// 	return NULL;
					// }

				   struct addrinfo hints;
		           struct addrinfo *result, *rp;
		           int sfd, s;

		           memset(&hints, 0, sizeof(struct addrinfo));
		           hints.ai_family = AF_INET;    
		           hints.ai_socktype = SOCK_STREAM; /* Use TCP */
		           hints.ai_flags = 0;    /* For wildcard IP address */
		           hints.ai_protocol = 0;          /* Any protocol */
		     	   int portnum=80;
		           stringstream strs;
  				   strs << portnum;
  				   string temp_str = strs.str();
 				   char* const port = (char*) temp_str.c_str();
				   char* const ipAddress = (char*) host.c_str();
				   cout << ipAddress << endl;
				   cout << port <<endl;
		           s = getaddrinfo(ipAddress, "http", &hints, &result);

		           if (s != 0) {
		               fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(s));
		               // exit(EXIT_FAILURE);
		               // continue;
		           }
		           	  // getaddrinfo() returns a list of address structures.
		              // Try each address until we successfully connect(2).
		              // If socket(2) (or connect(2)) fails, we (close the socket
		              // and) try the next address. 

		           for (rp = result; rp != NULL; rp = rp->ai_next) {

		           		if ((serverSock = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol)) < 0)
		           			continue;

		               	if (connect(serverSock, rp->ai_addr, rp->ai_addrlen) == 0){
		               		// cout <<"success ip address:" << 
		               		// 	inet_ntoa((*rp->ai_addr).sin_addr.s_addr)<<endl;
		               		break;                  /* Success */
		               	}

		               	perror("Create server socket error:");
					   	close(serverSock);
					   	pthread_exit(NULL);
					   	return NULL;
		           }

		           if (rp == NULL) {               /* No address succeeded */
		               fprintf(stderr, "Could not connect\n");
		               exit(EXIT_FAILURE);
		           }

		           freeaddrinfo(result);           /* No longer needed */




					// cout <<"after serverSock"<<endl;
					// server_addr.sin_addr.s_addr = inet_addr(serverIP);
					// server_addr.sin_family = AF_INET;
					// server_addr.sin_port = htons(80);

					// cout<<"before connect"<<endl;
					// if (connect(serverSock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
					// 	close(clientSock);
					// 	pthread_exit(NULL);
					// }
					cout<<"before send"<<endl;

					if (send(serverSock, buffer, MAX_GET_REQUEST_LENGTH, 0) < 0) {
						perror("Send to server error:");
						// close(clientSock);
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
				cout << "still alive" <<endl;
				//store in cache
			}
			cout << "still alive in while" <<endl;
		}
	} while(isPersistent);
	
	close(serverSock);
	close(clientSock);
	pthread_exit(NULL);

}



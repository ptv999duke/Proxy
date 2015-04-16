#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

int server(uint16_t port);
int client(const char * addr, uint16_t port);

#define MAX_MSG_LENGTH (512)
#define MAX_BACK_LOG (5)


int main(int argc, char ** argv)
{
	if (argc < 3) {
		printf("Command should be: myprog s <port> or myprog c <port> <address>\n");
		return 1;
	}
	int port = atoi(argv[2]);
	if (port < 1024 || port > 65535) {
		printf("Port number should be equal to or larger than 1024 and smaller than 65535\n");
		return 1;
	}
	if (argv[1][0] == 'c') {
		if(argv[3]==NULL){
			printf("NO IP address is given\n");
			return 1;
		}
		return client(argv[3], port);
	} else if (argv[1][0] == 's') {
		return server(port);
	} else {
		printf("unknown command type %s\nCommand should be: myprog s <port> or myprog c <port> <address>", argv[1]);
		return 1;
	}
	return 0;
}

int client(const char * addr, uint16_t port)
{
	int sock;
	struct sockaddr_in server_addr;
	char msg[MAX_MSG_LENGTH], reply[MAX_MSG_LENGTH*3];

	if ((sock = socket(AF_INET, SOCK_STREAM/* use tcp */, 0)) < 0) {
		perror("Create socket error:");
		return 1;
	}

	printf("Socket created\n");
	server_addr.sin_addr.s_addr = inet_addr(addr);
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	if (connect(sock, (struct sockaddr*)&server_addr, sizeof(server_addr)) < 0) {
		perror("Connect error:");
		return 1;
	}

	printf("Connected to server %s:%d\n", addr, port);

	int recv_len = 0;
	while (1) {
		fflush(stdin);
		printf("Enter message: \n");
		gets(msg);
		if (send(sock, msg, MAX_MSG_LENGTH, 0) < 0) {
			perror("Send error:");
			return 1;
		}
		recv_len = read(sock, reply, MAX_MSG_LENGTH*3);
		if (recv_len < 0) {
			perror("Recv error:");
			return 1;
		}
		reply[recv_len] = 0;
		printf("Server reply:\n%s\n", reply);
		memset(reply, 0, sizeof(reply));
	}
	close(sock);
	return 0;
}

void * newSock(void *new_socket){

	int recv_len;
	char buffer[MAX_MSG_LENGTH],msg[MAX_MSG_LENGTH*3];
	int new_s = *(int *) new_socket;

	printf("Running a new thread now\n");

	while (recv_len = recv(new_s, buffer, MAX_MSG_LENGTH, 0)){
		if (recv_len<0){
			perror("Receive Error:");
			exit(1);
		}
		strncat(msg, buffer, MAX_MSG_LENGTH);
		strncat(msg, buffer, MAX_MSG_LENGTH);
		strncat(msg, buffer, MAX_MSG_LENGTH);

		if ((send(new_s, msg, MAX_MSG_LENGTH*3, 0)) < 0) {
			perror("Send error:");
			exit(1);
		}
		memset(msg,0,sizeof(msg));
		memset(buffer,0,sizeof(buffer));
	}
	close(new_s);
	pthread_exit(NULL);

}

int server(uint16_t port)
{
	int sock,new_s;
	struct sockaddr_in server_addr;
	pthread_t thread;
	int recv_len;


	/* build address data structure */
	bzero((char *)&server_addr, sizeof(server_addr));
	server_addr.sin_addr.s_addr =INADDR_ANY;
	server_addr.sin_family = AF_INET;
	server_addr.sin_port = htons(port);

	/* setup passive open */
	if ((sock = socket(PF_INET, SOCK_STREAM/* use tcp */, 0)) < 0) {
		perror("Create socket error:");
		return 1;
	}

	if ((bind(sock,(struct sockaddr *)&server_addr,sizeof(server_addr)))<0){
		perror("Bind socket error:");
		return 1;
	}

	listen(sock,MAX_BACK_LOG);


	while (1){
	if ((new_s = accept(sock, (struct sockaddr *)&server_addr, &recv_len)) < 0) {
				perror("Accept error:");
				exit(1);
			}
			if (pthread_create(&thread, NULL, newSock, (void *)&new_s)<0){
				perror("Could not create thread\n");
				return 1;
			}

		}

	return 0;
}



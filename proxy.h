#ifndef PROXY_H
#define PROXY_H

#include <iostream>

using namespace std; 

class Proxy {
	public:
		Proxy(int, int);
		int server();
		int myPort;
		int myCacheSize;
		void * newSock(void *);
};

#endif
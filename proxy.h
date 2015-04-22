#include <iostream>
#include "Cache.h"

#ifndef PROXY_H
#define PROXY_H

using namespace std; 

class Proxy {
	public:
		Proxy(int, int);
		int server();
		int myPort;
		int myCacheSize;
		static void * newSock(void *, Cache );
};

#endif
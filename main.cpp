#include <iostream>
#include <stdio.h>
#include "proxy.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char * argv[]) {
	if(argc == 2) {
		char * port_str = argv[0];
		char * cache_size_str = argv[1];
		int port = atoi(port_str);
		int cache_size = atoi(cache_size_str);
		Proxy * proxy = new Proxy(port, cache_size);
		proxy->server();
	}
	else {
		cout << "incorrect arguments";
	}
}
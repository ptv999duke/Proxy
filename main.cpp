#include <iostream>
#include <stdio.h>
#include "proxy.h"
#include <stdlib.h>

using namespace std;

int main(int argc, char ** argv) {
	if(argc == 3) {
		string port_str = argv[1];
		string cache_size_str = argv[2];
		int port = atoi(port_str.c_str());
		int cache_size = atoi(cache_size_str.c_str());
		Proxy * proxy = new Proxy(port, cache_size);
		proxy->server();
	}
	else {
		cout << "incorrect arguments" << endl;
	}
}
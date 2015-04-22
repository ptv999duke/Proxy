#ifndef CACHENODE_H
#define CACHENODE_H

#include <stdlib.h>
#include <string>

using namespace std;

class CacheNode {
	public:
		bool operator==(const CacheNode&) const;
		bool operator <(const CacheNode&) const;
		CacheNode(char*, int);
		char* data;
		int size;
};

#endif
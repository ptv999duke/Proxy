#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <iostream>
#include <map>
#include "CacheNode.h"
#include "KeyNode.h"
#include <string>

using namespace std; 

class Cache {
	protected: 
		map<KeyNode, CacheNode> * cacheMap;
		int myRemainingSize;
		static const int MAX_AGE = 4000;
		map<KeyNode, CacheNode>::iterator containsKey(KeyNode);
	public:
		Cache(int);
		char* getFromCache(char*);
		void addToCache(char*, char*, int);
};

#endif
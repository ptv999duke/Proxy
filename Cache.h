#ifndef CACHE_H
#define CACHE_H

#include <stdlib.h>
#include <iostream>
#include <map>
#include "CacheNode.h"
#include "KeyNode.h"
#include <string>
#include <pthread.h>

using namespace std; 

class Cache {
	protected: 
		map<KeyNode, CacheNode> * cacheMap;
		int myRemainingSize;
		static const int MAX_AGE = 4000;
		map<KeyNode, CacheNode>::iterator containsKey(KeyNode);
		pthread_mutex_t mutex;
	public:
		Cache(int);
		int getFromCache(char*, char*);
		void addToCache(char*, char*, int);
};

#endif
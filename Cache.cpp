#include "Cache.h"
#include <cstring>
#include <list>
#include <vector>

using namespace std;

Cache::Cache(int size) {
	myRemainingSize = size;
	cacheMap = new map<KeyNode, CacheNode>;
	pthread_mutex_init(&mutex, NULL);
}

int Cache::getFromCache(char* desiredKey, char * output) {
	pthread_mutex_lock(&mutex);
	string sKey(desiredKey);
	KeyNode desiredKeyNode(sKey, MAX_AGE);
	map<KeyNode, CacheNode>::iterator it;
	vector<pair<KeyNode, CacheNode> > v;
	int ret;
	if(containsKey(desiredKeyNode) == cacheMap->end()) {
		pthread_mutex_unlock(&mutex);
		return -1;
	}
	for(it = cacheMap->begin(); it != cacheMap->end(); it++) {
		KeyNode kn = it->first;
		CacheNode cn = it->second;
		v.push_back(make_pair(kn, cn));
		if(kn == desiredKeyNode) {
			v.back().first.age = MAX_AGE;
			output = (char*) malloc(sizeof(char) * cn.size);
			memcpy(output, cn.data, cn.size);
			ret = cn.size;
		}
		else {
			v.back().first.age = kn.age - 1;
		}
		cacheMap->erase(kn);
	}
	while(!v.empty()) {
		cacheMap->insert(v.back());
		v.pop_back();
	}
	pthread_mutex_unlock(&mutex);
	return ret;
}

void Cache::addToCache(char* key, char* data, int size) {
	pthread_mutex_lock(&mutex);
	string sKey(key);
	KeyNode newKn(sKey, MAX_AGE);
	char * sdata = (char*) malloc(sizeof(char) * size);
	memcpy(sdata, data, size);
	//string sdata(data);
	map<KeyNode, CacheNode>::iterator exists = containsKey(newKn);
	if(exists != cacheMap->end()) {
		int oldSize = exists->second.size;
		cacheMap->erase(exists);
		myRemainingSize += oldSize;
	}
	if(myRemainingSize >= size) {
		pair<KeyNode, CacheNode> p = make_pair(newKn, CacheNode(sdata,size));
		cacheMap->insert(p);
		myRemainingSize -= size;
		pthread_mutex_unlock(&mutex);
		return;
	}
	else{
		map<KeyNode, CacheNode>::iterator it;
		for(it = cacheMap->begin(); it != cacheMap->end();) {
			CacheNode cn = it->second;
			myRemainingSize += cn.size;
			cacheMap->erase(it);
			if(myRemainingSize >= size) {
				pair<KeyNode, CacheNode> p = make_pair(newKn, CacheNode(sdata,size));
				cacheMap->insert(p);
				myRemainingSize -= size;
				break;
			}
			else{
				++ it;
			}

		}
	}
	pthread_mutex_unlock(&mutex);
}

map<KeyNode, CacheNode>::iterator Cache::containsKey(KeyNode desiredKeyNode) {
	map<KeyNode, CacheNode>::iterator it;
	for(it = cacheMap->begin(); it != cacheMap->end(); it++) {
		KeyNode kn = it->first;
		if(kn == desiredKeyNode) {
			return it;
		}
	}
	return it;
}
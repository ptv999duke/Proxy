#include "Cache.h"
#include <cstring>

using namespace std;

Cache::Cache(int size) {
	myRemainingSize = size;
	cacheMap = new map<KeyNode, CacheNode>;
}

char* Cache::getFromCache(char* desiredKey) {
	//string keyString(desiredKey);
	KeyNode desiredKeyNode(MAX_AGE);
	map<KeyNode, CacheNode>::iterator it;
	char* output;
	for(it = cacheMap->begin(); it != cacheMap->end(); it++) {
		KeyNode kn = it->first;
		CacheNode cn = it->second;
		if(desiredKeyNode == kn) {
			output = new char[cn.data.length() + 1];
			strcpy(output, cn.data.c_str());
		}
		else {
			cn.age = cn.age - 1;
		}
	}
	return output;
}

void Cache::addToCache(char* key, char* data, int size) {
	KeyNode newKn(MAX_AGE);
	string sdata(data);
	map<KeyNode, CacheNode>::iterator exists = cacheMap->find(newKn);
	if(exists != cacheMap->end()) {
		int oldSize = exists->second.size;
		cacheMap->erase(exists);
		myRemainingSize += oldSize;
	}
	if(myRemainingSize >= size) {
		cacheMap->insert(make_pair(newKn, CacheNode(sdata,size,MAX_AGE)));
		myRemainingSize -= size;
		return;
	}
	else{
		map<KeyNode, CacheNode>::iterator it;
		for(it = cacheMap->begin(); it != cacheMap->end();) {
			CacheNode cn = it->second;
			myRemainingSize += cn.size;
			cacheMap->erase(it);
			if(myRemainingSize >= size) {
				cacheMap->insert(make_pair(newKn, CacheNode(sdata,size, MAX_AGE)));
				myRemainingSize += size;
				break;
			}
			else{
				++ it;
			}

		}
	}
}
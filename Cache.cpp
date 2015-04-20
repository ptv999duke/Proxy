#include "Cache.h"
#include <cstring>

using namespace std;

Cache::Cache(int size) {
	myRemainingSize = size;
	cacheMap = new map<KeyNode, CacheNode>;
}

char* Cache::getFromCache(char* desiredKey) {
	cout << "get" << endl;
	string sKey(desiredKey);
	KeyNode desiredKeyNode(sKey, MAX_AGE);
	map<KeyNode, CacheNode>::iterator it;
	char* output;
	for(it = cacheMap->begin(); it != cacheMap->end(); it++) {
		KeyNode kn = it->first;
		CacheNode cn = it->second;
		cout << "key " + kn.key << endl;
		cout <<  kn.age << endl;
		if(kn == desiredKeyNode) {
			kn.age = MAX_AGE;
			cn.age = MAX_AGE;
			output = new char[cn.size + 1];
			strcpy(output, cn.data.c_str());
		}
		else {
			kn.age = kn.age - 1;
			cn.age = cn.age - 1;
			cout << "asdf" << endl;
			cout << kn.age << endl;
		}
	}
	return output;
}

void Cache::addToCache(char* key, char* data, int size) {
	string sKey(key);
	KeyNode newKn(sKey, MAX_AGE);
	string sdata(data);
	map<KeyNode, CacheNode>::iterator exists = containsKey(newKn);
	if(exists != cacheMap->end()) {
		int oldSize = exists->second.size;
		cacheMap->erase(exists);
		myRemainingSize += oldSize;
	}
	if(myRemainingSize >= size) {
		pair<KeyNode, CacheNode> p = make_pair(newKn, CacheNode(sdata,size, MAX_AGE));
		cacheMap->insert(p);
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
				pair<KeyNode, CacheNode> p = make_pair(newKn, CacheNode(sdata,size, MAX_AGE));
				cacheMap->insert(p);
				myRemainingSize += size;
				break;
			}
			else{
				++ it;
			}

		}
	}
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
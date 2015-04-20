#include "Cache.h"
#include <cstring>
#include <list>
#include <vector>

using namespace std;

Cache::Cache(int size) {
	myRemainingSize = size;
	cacheMap = new map<KeyNode, CacheNode>;
}

char* Cache::getFromCache(char* desiredKey) {
	string sKey(desiredKey);
	KeyNode desiredKeyNode(sKey, MAX_AGE);
	map<KeyNode, CacheNode>::iterator it;
	char* output;
	vector<pair<KeyNode, CacheNode> > v;
	if(containsKey(desiredKeyNode) == cacheMap->end()) {
		return '\0';
	}
	for(it = cacheMap->begin(); it != cacheMap->end(); it++) {
		KeyNode kn = it->first;
		CacheNode cn = it->second;
		v.push_back(make_pair(kn, cn));
		if(kn == desiredKeyNode) {
			v.back().first.age = MAX_AGE;
			output = new char[cn.size + 1];
			strcpy(output, cn.data.c_str());
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
		//cout << "zxc" << endl;
		map<KeyNode, CacheNode>::iterator it;
		for(it = cacheMap->begin(); it != cacheMap->end();) {
			cout << "removing key " + it->first.key << endl;
			cout << "value " + it->second.data << endl;
			cout << it->first.age << endl;
			CacheNode cn = it->second;
			myRemainingSize += cn.size;
			cacheMap->erase(it);
			if(myRemainingSize >= size) {
				pair<KeyNode, CacheNode> p = make_pair(newKn, CacheNode(sdata,size, MAX_AGE));
				cacheMap->insert(p);
				myRemainingSize -= size;
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
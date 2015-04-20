#include "CacheNode.h"

using namespace std;

CacheNode::CacheNode(string cdata, int csize, int cage) {
	data = cdata;
	size = csize;
}

bool CacheNode::operator==(const CacheNode& cn) const {
	return data == cn.data;
}

bool CacheNode::operator <(const CacheNode& cn) const {
	return age < cn.age;
}
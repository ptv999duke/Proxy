#include "CacheNode.h"

using namespace std;

CacheNode::CacheNode(char* cdata, int csize) {
	data = cdata;
	size = csize;
}

bool CacheNode::operator==(const CacheNode& cn) const {
	return data == cn.data;
}

bool CacheNode::operator <(const CacheNode& cn) const {
	return data < cn.data;
}
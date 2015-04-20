#ifndef KEYNODE_H
#define KEYNODE_H

#include <string>
using namespace std; 

class KeyNode {
	public: 
		KeyNode(string, int);
		bool operator==(const KeyNode&) const;
		bool operator <(const KeyNode&) const;
		bool operator <=(const KeyNode&) const;
		bool operator >=(const KeyNode&) const;
		bool operator >(const KeyNode&) const;
		int age;
		string key;
};

#endif
#ifndef KEYNODE_H
#define KEYNODE_H

using namespace std; 

class KeyNode {
	public: 
		KeyNode(int);
		bool operator==(const KeyNode&) const;
		bool operator <(const KeyNode&) const;
		int age;
};

#endif
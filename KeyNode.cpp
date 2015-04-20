#include "KeyNode.h"
#include <stdio.h>
#include <iostream>

KeyNode::KeyNode(string ckey, int cage) {
	age = cage;
	key = ckey;
}

bool KeyNode::operator==(const KeyNode& kn) const{
	bool ret = key == kn.key;
	return ret;
}

bool KeyNode::operator <(const KeyNode& kn) const{
	cout << "less" << endl;
	if(age != kn.age) {
		return age < kn.age;
	}
	else{
		return key < kn.key;
	}
}

bool KeyNode::operator <= (const KeyNode& kn) const{
	cout << "less equal" << endl;
	return age <= kn.age && key <= kn.key;
}

bool KeyNode::operator >= (const KeyNode& kn) const{
	cout << "greater equal" << endl;
	return age >= kn.age && key>= kn.key;
}

bool KeyNode::operator >(const KeyNode& kn) const{
	cout << "greater" << endl;
	if(age != kn.age) {
		return age > kn.age;
	}
	else{
		return key > kn.key;
	}
}

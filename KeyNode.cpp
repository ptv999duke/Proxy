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
	if(age != kn.age) {
		return age < kn.age;
	}
	else{
		return key < kn.key;
	}
}


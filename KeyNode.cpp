#include "KeyNode.h"

KeyNode::KeyNode(int cage) {
	age = cage;
}

bool KeyNode::operator==(const KeyNode& kn) const{

}

bool KeyNode::operator <(const KeyNode& kn) const{
	return age < kn.age;
}
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include "Cache.h"

using namespace std;

int main(int argc, char ** argv) {
	char* firstKey = "a";
	char* firstValue = "first";
	char* secondKey = "b"; 
	char* secondValue = "second";
	char* thirdKey = "c";
	char* thirdValue = "third";
	Cache * cache = new Cache(100);
	cache->addToCache(firstKey, firstValue, 5);
	cache->addToCache(secondKey, secondValue, 6);
	cout << cache->getFromCache(firstKey) << endl;
	cout << cache->getFromCache(secondKey) << endl;

}
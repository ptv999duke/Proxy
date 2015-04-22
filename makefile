make: *.cpp *.h
	g++ -o proxy *.cpp *.h -pthread
	#g++ -o cachetest Cache.cpp Cache.h CacheNode.cpp CacheNode.h KeyNode.cpp KeyNode.h cachetest.cpp 
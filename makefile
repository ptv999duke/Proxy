make: *.cpp *.h
	g++ -o proxy *.cpp *.h -pthread

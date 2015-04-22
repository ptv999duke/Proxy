make: *.cpp *.h
	g++ -o proxy *.h *.cpp -pthread
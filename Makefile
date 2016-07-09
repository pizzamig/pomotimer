CXX=clang++

pomotimer: pomotimer.cpp pomotimer.h
	$(CXX) -g pomotimer.cpp -o pomotimer -lrt -Wall -std=c++11

clean:
	rm -f pomotimer

CXX=clang++
LDFLAGS+=-lrt -lncurses -lpthread -L/usr/local/lib
CFLAGS+=-Wall -std=c++11 -g -O0

pomotimer: main.cpp pomotimer.cpp pomotimer.h
	$(CXX) $(CFLAGS) $(LDFLAGS) pomotimer.cpp main.cpp -o pomotimer

clean:
	rm -f pomotimer

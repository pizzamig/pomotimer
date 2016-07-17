CXX?=clang++
export LDFLAGS+=-lrt -lncurses -lpthread -L/usr/local/lib
export CFLAGS+=-Wall -std=c++11 -g -O0

SRCS=main.cpp pomotimer.cpp nctk.cpp tracker.cpp
HDRS=pomotimer.h observer.h nctk.h tracker.cpp

all: pomotimer doc buildtests

pomotimer: $(SRCS) $(HDRS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(SRCS) -o pomotimer

clean:
	rm -f pomotimer
	rm -rf html
	$(MAKE) -C tests clean

doc: $(SRCS) $(HDRS)
	doxygen Doxyfile

buildtests:
	$(MAKE) -C tests build

test:
	$(MAKE) -C tests run
	
.PHONY: all clean doc tests

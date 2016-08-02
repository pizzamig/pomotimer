CXX?=clang++
export LDFLAGS+=-L/usr/local/lib -lrt -lncurses -lpthread
export CFLAGS+=-Wall -std=c++11 -g -O0

SRCS=main.cpp pomotimer.cpp nctk.cpp tracker.cpp time.cpp
HDRS=pomotimer.h observer.h nctk.h tracker.h time.h
OBJS=$(SRCS:%.cpp=%.o)

all: pomotimer doc buildtests

%.o: %.cpp %.h
	$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

pomotimer: $(OBJS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(OBJS) -o pomotimer $(LDFLAGS)

clean:
	rm -f pomotimer
	rm -rf html
	rm -f $(OBJS)
	$(MAKE) -C tests clean

doc: $(SRCS) $(HDRS)
	doxygen Doxyfile

buildtests:
	$(MAKE) -C tests build

test:
	$(MAKE) -C tests gcov
	
.PHONY: all clean doc tests

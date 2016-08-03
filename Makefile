CXX?=clang++
export LDFLAGS+=-L/usr/local/lib -lrt -lncurses -lpthread
export CXXFLAGS+=-Wall -std=c++11 -g -O0

SRCS=main.cpp pomotimer.cpp nctk.cpp tracker.cpp time.cpp
HDRS=pomotimer.h observer.h nctk.h tracker.h time.h
OBJS=$(SRCS:%.cpp=%.o)

COMPILER!=	$(CXX) --version | head -n 1
ifeq (, $(strip $(filter clang,$(COMPILER))) )
	ifeq (, $(strip $(filter g++,$(COMPILER)), g++ ) )
		CXX=$(error Compiler $(COMPILER) not supported)
	else
		COMPILER=g++
	endif
else
	COMPILER=clang
endif
export COMPILER

all: pomotimer doc buildtests

%.o: %.cpp %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

pomotimer: $(OBJS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) -o pomotimer $(LDFLAGS)

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

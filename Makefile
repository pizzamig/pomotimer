CXX=clang++
LDFLAGS+=-lrt -lncurses -lpthread -L/usr/local/lib
CFLAGS+=-Wall -std=c++11 -g -O0

SRCS=main.cpp pomotimer.cpp nctk.cpp
HDRS=pomotimer.h observer.h nctk.h

pomotimer: $(SRCS) $(HDRS)
	$(CXX) $(CFLAGS) $(LDFLAGS) $(SRCS) -o pomotimer

clean:
	rm -f pomotimer
	rm -rf html

doc: $(SRCS) $(HDRS)
	doxygen Doxyfile

.PHONY: doc clean
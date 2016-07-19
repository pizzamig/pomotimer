# Makefile for unittests

UUT=tracker.cpp
UUTO=$(UUT:.cpp=.o)
EXE=trackerTest
SRCS=pomotimer_tracker.cpp
OBJS=$(SRCS:.cpp=.o) $(UUTO)

$(UUTO): ../../$(UUT)
	$(CXX) $(CFLAGS) -c ../../$(UUT) -o $(UUTO)

%.o: ../%.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

$(EXE): $(OBJS) $(UUTO)
	$(CXX) $(CFLAGS) $(LDFLAGS) $? -o $@

$(UUT):
	ln -s ../../$@

%.cpp:
	ln -s ../$@

$(UUT:.cpp=.gcda): $(EXE)
	./$(EXE)

clean:
	rm -f $(OBJS) *.gcno *.gcda *.gcov $(EXE)
	rm -f $(SRCS) $(UUT)

gcov: $(SRCS) $(UUT) $(UUT:.cpp=.gcda)
	gcov $(SRCS) $(UUT)

.PHONY: clean gcov 

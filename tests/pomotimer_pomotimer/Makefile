# Makefile for unittests

UUT=pomotimer.cpp
UUTO=$(UUT:.cpp=.o)
EXE=pomotimerTest
SRCS=pomotimer_pomotimer.cpp
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

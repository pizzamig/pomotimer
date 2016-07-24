export LDFLAGS+=-lcppunit
export CFLAGS+=-coverage

UNITS=config pomodoro pomotimer tracker
TEST_SRCS=$(UNITS:%=pomotimer_%.cpp)
TEST_OBJS=$(UNITS:%=pomotimer_%.o)
EXES=$(UNITS:=Test)
UUTS=pomotimer.cpp tracker.cpp
UUTO=$(UUTS:%.cpp=%.o)

pomotimer.o: ../pomotimer.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

tracker.o: ../tracker.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.cpp:
	ln -s ../$@

configTest: pomotimer_config.o pomotimer.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $? -o $@

pomodoroTest: pomotimer_pomodoro.o pomotimer.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $? -o $@

pomotimerTest: pomotimer_pomotimer.o pomotimer.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $? -o $@

trackerTest: pomotimer_tracker.o tracker.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $? -o $@

build: $(EXES)

run: build
	@rm -f log
	./configTest | tee -a log
	./pomodoroTest | tee -a log
	./pomotimerTest | tee -a log
	./trackerTest | tee -a log

gcov: run $(UUTS)
	gcov *.cpp
	@./travis-test-output.sh

clean:
	rm -f log $(EXES)
	rm -f $(TEST_OBJS)
	rm -f *.gcov *.gcda *.gcno
	rm -f $(UUTO) $(UUTS)

.PHONY: build clean run gcov

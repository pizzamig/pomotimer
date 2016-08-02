export LDFLAGS+=-lcppunit
export CFLAGS+=-coverage

PUNITS=config pomodoro pomotimer tracker
UUNITS=observer time
TEST_SRCS=$(PUNITS:%=pomotimer_%.cpp) $(UUNITS:%=utility_%.cpp)
TEST_OBJS=$(TEST_SRCS:%.cpp=%.o)
EXES=$(PUNITS:=Test) $(UUNITS:=Test)
UUTS=pomotimer.cpp tracker.cpp time.cpp
UUTO=$(UUTS:%.cpp=%.o)

pomotimer.o: ../pomotimer.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

tracker.o: ../tracker.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

time.o: ../time.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.cpp:
	ln -s ../$@

configTest: pomotimer_config.o pomotimer.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ $(LDFLAGS) -o $@

pomodoroTest: pomotimer_pomodoro.o pomotimer.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ $(LDFLAGS) -o $@

pomotimerTest: pomotimer_pomotimer.o pomotimer.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ $(LDFLAGS) -o $@

trackerTest: pomotimer_tracker.o tracker.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ $(LDFLAGS) -o $@

observerTest: utility_observer.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ $(LDFLAGS) -o $@

timeTest: utility_time.o time.o
	$(CXX) $(CFLAGS) $(LDFLAGS) $^ $(LDFLAGS) -o $@

build: $(EXES)

run: build
	@rm -f log
	./configTest | tee -a log
	./pomodoroTest | tee -a log
	./pomotimerTest | tee -a log
	./trackerTest | tee -a log
	./observerTest | tee -a log
	./timeTest | tee -a log

gcov: run $(UUTS)
	gcov -c *.cpp
	@./travis-test-output.sh

clean:
	rm -f log $(EXES)
	rm -f $(TEST_OBJS)
	rm -f *.gcov *.gcda *.gcno
	rm -f $(UUTO) $(UUTS)

.PHONY: build clean run gcov

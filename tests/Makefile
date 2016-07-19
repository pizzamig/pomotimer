export LDFLAGS+=-lcppunit
export CFLAGS+=-coverage

EXES=configTest pomodoroTest pomotimerTest trackerTest
UUTS=pomotimer.cpp tracker.cpp

pomotimer.o: ../pomotimer.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

tracker.o: ../tracker.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CFLAGS) -c $< -o $@

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
	ln -s ../pomotimer.cpp
	ln -s ../tracker.cpp
	gcov pomotimer.cpp tracker.cpp
	@./travis-test-output.sh

clean:
	rm -f log $(EXES) *.o
	rm -f *.gcov *.gcda *.gcno
	rm -f pomotimer.cpp tracker.cpp
	$(MAKE) -C pomotimer_config clean
	$(MAKE) -C pomotimer_pomodoro clean
	$(MAKE) -C pomotimer_pomotimer clean
	$(MAKE) -C pomotimer_tracker clean

.PHONY: build clean run configTest

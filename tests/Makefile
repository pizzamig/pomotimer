export LDFLAGS+=-lcppunit
export CFLAGS+=-coverage

configTest: pomotimer_config.cpp ../pomotimer.cpp
	$(MAKE) -C pomotimer_config $@

pomodoroTest: pomotimer_pomodoro.cpp ../pomotimer.cpp
	$(MAKE) -C pomotimer_pomodoro $@

pomotimerTest: pomotimer_pomotimer.cpp ../pomotimer.cpp
	$(MAKE) -C pomotimer_pomotimer $@

trackerTest: pomotimer_tracker.cpp ../tracker.cpp
	$(MAKE) -C pomotimer_tracker $@

build: configTest pomodoroTest pomotimerTest trackerTest

run: build
	@rm -f log
	./pomotimer_config/configTest | tee -a log
	./pomotimer_pomodoro/pomodoroTest | tee -a log
	./pomotimer_pomotimer/pomotimerTest | tee -a log
	./pomotimer_tracker/trackerTest | tee -a log
	$(MAKE) -C pomotimer_config gcov
	$(MAKE) -C pomotimer_pomodoro gcov
	$(MAKE) -C pomotimer_pomotimer gcov
	$(MAKE) -C pomotimer_tracker gcov
	@./travis-test-output.sh

clean:
	rm -f log
	rm -f *.gcov *.gcda *.gcno
	$(MAKE) -C pomotimer_config clean
	$(MAKE) -C pomotimer_pomodoro clean
	$(MAKE) -C pomotimer_pomotimer clean
	$(MAKE) -C pomotimer_tracker clean

.PHONY: build clean run configTest

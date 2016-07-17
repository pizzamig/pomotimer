LDFLAGS+=-lcppunit

configTest: pomodoro_config.cpp ../pomotimer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_config.cpp ../pomotimer.cpp -o configTest

pomodoroTest: pomodoro_pomodoro.cpp ../pomotimer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_pomodoro.cpp ../pomotimer.cpp -o pomodoroTest

pomotimerTest: pomodoro_pomotimer.cpp ../pomotimer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_pomotimer.cpp ../pomotimer.cpp -o pomotimerTest

trackerTest: pomodoro_tracker.cpp ../tracker.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_tracker.cpp ../tracker.cpp -o trackerTest

build: configTest pomodoroTest pomotimerTest trackerTest

run: build
	@rm -f log
	./configTest | tee -a log
	./pomodoroTest | tee -a log
	./pomotimerTest | tee -a log
	./trackerTest | tee -a log
	@./travis-test-output.sh

clean:
	rm -f configTest pomodoroTest pomotimerTest trackerTest log

.PHONY: build clean run

LDFLAGS+=-lcppunit

configTest: pomodoro_config.cpp ../pomotimer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_config.cpp ../pomotimer.cpp -o configTest

pomodoroTest: pomodoro_pomodoro.cpp ../pomotimer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_pomodoro.cpp ../pomotimer.cpp -o pomodoroTest

pomotimerTest: pomodoro_pomotimer.cpp ../pomotimer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_pomotimer.cpp ../pomotimer.cpp -o pomotimerTest

build: configTest pomodoroTest pomotimerTest

run: build
	@rm -f log
	./configTest | tee -a log
	./pomodoroTest | tee -a log
	./pomotimerTest | tee -a log
	@./travis-test-output.sh

clean:
	rm -f configTest pomodoroTest pomotimerTest log

.PHONY: build clean run

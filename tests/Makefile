LDFLAGS+=-lcppunit

configTest: pomodoro_config.cpp ../pomotimer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_config.cpp ../pomotimer.cpp -o configTest

build: configTest

run: build
	@rm -f log
	./configTest | tee -a log
	@./travis-test-output.sh

clean:
	rm -f configTest log
.PHONY: build clean run

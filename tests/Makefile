LDFLAGS+=-lcppunit

configTest: pomodoro_config.cpp ../pomotimer.cpp
	$(CXX) $(CFLAGS) $(LDFLAGS) pomodoro_config.cpp ../pomotimer.cpp -o configTest

build: configTest

run:
	./configTest

clean:
	rm -f configTest
.PHONY: build clean run

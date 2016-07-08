pomotimer: pomotimer.cpp pomotimer.h
	$(CXX) -g pomotimer.cpp -o pomotimer -lrt

clean:
	rm -f pomotimer
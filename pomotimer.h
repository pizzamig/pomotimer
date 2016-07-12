#pragma once
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>

namespace pomotimer {

class Config
{
public:
  Config();
  Config(uint32_t focus, uint32_t shortBreak,
		uint32_t longBreak, uint32_t loopSize );
	uint32_t getFocus() const { return focus; }
	uint32_t getShortBreak() const { return shortBreak; }
	uint32_t getLongBreak() const { return longBreak; }
	uint32_t getLoopSize() const { return loopSize; }
private:
  uint32_t focus;
  uint32_t shortBreak;
  uint32_t longBreak;
  uint32_t loopSize;
};

enum Timer { FOCUS, SHORT_BREAK, LONG_BREAK };

class Pomodoro {
public:
	Pomodoro( Config &c );
	uint32_t getTime() const { return time; }
	void update();
	Timer getTimerType() const { return type; }
	pthread_mutex_t * getMutex() { return &mutex; }
	pthread_cond_t * getCond() { return &cond; }
private:
	Timer type;
	uint32_t time;
	uint32_t loopCounter; // short break counter
	Config & localConfig;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
};

class Pomotimer {
public:
	Pomotimer( Config & );
	~Pomotimer();
	void start();
	void stop();
	void pause();
private:
	static void* mainThread( void * );
	static void timerThread( union sigval si );
	enum Status { RUN, STOP, EXIT };
	Status is; // internal status
	Status ns; // new status
	Config & conf;
	Pomodoro pomo;
	pthread_t tid;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	// sigevent section
	timer_t timerId;
	struct itimerspec ts;
	struct sigevent se;
	
};

} // namespace

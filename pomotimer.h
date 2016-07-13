#pragma once
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <vector>

#include "observer.h"

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
	void reset();
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

// change timer observer
class changeTimerObs
{
public:
	void virtual changeTimer( Timer t ) = 0;
};

class timeObs
{
public:
	void virtual newTime( uint32_t t ) = 0;
};

class Pomotimer : public nctk::Observable< Timer >,
									public nctk::Observable< uint32_t >
{
public:
	Pomotimer( Config & );
	~Pomotimer();
	void start();
	void stop();
	void pause();
	uint32_t getTime() const { return pomo.getTime(); }
	using nctk::Observable< Timer >::addObs;
	using nctk::Observable< uint32_t >::addObs;
private:
	using nctk::Observable< Timer >::notifyAllObs;
	using nctk::Observable< uint32_t >::notifyAllObs;
	static void* mainThread( void * );
	static void timerThread( union sigval si );
	enum Status { RUN, STOP, PAUSE, EXIT };
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

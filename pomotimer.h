#pragma once
#include <stdint.h>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <vector>
#include <mutex>

#include "observer.h"

namespace pomotimer {

	/** The pomodoro configuration class
	 * With this class you can configure the length of all pomodoro timers
	 * and the length of the loop sequence
	 */
class Config
{
public:
	/** The configurable constructor
	 * @param focus How long the focus time is, in seconds; default 25 minutes
	 * @param shortBreak How long the short break is, in seconds; default 5 minutes
	 * @param longBreak How long the long break is, in seconds; default 20 minutes
	 * @param loopSize How many focus times are needed to get a long break; default 4
	 */
	Config(uint32_t focus = 1500, uint32_t shortBreak = 300,
		uint32_t longBreak = 1200, uint32_t loopSize = 4 );
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
	std::mutex * getMutex() { return &mtx; }
private:
	Timer type;
	uint32_t time;
	uint32_t loopCounter; // short break counter
	Config & localConfig;
	std::mutex mtx;
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

} // namespace pomotimer

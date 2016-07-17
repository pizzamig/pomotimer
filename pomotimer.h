#pragma once
#include <cstdint>
#include <pthread.h>
#include <time.h>
#include <signal.h>
#include <vector>
#include <mutex>

#include "observer.h"

namespace pomotimer {

	/** The pomodoro configuration class.
	 * With this class you can configure the length of all pomodoro timers
	 * and the length of the loop sequence
	 */
class Config
{
public:
	/** The configurable constructor.
	 * If the parameter is invalid (like 0 seconds), it will be automatically fixed to
	 * a valid value
	 * @param focus How long the focus time is, in seconds; default 25 minutes, minimum 10 seconds
	 * @param shortBreak How long the short break is, in seconds; default 5 minutes, minimum 10 seconds
	 * @param longBreak How long the long break is, in seconds; default 20 minutes, minimum 10 seconds
	 * @param loopSize How many focus times are needed to get a long break; default 4, minimum 2
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

enum class TimerType : uint8_t { FOCUS, SHORT_BREAK, LONG_BREAK };

/** The Pomodoro timer state machine.
 * This class implements the state machine under a pomodoro timer
 * It tracks the status of the timer; to update the interal status
 * the update() member function has to be called every second.
 * All methods are thread-safe
 */
class Pomodoro {
public:
	/** The initializing constructor.
	 * The constructor initializes the state machine to its initial status:
	 * * Timer set to Focus
	 * * Time resetted
	 * * Loop counter resetted
	 */
	Pomodoro( Config &c );
	/** Updating the internal status.
	 * This function update the internal status, meanings that 1 second is passed since last update
	 * The function decrement the time left in the current timer of 1 second
	 * In the current timer is expired, the function change the current timer to the next one
	 * following the Pomodoro rules.
	 */
	void update();
	/** Resetting the status of the Pomodoro timer.
	 * This function reset the timer, bringing back it to the initial status
	 * ( timer set to Focus, time resetted, loop counter resetted )
	 */
	void reset();
	/** Get the left time of the current timer (in seconds).
	 * This function return the amount of seconds remaining in the current timer
	 */
	uint32_t getTime() const { return time; }
	/** Get the current timer type.
	 */
	TimerType getTimerType() const { return type; }

private:
	TimerType type;
	uint32_t time;
	uint32_t loopCounter; // focus counter
	Config & localConfig;
	std::mutex mtx;
};

/** The Pomodoro timer.
 * This class represents a complete pomodoro timer.
 * It provides two observer interface
 * * when TimerType changes
 * * when the remaining time changes
 * It has a basic control interface with start(). stop() and pause() functions.
 * All methods are thread-safe.
 * This implementation is fully based on POSIX: it uses SIGEV_THREAD signalling mechanisms and
 * pthread_* class of functions
 * The SIGEV_THREAD signalling mechanism arms a periodic timer that, when expired, wake up the timer thread
 * The timer thread, then, updates the Pomodoro and, in case, send event notifications
 */
class Pomotimer : public utility::Observable< TimerType >,
									public utility::Observable< uint32_t >
{
public:
	/** The constructor.
	 * The constructor initialize all timer elements and it needs only the pomodoro configuration
	 * @param c The pomodoro configuration
	 */
	Pomotimer( Config & c );
	/** The clean-up destructor.
	 * The destructor is needed to clean-up all activities that were going on.
	 * In particular, the desctructor stops all activities and waits that
	 * the main thread exits.
	 */
	~Pomotimer();
	/** Start or restart the timer.
	 * This function starts the pomodoro timer.
	 * There's no function difference between start and restart.
	 * A start, when the timer is stopped, cause a timer type notification
	 */
	void start();
	/** Stop the timer.
	 * This function stops the pomodoro timer, bringing it back to its initial status.
	 * stop() implictely invokes a pomodoro reset
	 */
	void stop();
	/** Pause the timer.
	 * This function stops the pomodoro timer, but leaving the status untouched.
	 * No pomodoro reset occur in this case
	 * @note if the timer type is not FOCUS, stop() can cause a timer type notification
	 */
	void pause();
	/** Get the left time of the current timer (in seconds).
	 * This function return the amount of seconds remaining in the current timer
	 */
	uint32_t getTime() const { return pomo.getTime(); }

	using utility::Observable< TimerType >::addObs;
	using utility::Observable< uint32_t >::addObs;

private:
	using utility::Observable< TimerType >::notifyAllObs;
	using utility::Observable< uint32_t >::notifyAllObs;
	/** The thread commanding the timer.
	 * This is the main thread, taking care of activating the internal timer and so on.
	 * In general, this thread waits for a command and then execute it, arming or disarming the sigev_thread
	 */
	static void* mainThread( void * );
	/** The timer thread
	 * This is the thread executed when the SIGEV_THREAD timer expires
	 * It's responsible to update the Pomodoro status calling update()
	 * After the update, it's responsible to manage all registered notifications
	 */
	static void timerThread( union sigval si );
	
	enum Status { RUN, STOP, PAUSE, EXIT };
	Status is; //! The current internal status
	Status ns; //! The new status, issued by functions like start(), stop() or pause()
	Config & conf;
	Pomodoro pomo;
	// pthread stuff
	pthread_t tid;
	pthread_mutex_t mutex;
	pthread_cond_t cond;
	// SIGEV_THREAD section
	timer_t timerId;
	struct itimerspec ts;
	struct sigevent se;
};

} // namespace pomotimer

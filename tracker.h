#include "pomotimer.h"
#include "observer.h"
#include <mutex>

namespace pomotimer {

	/** A pomodoro tracker
	 * This class collect some statistical information about the number of pomodoro
	 * used to perform a task
	 */
	class Tracker : public utility::Observer< TimerType > {
	public:
		/** The initializing constructor.
		 * The constructor initilize the internal counters and register itself
		 * as an observer of timer type events
		 */
		Tracker(Pomotimer &p);
		/** The number of started pomos.
		 * This function get the number of observer pomodoros that are started
		 * It's thread safe.
		 */
		uint32_t getStartedPomo();
		/** The number of completed pomos.
		 * This function get the number of observer pomodoros that are completed
		 * It's thread safe.
		 */
		uint32_t getFinishedPomo();
		/** Resetting the counters.
		 * This functions reset all timers.
		 * It's thread safe.
		 */
		void reset();
		/** The observer function
		 * The function invoked by the observable, monitoring the activity of the timer
		 */
		virtual void notify( TimerType t );
	private:
		Pomotimer & pt;
		// counters
		uint32_t started;
		uint32_t finished;
		// the internal mutex
		std::mutex mtx;
	};
} // namespace pomotimer

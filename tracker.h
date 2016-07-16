#include "pomotimer.h"
#include "observer.h"
#include <mutex>

namespace pomotimer {

	/** A pomodoro tracker
	 * This class collect some statistical information about the number of pomodoro
	 * used to perform a task
	 */
	class Tracker : public utility::Obsever< TimerType > {
	public:
		Tracker(Pomotimer &p);
		uint32_t getStartedPomo();
		uint32_t getFinishedPomo();
		virtual void notify( TimerType t );
		void reset();
	private:
		Pomotimer & pt;
		timerObs obs;
		// counters
		uint32_t started;
		uint32_t finished;
		// the internal mutex
		std::mutex mtx;
	};
} // namespace pomotimer

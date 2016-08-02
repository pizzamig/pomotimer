#include "tracker.h"

pomotimer::Tracker::Tracker(pomotimer::Pomotimer &p) :
	started(0), finished(0), mtx()
{
	p.addObs( this );
}

uint32_t
pomotimer::Tracker::getStartedPomo()
{
	std::lock_guard<std::mutex> lock(mtx);
	return started;
}

uint32_t
pomotimer::Tracker::getFinishedPomo()
{
	std::lock_guard<std::mutex> lock(mtx);
	return finished;
}

void
pomotimer::Tracker::reset()
{
	std::lock_guard<std::mutex> lock(mtx);
	started = 0;
	finished = 0;
}

void
pomotimer::Tracker::notify( pomotimer::TimerType t )
{
	std::lock_guard<std::mutex> lock(mtx);
	if( t == pomotimer::TimerType::FOCUS ) {
		started++;
	} else {
		finished++;
	}
}


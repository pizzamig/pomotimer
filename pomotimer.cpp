#include "pomotimer.h"

#include <iostream>

pomotimer::Config::Config(uint32_t focus, uint32_t shortBreak,
	uint32_t longBreak, uint32_t loopSize) :
	focus( focus ), shortBreak( shortBreak ),
	longBreak( longBreak ), loopSize( loopSize )
{}

pomotimer::Config::Config() : focus( 25*60 ), shortBreak( 5*60 ),
	longBreak( 20*60 ), loopSize( 4 )
{}

pomotimer::Pomodoro::Pomodoro(Config &c) : type(FOCUS), time(c.getFocus()), loopCounter(0),
	localConfig( c )
{
	mutex = PTHREAD_MUTEX_INITIALIZER;
	cond = PTHREAD_COND_INITIALIZER;
}

void
pomotimer::Pomodoro::reset()
{
	pthread_mutex_lock( &mutex );
	time = localConfig.getFocus();
	type = FOCUS;
	loopCounter = 0;
	pthread_mutex_unlock( &mutex );
}

void
pomotimer::Pomodoro::update()
{
	pthread_mutex_lock( &mutex );
	if( time == 0 ) {
		// change the timer type
		switch( type ) {
			case FOCUS:
				loopCounter++;
				if( loopCounter == localConfig.getLoopSize() ) {
					type=LONG_BREAK;
					time=localConfig.getLongBreak();
					loopCounter=0;
				} else {
					type=SHORT_BREAK;
					time=localConfig.getShortBreak();
				}
				break;
			case SHORT_BREAK:
				type=FOCUS;
				time=localConfig.getFocus();
				break;
			case LONG_BREAK:
				type=FOCUS;
				time=localConfig.getFocus();
				break;
		}
	}
	--time;
	pthread_mutex_unlock( &mutex );
}

pomotimer::Pomotimer::Pomotimer(Config &c)
	: is(STOP), ns(STOP), conf(c), pomo( conf ), tid( 0 )
{
	mutex = PTHREAD_MUTEX_INITIALIZER;
	cond = PTHREAD_COND_INITIALIZER;
	obs_mutex = PTHREAD_MUTEX_INITIALIZER;
	/*
	Set the sigevent structure to cause the signal to be
	delivered by creating a new thread.
	*/
	se.sigev_notify = SIGEV_THREAD; // notification type
	se.sigev_value.sival_ptr = this; // the thread argument 
	se.sigev_notify_function = timerThread; // the thread
	se.sigev_notify_attributes = nullptr;
	/* specifying a reapiting timer */
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 0;
	ts.it_interval.tv_sec = 1;
	ts.it_interval.tv_nsec = 0;
	// TODO: the return code should be evaluated
	timer_create( CLOCK_MONOTONIC, &se, &timerId );
	pthread_create( &tid, NULL, mainThread, this );
}

void*
pomotimer::Pomotimer::mainThread( void * arg )
{
	Pomotimer * p = static_cast<Pomotimer *>(arg);
	pthread_mutex_lock( &(p->mutex) );
	while( true ) {
		pthread_cond_wait( &(p->cond), &(p->mutex) );
		if ( p->ns == EXIT ) {
			break;
		}
		if ( p->is == p->ns ) {
			continue;
		}
		switch(p->ns) {
			case RUN:
				p->ts.it_value.tv_sec = 1;
				p->ts.it_interval.tv_sec = 1;
				timer_settime( p->timerId, 0, &(p->ts), 0 );
				break;
			case STOP:
				p->ts.it_value.tv_sec = 0;
				p->ts.it_interval.tv_sec = 0;
				timer_settime( p->timerId, 0, &(p->ts), 0 );
				p->pomo.reset();
				break;
			case PAUSE:
				p->ts.it_value.tv_sec = 0;
				p->ts.it_interval.tv_sec = 0;
				timer_settime( p->timerId, 0, &(p->ts), 0 );
			case EXIT:
				break;
		}
		p->is = p->ns;
	}
	pthread_mutex_unlock( &(p->mutex) );
	return p;
}

void
pomotimer::Pomotimer::start()
{
	pthread_mutex_lock( &mutex );
	ns = RUN;
	pthread_cond_signal( &cond );
	pthread_mutex_unlock( &mutex );
}

void
pomotimer::Pomotimer::stop()
{
	pthread_mutex_lock( &mutex );
	ns = STOP;
	pthread_cond_signal( &cond );
	pthread_mutex_unlock( &mutex );
}

void
pomotimer::Pomotimer::pause()
{
	pthread_mutex_lock( &mutex );
	ns = PAUSE;
	pthread_cond_signal( &cond );
	pthread_mutex_unlock( &mutex );
}

pomotimer::Pomotimer::~Pomotimer()
{
	pthread_mutex_lock ( &mutex );
	ns = EXIT;
	pthread_cond_signal( &cond );
	pthread_mutex_unlock( &mutex );
	pthread_join( tid, nullptr );
}

void
pomotimer::Pomotimer::addChangeTimerObs( changeTimerObs * o )
{
	if( o == nullptr ) {
		return;
	}
	pthread_mutex_lock( &obs_mutex );
	changeTimerObservers.push_back( o );
	pthread_mutex_unlock( &obs_mutex );
}

void
pomotimer::Pomotimer::addTimeObs( timeObs * o )
{
	if( o == nullptr ) {
		return;
	}
	pthread_mutex_lock( &obs_mutex );
	timeObservers.push_back( o );
	pthread_mutex_unlock( &obs_mutex );
}

void
pomotimer::Pomotimer::allChangeTimerObsNotify(Timer t)
{
	pthread_mutex_lock( &obs_mutex );
	for( auto it = changeTimerObservers.begin(); it != changeTimerObservers.end(); ++it ) {
		(*it)->changeTimer(t);
	}
	pthread_mutex_unlock( &obs_mutex );
}

void
pomotimer::Pomotimer::allTimeObsNotify(uint32_t t)
{
	pthread_mutex_lock( &obs_mutex );
	for( auto it = timeObservers.begin(); it != timeObservers.end(); ++it ) {
		(*it)->newTime(t);
	}
	pthread_mutex_unlock( &obs_mutex );
}

void
pomotimer::Pomotimer::timerThread( union sigval si )
{
	Pomotimer * p = static_cast<Pomotimer *>(si.sival_ptr);
	if( p->ns == STOP || p->ns == PAUSE ) {
		return;
	}
	Timer tOld = p->pomo.getTimerType();
	p->pomo.update();
	uint32_t newTime = p->pomo.getTime();
	p->allTimeObsNotify( newTime );
	Timer tNew = p->pomo.getTimerType();
	if ( tOld == tNew ) {
		return;
	}
	p->allChangeTimerObsNotify( tNew );
}

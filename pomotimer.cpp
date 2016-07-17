#include "pomotimer.h"

#include <iostream>

pomotimer::Config::Config(uint32_t focus, uint32_t shortBreak,
	uint32_t longBreak, uint32_t loopSize) :
	focus( focus ), shortBreak( shortBreak ),
	longBreak( longBreak ), loopSize( loopSize )
{
	// verifying the validity of the config input
	const uint32_t minimumTime = 10;
	const uint32_t minimumLoop = 2;
	if ( focus < minimumTime ) {
		this->focus = minimumTime;
	}
	if ( shortBreak < minimumTime ) {
		this->shortBreak = minimumTime;
	}
	if ( longBreak < minimumTime ) {
		this->longBreak = minimumTime;
	}
	if ( loopSize < minimumLoop ) {
		this->loopSize = minimumLoop;
	}
}

pomotimer::Pomodoro::Pomodoro(Config &c) : type(TimerType::FOCUS), time(c.getFocus()), loopCounter(0),
	localConfig( c ), mtx()
{}

void
pomotimer::Pomodoro::reset()
{
	std::lock_guard<std::mutex> lock(mtx);
	time = localConfig.getFocus();
	type = TimerType::FOCUS;
	loopCounter = 0;
}

void
pomotimer::Pomodoro::update()
{
	std::lock_guard<std::mutex> lock(mtx);
	if( time == 1 ) {
		// change the timer type
		switch( type ) {
			case TimerType::FOCUS:
				loopCounter++;
				if( loopCounter == localConfig.getLoopSize() ) {
					type=TimerType::LONG_BREAK;
					time=localConfig.getLongBreak();
					loopCounter=0;
				} else {
					type=TimerType::SHORT_BREAK;
					time=localConfig.getShortBreak();
				}
				break;
			case TimerType::SHORT_BREAK:
				type=TimerType::FOCUS;
				time=localConfig.getFocus();
				break;
			case TimerType::LONG_BREAK:
				type=TimerType::FOCUS;
				time=localConfig.getFocus();
				break;
		}
	} else {
		--time;
	}
}

pomotimer::Pomotimer::Pomotimer(Config &c)
	: is(STOP), ns(STOP), conf(c), pomo( conf ), tid( 0 )
{
	mutex = PTHREAD_MUTEX_INITIALIZER;
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
	// create the thread and wait its start
	// to avoid race condition with the exit
	pthread_mutex_lock( &mutex );
	pthread_create( &tid, NULL, mainThread, this );
	pthread_cond_wait( &cond, &mutex );
	pthread_mutex_unlock( &mutex );
}

void*
pomotimer::Pomotimer::mainThread( void * arg )
{
	Pomotimer * p = static_cast<Pomotimer *>(arg);
	pthread_mutex_lock( &(p->mutex) );
	// signalling the constructor that I'm here and then wait for instructions
	pthread_cond_signal( &(p->cond) );
	while( true ) {
		pthread_cond_wait( &(p->cond), &(p->mutex) );
		if ( p->ns == EXIT ) {
			break;
		}
		if ( p->is == p->ns ) {
			continue;
		}
		bool timerNotification=false;
		switch(p->ns) {
			case RUN:
				p->ts.it_value.tv_sec = 1;
				p->ts.it_interval.tv_sec = 1;
				timer_settime( p->timerId, 0, &(p->ts), 0 );
				if( p->is == STOP ) {
					timerNotification = true;
				}
				break;
			case STOP:
				p->ts.it_value.tv_sec = 0;
				p->ts.it_interval.tv_sec = 0;
				timer_settime( p->timerId, 0, &(p->ts), 0 );
				if( p->pomo.getTimerType() != pomotimer::TimerType::FOCUS ) {
					timerNotification = true;
				}
				p->pomo.reset();
				break;
			case PAUSE:
				p->ts.it_value.tv_sec = 0;
				p->ts.it_interval.tv_sec = 0;
				timer_settime( p->timerId, 0, &(p->ts), 0 );
			case EXIT:
				break;
		}
		if( timerNotification ) {
			p->notifyAllObs( p->pomo.getTimerType() );
		}
		if( p->ns == STOP ) {
			uint32_t newTime = p->pomo.getTime();
			p->notifyAllObs( newTime );
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
pomotimer::Pomotimer::timerThread( union sigval si )
{
	Pomotimer * p = static_cast<Pomotimer *>(si.sival_ptr);
	pthread_mutex_lock ( &(p->mutex) );
	if( p->ns == STOP || p->ns == PAUSE || p->ns == EXIT) {
		pthread_mutex_unlock ( &(p->mutex) );
		return;
	}
	pthread_mutex_unlock ( &(p->mutex) );
	TimerType tOld = p->pomo.getTimerType();
	p->pomo.update();
	TimerType tNew = p->pomo.getTimerType();
	if ( tOld != tNew ) {
		p->notifyAllObs( tNew );
	}
	uint32_t newTime = p->pomo.getTime();
	p->notifyAllObs( newTime );
}

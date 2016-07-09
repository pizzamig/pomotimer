#include <time.h>
#include <signal.h>
#include <err.h>
#include <sysexits.h>
#include <signal.h>
#include <pthread.h>

#include "pomotimer.h"

#include <iostream>

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

pomotimer::Config::Config(uint32_t focus, uint32_t shortBreak,
	uint32_t longBreak, uint32_t loopSize) :
	focus( focus ), shortBreak( shortBreak ),
	longBreak( longBreak ), loopSize( loopSize )
{}

pomotimer::Config::Config() : focus( 25*60 ), shortBreak( 5*60 ),
	longBreak( 20*60 ), loopSize( 4 )
{}

pomotimer::Pomodoro::Pomodoro() : type(FOCUS), time(25), loopCounter(0)
{}

void
pomotimer::Pomodoro::update()
{
	if( time == 0 ) {
		// change the timer type
		switch( type ) {
			case FOCUS:
				loopCounter++;
				if( loopCounter == 4 ) {
					type=LONG_BREAK;
					time=20;
					loopCounter=0;
				} else {
					type=SHORT_BREAK;
					time=5;
				}
				break;
			case SHORT_BREAK:
				type=FOCUS;
				time=25;
				break;
			case LONG_BREAK:
				type=FOCUS;
				time=25;
				break;
		}
	}
	--time;
}

void
second_thread(union sigval si)
{
	pomotimer::Pomodoro * pomo=(pomotimer::Pomodoro *)si.sival_ptr;
	pthread_mutex_lock( &mutex );
	pomo->update();
	auto timeLeft=pomo->getTime();
	pthread_cond_signal( &cond );
	pthread_mutex_unlock( &mutex );
	std::cout << "still " << timeLeft << " left " << std::endl;
	return;
}

int main()
{
	int rc;
	timer_t tid;
	struct itimerspec ts;
	struct sigevent se;
	pomotimer::Pomodoro tomato;
	/*
	Set the sigevent structure to cause the signal to be
	delivered by creating a new thread.
	*/
	se.sigev_notify = SIGEV_THREAD; // the notification type
	se.sigev_value.sival_ptr = &tomato; // the associated timer
	se.sigev_notify_function = second_thread; // the thread function
	se.sigev_notify_attributes = NULL; // attributes for the notification thread

	/*
	Specify a repeating timer that fires each 1 seconds.
	*/
	ts.it_value.tv_sec = 1;
	ts.it_value.tv_nsec = 0;
	ts.it_interval.tv_sec = 1;
	ts.it_interval.tv_nsec = 0;
	
	rc = timer_create (CLOCK_MONOTONIC, &se, &tid );
	pomotimer::Status pomoStatus = pomotimer::RUN;
	
	timer_settime( tid, 0, &ts, 0 );
	if ( rc == 0 ) {
		std::cout << "timer created - " << tid << std::endl;
	} else {
		err(EX_OSERR, nullptr);
	}
	pthread_mutex_lock( &mutex );
	while( tomato.getTimerType() != pomotimer::Timer::LONG_BREAK ) {
		pthread_cond_wait( &cond, &mutex );
	}
	pthread_mutex_unlock( &mutex );
	return 0;
}


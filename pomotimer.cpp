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

pomotimer::Pomodoro::Pomodoro(Config &c) : type(FOCUS), time(25), loopCounter(0),
	localConfig( c )
{
	time = c.getFocus();
	mutex = PTHREAD_MUTEX_INITIALIZER;
	cond = PTHREAD_COND_INITIALIZER;
}

void
pomotimer::Pomodoro::update()
{
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
}

#if 1
void
secTimer(union sigval si)
{
	pomotimer::Pomodoro * pomo=(pomotimer::Pomodoro *)si.sival_ptr;
	pthread_mutex_lock( pomo->getMutex() );
	pomo->update();
	pthread_cond_signal( pomo->getCond() );
	pthread_mutex_unlock( pomo->getMutex() );
	return;
}
#endif

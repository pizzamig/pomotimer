#include "pomotimer.h"
#include "observer.h"
#include "nctk.h"
#include <unistd.h>
#include <ncurses.h>

#include <iostream>

class ncTimeObs : public utility::Observer< uint32_t >
{
	virtual void notify( uint32_t t )
	{
		printw("Time to end is %u\n", t );
		refresh();
	}
};

class ncTTypeObs : public utility::Observer< pomotimer::TimerType >
{
	virtual void notify ( pomotimer::TimerType t )
	{
		switch ( t ) {
			case pomotimer::TimerType::FOCUS:
				printw("Pomodoro switched to FOCUS\n");refresh();
				break;
			case pomotimer::TimerType::SHORT_BREAK:
				printw("Pomodoro switched to SHORT BREAK\n");refresh();
				break;
			case pomotimer::TimerType::LONG_BREAK:
				printw("Pomodoro switched to LONG BREAK\n");refresh();
				break;
		}
	}
};

int main()
{
	pomotimer::Config config(6,2,4,2); // short times, for testing
	auto * tomato = new pomotimer::Pomotimer(config);
	nctk::Application app;
	ncTimeObs timeSlot;
	ncTTypeObs typeSlot;
	tomato->addObs( &timeSlot );
	tomato->addObs( &typeSlot );
	printw( "rows: %d, cols: %d\n", app.getRows(), app.getCols() );
	printw( "start\n"); refresh();
	tomato->start();
	sleep( 5 );
	printw( "stop\n"); refresh();
	tomato->stop();
	sleep( 2 );
	printw( "start\n"); refresh();
	tomato->start();
	sleep( 5 );
	printw( "pause\n"); refresh();
	tomato->pause();
	sleep( 2 );
	printw( "restart\n"); refresh();
	tomato->start();
	sleep( 20 );
	printw( "stop\n"); refresh();
	tomato->stop();
	delete tomato;
	getch();
	return 0;
}


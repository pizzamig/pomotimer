#include "pomotimer.h"
#include "observer.h"
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
void
cli_init()
{
	initscr();
	raw();
	keypad(stdscr, TRUE);
	noecho();
}

void
cli_close()
{
	echo();
	endwin();
}

int main()
{
	pomotimer::Config config(6,2,4,2); // short times, for testing
	auto * tomato = new pomotimer::Pomotimer(config);
	cli_init();
	ncTimeObs timeSlot;
	ncTTypeObs typeSlot;
	tomato->addObs( &timeSlot );
	tomato->addObs( &typeSlot );
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
	cli_close();
	return 0;
}


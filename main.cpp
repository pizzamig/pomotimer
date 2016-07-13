#include "pomotimer.h"

#include <unistd.h>
#include <ncurses.h>

#include <iostream>

class ncTimeObs : public pomotimer::timeObs
{
public:
	void virtual newTime( uint32_t );
};

void
ncTimeObs:: newTime( uint32_t t )
{
	printw("Time to end is %u\n", t );
	refresh();
}

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
	pomotimer::Config config(10,2,4,2); // short times, for testing
	auto * tomato = new pomotimer::Pomotimer(config);
	cli_init();
	ncTimeObs timeSlot;
	tomato->addTimeObs( &timeSlot ); 
	printw( "start\n");
	refresh();
	tomato->start();
	sleep( 5 );
	printw( "stop\n");
	tomato->stop();
	refresh();
	sleep( 1 );
	printw( "start\n");
	refresh();
	tomato->start();
	sleep( 5 );
	printw( "pause\n");
	tomato->pause();
	sleep( 1 );
	printw( "time after pause: %u\n", tomato->getTime() );
	refresh();
	delete tomato;
// 	while( tomato.getTimerType() != pomotimer::Timer::LONG_BREAK ) {
// 	}
	getch();
	cli_close();
	return 0;
}


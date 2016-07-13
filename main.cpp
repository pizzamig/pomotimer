#include "pomotimer.h"

#include <unistd.h>
#include <ncurses.h>

#include <iostream>


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
	printw( "time before start: %u\n", tomato->getTime() );
	tomato->start();
	printw( "time at start: %u\n", tomato->getTime() );
	sleep( 5 );
	printw( "time after 5s: %u\n", tomato->getTime() );
	refresh();
	tomato->stop();
	sleep( 1 );
	printw( "time after stop: %u\n", tomato->getTime() );
	refresh();
	printw( "time before start: %u\n", tomato->getTime() );
	tomato->start();
	printw( "time at start: %u\n", tomato->getTime() );
	sleep( 5 );
	printw( "time after 5s: %u\n", tomato->getTime() );
	refresh();
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


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
	tomato->start();
	sleep( 5 );
	tomato->stop();
	delete tomato;
// 	while( tomato.getTimerType() != pomotimer::Timer::LONG_BREAK ) {
// 	}
	cli_close();
	return 0;
}


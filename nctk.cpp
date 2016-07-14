#include "nctk.h"

nctk::Application::Application() : rows(0), cols(0)
{
	initscr();
	cbreak(); // we can use raw(), but we have to implement the signal handling
	keypad(stdscr, TRUE);
	noecho();
	getmaxyx(stdscr, rows, cols);
}

nctk::Application::~Application()
{
	echo();
	endwin();

}

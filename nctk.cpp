#include "nctk.h"

nctk::Application::Application() : rows(0), cols(0)
{
	initscr();
	cbreak(); // we can use raw(), but we have to implement the signal handling
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	getmaxyx(stdscr, rows, cols);
	refresh();
}

nctk::Application::~Application()
{
	echo();
	endwin();
}

#if 0
nctk::Window::Window( nctk::Window * parent )
{
	if ( parent != nullptr ) {
	}
}
nctk::Input::Input() : mtx()
{
	chReader = new std::thread(inputReader, this);
}

nctk::Input::~Input()
{
}

void
nctk::Input::inputReader(Input* ic)
{
	while(true) {
		char c;
		c=getch();
	}
}
#endif

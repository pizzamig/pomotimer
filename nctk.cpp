#include "nctk.h"
#include <sstream>

std::ostream&
operator<<(std::ostream& output, const nctk::Errors e)
{
	switch( e ) {
		case nctk::Errors::SIZE_TOO_SMALL:
			output << "Size too small";
			break;
		default:
			output << "Error type not supported";
	}
	return output;
}

nctk::Exception::Exception(nctk::Errors e) :
	std::exception(), e(e)
{
	std::ostringstream oss;
	oss << "nctk Exception: " << e ;
	msg = oss.str();
}

const char*
nctk::Exception::what() const noexcept
{
	return msg.c_str();
}

nctk::Size::Size(int lines, int cols) : lines(lines), cols(cols)
{}

static const nctk::Exception smallSize(nctk::Errors::SIZE_TOO_SMALL);

void
nctk::Application::init()
{
	initscr();
	cbreak(); // we can use raw(), but we have to implement the signal handling
	keypad(stdscr, TRUE);
	noecho();
	curs_set(0);
	rows=LINES;
	cols=COLS;
	refresh();
}

nctk::Application::Application() : rows(0), cols(0)
{
	init();
}

nctk::Application::Application(const Size & minSize) : rows(0), cols(0)
{
	init();
	if ( LINES < minSize.getLines() || 
		COLS < minSize.getCols() ) {
		throw smallSize;
	}
}

nctk::Application::~Application()
{
	echo();
	endwin();
}

nctk::Fatal::Fatal(nctk::Exception & e)
{
	printw("%s - press a key to exit", e.what()); refresh();
	getch();
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

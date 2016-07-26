#include "pomotimer.h"
#include "observer.h"
#include "nctk.h"
#include "time.h"
#include <unistd.h>
#include <ncurses.h>

#include <iostream>
#include <string>

class ncTimeObs : public utility::Observer< uint32_t >
{
private:
	WINDOW * w;
public:
	ncTimeObs( WINDOW * win ) : w(win) {
	}
	virtual void notify( uint32_t t )
	{
		int y,x;
		getmaxyx(w,y,x);
		y = y/2;
		wmove(w,y,0);
		wclrtoeol(w);
		utility::Time tm(t);
		std::string s=tm.getTimeStr();
		mvwprintw(w,y,(x-s.length())/2,"%s", s.c_str());
		wrefresh(w);
	}
};

class ncTTypeObs : public utility::Observer< pomotimer::TimerType >
{
private:
	WINDOW * w;
public:
	ncTTypeObs(WINDOW * win) : w(win) {}
	virtual void notify ( pomotimer::TimerType t )
	{
		int y,x;
		getmaxyx(w,y,x);
		y = y/2;
		wmove(w,y,0);
		wclrtoeol(w);
		std::string * s;
		switch ( t ) {
			case pomotimer::TimerType::FOCUS:
				s = new std::string("FOCUS");
				break;
			case pomotimer::TimerType::SHORT_BREAK:
				s = new std::string("SHORT BREAK");
				break;
			case pomotimer::TimerType::LONG_BREAK:
				s = new std::string("LONG BREAK");
				break;
		}
		mvwprintw(w,y,(x-s->length())/2,s->c_str());
		wrefresh(w);
		delete s;
	}
};

WINDOW * createTopWin()
{
	WINDOW * localWin;
	int height=(LINES-2)/2;
	int width=COLS;
	int starty=0;
	localWin = newwin(height,width, starty, 0);
	return localWin;
}

WINDOW * createBottomWin()
{
	WINDOW * localWin;
	int height=(LINES-2)/2;
	int width=COLS;
	int starty=LINES/2;
	localWin = newwin(height,width, starty, 0);
	return localWin;
}

WINDOW * createStatusWin()
{
	WINDOW * localWin;
	localWin = newwin(1,COLS, LINES-1, 0);
	return localWin;
}

void
initStatusWin( WINDOW * w )
{
	mvwprintw(w,0,0, "s to start, p to pause, t to stop, q to quit" );
	wrefresh( w );
}

#define MINCOLS 50
#define MINLINES 4
int main()
{
	pomotimer::Config config(6,2,4,2); // short times, for testing
	auto * tomato = new pomotimer::Pomotimer(config);
	nctk::Application app;
	if( LINES < MINLINES || COLS < MINCOLS ) {
		printw("windows to small"); refresh();
		getch();
		return 0;
	}
	WINDOW * topWin; // time
	WINDOW * bottomWin; // timer type
	WINDOW * statusWin;
	topWin = createTopWin();
	bottomWin = createBottomWin();
	statusWin = createStatusWin();
	// fixed part
	initStatusWin(statusWin);
	mvhline( (LINES-2)/2, 0, '-', COLS );
	ncTimeObs timeSlot(topWin);
	ncTTypeObs typeSlot(bottomWin);
	
	tomato->addObs( &timeSlot );
	tomato->addObs( &typeSlot );

	timeSlot.notify(tomato->getTime());
	typeSlot.notify(pomotimer::TimerType::FOCUS);
	while( true ) {
		int c;
		c=getch();
		switch( c ) {
			case 's':
				tomato->start();
				break;
			case 'p':
				tomato->pause();
				break;
			case 't':
				tomato->stop();
				break;
			case 'q':
				break;
			default:
				;
		}
		if( c=='q' )
			break;
	}
	delete tomato;
	return 0;
}


#include "pomotimer.h"
#include "observer.h"
#include "nctk.h"
#include <unistd.h>
#include <ncurses.h>

#include <iostream>
#include <string>

class ncTimeObs : public utility::Observer< uint32_t >
{
private:
	WINDOW * w;
// 	int x;
// 	int y;
public:
	ncTimeObs( WINDOW * win ) : w(win) {
// 		int row, cols;
	}
	virtual void notify( uint32_t t )
	{
		wmove(w,0,0);
		wclrtoeol(w);
// 		string s;
		mvwprintw(w,0,0,"%u", t);
// 		printw("Time to end is %u\n", t );
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
		wmove(w,0,0);
		wclrtoeol(w);
		switch ( t ) {
			case pomotimer::TimerType::FOCUS:
				mvwprintw(w,0,0,"FOCUS");
				break;
			case pomotimer::TimerType::SHORT_BREAK:
				mvwprintw(w,0,0,"SHORT BREAK");
				break;
			case pomotimer::TimerType::LONG_BREAK:
				mvwprintw(w,0,0,"LONG BREAK");
				break;
		}
		wrefresh(w);
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

int main()
{
	pomotimer::Config config(6,2,4,2); // short times, for testing
	auto * tomato = new pomotimer::Pomotimer(config);
	nctk::Application app;
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


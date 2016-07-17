#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../tracker.h"

pomotimer::Config::Config(uint32_t focus, uint32_t shortBreak,
	uint32_t longBreak, uint32_t loopSize) :
	focus( focus ), shortBreak( shortBreak ),
	longBreak( longBreak ), loopSize( loopSize )
{}

pomotimer::Pomodoro::Pomodoro(Config &c)
	: localConfig( c )
{}

pomotimer::Pomotimer::Pomotimer(Config &c)
	: is(STOP), ns(STOP), conf(c), pomo( conf ), tid( 0 )
{}

void*
pomotimer::Pomotimer::mainThread( void * arg )
{ return nullptr; }

void
pomotimer::Pomotimer::start()
{
	notifyAllObs( pomotimer::TimerType::FOCUS );
}

void
pomotimer::Pomotimer::stop()
{
	notifyAllObs( pomotimer::TimerType::SHORT_BREAK );
}

void
pomotimer::Pomotimer::pause()
{
}

pomotimer::Pomotimer::~Pomotimer()
{
}

class TrackerTest : public CppUnit::TestFixture {
private:
	pomotimer::Pomotimer * p;
	pomotimer::Config c;
	pomotimer::Tracker * t;
public:
	void setUp()
	{
		p = new pomotimer::Pomotimer( c );
		t = new pomotimer::Tracker( *p );
	}
	void tearDown()
	{
		delete p;
	}

	void testConstructor()
	{
		CPPUNIT_ASSERT( t->getStartedPomo() == 0 );
		CPPUNIT_ASSERT( t->getFinishedPomo() == 0 );
	}

	void testStartedPomo()
	{
		p->start();
		CPPUNIT_ASSERT( t->getStartedPomo() == 1 );
		CPPUNIT_ASSERT( t->getFinishedPomo() == 0 );
	}

	void testCompletedPomo()
	{
		p->start();
		p->stop();
		CPPUNIT_ASSERT( t->getStartedPomo() == 1 );
		CPPUNIT_ASSERT( t->getFinishedPomo() == 1 );
	}

	void testReset()
	{
		p->start();
		p->stop();
		CPPUNIT_ASSERT( t->getStartedPomo() == 1 );
		CPPUNIT_ASSERT( t->getFinishedPomo() == 1 );
		t->reset();
		CPPUNIT_ASSERT( t->getStartedPomo() == 0 );
		CPPUNIT_ASSERT( t->getFinishedPomo() == 0 );
	}

	void testMixedPomo()
	{
		p->start();
		p->start();
		p->stop();
		p->stop();
		p->start();
		CPPUNIT_ASSERT( t->getStartedPomo() == 3 );
		CPPUNIT_ASSERT( t->getFinishedPomo() == 2 );
		t->reset();
		p->start();
		p->stop();
		CPPUNIT_ASSERT( t->getStartedPomo() == 1 );
		CPPUNIT_ASSERT( t->getFinishedPomo() == 1 );
	}

	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteConfig = new CppUnit::TestSuite( "TrackerTest" );
		suiteConfig->addTest( new CppUnit::TestCaller< TrackerTest > (
			"testConstructor", &TrackerTest::testConstructor )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< TrackerTest > (
			"testStartedPomo", &TrackerTest::testStartedPomo )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< TrackerTest > (
			"testCompletedPomo", &TrackerTest::testCompletedPomo )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< TrackerTest > (
			"testReset", &TrackerTest::testReset )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< TrackerTest > (
			"testMixedPomo", &TrackerTest::testMixedPomo )) ;
		return suiteConfig;
	}
};

int main()
{
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( TrackerTest::suite() );
	runner.run( );
	return 0;
}

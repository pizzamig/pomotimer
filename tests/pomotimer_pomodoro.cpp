#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include <string>

#include "../pomotimer.h"

class PomodoroTest : public CppUnit::TestFixture {
private:
	pomotimer::Config defaultC;
	pomotimer::Config * shortC;
	pomotimer::Pomodoro * pomo;
	pomotimer::Pomodoro * fastPomo;
public:
	void setUp()
	{
		pomo = new pomotimer::Pomodoro( defaultC );
		shortC = new pomotimer::Config( 20,10,15,3 );
		fastPomo = new pomotimer::Pomodoro( *shortC );
	}
	void tearDown()
	{
		delete pomo;
		delete fastPomo;
		delete shortC;
	}
	void testConstructor()
	{
		CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( pomo->getTime() == defaultC.getFocus() );
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus() );
	}

	void testUpdateReset()
	{
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus() );
		fastPomo->update();
		CPPUNIT_ASSERT( fastPomo->getTime() != shortC->getFocus() );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus()-1 );
		fastPomo->reset();
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus() );

		for( uint32_t i=0; i<shortC->getFocus(); i++ ) {
			fastPomo->update();
		}
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::SHORT_BREAK );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getShortBreak() );
		fastPomo->update();
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::SHORT_BREAK );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getShortBreak()-1 );
		fastPomo->reset();
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus() );
	}
	void testUpdateShort()
	{
		for( uint32_t k=0; k<shortC->getLoopSize()-1; k++ ) {
			CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::FOCUS );
			CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus() );
			for( uint32_t i=0; i<shortC->getFocus(); i++ ) {
				fastPomo->update();
			}
			CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::SHORT_BREAK );
			CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getShortBreak() );
			for( uint32_t i=0; i<shortC->getShortBreak(); i++ ) {
				fastPomo->update();
			}
		}
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus() );
		for( uint32_t i=0; i<shortC->getFocus(); i++ ) {
			fastPomo->update();
		}
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::LONG_BREAK );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getLongBreak() );
		for( uint32_t i=0; i<shortC->getLongBreak(); i++ ) {
			fastPomo->update();
		}
		CPPUNIT_ASSERT( fastPomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus() );
	}
	void testUpdateDefault()
	{
		for( uint32_t k=0; k<defaultC.getLoopSize()-1; k++ ) {
			CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::FOCUS );
			CPPUNIT_ASSERT( pomo->getTime() == defaultC.getFocus() );
			for( uint32_t i=0; i<defaultC.getFocus(); i++ ) {
				pomo->update();
			}
			CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::SHORT_BREAK );
			CPPUNIT_ASSERT( pomo->getTime() == defaultC.getShortBreak() );
			for( uint32_t i=0; i<defaultC.getShortBreak(); i++ ) {
				pomo->update();
			}
		}
		CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( pomo->getTime() == defaultC.getFocus() );
		for( uint32_t i=0; i<defaultC.getFocus(); i++ ) {
			pomo->update();
		}
		CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::LONG_BREAK );
		CPPUNIT_ASSERT( pomo->getTime() == defaultC.getLongBreak() );
		for( uint32_t i=0; i<defaultC.getLongBreak(); i++ ) {
			pomo->update();
		}
		CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::FOCUS );
		CPPUNIT_ASSERT( pomo->getTime() == defaultC.getFocus() );
	}
	void testUpdateLoops()
	{
		for( int j=0; j<5; ++j ) {
			for( uint32_t k=0; k<defaultC.getLoopSize()-1; k++ ) {
				CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::FOCUS );
				CPPUNIT_ASSERT( pomo->getTime() == defaultC.getFocus() );
				for( uint32_t i=0; i<defaultC.getFocus(); i++ ) {
					pomo->update();
				}
				CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::SHORT_BREAK );
				CPPUNIT_ASSERT( pomo->getTime() == defaultC.getShortBreak() );
				for( uint32_t i=0; i<defaultC.getShortBreak(); i++ ) {
					pomo->update();
				}
			}
			CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::FOCUS );
			CPPUNIT_ASSERT( pomo->getTime() == defaultC.getFocus() );
			for( uint32_t i=0; i<defaultC.getFocus(); i++ ) {
				pomo->update();
			}
			CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::LONG_BREAK );
			CPPUNIT_ASSERT( pomo->getTime() == defaultC.getLongBreak() );
			for( uint32_t i=0; i<defaultC.getLongBreak(); i++ ) {
				pomo->update();
			}
			CPPUNIT_ASSERT( pomo->getTimerType() == pomotimer::TimerType::FOCUS );
			CPPUNIT_ASSERT( pomo->getTime() == defaultC.getFocus() );
		}
	}
	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteConfig = new CppUnit::TestSuite( "PomodoroTest" );
		suiteConfig->addTest( new CppUnit::TestCaller< PomodoroTest > (
			"testConstructor", &PomodoroTest::testConstructor )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< PomodoroTest > (
			"testUpdateReset", &PomodoroTest::testUpdateReset )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< PomodoroTest > (
			"testUpdateShort", &PomodoroTest::testUpdateShort )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< PomodoroTest > (
			"testUpdateDefault", &PomodoroTest::testUpdateDefault )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< PomodoroTest > (
			"testUpdateLoops", &PomodoroTest::testUpdateLoops )) ;
		return suiteConfig;
	}
};

int main()
{
	
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( PomodoroTest::suite() );
	runner.run( );
	return 0;
}

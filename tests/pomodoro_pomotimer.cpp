#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include <unistd.h>

#include <mutex>
#include <condition_variable>
#include "../pomotimer.h"

class timeObs : public utility::Observer< uint32_t >
{
private:
	uint32_t time;
	std::mutex mtx;
	std::mutex * shmtx;
	std::condition_variable * cv;
public:
	timeObs(std::mutex * shmtx, std::condition_variable * cv, uint32_t it=1500) :
		time(it), mtx() , shmtx(shmtx), cv(cv) {}
	virtual void notify( uint32_t t ) {
		{
			std::lock_guard<std::mutex> lock(mtx);
			time = t;
		}
		cv->notify_one();
	}
	uint32_t getTime() {
		std::lock_guard<std::mutex> lock(mtx);
		return time;
	}
};

class timerObs : public utility::Observer< pomotimer::TimerType >
{
private:
	pomotimer::TimerType timerType;
	std::mutex mtx;
public:
	timerObs(pomotimer::TimerType it=pomotimer::TimerType::FOCUS) : timerType(it) {}
	virtual void notify( pomotimer::TimerType t ) {
		std::lock_guard<std::mutex> lock(mtx);
		timerType = t;
	}
	pomotimer::TimerType getTimerType() {
		std::lock_guard<std::mutex> lock(mtx);
		return timerType;
	}
};

class PomotimerTest : public CppUnit::TestFixture {
private:
	pomotimer::Config defaultC;
	pomotimer::Config * shortC;
	pomotimer::Pomotimer * pomo;
	pomotimer::Pomotimer * fastPomo;
	timeObs * fastTo; // fast time observer
	timerObs * fastTro; // fast timer observer
	std::mutex timeMtx;
	std::condition_variable timeCv;
	std::mutex timerMtx;
	std::condition_variable timerCv;
public:
	void setUp()
	{
		pomo = new pomotimer::Pomotimer( defaultC );
		shortC = new pomotimer::Config( 20,10,15,3 );
		fastPomo = new pomotimer::Pomotimer( *shortC );
		fastTo = new timeObs( &timeMtx, &timeCv, 20 );
		fastPomo->addObs( fastTo );
		fastPomo->addObs( fastTro );
	}
	void tearDown()
	{
		delete pomo;
		delete fastPomo;
		delete shortC;
	}

	void testConstructor()
	{
		CPPUNIT_ASSERT( pomo->getTime() == defaultC.getFocus() );
		CPPUNIT_ASSERT( fastPomo->getTime() == shortC->getFocus() );
		CPPUNIT_ASSERT( fastPomo->getTime() == fastTo->getTime() );
	}

	void testPause()
	{
		fastPomo->start();
		{ // wait 3 seconds
			std::unique_lock<std::mutex> lk(timeMtx);
			timeCv.wait(lk,[this]{return (fastTo->getTime() == 17);});
			fastPomo->pause();
		}
		CPPUNIT_ASSERT( shortC->getFocus() != fastTo->getTime() );
		{ // stop it, after a pause
			std::unique_lock<std::mutex> lk(timeMtx);
			fastPomo->stop();
			timeCv.wait(lk);
		}
		CPPUNIT_ASSERT( shortC->getFocus() == fastTo->getTime() );
	}

	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteConfig = new CppUnit::TestSuite( "PomotimerTest" );
		suiteConfig->addTest( new CppUnit::TestCaller< PomotimerTest > (
			"testConstructor", &PomotimerTest::testConstructor )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< PomotimerTest > (
			"testPause", &PomotimerTest::testPause )) ;
		return suiteConfig;
	}
};

int main()
{
	
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( PomotimerTest::suite() );
	runner.run( );
	return 0;
}

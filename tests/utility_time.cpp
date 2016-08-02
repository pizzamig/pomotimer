#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include "../time.h"

class TimeTest : public CppUnit::TestFixture {
private:
	utility::Time * uut;
public:
	void setUp()
	{
		uut = new utility::Time( 1500 );
	}
	void tearDown()
	{
		delete uut;
	}

	void testConstructor()
	{
		std::string s1 = uut->getTimeStr();
		CPPUNIT_ASSERT( s1 == "00:25:00" );
	}

	void testConstructorStress()
	{
		utility::Time * uut0 = new utility::Time(0);
		utility::Time * uutMax = new utility::Time(utility::Time::maxTime);
		utility::Time * uut2 = new utility::Time(1000000);
		std::string s0 = uut0->getTimeStr();
		std::string sMax = uutMax->getTimeStr();
		std::string s2 = uut2->getTimeStr();
		CPPUNIT_ASSERT( s0 == "00:00:00" );
		CPPUNIT_ASSERT( sMax == "23:59:59" );
		CPPUNIT_ASSERT( s2 == "23:59:59" );
	}

	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteConfig = new CppUnit::TestSuite( "TimeTest" );
		suiteConfig->addTest( new CppUnit::TestCaller< TimeTest > (
			"testConstructor", &TimeTest::testConstructor )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< TimeTest > (
			"testConstructorStress", &TimeTest::testConstructorStress )) ;
		return suiteConfig;
	}
};

int main()
{
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( TimeTest::suite() );
	runner.run( );
	return 0;
}

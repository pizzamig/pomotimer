#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include <string>

#include "../pomotimer.h"

class ConfigTest : public CppUnit::TestFixture {
private:
	pomotimer::Config *defaultC, *customC;
	pomotimer::Config *wrongC1, *wrongC2, *wrongC3, *wrongC4;
public:
	void setUp()
	{
		defaultC = new pomotimer::Config();
		customC = new pomotimer::Config( 25,10,20,2 );
		wrongC1 = new pomotimer::Config( 200, 300, 400, 0 );
		wrongC2 = new pomotimer::Config( 5, 300, 400, 0 );
		wrongC3 = new pomotimer::Config( 5, 8, 400, 0 );
		wrongC4 = new pomotimer::Config( 5, 8, 2, 0 );
	}
	void tearDown()
	{
		delete defaultC;
		delete customC;
		delete wrongC1;
		delete wrongC2;
		delete wrongC3;
		delete wrongC4;
		
	}
	void testConstructor()
	{
		CPPUNIT_ASSERT( defaultC->getFocus() == 25*60 );
		CPPUNIT_ASSERT( defaultC->getShortBreak() == 5*60 );
		CPPUNIT_ASSERT( defaultC->getLongBreak() == 20*60 );
		CPPUNIT_ASSERT( defaultC->getLoopSize() == 4 );

		CPPUNIT_ASSERT( customC->getFocus() == 25 );
		CPPUNIT_ASSERT( customC->getShortBreak() == 10 );
		CPPUNIT_ASSERT( customC->getLongBreak() == 20 );
		CPPUNIT_ASSERT( customC->getLoopSize() == 2 );
	}
	void testWrongConfig1()
	{
		CPPUNIT_ASSERT( wrongC1->getFocus() == 200 );
		CPPUNIT_ASSERT( wrongC1->getShortBreak() == 300 );
		CPPUNIT_ASSERT( wrongC1->getLongBreak() == 400 );
		CPPUNIT_ASSERT( wrongC1->getLoopSize() == 2 );
	}
	void testWrongConfig2()
	{
		CPPUNIT_ASSERT( wrongC2->getFocus() == 10 );
		CPPUNIT_ASSERT( wrongC2->getShortBreak() == 300 );
		CPPUNIT_ASSERT( wrongC2->getLongBreak() == 400 );
		CPPUNIT_ASSERT( wrongC2->getLoopSize() == 2 );
	}
	void testWrongConfig3()
	{
		CPPUNIT_ASSERT( wrongC3->getFocus() == 10 );
		CPPUNIT_ASSERT( wrongC3->getShortBreak() == 10 );
		CPPUNIT_ASSERT( wrongC3->getLongBreak() == 400 );
		CPPUNIT_ASSERT( wrongC3->getLoopSize() == 2 );
	}
	void testWrongConfig4()
	{
		CPPUNIT_ASSERT( wrongC4->getFocus() == 10 );
		CPPUNIT_ASSERT( wrongC4->getShortBreak() == 10 );
		CPPUNIT_ASSERT( wrongC4->getLongBreak() == 10 );
		CPPUNIT_ASSERT( wrongC4->getLoopSize() == 2 );
	}
	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteConfig = new CppUnit::TestSuite( "ConfigTest" );
		suiteConfig->addTest( new CppUnit::TestCaller< ConfigTest > (
			"testConstructor", &ConfigTest::testConstructor )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< ConfigTest > (
			"testWrongConfig1", &ConfigTest::testWrongConfig1 )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< ConfigTest > (
			"testWrongConfig2", &ConfigTest::testWrongConfig2 )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< ConfigTest > (
			"testWrongConfig3", &ConfigTest::testWrongConfig3 )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< ConfigTest > (
			"testWrongConfig4", &ConfigTest::testWrongConfig4 )) ;
		return suiteConfig;
	}
};

int main()
{
	
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( ConfigTest::suite() );
	runner.run( );
	return 0;
}

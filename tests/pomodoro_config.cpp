// 	assertEquals( C1.getFocus(), 25*60 );
// 	assertEquals( C1.getShortBreak(), 5*60 );
// 	assertEquals( C1.getLongBreak(), 20*60 );
// 	assertEquals( C1.getLoopSize(), 4 );

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
		customC = new pomotimer::Config( 25,5,20,2 );
		wrongC1 = new pomotimer::Config( 200, 300, 400, 0 );
		wrongC2 = new pomotimer::Config( 200, 300, 400, 0 );
		wrongC3 = new pomotimer::Config( 200, 300, 400, 0 );
		wrongC4 = new pomotimer::Config( 200, 300, 400, 0 );
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
		CPPUNIT_ASSERT( customC->getShortBreak() == 5 );
		CPPUNIT_ASSERT( customC->getLongBreak() == 20 );
		CPPUNIT_ASSERT( customC->getLoopSize() == 2 );

		CPPUNIT_ASSERT( wrongC1->getFocus() == 200 );
		CPPUNIT_ASSERT( wrongC1->getShortBreak() == 300 );
		CPPUNIT_ASSERT( wrongC1->getLongBreak() == 400 );
// 		CPPUNIT_ASSERT( wrongC1->getLoopSize() == 1 );
	}
	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteConfig = new CppUnit::TestSuite( "ConfigTest" );
		suiteConfig->addTest( new CppUnit::TestCaller< ConfigTest > (
			"testConstructor", &ConfigTest::testConstructor )) ;
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

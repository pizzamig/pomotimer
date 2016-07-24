#include <cppunit/TestFixture.h>
#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <cppunit/ui/text/TestRunner.h>

#include <vector>

#include "../observer.h"

class ObserverStub : public utility::Observer<uint32_t>
{
public:
	virtual void notify(uint32_t t)
	{
		notifications.push_back( t );
	}

	std::vector<uint32_t> notifications;
};

class ObserverTest : public CppUnit::TestFixture, utility::Observable<uint32_t> {
private:
	ObserverStub s1;
	ObserverStub s2;
	ObserverStub s3;
public:
	void setUp()
	{
		if( s1.notifications.empty() == false ) {
			s1.notifications.clear();
		}
		if( s2.notifications.empty() == false ) {
			s2.notifications.clear();
		}
		if( s3.notifications.empty() == false ) {
			s3.notifications.clear();
		}
	}
	
	void tearDown()
	{
		removeAllObs();
	}
	
	void testNotifications()
	{
		addObs( &s1 );
		notifyAllObs( 10 );
		CPPUNIT_ASSERT( s1.notifications.size() == 1 );
		CPPUNIT_ASSERT( s1.notifications.at(0) == 10 );

		addObs( &s2 );
		notifyAllObs( 20 );
		CPPUNIT_ASSERT( s1.notifications.size() == 2 );
		CPPUNIT_ASSERT( s1.notifications.at(0) == 10 );
		CPPUNIT_ASSERT( s1.notifications.at(1) == 20 );
		CPPUNIT_ASSERT( s2.notifications.size() == 1 );
		CPPUNIT_ASSERT( s2.notifications.at(0) == 20 );

		addObs( &s3 );
		notifyAllObs( 30 );
		CPPUNIT_ASSERT( s1.notifications.size() == 3 );
		CPPUNIT_ASSERT( s1.notifications.at(0) == 10 );
		CPPUNIT_ASSERT( s1.notifications.at(1) == 20 );
		CPPUNIT_ASSERT( s1.notifications.at(2) == 30 );
		CPPUNIT_ASSERT( s2.notifications.size() == 2 );
		CPPUNIT_ASSERT( s2.notifications.at(0) == 20 );
		CPPUNIT_ASSERT( s2.notifications.at(1) == 30 );
		CPPUNIT_ASSERT( s3.notifications.size() == 1 );
		CPPUNIT_ASSERT( s3.notifications.at(0) == 30 );
	}

	void testInvalidNotifications()
	{
		addObs( nullptr );
		addObs( &s1 );
		notifyAllObs( 10 );
		CPPUNIT_ASSERT( s1.notifications.size() == 1 );
		CPPUNIT_ASSERT( s1.notifications.at(0) == 10 );
	}

	void testRemoveAll()
	{
		addObs( &s1 );
		notifyAllObs( 10 );
		CPPUNIT_ASSERT( s1.notifications.size() == 1 );
		CPPUNIT_ASSERT( s1.notifications.at(0) == 10 );

		addObs( &s2 );
		notifyAllObs( 20 );
		CPPUNIT_ASSERT( s1.notifications.size() == 2 );
		CPPUNIT_ASSERT( s1.notifications.at(0) == 10 );
		CPPUNIT_ASSERT( s1.notifications.at(1) == 20 );
		CPPUNIT_ASSERT( s2.notifications.size() == 1 );
		CPPUNIT_ASSERT( s2.notifications.at(0) == 20 );

		removeAllObs();
		addObs( &s3 );
		notifyAllObs( 30 );
		CPPUNIT_ASSERT( s1.notifications.size() == 2 );
		CPPUNIT_ASSERT( s1.notifications.at(0) == 10 );
		CPPUNIT_ASSERT( s1.notifications.at(1) == 20 );
		CPPUNIT_ASSERT( s2.notifications.size() == 1 );
		CPPUNIT_ASSERT( s2.notifications.at(0) == 20 );
		CPPUNIT_ASSERT( s3.notifications.size() == 1 );
		CPPUNIT_ASSERT( s3.notifications.at(0) == 30 );
	}
	
	static CppUnit::TestSuite *suite()
	{
		CppUnit::TestSuite *suiteConfig = new CppUnit::TestSuite( "ConfigTest" );
		suiteConfig->addTest( new CppUnit::TestCaller< ObserverTest > (
			"testNotifications", &ObserverTest::testNotifications )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< ObserverTest > (
			"testInvalidNotifications", &ObserverTest::testInvalidNotifications )) ;
		suiteConfig->addTest( new CppUnit::TestCaller< ObserverTest > (
			"testRemoveAll", &ObserverTest::testRemoveAll )) ;
		return suiteConfig;
	}
};

int main()
{
	CppUnit::TextUi::TestRunner runner;
	runner.addTest( ObserverTest::suite() );
	runner.run( );
	return 0;
}

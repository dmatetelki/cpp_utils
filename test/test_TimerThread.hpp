#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#define private public // reach TimerThread's private multimap

#include "TimerThread.hpp"

#include <time.h>




class TestTimerThread : public CxxTest::TestSuite
{

private:

  class DummyTimerUser : public TimerUser
  {

  public:

    DummyTimerUser( void ) : m_counter(0) { TRACE; }
    ~DummyTimerUser( void ) { TRACE; }
    void timerExpired( void ) { TRACE; m_counter++; }
    void timerDestroyed( void ) { TRACE; m_counter += 100; }

    int m_counter;

  };  // class DummyTimerUser



public:

  void testBasic( void )
  {
    TEST_HEADER;
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    tt->addTimerUser( user, 2 );

    sleep(4);
    tt->stop();
    sleep(1);

    TS_ASSERT_EQUALS( user->m_counter, 1 );

    delete tt;
    delete user;
  }

  void testBasicTimeSpec( void )
  {
    TEST_HEADER;
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    timespec ts = { 1, 123 };
    tt->addTimerUser( user, ts );

    sleep(4);
    tt->stop();
    sleep(1);

    TS_ASSERT_EQUALS( user->m_counter, 1 );

    delete tt;
    delete user;
  }

  void testPeriodic( void )
  {
    TEST_HEADER;
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    tt->addTimerUser( user, 2, 1 );

    sleep(6);
    tt->stop();
    sleep(1);

    TS_ASSERT_EQUALS( user->m_counter, 104 );

    delete tt;
    delete user;
  }

  void testPeriodicTimeSpec( void )
  {
    TEST_HEADER;

//     Logger::getInstance()->setLogLevel(Logger::DEBUG);
    
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    timespec ts = { 2, 0 };
    timespec tsperiod = { 1, 0 };
    tt->addTimerUser( user, ts, tsperiod );

    /// @bug What is wrong here?
    DummyTimerUser *user2 = new DummyTimerUser();
    int perMinute = 2000;
    timespec ts2 = { 0, 1000000000 / perMinute };
    tt->addTimerUser( user2, ts, ts2 );

    sleep(6);
    tt->stop();
    sleep(1);

    TS_ASSERT_EQUALS( user->m_counter, 4 );  // 4 times
//     TS_ASSERT_EQUALS( user2->m_counter, perMinute*4 );

    delete tt;
    delete user;
    delete user2;
  }

  void testDestroyed( void )
  {
    TEST_HEADER;
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    tt->addTimerUser( user, 10 );

    sleep(2);
    tt->stop();
    sleep(1);

    TS_ASSERT_EQUALS( user->m_counter, 100 );

    delete tt;
    delete user;
  }

  void testRemoved( void )
  {
    TEST_HEADER;
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    DummyTimerUser *user2 = new DummyTimerUser();
    DummyTimerUser *user3 = new DummyTimerUser();
    tt->addTimerUser( user, 10 );
    tt->addTimerUser( user2, 13 );
    tt->addTimerUser( user3, 15 );

    sleep(2);
    TS_ASSERT_EQUALS( tt->m_users.size(), 3 );


    tt->removeTimerUser( user2 );
    sleep(1);
    TS_ASSERT_EQUALS( tt->m_users.size(), 2 );

    tt->removeTimerUser( user3 );
    sleep(1);
    TS_ASSERT_EQUALS( tt->m_users.size(), 1 );

    tt->removeTimerUser( user2 );
    sleep(1);
    TS_ASSERT_EQUALS( tt->m_users.size(), 1 );

    tt->removeTimerUser( user );
    sleep(1);
    TS_ASSERT_EQUALS( tt->m_users.size(), 0 );

    tt->stop();
    sleep(1);

    delete tt;
    delete user;
    delete user2;
    delete user3;
  }

  void testRemovedMultiple( void )
  {
    TEST_HEADER;
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    tt->addTimerUser( user, 10 );
    tt->addTimerUser( user, 12 );
    tt->addTimerUser( user, 13 );
    tt->addTimerUser( user, 14 );

    TS_ASSERT_EQUALS( tt->m_users.size(), 4);

    sleep(2);
    tt->removeTimerUser( user );
    sleep(2);

    TS_ASSERT_EQUALS( tt->m_users.size(), 0);

    sleep(1);
    tt->stop();
    sleep(1);


    delete tt;
    delete user;
  }

};

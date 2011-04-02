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
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    timespec ts = { 2, 3 };
    timespec tsperiod = { 1, 2 };
    tt->addTimerUser( user, ts, tsperiod );

    /// @bug What is wrong here?
//     DummyTimerUser *user2 = new DummyTimerUser();
//     timespec ts2 = { 0, 100000000 };
//     tt->addTimerUser( user2, ts, ts2 );

    sleep(6);
    tt->stop();
    sleep(1);

    TS_ASSERT_EQUALS( user->m_counter, 104 );
//     TS_ASSERT_EQUALS( user2->m_counter, 110 );

    delete tt;
    delete user;
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
    tt->addTimerUser( user, 10 );

    sleep(2);
    tt->removeTimerUser( user );

    sleep(1);
    tt->stop();
    sleep(1);

    TS_ASSERT_EQUALS( user->m_counter, 100 );

    delete tt;
    delete user;
  }

  /// @bug this case does not work, investigate!
  void bugtestRemovedMultiple( void )
  {
    TEST_HEADER;
    TimerThread* tt = new TimerThread();
    tt->start();
    sleep(1);

    DummyTimerUser *user = new DummyTimerUser();
    tt->addTimerUser( user, 10 );
    DummyTimerUser *user2 = new DummyTimerUser();
    tt->addTimerUser( user2, 12 );
    DummyTimerUser *user3 = new DummyTimerUser();
    tt->addTimerUser( user3, 13 );
    DummyTimerUser *user4 = new DummyTimerUser();
    tt->addTimerUser( user4, 14 );

    TS_ASSERT_EQUALS( tt->m_users.size(), 4);

    sleep(2);
    tt->removeTimerUser( user );

    TS_ASSERT_EQUALS( tt->m_users.size(), 0);

    sleep(1);
    tt->stop();
    sleep(1);


    delete tt;
    delete user;
    delete user2;
    delete user3;
    delete user4;
  }

};

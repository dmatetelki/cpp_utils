#include <cxxtest/TestSuite.h>

#include "test_Common.hpp"

#include "TimerThread.hpp"


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

};

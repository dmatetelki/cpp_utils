#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#define protected public

#include "Timer.hpp"


class TestTimerUser : public CxxTest::TestSuite
{

private:

  class DummyTimerUser : public TimerUser
  {
  public:

    DummyTimerUser()
      : m_counter(0)
    {
      TRACE;
    }

    ~DummyTimerUser()
    {
      TRACE;
    }

    void timerExpired()
    {
      TRACE;
      m_counter++;
    }

    int m_counter;

  }; // class TimerUser

  public:

  void testBasicTimer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;

    timerUser.startTimer(1);
    sleep(2);

    // one expiration
    TS_ASSERT_EQUALS( timerUser.m_counter, 1 );
  }


  void testStopTimer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;

    timerUser.startTimer(10);
    sleep(1);
    timerUser.stopTimer();

    // no expiration
    TS_ASSERT_EQUALS( timerUser.m_counter, 0 );
  }


  void testPeriodicTimer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;

    // after 1 sec, expire periodically at each sec
    timerUser.startTimer(1, 0, 1, 0);

    sleep(4);
    timerUser.stopTimer();

    // 3 expiration  (+- 1)
    TS_ASSERT_DELTA( timerUser.m_counter, 3, 1 );

  }

  void test2TimerUsers( void )
  {
    TEST_HEADER;

    DummyTimerUser t1;
    DummyTimerUser t2;

    t1.startTimer(0, 5000);
    t2.startTimer(0, 6000);
    sleep(1);

    TS_ASSERT_EQUALS( t1.m_counter, 1 );
    TS_ASSERT_EQUALS( t2.m_counter, 1 );
  }

  void testStopNotStartedTimer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;

    timerUser.stopTimer();

    // timerDestroyed not called
    TS_ASSERT_EQUALS( timerUser.m_counter, 0 );
  }

};

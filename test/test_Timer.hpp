#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"


#include "Timer.hpp"
#include "Thread.hpp"

#include <signal.h>
#include <climits>

class TestTimer : public CxxTest::TestSuite
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

    void timerDestroyed()
    {
      TRACE;
      m_counter += 100;
    }

    int m_counter;

  }; // class TimerUser

  public:

  void testBasicTimer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser );
    timer.setTimer(timerId, 2);
    sleep( 4 );

    // one expiration, no destroy
    TS_ASSERT_EQUALS( timerUser.m_counter, 1 );
  }


  void testStopTimer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser );
    timer.setTimer(timerId, 10);
    sleep( 2 );
    timer.stopTimer( timerId );

    // no expiration, just destroy
    TS_ASSERT_EQUALS( timerUser.m_counter, 100 );
  }


  void testPeriodicTimer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser );

    // after 1 sec, expire periodically at each sec
    timer.setTimer(timerId, 1, 0, 1, 0);

    sleep(4);
    timer.stopTimer( timerId );

    // 1 destroy(stop) + 3 expiration  (+- 1)
    TS_ASSERT_DELTA( timerUser.m_counter, 103, 1 );

  }

  void testOneUserManyTimers( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser );
    timer.setTimer(timerId, 1);

    timer_t timerId2 = timer.createTimer( &timerUser );
    timer.setTimer(timerId2, 2);

    timer_t timerId3 = timer.createTimer( &timerUser );
    timer.setTimer(timerId3, 3);

    sleep(4);
    timer.stopTimer( timerId );
    TS_ASSERT_EQUALS( timerUser.m_counter, 100 + 1 + 1 + 1 );
  }

  void testMenyUserManyTimers( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    DummyTimerUser timerUser2;
    DummyTimerUser timerUser3;

    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser );
    timer.setTimer(timerId, 1);
    timer_t timerId2 = timer.createTimer( &timerUser );
    timer.setTimer(timerId2, 2);
    timer_t timerId3 = timer.createTimer( &timerUser );
    timer.setTimer(timerId3, 3);

    timer_t timerId4 = timer.createTimer( &timerUser2 );
    timer.setTimer(timerId4, 1);
    timer_t timerId5 = timer.createTimer( &timerUser2 );
    timer.setTimer(timerId5, 2);

    sleep(4);
    TS_ASSERT_EQUALS( timerUser.m_counter, 1 + 1 + 1 );
    TS_ASSERT_EQUALS( timerUser2.m_counter, 1 + 1 );
    TS_ASSERT_EQUALS( timerUser3.m_counter, 0 );
  }

  void test2Timer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;

    Timer timer;
    Timer timer2;

    timer_t timerId = timer.createTimer( &timerUser );
    timer.setTimer(timerId, 1);
    timer_t timerId2 = timer.createTimer( &timerUser );
    timer.setTimer(timerId2, 2);
    timer_t timerId3 = timer.createTimer( &timerUser );
    timer.setTimer(timerId3, 3);

    timer_t timerId4 = timer.createTimer( &timerUser );
    timer2.setTimer(timerId4, 1);
    timer_t timerId5 = timer.createTimer( &timerUser );
    timer2.setTimer(timerId5, 2);


    sleep(4);
    TS_ASSERT_EQUALS( timerUser.m_counter, (1 + 1 + 1) + (1 + 1) );
  }

  void testStopTimerWhichIsNotInTheMap()
  {
    TEST_HEADER;

    Timer timer;

    timer_t timerId = (void*)1234;
    TS_ASSERT_EQUALS(timer.stopTimer( timerId ), false);
  }

  void testSetTimerWhichIsNotInTheMap()
  {
    TEST_HEADER;

    Timer timer;

    timer_t timerId = (void*)1234;
    TS_ASSERT_EQUALS(timer.setTimer( timerId, 1 ), false);
  }

};

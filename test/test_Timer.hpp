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

    DummyTimerUser() : m_counter( 0 ) { TRACE; }
    void timerExpired() { /*TRACE;*/ m_counter += 100; }
    void timerDestroyed() { TRACE; m_counter++; };

    int m_counter;

  }; // class TimerUser

  public:

  void testBasicTimerThread( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    Timer timer;

    timer.createTimer( &timerUser, 2 );
    sleep( 4 );
    // no destroy

    TS_ASSERT_EQUALS( timerUser.m_counter, 100 );
  }


  void testStopTimerThread( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser, 10 );
    sleep( 2 );
    timer.stopTimer( timerId );

    TS_ASSERT_EQUALS( timerUser.m_counter, 1 );
  }




  void testPeriodicTimerThread( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser,
                                         1, 0,
                                         1, 0 );

    sleep(4);
    timer.stopTimer( timerId );

    sleep(4); // did it really stopped?
    TS_ASSERT_EQUALS( timerUser.m_counter, 400 + 1 );

  }


  void testTimerThreadHighFreq( void )
  {
    TEST_HEADER;

    int nano = 1000000000; // 10^9
    int freq = 200;

    DummyTimerUser timerUser;
    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser,
                                         1, 0,
                                         0, nano / freq );

    int circle = 2;
    sleep( 1 + circle );
    timer.stopTimer( timerId );

    TS_ASSERT_DELTA ( timerUser.m_counter, 100 * freq * circle + 1,
                      (100 * freq * circle + 1 ) * 0.9);
    sleep(1);
  }

  void testOneUserManyTimers( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser,
                                         1, 0,
                                         1, 0 );

    timer_t timerId2 = timer.createTimer( &timerUser,
                                          1, 0,
                                          2, 0 );

    timer_t timerId3 = timer.createTimer( &timerUser,
                                          1, 0,
                                          3, 0 );

    sleep(4);
    timer.stopTimer( timerId );
    TS_ASSERT_EQUALS( timerUser.m_counter, 400 + 200 + 200 + 1 );

    sleep(4);
    TS_ASSERT_EQUALS( timerUser.m_counter, 400 + 200 + 200 + 200 + 100 + 1 );

    timer.stopTimer( timerId2 );
    timer.stopTimer( timerId3 );

    sleep(1);
    TS_ASSERT_EQUALS( timerUser.m_counter, 400 + 200 + 200 + 200 + 100 + 3 );

  }

  void testMenyUserManyTimers( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    DummyTimerUser timerUser2;
    DummyTimerUser timerUser3;

    Timer timer;

    timer_t timerId = timer.createTimer( &timerUser,
                                         1, 0,
                                         1, 0 );

    timer_t timerId2 = timer.createTimer( &timerUser,
                                          1, 0,
                                          2, 0 );

    timer_t timerId3 = timer.createTimer( &timerUser,
                                          1, 0,
                                          3, 0 );

    timer_t timerId4 = timer.createTimer( &timerUser2,
                                         1, 0,
                                         1, 0 );

    timer_t timerId5 = timer.createTimer( &timerUser2,
                                          1, 0,
                                          2, 0 );


    sleep(4);
    TS_ASSERT_EQUALS( timerUser.m_counter, 400 + 200 + 200 );
    TS_ASSERT_EQUALS( timerUser2.m_counter, 400 + 200 );

    timer.stopTimer( timerId );
    timer.stopTimer( timerId2 );
    timer.stopTimer( timerId3 );
    timer.stopTimer( timerId4 );
    timer.stopTimer( timerId5 );

    sleep(1);
  }

    void test2Timer( void )
  {
    TEST_HEADER;

    DummyTimerUser timerUser;
    DummyTimerUser timerUser2;
    DummyTimerUser itmerUser3;

    Timer timer;
    Timer timer2;

    timer_t timerId = timer.createTimer( &timerUser,
                                         1, 0,
                                         1, 0 );

    timer_t timerId2 = timer.createTimer( &timerUser,
                                          1, 0,
                                          2, 0 );

    timer_t timerId3 = timer2.createTimer( &timerUser,
                                          1, 0,
                                          3, 0 );

    timer_t timerId4 = timer.createTimer( &timerUser2,
                                         1, 0,
                                         1, 0 );

    timer_t timerId5 = timer2.createTimer( &timerUser2,
                                          1, 0,
                                          2, 0 );


    sleep(4);
    TS_ASSERT_EQUALS( timerUser.m_counter, 400 + 200 + 200 );
    TS_ASSERT_EQUALS( timerUser2.m_counter, 400 + 200 );

    timer.stopTimer( timerId );
    timer.stopTimer( timerId2 );
    timer2.stopTimer( timerId3 );
    timer.stopTimer( timerId4 );
    timer2.stopTimer( timerId5 );

    sleep(1);
  }

};

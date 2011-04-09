#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

// #define private public // reach TimerThread's private multimap

#include "Timer.hpp"

// #include <time.h>




class TestTimer : public CxxTest::TestSuite
{

private:

  class DummyTimer : public Timer
  {
  public:

    DummyTimer(int maxPeriodicCount = 5)
      : m_counter(0)
      , m_maxPeriodicCount(maxPeriodicCount)
    {
      TRACE;
    }

    void timerExpired()
    {
      TRACE;
      m_counter += 100;
    }

    void periodicTimerExpired()
    {
      TRACE;
      static int count = 0;
      m_counter++;
      count++;
      if ( count >= m_maxPeriodicCount ) {
        stopTimer();
      }
    }

    int m_counter;

  private:

    int m_maxPeriodicCount;

  };


public:

  void testBasic( void )
  {
    TEST_HEADER;

    DummyTimer t;
    t.createTimer(2);
    t.wait();
    TS_ASSERT_EQUALS( t.m_counter, 100 );
  }

  void testBasicPeriodic( void )
  {
    TEST_HEADER;

    DummyTimer t;
    t.createTimer(2,0,1);
    t.wait();
    TS_ASSERT_EQUALS( t.m_counter, 105 );
  }


};

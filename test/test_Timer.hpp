#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"


#include "Timer.hpp"
#include "Thread.hpp"

#include <signal.h>

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

private:

  class DummyTimerThread : public Timer, public Thread
  {
  public:

    DummyTimerThread(int maxPeriodicCount = 5)
      : m_counter(0)
      , m_maxPeriodicCount(maxPeriodicCount)
    {
      TRACE;
    }

  private:

    void* run()
    {
      TRACE;
      createTimer(2);
      wait();
      return 0;
    }

  public:

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

  void testBasicTimerThread( void )
  {
    TEST_HEADER;

    // the main thread shall ignore the SIGALRM
    sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGALRM );
    sigprocmask(SIG_BLOCK, &set, NULL);

    DummyTimerThread t;

    t.start();
    sleep(4);
    t.join();

    TS_ASSERT_EQUALS( t.m_counter, 100 );
  }

};

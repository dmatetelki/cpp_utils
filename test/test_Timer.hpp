#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"


#include "Timer.hpp"
#include "Thread.hpp"

#include <signal.h>
#include <climits>

class TestTimer : public CxxTest::TestSuite
{

private:

  class DummyTimer : public Timer
  {
  public:

    DummyTimer(int maxPeriodicCount = 5, const int signal = SIGALRM)
      : Timer(signal)
      , m_counter(0)
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

    DummyTimerThread( const int maxPeriodicCount = INT_MAX - 1,
                      const int signal = SIGALRM,
                      const time_t interval_sec = 2,
                      const long interval_nsec = 0,
                      const time_t initExpr_sec = 0,
                      const long initExpr_nsec = 0 )
      : Timer(signal)
      , m_counter(0)
      , m_maxPeriodicCount(maxPeriodicCount)
      , m_interval_sec(interval_sec)
      , m_interval_nsec(interval_nsec)
      , m_initExpr_sec(initExpr_sec)
      , m_initExpr_nsec(initExpr_nsec)
    {
      TRACE;
    }

  private:

    void* run()
    {
      TRACE;
      createTimer( m_interval_sec,
                   m_interval_nsec,
                   m_initExpr_sec,
                   m_initExpr_nsec);
      wait();
      return 0;
    }

  public:

    void timerExpired()
    {
      TRACE;
      m_counter += 100;
    }

    inline void periodicTimerExpired()
    {
      // logging take too much time at high freq
      // TRACE;
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
    time_t m_interval_sec;
    long m_interval_nsec;
    time_t m_initExpr_sec;
    long m_initExpr_nsec;

  };  // class DummyTimerThread

  public:

  void testBasicTimerThread( void )
  {
    TEST_HEADER;

    // the main thread shall ignore the SIGALRM
    sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGALRM );
    sigprocmask(SIG_BLOCK, &set, NULL);

    DummyTimerThread t(5);

    t.start();
    sleep(4);
    t.join();

    TS_ASSERT_EQUALS( t.m_counter, 100 );
  }

  void testCustomSignal( void )
  {
    TEST_HEADER;

    int customSignal = SIGRTMIN;

    // the main thread shall ignore the customSignal
    sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, customSignal );
    sigprocmask( SIG_BLOCK, &set, NULL);


    DummyTimerThread t( 5, customSignal );

    t.start();
//     timespec ts = { 4, 0 };
//     nanosleep( &ts , 0 );
    sleep(4);
    t.join();

    TS_ASSERT_EQUALS( t.m_counter, 100 );

    sigprocmask( SIG_UNBLOCK, &set, NULL );
  }

  void testTimerThreadHighFreq( void )
  {
    TEST_HEADER;

    // the main thread shall ignore the SIGALRM
    sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGALRM );
    sigprocmask(SIG_BLOCK, &set, NULL);

    int nano = 1000000000; // 10^9
    int freq = 80000;
    DummyTimerThread t(INT_MAX - 1,
                       SIGALRM,
                       1, 0,
                       0, nano / freq );

    t.start();
    int circle = 10;
    sleep( 1 + circle );
    t.gracefulStop();
    t.join();

    // expected 800000 + 100 got 795510
    // accurcy: ~ > 99.5%
    TS_ASSERT_DELTA ( t.m_counter, 100 + freq * circle,  (100 + freq * circle) * 0.995);

    sigprocmask( SIG_UNBLOCK, &set, NULL );
  }

};

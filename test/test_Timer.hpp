#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"


#include "Timer.hpp"
#include "Thread.hpp"

#include <signal.h>
#include <climits>

class TestTimer : public CxxTest::TestSuite
{

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
      : Timer( signal )
      , m_counter( 0 )
      , m_maxPeriodicCount( maxPeriodicCount )
      , m_signal ( signal )
      , m_interval_sec( interval_sec )
      , m_interval_nsec( interval_nsec )
      , m_initExpr_sec( initExpr_sec )
      , m_initExpr_nsec( initExpr_nsec )
    {
      TRACE;
      LOG( Logger::FINEST, ( std::string( "params: " ) +
                             stringify( maxPeriodicCount ) + " " +
                             stringify( signal ) + " " +
                             stringify( interval_sec ) + " " +
                             stringify( interval_nsec ) + " " +
                             stringify( initExpr_sec ) + " " +
                             stringify( initExpr_nsec ) ).c_str() );
    }

    ~DummyTimerThread()
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
                   m_initExpr_nsec );
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
        stop();
      }
    }

    void stop()
    {
      stopTimer();
      sendSignal( m_signal );
    }

    int m_counter;

  private:

    int m_maxPeriodicCount;
    int m_signal;
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
    pthread_sigmask( SIG_BLOCK, &set, 0 );

    DummyTimerThread t(5);

    t.start();
    sleep(4);
    t.join();

    TS_ASSERT_EQUALS( t.m_counter, 100 );

    pthread_sigmask( SIG_UNBLOCK, &set, 0 );
  }

  void testStopTimerThread( void )
  {
    TEST_HEADER;

    // the main thread shall ignore the SIGALRM
    sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGALRM );
    pthread_sigmask( SIG_BLOCK, &set, 0 );

    DummyTimerThread t(5, SIGALRM, 10);

    t.start();
    sleep(1);
    t.stop();
    t.join();

    TS_ASSERT_EQUALS( t.m_counter, 0 );

    pthread_sigmask( SIG_UNBLOCK, &set, 0 );
  }

  void testPeriodicTimerThread( void )
  {
    TEST_HEADER;

    // the main thread shall ignore the SIGALRM
    sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGALRM );
    pthread_sigmask( SIG_BLOCK, &set, 0 );

    DummyTimerThread t( 1000,
                        SIGALRM,
                        1, 0,
                        1, 0 );

    t.start();
    sleep(4);
    t.stop();
    sleep(2);
    t.join();

    TS_ASSERT_EQUALS( t.m_counter, 102 );

    pthread_sigmask( SIG_UNBLOCK, &set, 0 );
  }

  void testCustomSignal( void )
  {
    TEST_HEADER;

    int customSignal = SIGRTMIN;

    // the main thread shall ignore the customSignal
    sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, customSignal );
    pthread_sigmask( SIG_BLOCK, &set, 0 );

    DummyTimerThread t( 5, customSignal );

    t.start();
    sleep(4);
    t.join();

    TS_ASSERT_EQUALS( t.m_counter, 100 );

    pthread_sigmask( SIG_UNBLOCK, &set, 0 );
  }

  void testTimerThreadHighFreq( void )
  {
    TEST_HEADER;

    // the main thread shall ignore the SIGALRM
    sigset_t set;
    sigemptyset( &set );
    sigaddset( &set, SIGALRM );
    pthread_sigmask( SIG_BLOCK, &set, 0 );

    int nano = 1000000000; // 10^9
    int freq = 200;
    DummyTimerThread t( INT_MAX - 1,
                        SIGALRM,
                        1, 0,
                        0, nano / freq );

    t.start();
    int circle = 4;
    sleep( 1 + circle );
    t.stop();
    t.join();

    // expected 800000 + 100 got 795510
    // accurcy: ~ > 99.5%
    TS_ASSERT_DELTA ( t.m_counter, 100 + freq * circle,  (100 + freq * circle) * 0.995);

    LOG( Logger::FINEST, ( std::string( "got: " ) + stringify( t.m_counter ) + " "
                         + "expected: " + stringify( 100 + freq * circle ) ).c_str() );

    pthread_sigmask( SIG_UNBLOCK, &set, 0 );
  }

};

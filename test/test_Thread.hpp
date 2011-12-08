#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#include "Thread.hpp"
#include "Common.hpp"
#include "Mutex.hpp"
#include "ScopedLock.hpp"

#include <stdlib.h> // malloc
#include <signal.h> // SIGINT


class TestThread : public CxxTest::TestSuite
{
private:

  class ThreadClass : public Thread
  {

  private:

    void* run( void )
    {
      TRACE;

      void* retVal = malloc(sizeof(int));
      *((int*)retVal) = 14;
      return retVal;
    }
  };

public:

  void testBasic( void )
  {
    TEST_HEADER;
    ThreadClass m;
    m.start();

    void *retVal = m.join();
    TS_ASSERT_EQUALS ( *((int*)retVal) , 14 );
    free(retVal);
  }


private:

  class ThreadClassWithSignal : public Thread
  {

  public:

    ThreadClassWithSignal()
    {
      TRACE;
      signal(SIGINT, signal_handler);
    }

    ~ThreadClassWithSignal()
    {
      TRACE;
    }

  private:

    void* run( void )
    {
      TRACE;

      // we will got a signal before the sleep finishes
      sleep(665);

      void* retVal = malloc(sizeof(int));
      *((int*)retVal) = 15;
      return retVal;
    }

    static void signal_handler(int sig)
    {
      if (sig==SIGINT) {

        void* retVal = malloc(sizeof(int));
        *((int*)retVal) = 16;
        pthread_exit(retVal);
      }
    }

  }; // class ThreadClassWithSignal


public:

  void testSignalSend( void )
  {
    TEST_HEADER;
    ThreadClassWithSignal m2 ;
    m2.start();
    sleep(1);
    m2.sendSignal(SIGINT);

    void *retVal = m2.join();
    TS_ASSERT(retVal);
    if (retVal != 0 ) {
      TS_ASSERT_EQUALS ( *((int*)retVal) , 16 );
      free((int*)retVal);
    }
  }


private:

  class EmptyThreadClass : public Thread
  {

  private:

    void* run( void ) {
      TRACE;
      return 0;
    }
  };

public:

  void testEmpty( void )
  {
    TEST_HEADER;

    EmptyThreadClass e;
    e.start();

    e.stop();
    void *retVal = e.join();
    TS_ASSERT_EQUALS ( retVal , (void *)0 );
  }

  void testJoiningNotStartedThread( void )
  {
    TEST_HEADER;

    EmptyThreadClass e;

    e.stop();
    e.join();
    void *retVal = e.join();
    TS_ASSERT_EQUALS ( retVal , (void *)0 );
  }

private:

  class EndlessThread : public Thread
  {

  private:

    void* run( void )
    {
      TRACE;

      /** @note if m_isRunning is not volatile, it can be optimized out
        * to a while(1), since body of the loop does not modifies it
        */
      while ( m_isRunning )
        sleep(1);

      return 0;
    }
  };

public:

  void testStatusIsVolatile()
  {
    TEST_HEADER;

    EndlessThread et;
    et.start();

    sleep(1);
    et.stop();
    et.join();
  }

};

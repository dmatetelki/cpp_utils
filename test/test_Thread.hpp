#include <cxxtest/TestSuite.h>

#include "test_Common.hpp"

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

    void* run( void ) {
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
    ThreadClass *m = new ThreadClass;
    m->start();

    void *retVal = m->join();
    TS_ASSERT_EQUALS ( *((int*)retVal) , 14 );
    free(retVal);
    delete m;
  }

/**
 * @note send a signal to a thread
 */

private:

  class ThreadClassWithSignal : public Thread
  {

  public:

    ThreadClassWithSignal() {
      TRACE;
      signal(SIGINT, signal_handler);
    }
    ~ThreadClassWithSignal() {
      TRACE;
    }


  private:

    void* run( void ) {
      TRACE;

      /** @note the function will get stopped before it finishes sleeping
       * If signal arrives after malloc, it will be a memory leak.
       */
      sleep(665);

//       void* retVal = malloc(sizeof(int));
//       *((int*)retVal) = 15;
//       return retVal;
      return 0;
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
    ThreadClassWithSignal *m2 = new ThreadClassWithSignal;
    m2->start();
    sleep(1);
    m2->sendSignal(SIGINT);

    void *retVal = m2->join();
    TS_ASSERT(retVal);
    if (retVal != 0 ) {
      TS_ASSERT_EQUALS ( *((int*)retVal) , 16 );
      free((int*)retVal);
    }
    delete m2;
  }

};

#include <cxxtest/TestSuite.h>

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
  public:

    ThreadClass() { TRACE(this); }
    ~ThreadClass() { TRACE(this); }


  private:

    ThreadClass(const ThreadClass&) { TRACE(this); }
    ThreadClass& operator=(const ThreadClass&) { TRACE(this); return*this; }

    void* run( void ) {
      TRACE(this);

      void* retVal = malloc(sizeof(int));
      *((int*)retVal) = 14;
      return retVal;
    }
  };

public:

  void testBasic( void )
  {
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
      TRACE(this);
      signal(SIGINT, signal_handler);
    }
    ~ThreadClassWithSignal() {
      TRACE(this);
    }


  private:

    ThreadClassWithSignal(const ThreadClassWithSignal&) { TRACE(this); }
    ThreadClassWithSignal& operator=(const ThreadClassWithSignal&) { TRACE(this); return*this; }

    void* run( void ) {
      TRACE(this);

      /** @note the function will get stopped before it finishes sleeping
       * If signal arrives after malloc, it will be a memory leak.
       */
      sleep(32);

      void* retVal = malloc(sizeof(int));
      *((int*)retVal) = 15;
      return retVal;
    }

    static void signal_handler(int sig)
    {
      TRACE("ThreadClassWithSignal::signal_handler");
      if (sig==SIGINT) {
        TRACE("signal_handler got SIGINT");

        void* retVal = malloc(sizeof(int));
        *((int*)retVal) = 16;
        pthread_exit(retVal);
      }
    }
  };

public:

  void testSignalSend( void )
  {
    ThreadClassWithSignal *m2 = new ThreadClassWithSignal;
    m2->start();
    m2->sendSignal(SIGINT);

    sleep(3);
    void *retVal = m2->join();
    TS_ASSERT_EQUALS ( *((int*)retVal) , 16 );
    free(retVal);
    delete m2;
  }

};

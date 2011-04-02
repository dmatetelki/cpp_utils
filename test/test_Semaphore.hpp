#include <cxxtest/TestSuite.h>

#include "Semaphore.hpp"
#include "Thread.hpp"
#include "Common.hpp"
#include "Fixture.hpp"

class TestSemaphore : public CxxTest::TestSuite
{
public:

  void testBasic( void )
  {
    TEST_HEADER;
    Semaphore s;
    TS_ASSERT_EQUALS( s.getCount(), 1 );

    TS_ASSERT_EQUALS( s.lock(), true );
    TS_ASSERT_EQUALS( s.getCount(), 0 );

    TS_ASSERT_EQUALS( s.unLock(), true );
    TS_ASSERT_EQUALS( s.getCount(), 1 );

  }

private:

  class ThreadClassWithSemaphore : public Thread
  {
  public:

    ThreadClassWithSemaphore(Semaphore &semaphore)
      : m_semaphore(semaphore)
    {
      TRACE;
    }
    ~ThreadClassWithSemaphore() {
      TRACE;
    }

    bool use( int timeout = 0 ) {
      TRACE;
      return m_semaphore.lock(timeout);
    }

    bool release( void ) {
      TRACE;
      return m_semaphore.unLock();
    }


  private:

    void* run( void ) {
      TRACE;
      while (m_isRunning) {
        sleep(1);
      }
      return 0;
    }

    Semaphore &m_semaphore;

  }; // class ThreadClassWithSemaphore

public:

  void testWithTwoThreads( void )
  {
    TEST_HEADER;

    Semaphore semaphore(2);
    TS_ASSERT_EQUALS( semaphore.getCount(), 2 );

    ThreadClassWithSemaphore *t1 = new ThreadClassWithSemaphore(semaphore);
    ThreadClassWithSemaphore *t2 = new ThreadClassWithSemaphore(semaphore);
    t1->start();
    t2->start();

    sleep(1);

    TS_ASSERT_EQUALS( t1->use(), true );
    TS_ASSERT_EQUALS( semaphore.getCount(), 1 );
    TS_ASSERT_EQUALS( t1->use(), true );
    TS_ASSERT_EQUALS( semaphore.getCount(), 0 );
    TS_ASSERT_EQUALS( t1->use(1), false );

    TS_ASSERT_EQUALS( t2->use(1), false );
    TS_ASSERT_EQUALS( semaphore.getCount(), 0 );
    TS_ASSERT_EQUALS( t1->release(), true );
    TS_ASSERT_EQUALS( semaphore.getCount(), 1 );
    TS_ASSERT_EQUALS( t2->use(1), true );
    TS_ASSERT_EQUALS( semaphore.getCount(), 0 );

    TS_ASSERT_EQUALS( t2->release(), true );
    TS_ASSERT_EQUALS( semaphore.getCount(), 1 );
    TS_ASSERT_EQUALS( t2->release(), true );
    TS_ASSERT_EQUALS( semaphore.getCount(), 2 );
    TS_ASSERT_EQUALS( t2->release(), false );

    t1->stop();
    t2->stop();
    t1->join();
    t2->join();
    delete t1;
    delete t2;
  }

};

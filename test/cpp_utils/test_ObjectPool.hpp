#include <cxxtest/TestSuite.h>

#include <unistd.h>

#include <cpp_utils/Common.hpp>
#include "Fixture.hpp"

#include <cpp_utils/ObjectPool.hpp>
#include <cpp_utils/Thread.hpp>

class TestObjectPool : public CxxTest::TestSuite
{

public:

  void testBasic( void )
  {
    TEST_HEADER;

    ObjectPool<int> op;

    int a(1);
    op.add(a);

    TS_ASSERT_EQUALS( op.acquire(), a );
  }

  void testPointers( void )
  {
    TEST_HEADER;

    ObjectPool<int*> op;

    int *a = new int(1);
    int *b = new int(2);
    op.add(a);
    op.add(b);

    int *tmp_a = op.acquire();
    int *tmp_b = op.acquire();

    TS_ASSERT_EQUALS( *tmp_a, *a );
    TS_ASSERT_EQUALS( *tmp_b, *b );

    // release will delete them
    op.release(tmp_a);
    op.release(tmp_b);
  }


private:

  class ObjecPoolUserThread : public Thread
  {
  public:
    ObjecPoolUserThread( ObjectPool<int*> &objectPool
    )
      : m_objectPool(objectPool)
    {
      TRACE;
    }

  private:
    void* run()
    {
      TRACE;
      int *a;
      try {
        a = m_objectPool.acquire();
        LOG( Logger::DEBUG, std::string("Acquired int: ").
                              append(TToStr(*a)).c_str() );
      } catch ( CancelledException ex ) {
        LOG( Logger::DEBUG, "Cancelled while acquiring" );
      }

      sleep(1);

      try {
        m_objectPool.release(a);
      } catch ( CancelledException ex ) {
        LOG( Logger::DEBUG, "Cancelled while releasing" );
      }
      return 0;
    }

    ObjectPool<int*> &m_objectPool;
  };

public:

  void testCompetingThreads( void )
  {
    TEST_HEADER;

    ObjectPool<int*> op;

    ObjecPoolUserThread t1(op);
    ObjecPoolUserThread t2(op);

    int *a = new int(27);
    op.add(a);

    t1.start();
    t2.start();

    t1.join();
    t2.join();

    // no need to delete "a", dtor of the ConqurrentDeque takes care of it
  }


public:

  void testCleanUp( void )
  {
    TEST_HEADER;

    ObjectPool<int*> cop;
    int *a = new int(12);
    int *b = new int(13);
    int *c = new int(14);
    cop.add(a);
    cop.add(b);
    cop.add(c);

//     ObjecPoolUserThread t1(cop);
//     ObjecPoolUserThread t2(cop);

//     t1.start();
//     t2.start();

    cop.clear();
  }

};

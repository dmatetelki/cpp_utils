#include <cxxtest/TestSuite.h>

#include "Common.hpp"
#include "Fixture.hpp"
#include "Mutex.hpp"

#include <errno.h> // EDEADLK, EPERM, ETIMEDOUT

class TestMutex : public CxxTest::TestSuite
{


public:

  void testBasic( void )
  {
    TEST_HEADER;
    Mutex m;
    TS_ASSERT_EQUALS ( m.lock() , 0 );
    // TS_ASSERT_EQUALS ( m.lock() , 0 );  that would be a deadlock
    TS_ASSERT_EQUALS ( m.tryLock(0), EBUSY );
    TS_ASSERT_EQUALS ( m.tryLock(2), ETIMEDOUT );
    TS_ASSERT_EQUALS ( m.unlock() , 0 );
    TS_ASSERT_EQUALS ( m.unlock() , 0 );
  }

  void testErrorCheck( void )
  {
    TEST_HEADER;
    Mutex m(PTHREAD_MUTEX_ERRORCHECK);
    TS_ASSERT_EQUALS ( m.lock() , 0 );
    TS_ASSERT_EQUALS ( m.lock(), EDEADLK );

    TS_ASSERT_EQUALS ( m.unlock() , 0 );
    TS_ASSERT_EQUALS ( m.unlock() , EPERM );
  }

  void testRecursive( void )
  {
    TEST_HEADER;
    Mutex m(PTHREAD_MUTEX_RECURSIVE);
    TS_ASSERT_EQUALS ( m.lock() , 0 );
    TS_ASSERT_EQUALS ( m.lock() , 0 );

    TS_ASSERT_EQUALS ( m.unlock() , 0 );
    TS_ASSERT_EQUALS ( m.unlock() , 0 );
    TS_ASSERT_EQUALS ( m.unlock() , EPERM );
  }


};

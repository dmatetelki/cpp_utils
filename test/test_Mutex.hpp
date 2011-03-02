#include <cxxtest/TestSuite.h>

#include "Common.hpp"
#include "Mutex.hpp"

#include <errno.h> // EDEADLK, EPERM

class TestMutex : public CxxTest::TestSuite
{


public:

  void testBasic( void )
  {
    Mutex m;
    TS_ASSERT_EQUALS ( m.lock() , 0 );
    // TS_ASSERT_EQUALS ( m.lock() , 0 );  that would be a deadlock
    TS_ASSERT_EQUALS ( m.tryLock(0), false );
    TS_ASSERT_EQUALS ( m.tryLock(2), false );
    TS_ASSERT_EQUALS ( m.unlock() , 0 );
    TS_ASSERT_EQUALS ( m.unlock() , 0 );
  }

  void testErrorCheck( void )
  {
    Mutex m(PTHREAD_MUTEX_ERRORCHECK);
    TS_ASSERT_EQUALS ( m.lock() , 0 );
    TS_ASSERT_EQUALS ( m.lock(), EDEADLK );

    TS_ASSERT_EQUALS ( m.unlock() , 0 );
    TS_ASSERT_EQUALS ( m.unlock() , EPERM );
  }

  void testRecursive( void )
  {
    Mutex m(PTHREAD_MUTEX_RECURSIVE);
    TS_ASSERT_EQUALS ( m.lock() , 0 );
    TS_ASSERT_EQUALS ( m.lock() , 0 );

    TS_ASSERT_EQUALS ( m.unlock() , 0 );
    TS_ASSERT_EQUALS ( m.unlock() , 0 );
    TS_ASSERT_EQUALS ( m.unlock() , EPERM );
  }


};

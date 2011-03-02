#include <cxxtest/TestSuite.h>

#include "ScopedLock.hpp"


class TestScopedLock : public CxxTest::TestSuite
{


public:

  void testBasic( void )
  {
    Mutex m;

    {
      ScopedLock sl(m);
      TS_ASSERT_EQUALS ( m.tryLock(0), false );
    }
    TS_ASSERT_EQUALS ( m.tryLock(0), true );
  }

};

#include <cxxtest/TestSuite.h>

#include "ScopedLock.hpp"
#include "test_Common.hpp"


class TestScopedLock : public CxxTest::TestSuite
{


public:

  void testBasic( void )
  {
    TEST_HEADER;
    Mutex m;

    {
      ScopedLock sl(m);
      TS_ASSERT_EQUALS ( m.tryLock(0), EBUSY );
    }
    TS_ASSERT_EQUALS ( m.tryLock(0), 0 );
  }

};

#include <cxxtest/TestSuite.h>

#include <cpp_utils/ScopedLock.hpp>

#include "Fixture.hpp"


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

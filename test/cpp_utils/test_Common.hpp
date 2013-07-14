#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"
#include "Common.hpp"

#include <time.h> // timespec

class TestCommon : public CxxTest::TestSuite
{


public:

  void testCoverageIncrease( void )
  {

    // coverage increase of addTotimespec
    long int sec = 0;
    long int nsec = 1000000000 + 1;
    timespec ts = addTotimespec(sec, nsec);
  }

};

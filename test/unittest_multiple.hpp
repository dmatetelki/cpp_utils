#include <cxxtest/TestSuite.h>



class MyTestSuite : public CxxTest::TestSuite
{

public:

  void testAddition()
  {
    TS_ASSERT_EQUALS( 1 + 1, 2 );

  }

};

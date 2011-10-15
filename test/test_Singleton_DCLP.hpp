#include <cxxtest/TestSuite.h>

#define private public // need to reach Singleton's private m_instance

#include "Common.hpp"
#include "Fixture.hpp"
#include "Singleton_DCLP.hpp"


class TestSingletonDCLPSuite : public CxxTest::TestSuite
{

private:

  class BasicSingleton : public Singleton_DCLP<BasicSingleton>
  {
    public:
      int getSeven()
      {
        TRACE;
        return 7;
      }
  };

public:

  void testBasic( void )
  {
    TEST_HEADER;

    TS_ASSERT_EQUALS( BasicSingleton::getInstance()->getSeven(), 7 );
  }


};

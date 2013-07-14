#include <cxxtest/TestSuite.h>

#include <cpp_utils/Mutex.hpp>
#include <cpp_utils/ConditionVariable.hpp>


class TestConditionVariable : public CxxTest::TestSuite
{


public:

  void testBasic( void )
  {
    Mutex m;
    ConditionVariable c(m);
  }

};

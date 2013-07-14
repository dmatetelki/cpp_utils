#include <cxxtest/TestSuite.h>

#include "Mutex.hpp"
#include "ConditionVariable.hpp"


class TestConditionVariable : public CxxTest::TestSuite
{


public:

  void testBasic( void )
  {
    Mutex m;
    ConditionVariable c(m);
  }

};

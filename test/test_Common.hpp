#ifndef TEST_COMMON_HPP
#define TEST_COMMON_HPP

#include <cxxtest/TestSuite.h>
#include <cxxtest/GlobalFixture.h>

#include "Common.hpp"
#include <iostream>

class TestCommon : public CxxTest::GlobalFixture
{
  bool setUpWorld()
  {
    Logger::createInstance();
    Logger::init(std::cout);
    return true;
  }

  bool tearDownWorld()
  {
    Logger::destroy();
    return true;
  }

};

static TestCommon testCommon;

#define TEST_HEADER \
  MSG( std::string("\n+++ ").append(__PRETTY_FUNCTION__).append(" +++\n").c_str());


#endif // TEST_COMMON_HPP
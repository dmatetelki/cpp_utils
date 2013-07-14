#include <cxxtest/TestSuite.h>

#include <cpp_utils/Common.hpp>
#include "Fixture.hpp"


class TestLoggerSuite : public CxxTest::TestSuite
{

public:

  void testBasic()
  {
    TEST_HEADER;

    int ears(2);

    LOG_BEGIN(Logger::DEBUG)
      LOG_PROP("noses", 1)
      LOG_PROP("ears", ears)
    LOG_END("An avarege human");
  }

private:

  static void f()
  {
    TRACE_STATIC;

    LOG_STATIC(Logger::INFO, "From static function");

    int a(4);
    std::string b("eer");
    LOG_BEGIN(Logger::DEBUG)
      LOG_SPROP(a)
      LOG_SPROP(b)
    LOG_END_STATIC("Properties from a static function");
  }

public:

  void testStatic()
  {
    TEST_HEADER;

    f();
  }

private:

  class A {
  public:

    A(const char *msg) : message(msg)
    {
      TRACE;

      LOG_BEGIN(Logger::DEBUG)
        LOG_PROP("message", message)
        LOG_SPROP(message)
      LOG_END("my member");
    }
  private:

    std::string message;

  };

public:

  void testMember()
  {
    TEST_HEADER;

    A a("hello");
  }

  void testNoPrefix()
  {
    TEST_HEADER;

    LOG(Logger::DEBUG, "With prefix");

    Logger::usePrefix(false);
    LOG(Logger::DEBUG, "Without prefix");

    Logger::usePrefix();
    LOG(Logger::DEBUG, "With prefix again");
  }

};

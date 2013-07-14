#include <cxxtest/TestSuite.h>

#include "Common.hpp"
#include "Fixture.hpp"
#include "Multiton.hpp"


class TestMultitonSuite : public CxxTest::TestSuite
{

private:

    class Dummy
    {
    public:
        void sayHi() {  LOG( Logger::FINEST, "Hi there!"); }

    };

    class DummyMultiton : public Multiton<std::string, Dummy>
    {

    };

public:

  void testBasic( void )
  {
    TEST_HEADER;

    DummyMultiton::getRef("foobar").sayHi();
    DummyMultiton::getPtr("foobar")->sayHi();

    DummyMultiton::destroy();
  }


};

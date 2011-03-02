#include <cxxtest/TestSuite.h>

#define private public // need to reach Singleton's private m_instance

#include "Singleton.hpp"
#include "Common.hpp"

class TestSingletonSuite : public CxxTest::TestSuite
{

  class BasicSingleton : public Singleton<BasicSingleton>
  {
    public:
      int getSeven() { return 7; }
  };

public:

  void testBasic( void )
  {
    // no instance yet
    TS_ASSERT_EQUALS (BasicSingleton::getInstance(), (BasicSingleton*)0);

    BasicSingleton::createInstance();
    TS_ASSERT_DIFFERS (BasicSingleton::getInstance(), (BasicSingleton*)0);

    BasicSingleton* p1;
    BasicSingleton* p2;
    p1 = BasicSingleton::getInstance();
    p2 = BasicSingleton::getInstance();
    TS_ASSERT_EQUALS ( p1, p2 );

    TS_ASSERT_EQUALS( BasicSingleton::getInstance()->getSeven(), 7 );

    BasicSingleton::createInstance();
    p2 = BasicSingleton::getInstance();
    TS_ASSERT_EQUALS ( (void*)p1, (void*)p2 );

    BasicSingleton::destroy();
    TS_ASSERT_EQUALS (BasicSingleton::getInstance(), (BasicSingleton*)0);
  }

/** @note TestMe class has to be unittested, but it uses a singleton: EvilClass
 * Which takes way too much time to use in a unittest (it's an LDAP connection,
 * calculates for too long, etc)
 *
 * So we plan to destroy EvilClass and put a DummyClass (which is derived from
 * EvilClass and implements the expensice calls with dummy ones) to TestMe
 */

  class TestMe
  {
  public:
    int fv()
    {
      TRACE(this);
      return EvilClass::getInstance()->getValue();
    }
  };

  class EvilClass : public Singleton<EvilClass>
  {
  public:
    virtual int getValue(void)
    {
      TRACE(this);
      return 665;
    }
  };

  class DummyClass : public EvilClass
  {
  public:
    int getValue(void)
    {
      TRACE(this);
      return 13;
    }
  };

  void testAdvancedTest( void )
  {
    TestMe tm;

    EvilClass::createInstance();
    TS_ASSERT_EQUALS ( tm.fv(), 665 );

    EvilClass::destroy();
    EvilClass::m_instance =  dynamic_cast<EvilClass*>(new DummyClass());
    TS_ASSERT_EQUALS ( tm.fv(), 13 );

    EvilClass::destroy();
  }

};

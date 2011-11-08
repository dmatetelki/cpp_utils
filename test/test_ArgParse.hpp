#include <cxxtest/TestSuite.h>

#include "Common.hpp"
#include "Fixture.hpp"


#define private public
#include "ArgParse.hpp"

#include <string>

class TestArgParseSuite : public CxxTest::TestSuite
{


public:

  void testBasic( void )
  {
    TEST_HEADER;

    ArgParse argParse("intro", "outro");

    TS_ASSERT_EQUALS( argParse.usage(), std::string(
      "intro\n\n"
      "usage:  [OPTIONS]\n\n"
      "Options:\n"
      "-h, --help                    Prints this help message\n\n"
      "outro") );
  }

  void testfindKeyinArgMap ( void )
  {
    TEST_HEADER;

    ArgParse argParse("intro", "outro");

    argParse.addArgument("-s, --unix-rocket",
                        "Unix rocket",
                        ArgParse::STRING );

    TS_ASSERT_DIFFERS ( argParse.findKeyinArgMap("-s"), argParse.m_params.end() );
    TS_ASSERT_DIFFERS ( argParse.findKeyinArgMap("--unix-rocket"), argParse.m_params.end() );

    TS_ASSERT_EQUALS( argParse.findKeyinArgMap("-u"), argParse.m_params.end() );
    TS_ASSERT_EQUALS( argParse.findKeyinArgMap("-r"), argParse.m_params.end() );
  }

  void asdtestAddArgs( void )
  {
    TEST_HEADER;

    ArgParse argParse("intro", "outro");

    argParse.addArgument("-s, --unix-socket",
                        "Unix socket",
                        ArgParse::STRING );
    argParse.addArgument("-u, --user",
                        "Username",
                        ArgParse::STRING );

    int argc = 3;
    char *argv[] = { (char*)"test",
                     (char*)"-u",
                     (char*)"username" };

    argParse.parseArgs(argc, argv);

    TS_ASSERT_EQUALS( argParse.foundArg("-s, --unix-socket"), false );
    TS_ASSERT_EQUALS( argParse.foundArg("-u, --user"), true );
  }

};

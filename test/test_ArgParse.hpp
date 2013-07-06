#include <cxxtest/TestSuite.h>

#include "Common.hpp"
#include "Fixture.hpp"


#define private public
#include "ArgParse.hpp"

#include <string>
#include <stdexcept>

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

  void testAddArgs( void )
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
    const char *argv[] = {  "test",
                            "-u",
                            "username" };

    argParse.parseArgs(argc, argv);

    TS_ASSERT_EQUALS( argParse.foundArg("-s, --unix-socket"), false );
    TS_ASSERT_EQUALS( argParse.foundArg("-u, --user"), true );
  }

  void testBadAddArgs( void )
  {
    TEST_HEADER;

    ArgParse argParse("intro", "outro");

    TS_ASSERT_THROWS( argParse.addArgument("s", "no dash"),
                      std::logic_error);

    TS_ASSERT_THROWS( argParse.addArgument("-i", "bad range", ArgParse::INT,
      ArgParse::OPTIONAL, ArgParse::OPTIONAL, "", "12..forever"),
                     std::logic_error);

    TS_ASSERT_THROWS( argParse.addArgument("-i", "bad range", ArgParse::FLOAT,
      ArgParse::OPTIONAL, ArgParse::OPTIONAL, "", "12.34..forever"),
                     std::logic_error);
  }

  void testgetArgs( void )
  {
    TEST_HEADER;

    ArgParse argParse("intro", "outro");

    argParse.addArgument("-i", "int",    ArgParse::INT);
    argParse.addArgument("-f", "float",  ArgParse::FLOAT);
    argParse.addArgument("-n", "none",   ArgParse::NONE);
    argParse.addArgument("-s", "string", ArgParse::STRING);
    argParse.addArgument("-b", "bool",   ArgParse::BOOL);

    int argc = 10;
    const char *argv[] = {  "test",
                            "-i", "12",
                            "-f", "12.12",
                            "-n",
                            "-s", "forever",
                            "-b", "false" };

    argParse.parseArgs(argc, argv);

    int i;
    float f;
    std::string s;
    bool b;

    TS_ASSERT_EQUALS( argParse.argAsInt("-i", i), true );
    TS_ASSERT_EQUALS( argParse.argAsFloat("-f", f), true );
    TS_ASSERT_EQUALS( argParse.argAsString("-s", s), true );
    TS_ASSERT_EQUALS( argParse.argAsBool("-b", b), true );

    TS_ASSERT_EQUALS(i, 12 );
    TS_ASSERT_DELTA(f, 12.12, 0.01 );
    TS_ASSERT_EQUALS(s, std::string("forever") );
    TS_ASSERT_EQUALS(b, false );
  }
};

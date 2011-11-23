// g++ mysqlclient_main.cpp src/Logger.cpp src/MysqlClient.cpp  src/ArgParse.cpp -I./include -lmysqlclient


#include "Logger.hpp"
#include "Common.hpp"

#include "ArgParse.hpp"
#include "MysqlClient.hpp"
#include "MysqlConnectionPool.hpp"

#include <string>
#include <list>
#include <iostream>
#include <stdexcept>

void setUpArgs(ArgParse &argParse)
{
  TRACE_STATIC;

  argParse.addArgument("--host",
                       "Hostname/IP",
                       ArgParse::STRING,
                       ArgParse::REQUIRED );
  argParse.addArgument("-u, --user",
                       "Username",
                       ArgParse::STRING,
                       ArgParse::REQUIRED );
  argParse.addArgument("-db, --database",
                       "Database",
                       ArgParse::STRING,
                       ArgParse::REQUIRED );
  argParse.addArgument("-p, --password",
                       "Password",
                       ArgParse::STRING,
                       ArgParse::REQUIRED );

  argParse.addArgument("-n, --number-of-connections",
                       "Number of connections. Default is 5",
                       ArgParse::INT );
}


void getArgs( int argc, char* argv[],
              ArgParse &argParse,
              std::string &host,
              std::string &user,
              std::string &db,
              std::string &pass,
              int &numberOfConnections )
{
  TRACE_STATIC;

  argParse.parseArgs(argc, argv);

  argParse.argAsString("--host", host);
  argParse.argAsString("-u, --user", user);
  argParse.argAsString("-db, --database", db);
  argParse.argAsString("-p, --password", pass);

  argParse.argAsInt("-n, --number-of-connections", numberOfConnections);
}


bool checkArgs( int argc, char* argv[],
                ArgParse &argParse,
                std::string &host,
                std::string &user,
                std::string &db,
                std::string &pass,
                int &numberOfConnections )
{
  TRACE_STATIC;

  try {
    getArgs( argc, argv,
             argParse,
             host, user, db, pass,
             numberOfConnections );
  } catch (std::runtime_error e) {
    if ( argParse.foundArg("-h, --help") ) {
      std::cout << argParse.usage() << std::endl;
      return false;
    }
    std::cerr << e.what() << std::endl
              << "Check usage: " << argv[0] << " --help" << std::endl;
    return false;
  }

  if ( argParse.foundArg("-h, --help") ) {
    std::cout << argParse.usage() << std::endl;
    return false;
  }

  return true;
}


void printResults(std::list<std::string> &results)
{
  TRACE_STATIC;

  LOG ( Logger::DEBUG, std::string("Got query result number of rows: ").
                          append(TToStr(results.size())).c_str() );

  for (std::list<std::string>::const_iterator it = results.begin();
       it != results.end();
       ++it ) {
    LOG ( Logger::DEBUG, (*it).c_str() );
  }
}


int main(int argc, char* argv[] )
{
  Logger::createInstance();
  Logger::init(std::cout);
  Logger::setLogLevel(Logger::FINEST);


  // args
  ArgParse argParse("Simple MySQL client",
                    "Report bugs to: denes.matetelki@gmail.com");
  setUpArgs(argParse);

  std::string host, user, db, pass;
  int numberOfConnections(5);

  if ( !checkArgs(argc, argv, argParse,
                  host, user, db, pass, numberOfConnections ) )
    return 1;

  // init
  MysqlClient::init();
  MysqlConnectionPool cp (
                  argParse.foundArg("--host") ? host.c_str() : NULL,
                  argParse.foundArg("-u, --user") ? user.c_str() : NULL,
                  argParse.foundArg("-p, --password") ? pass.c_str() : NULL,
                  argParse.foundArg("-db, --database") ? db .c_str() : NULL );

  for ( int i = 0; i < numberOfConnections; ++i )
    cp.create();


  // work
  std::list<std::string> results;
  MysqlClient *c = cp.acquire();
  if ( !c->querty("SELECT * FROM seats", results) ) {
    LOG ( Logger::ERR, "Could not execute query." );
  } else {
    printResults(results);
  }
  cp.release(c);

  // end
  cp.clear();
  MysqlClient::destroy();
  Logger::destroy();
  return 0;
}

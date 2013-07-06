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


void getArgs( int argc, const char* argv[],
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


bool checkArgs( int argc, const char* argv[],
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

  LOG_BEGIN(Logger::DEBUG)
    LOG_PROP("Number of rows", results.size())
  LOG_END_STATIC("Got query results.");

  for (std::list<std::string>::const_iterator it = results.begin(); it != results.end(); ++it )
    LOG_STATIC ( Logger::DEBUG, (*it).c_str() );
}


int main(int argc, const char* argv[] )
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
  MysqlClient *c = cp.acquire();
  MYSQL_RES *result;
  std::string queryLine("SELECT * FROM seats");
  if ( !c->querty(queryLine.c_str(), queryLine.length(), &result) ) {
    LOG_STATIC ( Logger::ERR, "Could not execute query." );
  } else {
    std::list<std::string> resultList;
    c->queryResultToStringList(result, resultList);
    printResults(resultList);
    mysql_free_result(result);
  }
  cp.release(c);

  // end
  cp.clear();
  MysqlClient::destroy();
  Logger::destroy();
  return 0;
}

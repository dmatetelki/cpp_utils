// gpp mysqlclient_main.cpp ../src/Logger.cpp ../src/MysqlClient.cpp  ../src/ArgParse.cpp -I../include -lmysqlclient -o mysqlclient

// ./mysqlclient -u USER -db MYSQL_DB -p PASS

#include "Logger.hpp"
#include "Common.hpp"

#include "ArgParse.hpp"
#include "MysqlClient.hpp"
#include <mysql/errmsg.h>

#include <string>
#include <list>
#include <iostream>
#include <stdexcept>

void setUpArgs(ArgParse &argParse)
{
  TRACE_STATIC;

  argParse.addArgument("--host",
                       "Hostname/IP",
                       ArgParse::STRING );
  argParse.addArgument("-u, --user",
                       "Username",
                       ArgParse::STRING,
                       ArgParse::REQUIRED, ArgParse::REQUIRED );
  argParse.addArgument("-db, --database",
                       "Database",
                       ArgParse::STRING,
                       ArgParse::REQUIRED, ArgParse::REQUIRED );
  argParse.addArgument("-p, --password",
                       "Password",
                       ArgParse::STRING,
                       ArgParse::REQUIRED, ArgParse::REQUIRED );
  argParse.addArgument("-port",
                       "Port",
                       ArgParse::INT );
  argParse.addArgument("-s, --unix-socket",
                       "Unix socket",
                       ArgParse::STRING );
  argParse.addArgument("-f, --client-flags",
                       "Client flags",
                       ArgParse::INT );
  argParse.addArgument("-r, --max-reconnect",
                       "Maximum number of retries if connection is lost. "
                       "Default is 5.",
                       ArgParse::INT,
                       ArgParse::REQUIRED );
}


void getArgs( int argc, char* argv[],
              ArgParse &argParse,
              std::string &host,
              std::string &user,
              std::string &db,
              std::string &pass,
              std::string &unixsocket,
              int &port,
              int &clientflags,
              int &retry )
{
  TRACE_STATIC;

  argParse.parseArgs(argc, argv);

  argParse.argAsString("--host", host);
  argParse.argAsString("-u, --user", user);
  argParse.argAsString("-db, --database", db);
  argParse.argAsString("-p, --password", pass);
  argParse.argAsInt("-port", port);
  argParse.argAsString("-s, --unix-socket", unixsocket);
  argParse.argAsInt("-f, --client-flags", clientflags);
  argParse.argAsInt("-r, --max-reconnect", retry);
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

  ArgParse argParse("Simple MySQL client",
                    "Report bugs to: denes.matetelki@gmail.com");

  setUpArgs(argParse);

  std::string host, user, db, pass, unixsocket;
  int port, clientflags, retry;

  try {
    getArgs( argc, argv,
             argParse,
             host, user, db, pass, unixsocket,
             port, clientflags, retry );
  } catch (std::runtime_error e) {
    if ( argParse.foundArg("-h, --help") ) {
      std::cout << argParse.usage() << std::endl;
      return 1;
    }
    std::cerr << e.what() << std::endl
              << "Check usage: " << argv[0] << " --help" << std::endl;
    return 1;
  }


  if ( argParse.foundArg("-h, --help") ) {
    std::cout << argParse.usage() << std::endl;
    return 1;
  }

  init_client_errs();

  MysqlClient mysqlClient (
    argParse.foundArg("--host") ? host.c_str() : NULL,
    argParse.foundArg("-u, --user") ? user.c_str() : NULL,
    argParse.foundArg("-p, --password") ? pass.c_str() : NULL,
    argParse.foundArg("-db, --database") ? db .c_str() : NULL,
    argParse.foundArg("-port") ? port : 0,
    argParse.foundArg("-s, --unix-socket") ? unixsocket.c_str() : NULL,
    argParse.foundArg("-f, --client-flags") ? clientflags : 0,
    argParse.foundArg("-r, --max-reconnect") ? retry : 5 );

  if ( !mysqlClient.connect() && !mysqlClient.reconnect() ) {
    finish_client_errs();
    Logger::destroy();
    return 0;
  }

  std::string queryMsg("SELECT * FROM seats");
  MYSQL_RES *queryResult;
  if ( !mysqlClient.querty(queryMsg.c_str(), queryMsg.length(), &queryResult) ) {
    LOG( Logger::ERR, "Could not execute query." );
    if ( !mysqlClient.reconnect() ) {
      LOG( Logger::ERR, "Reconnect failed, exiting." );
      finish_client_errs();
      Logger::destroy();
      return 0;
    }

    LOG( Logger::ERR, "Trying query again." );
    if ( !mysqlClient.querty(queryMsg.c_str(), queryMsg.length(), &queryResult) ) {
      LOG( Logger::ERR, "Query failed again, exiting." );
      finish_client_errs();
      Logger::destroy();
      return 0;
    }
  }


  std::list<std::string> results;
  MysqlClient::queryResultToStringList(queryResult, results);
  printResults(results);

  mysql_free_result(queryResult);
  finish_client_errs();
  Logger::destroy();
  return 0;
}

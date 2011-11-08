// g++ mysqlclient_main.cpp src/Logger.cpp src/MysqlClient.cpp  src/ArgParse.cpp -I./include -lmysqlclient


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
  argParse.addArgument("--host",
                       "Hostname/IP",
                       ArgParse::STRING );
  argParse.addArgument("-u, --user",
                       "Username",
                       ArgParse::STRING );
  argParse.addArgument("-db, --database",
                       "Database",
                       ArgParse::STRING );
  argParse.addArgument("-p, --password",
                       "Password",
                       ArgParse::STRING );
  argParse.addArgument("-port",
                       "Port",
                       ArgParse::INT );
  argParse.addArgument("-s, --unix-socket",
                       "Unix socket",
                       ArgParse::STRING );
  argParse.addArgument("-f, --client-flags",
                       "Client flags",
                       ArgParse::INT );
}


bool getArgs( int argc, char* argv[],
              ArgParse &argParse,
              std::string &host,
              std::string &user,
              std::string &db,
              std::string &pass,
              std::string &unixsocket,
              int &port,
              int &clientflags )
{
  try {
    argParse.parseArgs(argc, argv);
  } catch (std::runtime_error e) {
    std::cerr << e.what() << std::endl << std::endl;
    return false;
  }

  argParse.argAsString("--host", host);
  argParse.argAsString("-u, --user", user);
  argParse.argAsString("-db, --database", db);
  argParse.argAsString("-p, --password", pass);
  argParse.argAsInt("-port", port);
  argParse.argAsString("-s, --unix-socket", unixsocket);
  argParse.argAsInt("-f, --client-flags", clientflags);

  return true;
}


void printResults(std::list<std::string> &results)
{
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
  int port, clientflags;
  if ( !getArgs(argc, argv,
                argParse,
                host, user, db, pass, unixsocket,
                port, clientflags ) ||
       argParse.foundArg("-h, --help") )
  {
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
    argParse.foundArg("-f, --client-flags") ? clientflags : 0 );

  std::list<std::string> results;
  if ( !mysqlClient.querty("SELECT * FROM seats", results) ) {
    LOG ( Logger::ERR, "Could not execute query." );
  } else {
    printResults(results);
  }

  finish_client_errs();

  Logger::destroy();
  return 0;
}

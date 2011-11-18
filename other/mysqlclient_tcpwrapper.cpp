// gpp  mysqlclient_tcpwrapper.cpp  -o mysqltcpwrapper  -I../include ../src/Logger.cpp  ../src/Thread.cpp -lpthread ../src/Socket.cpp   -std=c++0x  ../src/WorkerThread.cpp MysqlTask.cpp EchoMessage.cpp ../src/ScopedLock.cpp ../src/ThreadPool.cpp ../src/ConditionVariable.cpp ../src/Mutex.cpp ../src/ArgParse.cpp ../src/MysqlConnectionPool.cpp -lmysqlclient -lrt ../src/MysqlClient.cpp  -o mysqlclient

#include "Logger.hpp"
#include "Common.hpp"

#include "ArgParse.hpp"
#include "MysqlClient.hpp"
#include "MysqlConnectionPool.hpp"

#include "TcpServer.hpp"
#include "Message.hpp"

#include "ThreadPool.hpp"
#include "WorkerThread.hpp"

#include "EchoMessage.hpp"
#include "MysqlTask.hpp"

#include <mysql/errmsg.h>

#include <string>
#include <list>
#include <iostream>
#include <stdexcept>







void setUpArgs(ArgParse &argParse)
{
  TRACE_STATIC;

  argParse.addArgument("--host",
                       "MySQL server hostname/IP",
                       ArgParse::STRING,
                       ArgParse::REQUIRED,
                       ArgParse::REQUIRED);
  argParse.addArgument("-u, --user",
                       "MsSQL username",
                       ArgParse::STRING,
                       ArgParse::REQUIRED,
                       ArgParse::REQUIRED );
  argParse.addArgument("-db, --database",
                       "MySQL database",
                       ArgParse::STRING,
                       ArgParse::REQUIRED,
                       ArgParse::REQUIRED );
  argParse.addArgument("-p, --password",
                       "MySQL password",
                       ArgParse::STRING,
                       ArgParse::REQUIRED,
                       ArgParse::REQUIRED );
  argParse.addArgument("-n, --number-of-connections",
                       "MySQL connections in connection pool. Default is 5",
                       ArgParse::INT );

  argParse.addArgument("--port",
                       "Listening port. Default is 4455",
                       ArgParse::INT );
  argParse.addArgument("-cl, --clients",
                       "Maximum number of served clients. Default is 5.",
                       ArgParse::INT );
  argParse.addArgument("--pending",
                       "Maximum number of pending clients. Default is 5.",
                       ArgParse::INT );

  argParse.addArgument("-t, --worker-threads",
                       "Number of worker threads. Default is 5.",
                       ArgParse::INT );
}


void getArgs( int argc, char* argv[],
              ArgParse &argParse,
              std::string &host,
              std::string &user,
              std::string &db,
              std::string &pass,
              int &conns,
              int &port,
              int &clients,
              int &pending,
              int &threads
            )
{
  TRACE_STATIC;

  argParse.parseArgs(argc, argv);

  argParse.argAsString("--host", host);
  argParse.argAsString("-u, --user", user);
  argParse.argAsString("-db, --database", db);
  argParse.argAsString("-p, --password", pass);
  argParse.argAsInt("-n, --number-of-connections", conns);

  argParse.argAsInt("--port", port);
  argParse.argAsInt("-cl, --clients", clients);
  argParse.argAsInt("--pending", pending);

  argParse.argAsInt("-t, --worker-threads", threads);
}


bool checkArgs( int argc, char* argv[],
                ArgParse &argParse,
                std::string &host,
                std::string &user,
                std::string &db,
                std::string &pass,
                int &conns,
                int &port,
                int &clients,
                int &pending,
                int &threads
              )
{
  TRACE_STATIC;

  if ( argc == 1 || ( argc == 2 && argv[1][0] != '-' ) ) {
    std::cout << argParse.usage() << std::endl;
    return false;
  }

  try {
    getArgs( argc, argv,
             argParse,
             host, user, db, pass,
             conns, port, clients, pending, threads );
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
  ArgParse argParse("TCP server wrapper on a MySQL client",
                    "Report bugs to: denes.matetelki@gmail.com");
  setUpArgs(argParse);

  std::string host, user, db, pass;
  int conns(5), port(4455), clients(5), pending(5), threads(5);

  if ( !checkArgs(argc, argv, argParse,
                  host, user, db, pass,
                  conns, port, clients, pending, threads ) )
    return 1;

  /*

  // init MySQL connection pool
  init_client_errs();
  MysqlConnectionPool mysqlConnectionPool (
                  argParse.foundArg("--host") ? host.c_str() : NULL,
                  argParse.foundArg("-u, --user") ? user.c_str() : NULL,
                  argParse.foundArg("-p, --password") ? pass.c_str() : NULL,
                  argParse.foundArg("-db, --database") ? db .c_str() : NULL );

  for ( int i = 0; i < conns; ++i )
    mysqlConnectionPool.create();


  // threadpool
  ThreadPool threadPool;
  for ( int i = 0; i < threads; ++i )
    threadPool.pushWorkerThread(new WorkerThread(threadPool));

  threadPool.startWorkerThreads();

  // TCP server
  MsgParam msgParam(&mysqlConnectionPool, &threadPool);
  TcpServer<EchoMessage> tcpServer(std::string("127.0.0.1"),
                                   TToStr(port),
                                   &msgParam,
                                   clients,
                                   pending );

  if ( !tcpServer.start() ) {
    LOG( Logger::ERR, "Failed to start TCP server, exiting...");
    mysqlConnectionPool.clear();
    finish_client_errs();
    Logger::destroy();
    return 1;
  }

  // never reached
  sleep(1);
  tcpServer.stop();


  // end
  mysqlConnectionPool.clear();
  finish_client_errs();

  */
  Logger::destroy();
  return 0;
}

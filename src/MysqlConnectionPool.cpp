#include "MysqlConnectionPool.hpp"

#include "Logger.hpp"


MysqlConnectionPool::MysqlConnectionPool()
{
  TRACE;
}


MysqlConnectionPool::~MysqlConnectionPool()
{
  TRACE;
}


MysqlClient* MysqlConnectionPool::create( const char* host,
                                          const char* user,
                                          const char* passwd,
                                          const char* db,
                                          unsigned int port,
                                          const char* unix_socket,
                                          long unsigned int clientflag )
{
  TRACE;

  MysqlClient *client = new MysqlClient(host,
                                        user,
                                        passwd,
                                        db,
                                        port,
                                        unix_socket,
                                        clientflag);

  return client;
}


bool MysqlConnectionPool::reset(const MysqlClient* client)
{
  TRACE;

  // The MysqlClient is stateless

  return true;
}
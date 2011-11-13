#include "MysqlConnectionPool.hpp"

#include "Logger.hpp"


MysqlConnectionPool::MysqlConnectionPool( const char *host,
                                          const char *user,
                                          const char *passwd,
                                          const char *db )
  : m_host(host)
  , m_user(user)
  , m_passwd(passwd)
  , m_db(db)
{
  TRACE;
}

MysqlConnectionPool::~MysqlConnectionPool()
{
  TRACE;
}


void MysqlConnectionPool::create()
{
  TRACE;

  MysqlClient *client = new MysqlClient ( m_host, m_user, m_passwd, m_db );
  client->connect();
  add(client);
}

#include "MysqlClient.hpp"

#include <mysql/errmsg.h>

#include "Logger.hpp"
#include "Common.hpp"


MysqlClient::MysqlClient( const char *host,
                          const char *user,
                          const char *passwd,
                          const char *db,
                          const unsigned int port,
                          const char *unix_socket,
                          const unsigned long clientflag,
                          const int maxRetry
                        )
  : m_host(host)
  , m_user(user)
  , m_passwd(passwd)
  , m_db(db)
  , m_port(port)
  , m_unix_socket(unix_socket)
  , m_clientflag(clientflag)
  , m_maxRetry(maxRetry)
  , m_connected(false)
  , m_connection(0)
{
  TRACE;
}


MysqlClient::~MysqlClient()
{
  TRACE;
  mysql_close(m_connection);
}


bool
MysqlClient::connect()
{
  TRACE;
  m_connection = mysql_init(NULL);

  if ( m_connection == NULL ) {
    LOG ( Logger::ERR, std::string("MySQL Initialization Failed!").c_str());
    return false;
  }

  m_connection = mysql_real_connect(  m_connection,
                                      m_host,
                                      m_user,
                                      m_passwd,
                                      m_db,
                                      m_port,
                                      m_unix_socket,
                                      m_clientflag );

  if ( m_connection == NULL ) {
    LOG ( Logger::ERR, std::string("MySQL Connection Failed! ").
                        append(mysql_error(m_connection)).c_str());
    return false;
  }

  LOG ( Logger::INFO,
        std::string("MySQL client connected to ").append(m_host ? m_host : "localhost")
          .append(", on port: ").append(m_port ? TToStr(m_port) : "TCP/IP")
          .append(", as user: ").append(m_user ? m_user : "CURRENT_USER")
          .append(", with passwd: ").append(m_passwd ? m_passwd : "NULL")
          .append(", to DB: ").append(m_db ? m_db : "NULL")
          .append(", with unix_socket: ").append(m_unix_socket ? m_unix_socket : "NULL")
          .append(", and flags: ").append(TToStr(m_clientflag))
            .c_str() );

  m_connected = true;
  return true;
}


bool
MysqlClient::querty(const char* queryMsg,
                    const int queryMsgLen,
                    MYSQL_RES **result)
{
  TRACE;

  if ( !m_connected ) {
    LOG( Logger::ERR, "Not connected to MySQL server." );
    return false;
  }

  if ( mysql_real_query(m_connection, queryMsg, queryMsgLen) != 0 ) {
    LOG ( Logger::ERR, std::string("MySQL query failed! ").
                        append(mysql_error(m_connection)).c_str());

    return false;
  }

  MYSQL_RES *res_set;
  res_set = mysql_store_result(m_connection);

  if ( res_set == NULL ) {
    LOG ( Logger::ERR, std::string("MySQL store result failed! ").
                        append(mysql_error(m_connection)).c_str());
    return false;
  }

  *result = res_set;
  return true;
}


bool
MysqlClient::reconnect()
{
  TRACE;

  for (int i = 0; i < m_maxRetry; ++i ) {

    LOG( Logger::INFO, std::string("Reconnecting to MySQL server, ").
                        append(TToStr(i+1)).
                        append(" try of maximum: ").
                        append(TToStr(m_maxRetry)).c_str() );

    if ( connect() )
      return true;

    sleep(1);
  }

  LOG( Logger::ERR, "Maximum number of retries reached, giving up." );
  return false;
}


void
MysqlClient::queryResultToStringList(const MYSQL_RES *res_set,
                                     std::list<std::string> &result)
{
  TRACE_STATIC;

  unsigned int num_fields = mysql_num_fields(const_cast<MYSQL_RES *>(res_set));
  MYSQL_ROW row;

  while ((row = mysql_fetch_row(const_cast<MYSQL_RES *>(res_set))) != NULL) {
    std::string rowString;
    for( unsigned int i = 0; i < num_fields; ++i ) {
      rowString.append(row[i] ? row[i] : "NULL");
      rowString.append(",");
    }
    if (num_fields > 0)
      rowString.erase(rowString.end()-1);

    result.push_back(rowString);
  }
}

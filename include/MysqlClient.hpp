#ifndef MYSQL_CLIENT_HPP
#define MYSQL_CLIENT_HPP

#include <mysql/mysql.h>

#include <string>
#include <list>

class MysqlClient
{

public:

  /// @note Call init_client_errs() / finish_client_errs() before / after
  MysqlClient ( const char *host                = NULL,
                const char *user                = NULL,
                const char *passwd              = NULL,
                const char *db                  = NULL,
                const unsigned int port         = 0,
                const char *unix_socket         = NULL,
                const unsigned long clientflag  = 0,
                const int maxRetry              = 5
              );

  ~MysqlClient();

  /// @note clients side shall handle connect failure (reconnect)
  bool connect();

  /// @note clients side shall handle querty failure (reconnect)
  /// @note call mysql_free_result( result ) after
  bool querty(const char* queryMsg,
              const int queryMsgLen,
              MYSQL_RES **result);

  bool reconnect();

  /// @note Slow. Use only to log during debug
  /// @todo optimize this
  static void queryResultToStringList(const MYSQL_RES *res_set,
                                      std::list<std::string> &result);


private:

  MysqlClient(const MysqlClient&);
  MysqlClient& operator=(const MysqlClient&);


  const char     *m_host;
  const char     *m_user;
  const char     *m_passwd;
  const char     *m_db;
  unsigned int    m_port;
  const char     *m_unix_socket;
  unsigned long   m_clientflag;
  int             m_maxRetry;

  bool            m_connected;
  MYSQL          *m_connection;
};


#endif // MYSQL_CLIENT_HPP

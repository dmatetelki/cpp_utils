#ifndef MYSQL_CLIENT_HPP
#define MYSQL_CLIENT_HPP

#include <mysql/mysql.h>

#include <string>
#include <list>

class MysqlClient
{

public:

  /**
   * For more details about the params,
   * check the documentation of mysql_real_connect
   *
   * @note Call init_client_errs() / finish_client_errs() before / after.
   *
   * @param host May be either a host name or an IP address.
   * If host is NULL or the string "localhost",
   * a connection to the local host is assumed.
   * @param user If user is NULL or the empty string "",
   * the current user is assumed.
   * @param passwd If passwd is NULL, only entries in the user table
   * for the user that have a blank (empty) password field
   * are checked for a match.
   * @param db If db is not NULL, the connection sets the default
   * database to this value.
   * @param port If port is not 0, the value is used as the port number
   * for the TCP/IP connection. Note that the host parameter determines
   * the type of the connection.
   * @param unix_socket If unix_socket is not NULL, the string specifies
   * the socket or named pipe that should be used. Note that the host parameter
   * determines the type of the connection.
   * @param clientflag Usually 0, but can be set to a combination
   * of flags to enable certain features.
   */
  MysqlClient ( const char *host         = NULL,
                const char *user         = NULL,
                const char *passwd       = NULL,
                const char *db           = NULL,
                unsigned int port        = 0,
                const char *unix_socket  = NULL,
                unsigned long clientflag = 0 );

  ~MysqlClient();

  bool connect();

  bool querty(const std::string queryLine,
              std::list<std::string> &result);

private:

  MysqlClient(const MysqlClient&);
  MysqlClient& operator=(const MysqlClient&);

  /// @todo optimize this
  void queryResultToStringList(MYSQL_RES *res_set,
                               std::list<std::string> &result);

  const char     *m_host;
  const char     *m_user;
  const char     *m_passwd;
  const char     *m_db;
  unsigned int    m_port;
  const char     *m_unix_socket;
  unsigned long   m_clientflag;

  bool            m_connected;
  MYSQL          *m_connection;
};


#endif // MYSQL_CLIENT_HPP

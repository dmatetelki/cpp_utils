#ifndef MYSQL_CONNECTION_POOL_HPP
#define MYSQL_CONNECTION_POOL_HPP

#include "ObjectPool.hpp"
#include "MysqlClient.hpp"


class MysqlConnectionPool : public ObjectPool<MysqlClient *>
{
public:

  MysqlConnectionPool();
  ~MysqlConnectionPool();

  MysqlClient* create( const char *host         = NULL,
                       const char *user         = NULL,
                       const char *passwd       = NULL,
                       const char *db           = NULL,
                       unsigned int port        = 0,
                       const char *unix_socket  = NULL,
                       unsigned long clientflag = 0 );

  bool reset(const MysqlClient* client);
};


#endif // MYSQL_CONNECTION_POOL_HPP

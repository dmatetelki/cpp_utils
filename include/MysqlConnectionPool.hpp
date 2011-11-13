#ifndef MYSQL_CONNECTION_POOL_HPP
#define MYSQL_CONNECTION_POOL_HPP

#include "ObjectPool.hpp"
#include "MysqlClient.hpp"


class MysqlConnectionPool : public ObjectPool<MysqlClient *>
{
public:

  MysqlConnectionPool( const char *host         = NULL,
                       const char *user         = NULL,
                       const char *passwd       = NULL,
                       const char *db           = NULL );
  ~MysqlConnectionPool();

  void create();

  /// @note Shall this be a specialized ObjectPool::clear?
  void clear();

private:

  MysqlConnectionPool(const MysqlConnectionPool&);
  MysqlConnectionPool& operator=(const MysqlConnectionPool&);

  const char *m_host;
  const char *m_user;
  const char *m_passwd;
  const char *m_db;

};


#endif // MYSQL_CONNECTION_POOL_HPP

#ifndef MYSQL_TASK_HPP
#define MYSQL_TASK_HPP

#include "Task.hpp"
#include "MysqlConnectionPool.hpp"
#include "Connection.hpp"
#include "EchoMessage.hpp"


class MysqlTask : public Task
{

public:

  MysqlTask( MysqlConnectionPool *cp,
             Connection<EchoMessage> *connection,
             const std::string message );

  void run();

private:

  MysqlTask(const MysqlTask&);
  MysqlTask& operator=(const MysqlTask&);

  MysqlConnectionPool       *m_connectionPool;
  Connection<EchoMessage>   *m_connection;
  std::string                m_message;

};

#endif // MYSQL_TASK_HPP

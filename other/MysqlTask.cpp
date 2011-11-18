#include "MysqlTask.hpp"

#include <mysql/mysql.h>


MysqlTask::MysqlTask( MysqlConnectionPool *cp,
                      Connection<EchoMessage> *connection,
                      const std::string message )
  : m_connectionPool(cp)
  , m_connection(connection)
  , m_message(message)
{
  TRACE;
}

void MysqlTask::run()
{
  TRACE;
  LOG( Logger::FINEST, std::string("I'm a task, provessing message: \"").
                        append(m_message).append("\"").c_str() );

  MYSQL_RES *res_set(0);
  MysqlClient *c = m_connectionPool->acquire();
  if ( !c->querty(m_message.c_str(), m_message.length(), &res_set) ) {

    std::string errorMsg("Could not execute query.");
    LOG ( Logger::ERR, errorMsg.c_str() );
     m_connection->send(errorMsg.c_str(), errorMsg.length() );
  } else {

    std::list<std::string> results;
    MysqlClient::queryResultToStringList(res_set, results);

    std::string joinedLines;
    std::list<std::string>::const_iterator it;
    for(it = results.begin(); it != results.end(); ++it)
      joinedLines.append(*it).append(";");

    m_connection->send(joinedLines.c_str(), joinedLines.length() );
  }
  m_connectionPool->release(c);
}

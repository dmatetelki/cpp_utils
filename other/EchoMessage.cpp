#include "EchoMessage.hpp"

#include "Logger.hpp"

#include "MysqlTask.hpp"


EchoMessage::EchoMessage( Connection<EchoMessage>  *connection,
                          void                     *msgParam )
  : Message<EchoMessage>(connection, msgParam)
{
  TRACE;
}

bool EchoMessage::buildMessage( const void   *msgPart,
                                const size_t  msgLen )
{
  TRACE;
  m_buffer = std::string( (const char*) msgPart, msgLen );
  onMessageReady();
  return true;
}

void EchoMessage::onMessageReady()
{
  TRACE;

  LOG( Logger::INFO, std::string("Got message: \"").
                      append(m_buffer).append("\" from: ").
                      append(m_connection->getHost().append(":").
                      append(m_connection->getPort()) ).c_str() );


  MsgParam *msgParam = static_cast<MsgParam*>(m_param);
  msgParam->m_tp->pushTask(new MysqlTask( msgParam->m_cp,
                                          m_connection,
                                          m_buffer ));
}


size_t EchoMessage::getExpectedLength()
{
  TRACE;
  return 0;
}

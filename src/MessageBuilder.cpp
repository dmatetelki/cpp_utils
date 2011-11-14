#include "MessageBuilder.hpp"

#include "Logger.hpp"

#include <string.h> // memcpy



MessageBuilder::MessageBuilder( TcpConnection *connection,
                                const int bufferLength )
  : m_connection(connection)
  , m_buffer(0)
  , m_bufferLength(bufferLength)
  , m_bufferUsed(0)
{
  TRACE;

  m_buffer = new unsigned char[bufferLength];
}


MessageBuilder::~MessageBuilder()
{
  TRACE;

  delete [] m_buffer ;
}


bool MessageBuilder::buildMessage( const unsigned char *message,
                                   const int length )
{
  TRACE;

  /// @todo implement composing the message
  memcpy(message, m_buffer, length );
  m_bufferUsed = length;

  m_connection->onMessageReady ( m_buffer, m_bufferUsed );
  return true;
}

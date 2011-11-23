#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Logger.hpp"


#include <string>
#include <stddef.h> // size_t

  /** Append msgParts with buildMessage() to m_buffer.
   *  Call onMessageReady() if the length of the buffer equals the value from
   * getExpectedLength().
   */

class SocketConnection;


class Message
{
public:

  Message( SocketConnection *connection,
           void             *msgParam = 0 )
    : m_connection(connection)
    , m_param(msgParam)
    , m_buffer()
  {
    TRACE;
  };

  Message( void *msgParam = 0 )
    : m_connection(0)
    , m_param(msgParam)
    , m_buffer()
  {
    TRACE;
  };

  virtual ~Message() {};
  virtual Message* clone() = 0;

  virtual bool buildMessage( const void   *msgPart,
                             const size_t  msgLen ) = 0;
  virtual void onMessageReady() = 0;

  void setConnection(SocketConnection* conn )
  {
    TRACE;
    m_connection = conn;
  }

protected:

  virtual size_t getExpectedLength() = 0;


  SocketConnection   *m_connection;
  void               *m_param;
  std::string         m_buffer;

private:

  Message(const Message &);
  Message& operator=(const Message &);
};


#endif // MESSAGE_HPP

#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include "Connection.hpp"

#include <string>
#include <stddef.h> // size_t

  /** Append msgParts with buildMessage() to m_buffer.
   *  Call onMessageReady() if the length of the buffer equals the value from
   * getExpectedLength().
   */

template <typename T>
class Message
{
public:

  Message( Connection<T>  *connection,
           void           *msgParam = 0 )
    : m_connection(connection)
    , m_param(msgParam)
    , m_buffer()
  {};

  virtual ~Message() {};

  virtual bool buildMessage( const void   *msgPart,
                             const size_t  msgLen ) = 0;
  virtual void onMessageReady() = 0;

protected:

  virtual size_t getExpectedLength() = 0;


  Connection<T>  *m_connection;
  void           *m_param;
  std::string     m_buffer;

private:

  Message<T>(const Message<T> &);
  Message<T>& operator=(const Message<T> &);
};


#endif // MESSAGE_HPP

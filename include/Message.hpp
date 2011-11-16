#ifndef MESSAGE_HPP
#define MESSAGE_HPP

#include <string>
#include <stddef.h> // size_t

  /** Append msgParts with buildMessage() to m_buffer.
   *  Call onMessageReady() if the length of the buffer equals the value from
   * getExpectedLength().
   */

class Message
{
public:

  Message( void * msgParam = 0 )
    : m_buffer()
    , m_param(msgParam) {};

  virtual ~Message() {};

  virtual bool buildMessage( const void   *msgPart,
                             const size_t  msgLen ) = 0;
  virtual void onMessageReady() = 0;

protected:

  virtual size_t getExpectedLength() = 0;

  /// @todo shall i use dinamic array?
  std::string   m_buffer;
  void         *m_param;

};


#endif // MESSAGE_HPP

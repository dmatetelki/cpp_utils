#ifndef ECHO_MESSAGE_HPP
#define ECHO_MESSAGE_HPP

#include "Message.hpp"
#include "Connection.hpp"
#include "ThreadPool.hpp"
#include "MysqlConnectionPool.hpp"

struct MsgParam
{
    MysqlConnectionPool *m_cp;
    ThreadPool          *m_tp;
    MsgParam(MysqlConnectionPool *cp, ThreadPool *tp) : m_cp(cp), m_tp(tp) {};
};


class EchoMessage : public Message<EchoMessage>
{
public:

  EchoMessage( Connection<EchoMessage>  *connection,
               void                     *msgParam = 0);

  bool buildMessage( const void   *msgPart,
                     const size_t  msgLen );

  void onMessageReady();

protected:

  size_t getExpectedLength();

};

#endif // ECHO_MESSAGE_HPP

#ifndef MESSAGE_BUILDER_HPP
#define MESSAGE_BUILDER_HPP

class TcpConnection;


class MessageBuilder
{
public:

  MessageBuilder( TcpConnection *connection,
                  const int bufferLength = 1024 );

  virtual ~MessageBuilder();

  bool buildMessage( const unsigned char* message,
                     const int length ) ;


private:

  MessageBuilder(const MessageBuilder&);
  MessageBuilder& operator=(const MessageBuilder&);


  TcpConnection  *m_connection;
  unsigned char  *m_buffer;
  int             m_bufferLength;
  int             m_bufferUsed;
};

#endif // MESSAGE_BUILDER_HPP

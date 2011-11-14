#ifndef MESSAGE_BUILDER_HPP
#define MESSAGE_BUILDER_HPP

class MessageBuilder
{
public:

  bool buildMessage( const unsigned char* message, unsigned int length ) ;

protected:

  MessageBuilder( const int bufferLength ) ;
  virtual ~MessageBuilder() ;
  virtual void onMessageReady( const unsigned char* message, unsigned int length ) = 0 ;

private:

  unsigned char *m_buffer;
  int            m_bufferLength;
  int            m_bufferUsed;
};

#endif // MESSAGE_BUILDER_HPP

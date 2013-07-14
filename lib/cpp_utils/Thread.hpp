#ifndef THREAD_HPP
#define THREAD_HPP

#include <pthread.h>


class Thread
{
public:

  Thread();
  virtual ~Thread();

  void start();
  void* join() const;
  virtual void stop();
  void sendSignal( const int nSignal ) const;
  bool isRunning() const;

protected:

  volatile bool m_isRunning;

private:

  virtual void* run() = 0;
  static void* threadStarter( void* pData );

  pthread_t m_threadHandler;

};


#endif // THREAD_HPP

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
  void stop();
  void sendSignal( const int nSignal ) const;

private:

  virtual void* run() = 0;
  static void* threadStarter( void* pData );

protected:

  bool m_isRunning;

private:

  pthread_t m_threadHandler;
};


#endif // THREAD_HPP

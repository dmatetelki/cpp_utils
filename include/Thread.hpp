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
      void sendSignal( const int nSignal ) const;

  private:

      virtual void* run() = 0;
      static void* threadStarter( void* pData );

  private:

      pthread_t m_nThread;

};


#endif // THREAD_HPP

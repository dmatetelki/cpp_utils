#include <iostream>
#include <stdexcept>

#include <pthread.h>
pthread_mutex_t p_mutex;

#include <boost/thread/mutex.hpp>
boost::mutex b_mutex;

#define TRACE std::cout << __FILE__ << " @ " << __PRETTY_FUNCTION__ << ":" <<  __LINE__ << std::endl;


class ScopedLock
{

public:

  ScopedLock(pthread_mutex_t const& mutex) : m_mutex(mutex)
  {
    TRACE;
    pthread_mutex_lock( &m_mutex );
  }


  ~ScopedLock(void )
  {
   TRACE;
    pthread_mutex_unlock( &m_mutex );
  }

private:

  pthread_mutex_t m_mutex;

};


void fv ()
{
  ScopedLock sl(p_mutex);

  throw std::logic_error("p_thread stuff");
}


void fv2 ()
{
  boost::mutex::scoped_lock lock(b_mutex);

  throw std::logic_error("boost_thread stuff");
}


int main()
{
  pthread_mutex_init( &p_mutex, NULL );
  try {
    fv();
  } catch (...) {
    TRACE;

    if (pthread_mutex_trylock( &p_mutex) == 0 ) {
      std::cout << "pthread mutex is OK, unlocked " << std::endl;
      pthread_mutex_unlock( &p_mutex );
    } else {
      std::cout << "pthread mutex is STILL LOCKED!" << std::endl;
    }
  }
  pthread_mutex_destroy( &p_mutex );


  try {
    fv2();
  } catch (...) {
    TRACE;
    if ( b_mutex.try_lock() == true ) {
      std::cout << "boost mutex is OK, unlocked " << std::endl;
      b_mutex.unlock();
    } else {
      std::cout << "boost mutex is STILL LOCKED!" << std::endl;
    }
  }

  return 0;
}

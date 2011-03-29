#ifndef MUTEX_HPP
#define MUTEX_HPP

#include <pthread.h>

class Mutex 
{

public:

  Mutex(int kind = PTHREAD_MUTEX_DEFAULT);
  ~Mutex();

  /**
   * Locks mutex. If already locked, the calling thread shall block until
   * the mutex becomes available.
   * @returns If successful, zero; otherwise, an error number.
   */
  int lock();

  /**
   *
   */
  int unlock();

  /**
   * If currently locked, the call shall return immediately.
   * @returns Zero if a lock acquired. Otherwise, an error number.
   */
  int tryLock(const long int intervalSec = 0);

  pthread_mutex_t* getPThreadMutex();


private:

  Mutex(const Mutex& m);
  Mutex& operator=(const Mutex& m);

  pthread_mutex_t m_mutex;

};

#endif // MUTEX_HPP


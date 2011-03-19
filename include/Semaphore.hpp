#ifndef SEMAPHORE_HPP
#define SEMAPHORE_HPP

#include "Mutex.hpp"
#include "ConditionVariable.hpp"


class Semaphore
{

public:

  Semaphore( int maxCount = 1 );
  ~Semaphore( void );

  bool lock( int interval = 0 );
  bool unLock( void );
  int getCount( void ) const;

private:

  int m_maxCount;
  int m_count;
  mutable Mutex m_mutex;
  ConditionVariable m_condVar;
};

#endif // SEMAPHORE_HPP

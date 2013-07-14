#ifndef SCOPEDLOCK_HPP
#define SCOPEDLOCK_HPP

#include "Mutex.hpp"


class ScopedLock
{

public:

  ScopedLock( Mutex& mutex );
  ~ScopedLock();

private:

  ScopedLock(const ScopedLock&);
  ScopedLock& operator=(const ScopedLock&);

  Mutex& m_mutex;

};


#endif // SCOPEDLOCK_HPP

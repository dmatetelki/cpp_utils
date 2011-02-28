#ifndef SCOPEDLOCK_HPP
#define SCOPEDLOCK_HPP

#include "Mutex.hpp"


class ScopedLock
{

public:

  ScopedLock( const Mutex* const mutex );
  ~ScopedLock();

private:

  ScopedLock(const ScopedLock&);
  ScopedLock& operator=(const ScopedLock&);

  const Mutex* const m_mutex;

};


#endif // SCOPEDLOCK_HPP

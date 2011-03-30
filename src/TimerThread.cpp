#include "TimerThread.hpp"

#include "Common.hpp"
#include "ScopedLock.hpp"

#include <errno.h> // ETIMEDOUT



TimerThread::TimerThread()
  : m_mutex()
  , m_condVar(m_mutex)
  , m_users()
{
  TRACE;
}


TimerThread::~TimerThread()
{
  TRACE;
}


void TimerThread::addTimerUser(TimerUser* user,
                               const time_t expiration,
                               const time_t periodTime)
{
  TRACE;
  ScopedLock sl( m_mutex );
  if ( not m_isRunning ) return;
  UserEntry userEntry = { periodTime, user };
  m_users.insert( std::pair<time_t, UserEntry>( expiration, userEntry ) );
  m_condVar.signal();
}



bool TimerThread::removeTimerUser ( void* timerUser )
{
  TRACE;
  ScopedLock sl( m_mutex );
  if ( not m_isRunning ) return false;
  std::multimap<time_t, UserEntry>::iterator it, tmp;
  bool found(false);
  for ( it = m_users.begin(); it != m_users.end(); ) {
    tmp = it++;

    /// @todo solveth e abstract pointer problem
    if ( (void*)(it->second.user) == (void*)timerUser ) {
      m_users.erase(tmp);
      m_condVar.signal();
      found = true;  // one usercan be registered multiple times
    }
  }
  return found;
}

// override to signal as well
void TimerThread::stop()
{
  TRACE;
  ScopedLock sl( m_mutex );
  m_isRunning = false;
  m_condVar.signal();
}


void* TimerThread::run( void )
{
  TRACE;
  time_t nextExpiration;

  std::multimap<time_t, UserEntry> tmp;
  std::multimap<time_t, UserEntry>::iterator it;
  std::pair<std::multimap<time_t, UserEntry>::iterator,
            std::multimap<time_t, UserEntry>::iterator> ret;

  while( m_isRunning ) {

    m_mutex.lock();
    while ( m_users.empty() and m_isRunning ) {
      m_condVar.wait();
    }
    m_mutex.unlock();

    if ( not m_isRunning) return 0;
    nextExpiration = m_users.begin()->first;

    m_mutex.lock();
    // timer deleted / added, get nextExpiration again
    if ( m_condVar.wait( nextExpiration ) != ETIMEDOUT ) {
      continue;
    }
    m_mutex.unlock();

    // notify & remove
    m_mutex.lock();
    ret = m_users.equal_range( nextExpiration );

    /// @todo modify key values in multimap, must be a better way
    tmp.clear();
    for ( it = ret.first; it != ret.second; it++ ) {
      it->second.user->timerExpired();
      if ( it->second.periodTime ) tmp.insert(std::pair<time_t, UserEntry>(
                  it->second.periodTime, it->second ) );
    }
    m_users.erase( nextExpiration );
    m_users.insert( tmp.begin(), tmp.end() );
    m_mutex.unlock();

  }

  if ( not m_users.empty() ) {
    for ( it = m_users.begin(); it != m_users.end(); it++ ) {
      it->second.user->timerDestroyed();
    }
  }
  return 0;
}

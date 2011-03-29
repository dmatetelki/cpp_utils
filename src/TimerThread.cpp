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
  UserEntry userEntry = { periodTime, user };
  m_users.insert( std::pair<time_t, UserEntry>( expiration, userEntry ) );
  m_condVar.signal();
}



bool TimerThread::removeTimerUser ( UserEntry userEntry )
{
  TRACE;
  ScopedLock sl( m_mutex );
  std::multimap<time_t, UserEntry>::iterator it;
  bool found(false);
  for ( it = m_users.begin(); it != m_users.end(); it++ ) {
    if ( it->second.user == userEntry.user ) {
      m_users.erase(it);
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

    while ( m_users.empty() and m_isRunning ) {
      m_condVar.wait();
    }

    if ( not m_isRunning) {
      LOG( Logger::FINEST, "return empty handed");
      return 0;
    }

    nextExpiration = m_users.begin()->first;

    // timer deleted / added, get nextExpiration again
    if ( m_condVar.wait( nextExpiration ) != ETIMEDOUT ) {
      continue;
    }

    // notify & remove
    /// @todo lock here?
    //     m_mutex.lock();
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
    //     m_mutex.unlock();

  }

  if ( not m_users.empty() ) {
    LOG( Logger::FINEST, "return full handed");
    for ( it = m_users.begin(); it != m_users.end(); it++ ) {
      it->second.user->timerDestroyed();
    }
  }
  return 0;
}

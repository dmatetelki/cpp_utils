#include "TimerThreadMultimap.hpp"

#include "Common.hpp"
#include "ScopedLock.hpp"

#include <errno.h> // ETIMEDOUT



TimerThreadMultimap::TimerThreadMultimap()
  : Thread()
  , m_mutex()
  , m_condVar( m_mutex )
  , m_users()
{
  TRACE;
}


TimerThreadMultimap::~TimerThreadMultimap()
{
  TRACE;
}


void TimerThreadMultimap::addTimerUser( TimerUser* user,
                                const time_t expiration,
                                const time_t periodTime )
{
  TRACE;

  timespec periodTimeTS = { periodTime, 0 };
  timespec expirationTS = { expiration, 0 };

  addTimerUser( user, expirationTS, periodTimeTS );
}


void TimerThreadMultimap::addTimerUser( TimerUser* user,
                                const timespec expiration,
                                const timespec periodTime )
{
  TRACE;
  ScopedLock sl( m_mutex );
  if ( not m_isRunning ) return;

  timespec ts;
  clock_gettime( CLOCK_REALTIME, &ts );
  ts = timespecAdd( ts, expiration );

  UserEntry userEntry = { periodTime, user };
  m_users.insert( std::pair<timespec, UserEntry>( ts, userEntry ) );

  m_condVar.signal();
}


bool TimerThreadMultimap::removeTimerUser( TimerUser* timerUser )
{
  TRACE;
  ScopedLock sl( m_mutex );
  if ( not m_isRunning ) return false;

  std::multimap<timespec, UserEntry>::iterator it, tmp;
  bool found( false );
  for ( it = m_users.begin(); it != m_users.end(); ) {

    if ( it->second.user == timerUser ) {
      tmp = it++;
      m_users.erase( tmp );
      m_condVar.signal();
      found = true;  // one user can be registered multiple times
    } else {
      it++;
    }
  }
  return found;
}


void TimerThreadMultimap::stop()
{
  TRACE;
  ScopedLock sl( m_mutex );
  m_isRunning = false;
  m_condVar.broadcast();
}


void TimerThreadMultimap::notifyAndRemove( const timespec t )
{
  TRACE;
  ScopedLock sl( m_mutex );
  if ( not m_isRunning ) return;

  timespec ts;
  std::multimap<timespec, UserEntry, timespec_cmp> tmp;
  std::multimap<timespec, UserEntry>::iterator it;
  std::pair<std::multimap<timespec, UserEntry>::iterator,
            std::multimap<timespec, UserEntry>::iterator> ret;

  ret = m_users.equal_range( t );

  /// @todo modify key values in multimap, must be a better way
  tmp.clear();
  for ( it = ret.first; it != ret.second; it++ ) {
    it->second.user->timerExpired();
    if ( it->second.periodTime.tv_sec != 0 or it->second.periodTime.tv_nsec != 0) {

      clock_gettime( CLOCK_REALTIME, &ts );
      ts = timespecAdd( ts, it->second.periodTime );

      tmp.insert(std::pair<timespec, UserEntry>( ts, it->second ) );
      m_condVar.signal();
    }
  }
  m_users.erase( t );
  m_users.insert( tmp.begin(), tmp.end() );
}


void* TimerThreadMultimap::run( void )
{
  TRACE;
  timespec nextExpiration;
  timespec ts, wait;

  while( m_isRunning ) {

    m_mutex.lock();
    while ( m_users.empty() and m_isRunning ) {
      m_condVar.wait();
    }
    m_mutex.unlock();

    if ( not m_isRunning ) return 0;

    nextExpiration = m_users.begin()->first;
    clock_gettime( CLOCK_REALTIME, &ts );

    wait = timespecSubstract ( nextExpiration, ts );
    if ( wait.tv_sec == 0 and wait.tv_nsec == 0 ) {
      notifyAndRemove ( nextExpiration );
      continue;
    }
    m_mutex.lock();

    if ( m_condVar.wait( wait.tv_sec,
                         wait.tv_nsec ) != ETIMEDOUT ) {
      m_mutex.unlock();
      continue;
    }
    m_mutex.unlock();

    notifyAndRemove( nextExpiration );
  }

  // end ...
  if ( not m_users.empty() ) {
    std::multimap<timespec, UserEntry>::iterator it;
    for ( it = m_users.begin(); it != m_users.end(); it++ ) {
      it->second.user->timerDestroyed();
    }
  }
  return 0;
}

#ifndef TIMERTHREAD_HPP
#define TIMERTHREAD_HPP


#include "Thread.hpp"
#include "Mutex.hpp"
#include "ConditionVariable.hpp"

#include <map>
#include <time.h> // timespec

class TimerUser
{

public:

  virtual void timerExpired( void ) = 0;
  virtual void timerDestroyed( void ) = 0;

  virtual ~TimerUser() {}

}; // class TimerUser



class TimerThread : public Thread
{

private:

  typedef struct {
    timespec periodTime;
    TimerUser* user;
  } UserEntry;


public:

    TimerThread();

    virtual ~TimerThread();

    void addTimerUser( TimerUser* user,
                       const time_t expiration,
                       const time_t periodTime = 0 );

    void addTimerUser( TimerUser* user,
                       const timespec expiration,
                       const timespec periodTime = timespec_ctor() );

    bool removeTimerUser( TimerUser* timerUser );

    // override to signal as well
    void stop();


private:

  void notifyAndRemove( const timespec t );

  void* run( void );

  // ctor function
  inline static timespec timespec_ctor() {
    timespec tmp = { 0, 0 };
    return tmp;
  };

  // compare class for the multimap
  class timespec_cmp {
    public :
      bool operator()(const timespec& a, const timespec& b) const {
        if ( a.tv_sec < b.tv_sec ) return true;
        if ( a.tv_sec > b.tv_sec ) return false;
        return a.tv_nsec < b.tv_nsec;
      }
  };


  Mutex m_mutex;
  ConditionVariable m_condVar;
  std::multimap< timespec, UserEntry, timespec_cmp> m_users;

}; // class TimerThread


#endif // TIMERTHREAD_HPP

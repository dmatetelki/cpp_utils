#ifndef TIMERTHREAD_MULTIMAP_HPP
#define TIMERTHREAD_MULTIMAP_HPP


#include "Thread.hpp"
#include "Mutex.hpp"
#include "ConditionVariable.hpp"

#include <map>
#include <time.h> // timespec

#include "Timer.hpp" // TimerUser

// class TimerUser
// {
// 
// public:
// 
//   virtual void timerExpired() = 0;
//   virtual void timerDestroyed() = 0;
// 
//   virtual ~TimerUser() {}
// 
// }; // class TimerUser



class TimerThreadMultimap : public Thread
{

private:

  typedef struct {
    timespec periodTime;
    TimerUser* user;
  } UserEntry;


public:

    TimerThreadMultimap();

    virtual ~TimerThreadMultimap();

    void addTimerUser( TimerUser* user,
                       const time_t expiration,
                       const time_t periodTime = 0 );

    void addTimerUser( TimerUser* user,
                       const timespec expiration,
                       const timespec periodTime = timespec_init() );

    bool removeTimerUser( TimerUser* timerUser );

    // override to signal as well
    void stop();


private:

  void notifyAndRemove( const timespec t );

  void* run();

  // init function
  inline static timespec timespec_init() {
    timespec tmp = { 0, 0 };
    return tmp;
  };

  // compare class for the multimap
  class timespec_cmp {
    public :
      bool operator()( const timespec& a, const timespec& b ) const {
        if ( a.tv_sec < b.tv_sec ) return true;
        if ( a.tv_sec > b.tv_sec ) return false;
        return a.tv_nsec < b.tv_nsec;
      }
  };


  Mutex m_mutex;
  ConditionVariable m_condVar;
  std::multimap< timespec, UserEntry, timespec_cmp> m_users;

}; // class TimerThreadMultimap


#endif // TIMERTHREAD_MULTIMAP_HPP

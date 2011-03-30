#ifndef TIMERTHREAD_HPP
#define TIMERTHREAD_HPP


#include "Thread.hpp"
#include "Mutex.hpp"
#include "ConditionVariable.hpp"

#include <map>
#include <ctime> // time_t

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
    time_t periodTime;
    TimerUser* user;
  } UserEntry;


public:

    TimerThread();

    virtual ~TimerThread();

    void addTimerUser(TimerUser* user,
                      const time_t expiration,
                      const time_t periodTime = 0 );

    bool removeTimerUser ( void* timerUser );

    // override to signal as well
    void stop();


private:

  void* run( void );


  Mutex m_mutex;
  ConditionVariable m_condVar;
  std::multimap<time_t, UserEntry> m_users;

}; // class TimerThread


#endif // TIMERTHREAD_HPP

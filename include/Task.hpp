#ifndef TASK_HPP
#define TASK_HPP

#include <time.h>

class Task
{

public:

    Task();

    void run ();

    bool isItStucked () const;

private:

  time_t m_startedToRun;
  time_t m_timeOut;

};


#endif // TASK_HPP

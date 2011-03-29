#ifndef TASK_HPP
#define TASK_HPP

#include <time.h>

class Task
{

public:

//     Task() {};
    virtual ~Task() {};

    virtual void run () = 0;
    virtual bool isItStucked () const = 0;

protected:

//   time_t m_startedToRun;
//   time_t m_timeOut;

};


#endif // TASK_HPP

#ifndef TASK_HPP
#define TASK_HPP

#include <time.h>

class Task
{

public:

    virtual ~Task() {};
    virtual void run () = 0;

protected:


};


#endif // TASK_HPP

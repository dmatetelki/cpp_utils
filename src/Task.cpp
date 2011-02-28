#include "Task.hpp"

#include <iostream>

#include "Common.hpp"

Task::Task() : m_timeOut(5)
{
  TRACE(this);
}


void Task::run()
{
  TRACE(this);
  m_startedToRun = time(NULL);

  std::cout << "I'm a task..." << std::endl;

  // other stuff

  m_startedToRun = 0;
}


bool Task::isItStucked () const
{
  TRACE(this);
  return ( m_startedToRun + m_timeOut < time(NULL) );
}

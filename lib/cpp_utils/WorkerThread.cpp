#include "WorkerThread.hpp"

#include "Logger.hpp"


WorkerThread::WorkerThread( ThreadPool& tp )
  : m_tp(tp)
{
  TRACE;
}


void* WorkerThread::run()
{
  TRACE;
  while ( m_isRunning )
  {
    Task* task(0);
    try {
      task = m_tp.popTask();
      task->run();
      delete task;
    } catch (CancelledException) {
      LOG( Logger::FINEST, "Now I die.");
    }
  }
  return 0;
}

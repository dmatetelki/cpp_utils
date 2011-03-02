#include "ThreadPool.hpp"
#include "Common.hpp"


ThreadPool::ThreadPool()
{
  TRACE(this);
}

ThreadPool::~ThreadPool()
{
  TRACE(this);
  std::vector<Thread*>::iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    delete (*it);
  }
  m_threads.clear();
}

void ThreadPool::pushTask( Task* task )
{
  TRACE(this);
  m_tasks.push(task);
}


Task* ThreadPool::popTask()
{
  TRACE(this);
  return m_tasks.waitAndPop();
}


void ThreadPool::pushWorkerThread( Thread * thread)
{
  TRACE(this);
  m_threads.push_back( thread );
}


void ThreadPool::startWorkerThreads()
{
  TRACE(this);
  std::vector<Thread*>::iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ )
  {
    (*it)->start();
  }

}

void ThreadPool::stop()
{
  TRACE(this);
  std::vector<Thread*>::iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    (*it)->stop();
  }

  m_tasks.cancel();
}


void ThreadPool::join() const
{
  TRACE(this);
  std::vector<Thread*>::const_iterator it;
  for( it = m_threads.begin() ; it != m_threads.end(); it++ ) 
  {
    (*it)->join();
  }
}

#include <iostream>

#include "../ConcurrentQueue.hpp"
#include "../Thread.hpp"

#define TRACE std::cout << __FILE__ << " @ " << __PRETTY_FUNCTION__ << ":" <<  __LINE__ << std::endl;

class WaitingThread : public Thread
{

  public:

    WaitingThread (ConcurrentQueue<int>& cq)
      : m_isRunning( true ),
        m_cq(cq)
    {
      TRACE
    }

    ~WaitingThread()
    {
      TRACE
    }

    void stop ()
    {
      TRACE
      m_isRunning = false;
    }

  private:

    void* run ()
    {
      TRACE
      while ( m_isRunning )
      {
        std::cout << "waiting..." << std::endl;
        int retval = m_cq.waitAndPop();
        std::cout << "waiting...ENDED! Got retval: " << retval << std::endl;
      }

      return 0;
    }

    bool m_isRunning;
    ConcurrentQueue<int>& m_cq;
};

class fake_type
{

public:

  fake_type(int i) : m_int(i)
  {
    TRACE
  }

  ~fake_type()
  {
    TRACE
  }

  fake_type( const fake_type& m)
  {
    TRACE
    m_int = m.m_int;
  }

  fake_type& operator=( const fake_type& m)
  {
    TRACE
    m_int = m.m_int;
    return *this;
  }

private:
  int m_int;
};


int main()
{
  ConcurrentQueue<fake_type> cq1;
  cq1.push(fake_type(7));

  fake_type m = cq1.waitAndPop();

  // other case

  ConcurrentQueue<int> cq2;
  cq2.push(7);
  cq2.push(13);

  WaitingThread* wt1 = new WaitingThread(cq2);
  wt1->start();

  sleep(2);

  cq2.push(34);

  sleep(5);

  wt1->stop();

  sleep(5);

  return 0;
}

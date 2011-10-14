#ifndef SUBJECT_HPP
#define SUBJECT_HPP

#include <list>

class Observer;

class Subject
{
public:

  virtual ~Subject();

  virtual void attach( Observer* );
  virtual void detach( Observer* );
  virtual void notify();

private:
  std::list< Observer* > m_observers;

};

#endif // SUBJECT_HPP
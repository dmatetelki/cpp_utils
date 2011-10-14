#ifndef OBSERVER_HPP
#define OBSERVER_HPP

class Subject;

class Observer
{
public:

  virtual ~Observer();
  virtual void update( Subject* ) = 0;
};


#endif // OBSERVER_HPP

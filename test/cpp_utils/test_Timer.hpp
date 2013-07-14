#include <cxxtest/TestSuite.h>

#include "Fixture.hpp"

#include <cpp_utils/Timer.hpp>


class TestTimer : public CxxTest::TestSuite
{
public:

  void testCreateTimerInvalidClockId()
  {
    TEST_HEADER;

    TimerUser *timerUser(0);
    clockid_t clockId(10); // clockid_t is valid [0,6]

    TS_ASSERT_EQUALS(Timer::createTimer(timerUser, clockId), (void*)0);
  }

  void testSetTimerInvalidTimerId()
  {
    TEST_HEADER;

    timer_t timerId((void*)0);
    TS_ASSERT_EQUALS(Timer::setTimer(timerId, 1), false);
  }

  void testSetTimerInvalidTime()
  {
    TEST_HEADER;

    timer_t timerId = Timer::createTimer(0, 1);
    TS_ASSERT_EQUALS(Timer::setTimer(timerId, -1), false);
    Timer::deleteTimer(timerId);
  }

  void testDeleteTimerInvalidTimerId()
  {
    TEST_HEADER;

    timer_t timerId((void*)0);
    TS_ASSERT_EQUALS(Timer::deleteTimer(timerId), false);
  }

};



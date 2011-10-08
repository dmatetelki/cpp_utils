set (CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Wshadow "
               "-ggdb -fprofile-arcs -ftest-coverage --std=c++0x " )

add_definitions( ${CXX_FLAGS} )
# add_definitions( -DNO_TRACE )


find_package(CxxTest)
if(CXXTEST_FOUND)
  set(CXXTEST_USE_PERL TRUE)
#   set(CXXTEST_TESTGEN_ARGS "")

  include_directories(${CXXTEST_INCLUDE_DIR} ../include)
  enable_testing()
  CXXTEST_ADD_TEST(test
  generated_main.cpp
  Fixture.hpp

   test_Singleton.hpp
   test_Singleton_meyers.hpp
   test_Singleton_DCLP.hpp
#   test_Mutex.hpp
#   test_ScopedLock.hpp
#   test_ConditionalVariable.hpp
#   test_Thread.hpp
#   test_ThreadPool.hpp
#   test_Semaphore.hpp
#   test_Timer.hpp
#   test_Common.hpp
#   test_TimerThreadMultimap.hpp
  )
  target_link_libraries(test CppUtils gcov)
endif()

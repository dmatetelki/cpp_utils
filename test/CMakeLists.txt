
set(CMAKE_CXX_COMPILER "/usr/lib/colorgcc/bin/g++")

set (CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Wshadow "
               "-Wpointer-arith -Wcast-qual "
               "-ggdb -fprofile-arcs -ftest-coverage --std=c++0x " )

add_definitions( ${CXX_FLAGS} )


find_package(CxxTest)
if(CXXTEST_FOUND)
  set(CXXTEST_USE_PERL TRUE)
#   set(CXXTEST_TESTGEN_ARGS "")

  include_directories(${CXXTEST_INCLUDE_DIR} ../include)
  enable_testing()
  CXXTEST_ADD_TEST(testCppUtils
  generated_main.cpp
  Fixture.hpp

  test_Logger.hpp
  test_ArgParse.hpp
  test_Common.hpp
  test_ConditionalVariable.hpp
  test_Multiton.hpp
  test_Mutex.hpp
  test_ObjectPool.hpp
  test_ScopedLock.hpp
  test_Semaphore.hpp
  test_Singleton_DCLP.hpp
  test_Singleton_call_once.hpp
  # test_Singleton.hpp Cannot test private member, Ficture.hpp loads it
  test_Singleton_meyers.hpp
  test_Thread.hpp
  test_ThreadPool.hpp

  test_timerUser.hpp
  test_Timer.hpp
  test_Connection.hpp
  test_StreamConnection.hpp
  test_TcpConnection.hpp
  test_Message.hpp

  )
  target_link_libraries(testCppUtils CppUtils gcov)
endif()

add_custom_target( test
                   COMMAND ./run_test.sh ./testCppUtils
                   DEPENDS testCppUtils
)


set(CMAKE_CXX_COMPILER "/usr/lib/colorgcc/bin/g++")

set (CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Wshadow "
               "-Wpointer-arith -Wcast-qual "
               "-ggdb -fprofile-arcs -ftest-coverage --std=c++0x " )

add_definitions( ${CXX_FLAGS} )


find_package(CxxTest)
if(CXXTEST_FOUND)
  set(CXXTEST_USE_PERL TRUE)
#   set(CXXTEST_TESTGEN_ARGS "")

  include_directories(${CXXTEST_INCLUDE_DIR} ../lib/)
  enable_testing()
  CXXTEST_ADD_TEST(testCppUtils
  generated_main.cpp
  cpp_utils/Fixture.hpp

  cpp_utils/test_Logger.hpp
  cpp_utils/test_ArgParse.hpp
  cpp_utils/test_Common.hpp
  cpp_utils/test_ConditionalVariable.hpp
  cpp_utils/test_Multiton.hpp
  cpp_utils/test_Mutex.hpp
  cpp_utils/test_ObjectPool.hpp
  cpp_utils/test_ScopedLock.hpp
  cpp_utils/test_Semaphore.hpp
#   cpp_utils/test_Singleton_DCLP.hpp
#   cpp_utils/test_Singleton_call_once.hpp
  # cpp_utils/test_Singleton.hpp Cannot test private member, Ficture.hpp loads it
#   cpp_utils/test_Singleton_meyers.hpp
  cpp_utils/test_Thread.hpp
  cpp_utils/test_ThreadPool.hpp

  cpp_utils/test_timerUser.hpp
  cpp_utils/test_Timer.hpp
  cpp_utils/test_Connection.hpp
  cpp_utils/test_StreamConnection.hpp
  cpp_utils/test_TcpConnection.hpp
  cpp_utils/test_Message.hpp

  )
  target_link_libraries(testCppUtils CppUtils gcov)
endif()

add_custom_target( test
                   COMMAND ./tools/run_test.sh ./testCppUtils
                   DEPENDS testCppUtils
)

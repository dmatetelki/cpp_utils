cmake_minimum_required (VERSION 2.6)
project (CPP_UTILS_TEST)

message(STATUS "Test dir:")

set (CXX_FLAGS "-Wall -Wextra -pedantic")
add_definitions( ${CXX_FLAGS} )
message(STATUS "g++ flags: ${CXX_FLAGS}")

include_directories (../include)
message(STATUS "include dir: ${CPP_UTILS_LIB}/include")

add_executable (testThreadPool main_threadpool.cpp)
target_link_libraries (testThreadPool CppUtils)


find_package(CxxTest)
if(CXXTEST_FOUND)
  set(CXXTEST_USE_PERL TRUE)
  include_directories(${CXXTEST_INCLUDE_DIR})
  enable_testing()
#   CXXTEST_ADD_TEST(unittest_sos check_sos.cpp ${SOS_TEST_PATH}/check_sos.h)
#   target_link_libraries(unittest_sos CppUtils)
endif()
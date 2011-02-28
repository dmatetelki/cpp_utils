cmake_minimum_required (VERSION 2.6)
project (CPP_UTILS_LIB)

message(STATUS "Lib dir:")

set (CXX_FLAGS "-Wall -Wextra -pedantic")
add_definitions( ${CXX_FLAGS} )
message(STATUS "g++ flags: ${CXX_FLAGS}")

include_directories (../include)
message(STATUS "include dir: ${CPP_UTILS_LIB}/include")

aux_source_directory(../src CPP_UTILS_LIB_SOURCES)
message(STATUS "sources: ${CPP_UTILS_LIB_SOURCES}")

add_library (CppUtils ${CPP_UTILS_LIB_SOURCES})
target_link_libraries(CppUtils pthread rt)

cmake_minimum_required (VERSION 2.6)
project (CPP_UTILS_LIB)

set (CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Wshadow "
               "-ggdb -fprofile-arcs -ftest-coverage")
add_definitions( ${CXX_FLAGS} )
# add_definitions( -DNO_TRACE )

include_directories (../include)
aux_source_directory(../src CPP_UTILS_LIB_SOURCES)

add_library (CppUtils SHARED ${CPP_UTILS_LIB_SOURCES})
target_link_libraries(CppUtils pthread rt gcov)

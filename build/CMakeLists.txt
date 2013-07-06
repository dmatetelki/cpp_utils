cmake_minimum_required (VERSION 2.6)
project (CPP_UTILS_LIB)

set(CMAKE_CXX_COMPILER "/usr/lib/colorgcc/bin/g++")

set (CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Wshadow "
               "-Wpointer-arith -Wcast-qual "
               "-ggdb -fprofile-arcs -ftest-coverage -std=c++0x")
add_definitions( ${CXX_FLAGS} )

include_directories (../include)
aux_source_directory(../src CPP_UTILS_LIB_SOURCES)

add_library (CppUtils SHARED ${CPP_UTILS_LIB_SOURCES})

add_library( mysqlclient SHARED IMPORTED )
set_target_properties( mysqlclient PROPERTIES IMPORTED_LOCATION /usr/lib/mysql/libmysqlclient.so )

target_link_libraries(CppUtils pthread rt gcov ssl mysqlclient)

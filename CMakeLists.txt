cmake_minimum_required (VERSION 2.6)
project (CPP_UTILS_LIB)

add_subdirectory (build)
add_subdirectory (test)

find_package(Doxygen)
if(DOXYGEN_FOUND)
  add_custom_target(doc ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_SOURCE_DIR}/doxyfile)
endif(DOXYGEN_FOUND)


add_custom_target(reset
COMMAND rm -rf html

COMMAND rm -rf CMakeFiles
COMMAND rm -rf Makefile
COMMAND rm -rf cmake_install.cmake
COMMAND rm -rf CMakeCache.txt

COMMAND rm -rf build/CMakeFiles
COMMAND rm -rf build/Makefile
COMMAND rm -rf build/cmake_install.cmake
COMMAND rm -rf build/CMakeCache.txt
COMMAND rm -rf build/*.a

COMMAND rm -rf test/CMakeFiles
COMMAND rm -rf test/Makefile
COMMAND rm -rf test/cmake_install.cmake
COMMAND rm -rf test/CMakeCache.txt
COMMAND rm -rf test/CTestTestfile.cmake
COMMAND rm -rf test/cov
COMMAND rm -rf test/leak.log.core.*
COMMAND rm -rf test/gdb.out
COMMAND rm -rf test/*.gcno
COMMAND rm -rf test/lcov*.info
COMMAND rm -rf test/leak.log
COMMAND rm -rf test/test.out
COMMAND rm -rf test/generated_main.cpp
COMMAND rm -rf test/test

)

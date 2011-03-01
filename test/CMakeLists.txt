add_definitions(-Wall -Werror -pedantic -Weffc++ -Wshadow -ggdb -fprofile-arcs -ftest-coverage)

include_directories(../include)
#add_library (CppUtils src/*)


find_package(CxxTest)
if(CXXTEST_FOUND)
set(CXXTEST_USE_PERL TRUE)
include_directories(${CXXTEST_INCLUDE_DIR} ../include)
enable_testing()
CXXTEST_ADD_TEST(unittest_multiple generated_main.cpp unittest_multiple.hpp )
target_link_libraries(unittest_multiple CppUtils gcov)
endif()


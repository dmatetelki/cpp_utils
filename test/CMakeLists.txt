set (CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Wshadow -ggdb -fprofile-arcs -ftest-coverage")
add_definitions( ${CXX_FLAGS} )

find_package(CxxTest)
if(CXXTEST_FOUND)
set(CXXTEST_USE_PERL TRUE)
include_directories(${CXXTEST_INCLUDE_DIR} ../include)
enable_testing()
CXXTEST_ADD_TEST(test generated_main.cpp
test_threadpool.hpp
test_Singelton.hpp
)
target_link_libraries(test CppUtils gcov)
endif()

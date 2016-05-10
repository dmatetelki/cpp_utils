set (CXX_FLAGS "-Wall -Wextra -pedantic -Weffc++ -Wshadow "
               "-Wpointer-arith -Wcast-qual "
               "-ggdb -fprofile-arcs -ftest-coverage --std=c++0x " )

add_definitions( ${CXX_FLAGS} )

include_directories (. ../lib)
link_directories (../lib/cpp_utils)

add_executable ( tcpserver tcpserver_main.cpp )
target_link_libraries ( tcpserver CppUtils gcov)

add_executable ( tcpclient tcpclient_main.cpp )
target_link_libraries ( tcpclient CppUtils gcov )

add_executable ( sslserver sslserver_main.cpp )
target_link_libraries ( sslserver CppUtils ssl pthread rt gcov )

add_executable ( sslclient sslclient_main.cpp )
target_link_libraries ( sslclient CppUtils ssl pthread rt gcov )

# add_executable ( mysqlclient mysqlclient_main.cpp )
# add_library ( lib_mysql_client SHARED IMPORTED )
# # TODO use find_library
# set_target_properties ( lib_mysql_client PROPERTIES IMPORTED_LOCATION /usr/lib/x86_64-linux-gnu/libmysqlclient.so )
# target_link_libraries ( mysqlclient CppUtils lib_mysql_client )


add_custom_target( other DEPENDS tcpserver tcpclient sslserver sslclient
# mysqlclient
)

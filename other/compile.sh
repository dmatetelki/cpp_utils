#!/bin/bash

WORKING_DIR=`pwd`

INCLUDE_DIR="$WORKING_DIR/../lib"
LIB_DIR=/home/denes/projects/cpp_utils/cpp_utils/build
GCC_OPTIONS="-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-qual -ggdb -Weffc++ -std=c++0x"
GCC="g++"

BUILD_DIR="$WORKING_DIR/tmp"

for SRC_FILE in $(ls $INCLUDE_DIR/cpp_utils/*.cpp)
do
  echo "Compiling $SRC_FILE"
  $GCC -c $SRC_FILE -I$INCLUDE_DIR $GCC_OPTIONS
done

for SRC_FILE in $(ls *_main.cpp)
do
  echo "Compiling $SRC_FILE"
  $GCC -c $SRC_FILE -I$INCLUDE_DIR $GCC_OPTIONS
done

rm -rf $BUILD_DIR
mkdir $BUILD_DIR
mv *.o $BUILD_DIR
cd $BUILD_DIR


echo "Linking tcpclient_main.o"
$GCC tcpclient_main.o -L$LIB_DIR -lCppUtils -lpthread -o tcpclient

echo "Linking tcpserver_main.o"
$GCC tcpserver_main.o -L$LIB_DIR -lCppUtils -lpthread -o tcpserver

echo "Linking sslclient_main.o"
$GCC sslclient_main.o -L$LIB_DIR -lCppUtils -lpthread -lssl -lrt -o sslclient

echo "Linking sslserver_main.o"
$GCC sslserver_main.o -L$LIB_DIR -lCppUtils -lpthread -lssl -lrt -o sslserver

echo "Linking mysqlclient_main.o"
$GCC mysqlclient_main.o -L$LIB_DIR -lCppUtils -lrt -lpthread -L/usr/lib/mysql -lmysqlclient -o mysqlclient

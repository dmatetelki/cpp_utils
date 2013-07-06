#!/bin/bash

INCLUDE_DIR="../include"
GCC_OPTIONS="-Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-qual -ggdb -Weffc++ -std=c++0x"
GCC="/usr/lib/colorgcc/bin/g++"

BUILD_DIR="tmp/"

for SRC_FILE in $(ls ../src/*.cpp)
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
$GCC tcpclient_main.o Logger.o SocketClient.o TcpConnection.o Socket.o AddrInfo.o Connection.o Thread.o Poll.o -lpthread -o tcpclient

echo "Linking tcpserver_main.o"
$GCC tcpserver_main.o Logger.o SocketServer.o TcpConnection.o Socket.o AddrInfo.o Connection.o Thread.o Poll.o -lpthread -o tcpserver

echo "Linking sslclient_main.o"
$GCC sslclient_main.o Logger.o SocketClient.o TimerUser.o Timer.o TimedTcpConnection.o TcpConnection.o Socket.o AddrInfo.o Connection.o Thread.o Poll.o SslConnection.o -lpthread -lssl -lrt -o sslclient

echo "Linking sslserver_main.o"
$GCC sslserver_main.o Logger.o SocketServer.o TimerUser.o Timer.o TimedTcpConnection.o TcpConnection.o Socket.o AddrInfo.o Connection.o Thread.o Poll.o SslConnection.o -lpthread -lssl -lrt -o sslserver

echo "Linking mysqlclient_main.o"
$GCC mysqlclient_main.o Logger.o ArgParse.o ConditionVariable.o ScopedLock.o MysqlClient.o Mutex.o MysqlConnectionPool.o -lrt -lpthread -L/usr/lib/mysql -lmysqlclient -o mysqlclient

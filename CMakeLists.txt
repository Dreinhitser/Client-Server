cmake_minimum_required(VERSION 3.10)

project (test_task)

add_executable(server erproc.cpp erproc.hpp server/server.cpp)
add_executable(client erproc.cpp erproc.hpp client/client.cpp)
cmake_minimum_required(VERSION 3.10)
project(AsyncServer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -Wall")

# 自动收集所有源文件（包括根目录和 client 子目录）
file(GLOB SOURCES "*.cpp" "server/*.cpp")

add_executable(server ${SOURCES})
target_link_libraries(server PRIVATE boost_system pthread)
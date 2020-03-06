cmake_minimum_required(VERSION 3.10)
project(netlib)

set(CMAKE_CXX_STANDARD 11)

# 添加头文件目录
include_directories(./include)

add_subdirectory(src)
add_subdirectory(project)
add_subdirectory(test)


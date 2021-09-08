cmake_minimum_required(VERSION 3.9)
project(async)# VERSION 0.0.$ENV{GITHUB_RUN_NUMBER})

set(CMAKE_CXX_STANDARD 17)

add_subdirectory(lib)
add_subdirectory(src)
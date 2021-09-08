include_directories(${CMAKE_SOURCE_DIR}/lib)

add_executable(async_cli main.cpp)
target_link_libraries(async_cli async)


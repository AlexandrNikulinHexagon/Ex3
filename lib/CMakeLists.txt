set ( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
add_definitions(-D_SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING)

set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(async PRIVATE Threads::Threads)

add_library(async SHARED async.cpp async.h)


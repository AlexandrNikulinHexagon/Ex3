set ( CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON )
add_definitions(-D_SILENCE_CXX17_STRSTREAM_DEPRECATION_WARNING)

add_library(async SHARED async.cpp async.h)

target_link_libraries(async pthread)


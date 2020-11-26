cmake_minimum_required(VERSION 3.15)
project(mediaInfo)
include_directories("C:/mingw64/include")
link_directories("C:/mingw64/lib")
set(CMAKE_CXX_STANDARD 14)

add_executable(mediaInfo main.cpp)
target_link_libraries(mediaInfo avformat avcodec avfilter avdevice avutil postproc swresample swscale)
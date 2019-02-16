cmake_minimum_required (VERSION 3.10)
project (AudioVis VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3") # google how to do this w 'release' to make cleaner and more modern

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(SFML REQUIRED network audio graphics window system)
find_package(Threads REQUIRED)

add_subdirectory(src)

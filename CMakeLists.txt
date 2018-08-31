cmake_minimum_required (VERSION 3.10)
project (AudioVis VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")
find_package(SFML REQUIRED network audio graphics window system)

add_subdirectory(src)

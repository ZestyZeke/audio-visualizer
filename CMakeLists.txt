cmake_minimum_required (VERSION 3.10)
project (AudioVis VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-O3") # google how to do this w 'release' to make cleaner and more modern

find_package(Threads REQUIRED)

include(.conan/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS) # by default this macro overrides where binary is built to

add_subdirectory(src)

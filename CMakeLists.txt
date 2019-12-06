cmake_minimum_required (VERSION 3.10)
project (AudioVis VERSION 0.1)

set(CMAKE_CXX_STANDARD 17)
add_compile_options(
        "$<$<CONFIG:RELEASE>:-O3>"
        "$<$<CONFIG:DEBUG>:-Wall>"
)

find_package(Threads REQUIRED)

include(.conan/conanbuildinfo.cmake)
conan_basic_setup(NO_OUTPUT_DIRS) # by default this macro overrides where binary is built to

add_subdirectory(src)

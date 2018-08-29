cmake_minimum_required (VERSION 3.11)
project (AudioVis VERSION 0.1)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(src)


if(FALSE) # fake a block comment
option(BUILD_TESTING "Build the testing tree" ON)
if(BUILD_TESTING AND (PROJECT_SOURCE_DIR STREQUAL CMAKE_SOURCE_DIR))
    enable_testing()
    add_subdirectory(test)
endif()
endif() # end fake block comment

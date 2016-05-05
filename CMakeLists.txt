cmake_minimum_required(VERSION 2.6)
set(CMAKE_BUILD_TYPE Release )
add_definitions( -std=c++11 )
add_library(libcnpy2 cnpy.cpp )

# For testing
add_executable( example1 example1.cpp )
target_link_libraries( example1 libcnpy2 )

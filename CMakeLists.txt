project ( Example )

cmake_minimum_required(VERSION 3.20 )

set ( CMAKE_CXX_STANDARD 17 )

enable_testing()

add_subdirectory( lib )
add_subdirectory( application )

add_subdirectory( tests )


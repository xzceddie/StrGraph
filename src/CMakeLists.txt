cmake_minimum_required( VERSION 3.10 )
include_directories( ${CMAKE_SOURCE_DIR} )
add_subdirectory( include )

add_library( node STATIC node.cpp )

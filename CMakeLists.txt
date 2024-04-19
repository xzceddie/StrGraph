cmake_minimum_required( VERSION 3.10 )
set( CMAKE_CXX_STANDARD 20 )
set( CMAKE_EXPORT_COMPILE_COMMANDS true )

project( StrGraph )

add_subdirectory( src )
add_subdirectory( tests )
add_subdirectory( thirdparty )

include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/thirdparty/ )
# include_directories( ${CMAKE_SOURCE_DIR}/thirdparty/asio/asio/include/ )

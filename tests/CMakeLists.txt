cmake_minimum_required( VERSION 3.10 )
enable_testing()
include_directories( ${CMAKE_SOURCE_DIR} )
include_directories( ${CMAKE_SOURCE_DIR}/thirdparty/ )
include_directories( ${CMAKE_SOURCE_DIR}/thirdparty/Catch2/src/ )
include_directories( ${CMAKE_SOURCE_DIR}/thirdparty/pybind11/include/ )
include_directories( ${CMAKE_SOURCE_DIR}/thirdparty/pybind11/ )

add_executable( test_first test_first.cpp )
add_test( NAME "test_first" COMMAND test_first )
target_link_libraries( test_first PUBLIC Catch2::Catch2WithMain )

add_executable( test_node test_node.cpp )
add_test( NAME "test_node" COMMAND test_node )
target_link_libraries( test_node PUBLIC Catch2::Catch2WithMain )

add_executable( test_DAG test_DAG.cpp )
add_test( NAME "test_DAG" COMMAND test_DAG )
target_link_libraries( test_DAG PUBLIC Catch2::Catch2WithMain )

file(COPY ${CMAKE_SOURCE_DIR}/tests/test_pybind11.py DESTINATION ${CMAKE_BINARY_DIR}/tests/)
find_package(pybind11 REQUIRED)
pybind11_add_module(example ./test_pybind11.cpp)
add_test(
    NAME test_pybind11
    COMMAND ${PYTHON_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/test_pybind11.py
)

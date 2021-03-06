cmake_minimum_required(VERSION 3.0)

project(MultiHeadStack)
enable_testing()

execute_process(
	COMMAND git submodule update --init
	WORKING_DIRECTORY ${MultiHeadStack_SOURCE_DIR}
)

add_subdirectory(gtest)

add_executable(test_single_head test/SingleHeadTest.cpp)
target_link_libraries(test_single_head gtest_main)
add_test(NAME test_single_head COMMAND test_single_head "--gtest_output=xml:${CMAKE_BINARY_DIR}/TestsResults-single_head.xml")

add_executable(test_multi_head test/MultiHeadTest.cpp)
target_link_libraries(test_multi_head gtest_main)
add_test(NAME test_multi_head COMMAND test_multi_head "--gtest_output=xml:${CMAKE_BINARY_DIR}/TestsResults-multi_head.xml")

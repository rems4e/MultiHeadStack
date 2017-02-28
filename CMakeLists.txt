cmake_minimum_required(VERSION 3.0)

project(MultiHeadStack)
enable_testing()

execute_process(
	COMMAND git submodule update --init
	WORKING_DIRECTORY ${MultiHeadStack_SOURCE_DIR}
)

add_subdirectory(gtest)

add_executable(test1 test.cpp)
target_link_libraries(test1 gtest_main)
add_test(NAME test1 COMMAND test1 "--gtest_output=xml:${CMAKE_BINARY_DIR}/TestsResults.xml")

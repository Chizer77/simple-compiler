cmake_minimum_required(VERSION 3.20)
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
FATAL: In-source builds are not allowed.
	You should create a separete directory for build files.
")
endif()
project(simple-compiler)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE INC_SRC "include/*.h")
file(GLOB_RECURSE DIR_SRC  "src/*.cpp")
file(GLOB_RECURSE Compiler.cpp)
file(GLOB_RECURSE TEST_SRC "test/*.h")
file(GLOB_RECURSE TEST_SRC "test/*.cpp")

include_directories("include")

include_directories("test")

add_executable (compiler ${INC_SRC} ${DIR_SRC} Compiler.cpp)
add_executable(testCenter ${INC_SRC} ${DIR_SRC} ${TEST_SRC})







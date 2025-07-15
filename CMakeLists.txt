cmake_minimum_required(VERSION 3.10)
project(logging_lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(logging_lib STATIC
    src/file_logger.cpp
    include/logger.h
)

target_include_directories(logging_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

add_executable(logging_example
    example/main.cpp
)

target_link_libraries(logging_example
    logging_lib
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
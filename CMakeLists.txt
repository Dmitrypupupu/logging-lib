cmake_minimum_required(VERSION 3.10)
project(logging_lib)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(logging_lib STATIC
    src/file_logger.cpp
)

target_include_directories(logging_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

find_package(Threads REQUIRED)
target_link_libraries(logging_lib PUBLIC Threads::Threads)

add_executable(logger_example
    example/main.cpp
)

target_link_libraries(logger_example
    logging_lib
)

if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/test/CMakeLists.txt)
    add_subdirectory(test)
endif()
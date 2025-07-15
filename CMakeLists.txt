cmake_minimum_required(VERSION 3.10)
project(logging_lib)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Добавляем библиотеку с явным указанием всех исходников
add_library(logging_lib STATIC
    src/file_logger.cpp
    include/logger.h
)

# Указываем, что include - публичная директория для заголовков
target_include_directories(logging_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Создаем исполняемый файл
add_executable(logging_example
    example/main.cpp
)

# Связываем с библиотекой
target_link_libraries(logging_example
    logging_lib
)

set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
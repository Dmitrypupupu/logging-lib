cmake_minimum_required(VERSION 3.10)
project(logging_lib VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Опция для выбора типа сборки библиотеки
option(BUILD_SHARED_LIBS "Build as shared library" OFF)

# Настройка выходных директорий
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

# Подключение GoogleTest
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
FetchContent_MakeAvailable(googletest)

# Сборка основной библиотеки
add_library(logging_lib
    src/file_logger.cpp
)

# Настройка свойств библиотеки
set_target_properties(logging_lib PROPERTIES
    OUTPUT_NAME "logging_lib"
    VERSION ${PROJECT_VERSION}
    SOVERSION 1
)

# Публичные заголовки
target_include_directories(logging_lib PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

# Зависимости
find_package(Threads REQUIRED)
target_link_libraries(logging_lib PUBLIC Threads::Threads)

# Пример использования
add_executable(logger_example
    example/main.cpp
)
target_link_libraries(logger_example
    logging_lib
)

# Подключение тестов если есть папка tests
if(EXISTS "${CMAKE_CURRENT_SOURCE_DIR}/tests/CMakeLists.txt")
    enable_testing()
    add_subdirectory(tests)
endif()

# Установка библиотеки
install(TARGETS logging_lib
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)

install(DIRECTORY include/ DESTINATION include)
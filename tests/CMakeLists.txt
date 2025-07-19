# Исполняемый файл для тестов
add_executable(logger_tests
    logger_test.cpp
)

# Подключение зависимостей
target_link_libraries(logger_tests
    PRIVATE logging_lib
    PRIVATE gtest_main
    PRIVATE Threads::Threads
)

# Пути к заголовочным файлам
target_include_directories(logger_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

# Регистрация тестов
add_test(NAME logger_tests COMMAND logger_tests)

# Автоматическое обнаружение тестов Google Test
include(GoogleTest)
gtest_discover_tests(logger_tests)
add_executable(logger_tests
    logger_test.cpp
)

target_link_libraries(logger_tests
    PRIVATE logging_lib
    PRIVATE gtest_main
    PRIVATE Threads::Threads
)

target_include_directories(logger_tests PRIVATE
    ${CMAKE_SOURCE_DIR}/include
)

add_test(NAME logger_tests COMMAND logger_tests)

include(GoogleTest)
gtest_discover_tests(logger_tests)
#include <gtest/gtest.h>
#include "logger.h"
#include <fstream>
#include <filesystem>
#include<thread>

const std::string TEST_LOG_FILE = "test_log.log";

class LoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        if (std::filesystem::exists(TEST_LOG_FILE)){
            std::filesystem::remove(TEST_LOG_FILE);
        } 
    }

    void TearDown() override {
        if (std::filesystem::exists(TEST_LOG_FILE)) {
            std::filesystem::remove(TEST_LOG_FILE);
        }
    }
};

TEST_F(LoggerTest, CreatesLogFileOnFirstMessage){
    auto logger = createFileLogger(TEST_LOG_FILE);
    logger->log("test 1");
    EXPECT_TRUE(std::filesystem::exists(TEST_LOG_FILE));
}

TEST_F(LoggerTest, WritesCompleteMessageToFile) {
    const std::string test_message = "test 2";
    auto logger = createFileLogger(TEST_LOG_FILE);
    logger->log(test_message);

    std::ifstream log_file(TEST_LOG_FILE);
    std::string file_content((std::istreambuf_iterator<char>(log_file)), std::istreambuf_iterator<char>());

    EXPECT_TRUE(file_content.find(test_message) != std::string::npos);
    EXPECT_TRUE(file_content.find("[INFO]") != std::string::npos);
}

TEST_F(LoggerTest, FiltersMessagesByLogLevel) {
    auto logger = createFileLogger(TEST_LOG_FILE, LogLevel::WARNING);
    logger->log("Test 3.1: Debug", LogLevel::DEBUG);
    logger->log("Test 3.2: Warning", LogLevel::WARNING);

    std::ifstream log_file(TEST_LOG_FILE);
    std::string file_content((std::istreambuf_iterator<char>(log_file)), std::istreambuf_iterator<char>());

    EXPECT_TRUE(file_content.find("Test 3.1: Debug") == std::string::npos); 
    EXPECT_TRUE(file_content.find("Test 3.2: Warning") != std::string::npos);
}

void thread_log_task(ILogger* logger, int thread_id, int msg_count) {
        for (int j = 0; j < msg_count; ++j) {
            logger->log("Thread " + std::to_string(thread_id) + " msg " + std::to_string(j));
        }
    }

TEST_F(LoggerTest, HandlesConcurrentWrites) {
    auto logger = createFileLogger(TEST_LOG_FILE);
    const int num_threads = 5;
    const int messages_per_thread = 10;
    std::vector<std::thread> threads;
    
    // Создаем и запускаем потоки
    for (int i = 0; i < num_threads; ++i) {
        threads.emplace_back(thread_log_task, logger.get(), i, messages_per_thread);
    }

    // Ожидаем завершения всех потоков
    for (auto& thread : threads) {
        thread.join();
    }

    // Проверяем результат
    std::ifstream log_file(TEST_LOG_FILE);
    std::string file_content((std::istreambuf_iterator<char>(log_file)), 
                          std::istreambuf_iterator<char>());

    for (int i = 0; i < num_threads; ++i) {
        for (int j = 0; j < messages_per_thread; ++j) {
            EXPECT_TRUE(file_content.find("Thread " + std::to_string(i) + " msg " + std::to_string(j)) != std::string::npos);
        }
    }
}
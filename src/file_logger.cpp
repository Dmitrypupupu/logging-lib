#include "logger.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <memory>  // Добавляем для std::make_unique

// Переносим определение класса в начало файла
class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename, LogLevel level)
        : m_filename(filename), m_logLevel(level) {}
    
    void log(const std::string& message, LogLevel level) override {
        if (level < m_logLevel) return;
        
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ofstream file(m_filename, std::ios::app);
        
        if (!file.is_open()) {
            throw std::runtime_error("Cannot open log file: " + m_filename);
        }
        
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        file << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "] "
             << "[" << levelToString(level) << "] "
             << message << std::endl;
    }
    
    void setLogLevel(LogLevel level) override { m_logLevel = level; }
    LogLevel getLogLevel() const override { return m_logLevel; }

private:
    std::string levelToString(LogLevel level) {
        switch(level) {
            case LogLevel::DEBUG:   return "DEBUG";
            case LogLevel::INFO:    return "INFO";
            case LogLevel::WARNING: return "WARNING";
            case LogLevel::ERROR:   return "ERROR";
            default:               return "UNKNOWN";
        }
    }
    
    std::string m_filename;
    LogLevel m_logLevel;
    std::mutex m_mutex;
};

// Реализация фабричной функции
std::unique_ptr<ILogger> createFileLogger(const std::string& filename, LogLevel level) {
    return std::unique_ptr<ILogger>(new FileLogger(filename, level));  // Альтернатива make_unique
}
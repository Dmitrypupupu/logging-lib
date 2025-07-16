#include "logger.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <memory>

class FileLogger : public ILogger {
public:
    FileLogger(const std::string& filename, LogLevel level)
        : m_filename(filename), m_logLevel(level) {}
    
    bool log(const std::string& message, LogLevel level) override {
        if (level < m_logLevel) return true;
        
        std::lock_guard<std::mutex> lock(m_mutex);
        std::ofstream file(m_filename, std::ios::app);
        
        if (!file.is_open()) {
            return false;
        }
        
        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        
        file << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S") << "] "
             << "[" << levelToString(level) << "] "
             << message << std::endl;
        
        return true;
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

std::unique_ptr<ILogger> createFileLogger(const std::string& filename, LogLevel level) {
    return std::make_unique<FileLogger>(filename, level);
}
#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>

enum class LogLevel {
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

class ILogger {
public:
    virtual ~ILogger() = default;

    virtual bool log(const std::string& message,
                    LogLevel level = LogLevel::INFO) = 0;

    virtual void setLogLevel(LogLevel level) = 0;
    virtual LogLevel getLogLevel() const = 0;
};

std::unique_ptr<ILogger> createFileLogger(const std::string& filename,
                                        LogLevel level = LogLevel::INFO);

#endif // LOGGER_H
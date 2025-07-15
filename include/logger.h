#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>

// Енум уровней важности сообщений
enum class LogLevel {
  DEBUG,   // Отладочные сообщения
  INFO,    // Информационные сообщения
  WARNING, // Предупреждения
  ERROR    // Ошибки
};

class ILogger {
public:
  virtual ~ILogger() = default;

  // Запись сообщения в журнал
  virtual void log(const std::string &message,
                   LogLevel level = LogLevel::INFO) = 0;

  // Установка уровня логирования
  virtual void setLogLevel(LogLevel level) = 0;

  // Получение текущего уровня логирования
  virtual LogLevel getLogLevel() const = 0;
};

#endif // LOGGER_H
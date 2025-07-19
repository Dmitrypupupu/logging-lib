/**
 * @file file_logger.cpp
 * @brief Реализация файлового логгера
 */

#include "logger.h"
#include <chrono>
#include <fstream>
#include <iomanip>
#include <memory>
#include <mutex>

/**
 * @class FileLogger
 * @brief Класс для записи логов в файл
 *
 * Реализует интерфейс ILogger, обеспечивая потокобезопасную запись
 * сообщений в файл с временными метками и фильтрацией по уровням.
 */
class FileLogger : public ILogger {
public:
    /**
     * @brief Конструктор файлового логгера
     * @param filename Путь к файлу для записи логов
     * @param level Минимальный уровень логирования
     */
    FileLogger(const std::string &filename, LogLevel level)
        : m_filename(filename), m_logLevel(level) {}

    /**
     * @brief Записывает сообщение в лог-файл
     * @param message Текст сообщения
     * @param level Уровень важности сообщения
     * @return true если запись успешна, false при ошибке
     *
     * @note Сообщения с уровнем ниже m_logLevel игнорируются
     * @warning Метод потокобезопасен (использует мьютекс)
     */
    bool log(const std::string &message, LogLevel level) override {
        if (level < m_logLevel)
            return true;

        std::lock_guard<std::mutex> lock(m_mutex);
        std::ofstream file(m_filename, std::ios::app);

        if (!file.is_open()) {
            return false;
        }

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);

        file << "[" << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
             << "] "
             << "[" << levelToString(level) << "] " << message << std::endl;

        return true;
    }

    /**
     * @brief Устанавливает минимальный уровень логирования
     * @param level Новый минимальный уровень
     */
    void setLogLevel(LogLevel level) override { m_logLevel = level; }

    /**
     * @brief Возвращает текущий уровень логирования
     * @return Текущий минимальный уровень
     */
    LogLevel getLogLevel() const override { return m_logLevel; }

private:
    /**
     * @brief Преобразует LogLevel в строку
     * @param level Уровень логирования
     * @return Строковое представление уровня
     */
    std::string levelToString(LogLevel level) {
        switch (level) {
        case LogLevel::DEBUG:   return "DEBUG";
        case LogLevel::INFO:    return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::ERROR:   return "ERROR";
        default:                return "UNKNOWN";
        }
    }

    std::string m_filename; ///< Путь к файлу лога
    LogLevel m_logLevel;    ///< Текущий минимальный уровень логирования
    std::mutex m_mutex;     ///< Мьютекс для синхронизации доступа
};

/**
 * @brief Фабричная функция создания файлового логгера
 * @param filename Путь к файлу для записи логов
 * @param level Начальный уровень логирования (по умолчанию INFO)
 * @return Указатель на интерфейс логгера
 *
 * @par Пример использования:
 * @code
 * auto logger = createFileLogger("app.log", LogLevel::DEBUG);
 * logger->log("Test message");
 * @endcode
 */
std::unique_ptr<ILogger> createFileLogger(const std::string &filename,
                                        LogLevel level) {
    return std::make_unique<FileLogger>(filename, level);
}
#ifndef LOGGER_H
#define LOGGER_H

#include <memory>
#include <string>

/**
 * @brief Уровни важности лог-сообщений
 * 
 * DEBUG   - Отладочные сообщения (наибольшая детализация)
 * INFO    - Информационные сообщения (стандартный уровень)
 * WARNING - Предупреждения (потенциальные проблемы)
 * ERROR   - Критические ошибки (требуют немедленного внимания)
 */
enum class LogLevel { 
    DEBUG,   
    INFO,    
    WARNING, 
    ERROR    
};

/**
 * @brief Интерфейс логгера
 * 
 * Базовый класс для всех реализаций логгеров.
 * Определяет стандартный интерфейс для работы с системой логирования.
 */
class ILogger {
public:
    /**
     * @brief Виртуальный деструктор для корректного удаления производных классов
     */
    virtual ~ILogger() = default;

    /**
     * @brief Запись сообщения в лог
     * @param message Текст сообщения
     * @param level Уровень важности сообщения (по умолчанию INFO)
     * @return true если запись прошла успешно, false в случае ошибки
     */
    virtual bool log(const std::string &message,
                    LogLevel level = LogLevel::INFO) = 0;

    /**
     * @brief Установка минимального уровня логирования
     * @param level Минимальный уровень для записи (сообщения с более низким уровнем игнорируются)
     */
    virtual void setLogLevel(LogLevel level) = 0;

    /**
     * @brief Получение текущего уровня логирования
     * @return Текущий минимальный уровень логирования
     */
    virtual LogLevel getLogLevel() const = 0;
};

/**
 * @brief Фабричная функция для создания файлового логгера
 * @param filename Путь к файлу для записи логов
 * @param level Начальный уровень логирования (по умолчанию INFO)
 * @return Уникальный указатель на объект логгера
 * 
 * @note Потокобезопасная реализация. Автоматически добавляет временные метки.
 */
std::unique_ptr<ILogger> createFileLogger(const std::string &filename,
                                         LogLevel level = LogLevel::INFO);

#endif // LOGGER_H
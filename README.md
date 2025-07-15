# Библиотека логирования для C++

![C++](https://img.shields.io/badge/C++-14-blue)
![CMake](https://img.shields.io/badge/CMake-3.10%2B-brightgreen)
![Платформы](https://img.shields.io/badge/Платформы-Linux%20|%20macOS%20|%20Windows-lightgrey)
![Лицензия](https://img.shields.io/badge/Лицензия-MIT-green)

Простая и эффективная библиотека для логирования в C++ с поддержкой разных уровней важности и записью в файл.

## Особенности

- 🚀 4 уровня логирования: `DEBUG`, `INFO`, `WARNING`, `ERROR`
- ⏱ Автоматическое добавление временных меток
- 🔒 Потокобезопасная реализация (использует mutex)
- 📁 Запись в указанный файл с дозаписью
- 🎚 Динамическое изменение уровня логирования
- 🏗 Простая интеграция через CMake

## Быстрый старт

### Клонирование и сборка

```bash
git clone https://github.com/ваш-логин/logging_lib.git
cd logging_lib
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .
Пример использования
cpp
#include "logger.h"

int main() {
    auto logger = createFileLogger("myapp.log", LogLevel::INFO);
    
    logger->log("Программа запущена");
    logger->log("Отладочное сообщение", LogLevel::DEBUG); // Не запишется
    logger->setLogLevel(LogLevel::DEBUG);
    logger->log("Теперь отладка видна", LogLevel::DEBUG);
    
    return 0;
}
Пример вывода в myapp.log:

text
[2023-11-20 10:15:42] [INFO] Программа запущена
[2023-11-20 10:15:43] [DEBUG] Теперь отладка видна
Полная документация
Структура проекта
text
.
├── include/
│   └── logger.h          # Интерфейс логгера
├── src/
│   └── file_logger.cpp   # Реализация файлового логгера
├── example/
│   └── main.cpp          # Пример использования
└── CMakeLists.txt        # Конфигурация сборки
API интерфейс
cpp
// Создание логгера
std::unique_ptr<ILogger> createFileLogger(
    const std::string& filename, 
    LogLevel level = LogLevel::INFO
);

// Уровни логирования
enum class LogLevel {
    DEBUG,    // Отладочная информация
    INFO,     // Обычные сообщения
    WARNING,  // Предупреждения
    ERROR     // Критические ошибки
};

// Методы ILogger
void log(const std::string& message, LogLevel level = LogLevel::INFO);
void setLogLevel(LogLevel level);  // Установить минимальный уровень
LogLevel getLogLevel() const;      // Получить текущий уровень
Интеграция в ваш проект
Добавьте как подмодуль:

bash
git submodule add https://github.com/ваш-логин/logging_lib.git
В ваш CMakeLists.txt:

cmake
add_subdirectory(logging_lib)
target_link_libraries(ваш_проект PRIVATE logging_lib)
В коде:

cpp
#include <logger.h>
Поддерживаемые платформы
Linux (gcc/clang)

macOS (Xcode/clang)

Windows (MSVC 2019+)

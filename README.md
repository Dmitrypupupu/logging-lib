# 📝 Logging Library | C++ 

![C++](https://img.shields.io/badge/C++-17-blue.svg)
![CMake](https://img.shields.io/badge/CMake-3.10+-brightgreen.svg)
![License](https://img.shields.io/badge/License-MIT-orange.svg)
![Platform](https://img.shields.io/badge/Platform-Linux%20|%20macOS%20|%20Windows-lightgrey.svg)

Простая и эффективная библиотека для многопоточного логирования на C++ с поддержкой различных уровней важности сообщений.

## 🌟 Особенности

- 🚀 **Многопоточная** потокобезопасная запись логов
- 📊 **4 уровня** логирования: `DEBUG`, `INFO`, `WARNING`, `ERROR`
- ⏱ **Автоматическое** добавление временных меток
- 📂 Запись в **файл** с ротацией (по требованию)
- 🧩 **Простая интеграция** в существующие проекты
- 🛡 **Потокобезопасность** через `std::mutex`

## 📂 Структура проекта
project/
├── CMakeLists.txt # Конфигурация сборки
├── include/
│ └── logger.h # Интерфейс логгера
├── src/
│ └── file_logger.cpp # Реализация файлового логгера
├── example/
│ └── main.cpp # Пример использования
└── tests/ # Тесты 

## 🚀 Быстрый старт

### Сборка проекта

```bash
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
make
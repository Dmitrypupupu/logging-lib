# Библиотека логирования

![C++](https://img.shields.io/badge/C++-11%2F14%2F17-blue)
![CMake](https://img.shields.io/badge/CMake-3.10%2B-brightgreen)
![Лицензия](https://img.shields.io/badge/Лицензия-MIT-green)

Легковесная библиотека логирования на C++ с записью в файл и настраиваемыми уровнями логирования.

## Возможности

- 📝 Четыре уровня логирования: DEBUG, INFO, WARNING, ERROR
- ⏱️ Автоматическое добавление временных меток
- 🔒 Потокобезопасная реализация
- 📁 Запись в файл с настраиваемым путем
- 🎚 Возможность изменения уровня логирования во время выполнения
- 🏗 Простая интеграция в CMake-проекты

## Требования

- Компилятор с поддержкой C++14 (g++, clang++, MSVC)
- CMake 3.10+
- На macOS: Xcode Command Line Tools

## Установка и сборка

### Linux/macOS

```bash
# Клонировать репозиторий
git clone https://github.com/yourusername/logging_lib.git
cd logging_lib

# Собрать проект
mkdir build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release
cmake --build .

# Запустить пример
./bin/logging_example

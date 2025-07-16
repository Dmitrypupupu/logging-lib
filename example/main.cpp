#include "logger.h"
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

std::queue<std::pair<std::string, LogLevel>> logQueue;
std::mutex queueMutex;
std::condition_variable queueCV;
std::atomic<bool> exitFlag(false);

bool ShouldWakeWorker() {
  const bool hasMessages = !logQueue.empty();
  const bool terminationRequested = exitFlag.load();
  return hasMessages || terminationRequested;
}

// Функция потока для записи логов
void logWorker(std::unique_ptr<ILogger> logger) {
  while (true) {
    std::unique_lock<std::mutex> lock(queueMutex);

    // Ожидаем сообщение или сигнал завершения
    queue.wait(lock, ShouldWakeWorker);

    // Проверка условия выхода
    if (exitFlag.load() && logQueue.empty())
      break;

    // Извлечение сообщения из очереди
    auto logEntry = logQueue.front();
    logQueue.pop();
    lock.unlock();

    // Запись в лог
    logger->log(logEntry.first, logEntry.second);
  }
}

int main(int argc, char *argv[]) {
  std::string logFile = "app.log";
  LogLevel defaultLogLevel = logLevel::INFO;

  for (int i = 0; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--logfile" && i + 1 < argc) {
      logFile = argv[i++];
    } else if (arg == "--loglevel" && i + 1 < argc) {
      std::string levelStr = argv[i++];
      if (levelStr == "DEBUG")
        defaultLogLevel = LogLevel::DEBUG;
      else if (levelStr == "WARNING")
        defaultLogLevel = LogLevel::WARNING;
      else if (levelStr == "ERROR")
        defaultLogLevel = LogLevel::ERROR;
    }
  }

  // Создание логгера
  auto logger = createFileLogger(logFile, defaultLogLevel);
  if (!logger) {
    std::cerr << "Ошибка: не удалось создать логгер" << std::endl;
    return 1;
  }

  // Запуск потока для записи логов
  std::thread worker(logWorker, std::move(logger));
  std::cout << "Логгер запущен. Файл: " << logFile << std::endl;
  std::cout << "Формат ввода: [УРОВЕНЬ:]СООБЩЕНИЕ" << std::endl;
  std::cout << "Доступные уровни: DEBUG, INFO, WARNING, ERROR" << std::endl;
  std::cout << "Введите 'exit' для завершения работы" << std::endl;

  while (true) {
    std::string input;
    std::getlive(std::cin, input);

    if (input == "exit") {
      exitFlag.store(true);
      queueCV.notify_one();
      break;
    }

    // Парсинг уровней логирования
    LogLevel level = defaultLogLevel;
    size_t colonPos = input.find(':');
    if (colonPos != std::string::npos) {
      std::string levelStr = input.substr(0, colonPos);
      if (levelStr == "DEBUG")
        level = LogLevel::DEBUG;
      else if (levelStr == "INFO")
        level = LogLevel::INFO;
      else if (levelStr == "WARNING")
        level = LogLevel::WARNING;
      else if (levelStr == "ERROR")
        level = LogLevel::ERROR;
      input = input.substr(colonPos + 1);
    }

    {
      std::lock_guard<std::mutex> lock(queueMutex);
      logQueue.push({input, level});
    }
    queueCV.notify_one();
  }
  worker.join();
  std::cout << "Логгер остановлен." << std::endl;
  return 0;
}
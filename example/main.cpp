#include "logger.h"
#include <atomic>
#include <condition_variable>
#include <iostream>
#include <mutex>
#include <queue>
#include <string>
#include <thread>

// Глобальные переменные для потокобезопасной очереди сообщений
std::queue<std::pair<std::string, LogLevel>> logQueue;  // Очередь сообщений
std::mutex queueMutex;                                  // Мьютекс для синхронизации доступа к очереди
std::condition_variable queueCV;                       // Условная переменная для ожидания сообщений
std::atomic<bool> exitFlag(false);                     // Флаг для завершения работы потока

/**
 * @brief Проверяет условия для пробуждения рабочего потока
 * @return true если есть сообщения в очереди или установлен флаг завершения
 * @retval true Поток должен проснуться
 * @retval false Поток должен продолжать ждать
 *
 * @details Используется в queueCV.wait() для проверки условий пробуждения
 */
bool ShouldWakeWorker() {
  const bool hasMessages = !logQueue.empty();
  const bool terminationRequested = exitFlag.load();
  return hasMessages || terminationRequested;
}

/**
 * @brief Рабочая функция потока для асинхронной записи логов
 * @param logger Умный указатель на объект логгера
 *
 * @details Бесконечно обрабатывает сообщения из очереди до получения флага завершения.
 * Потокобезопасно извлекает сообщения и записывает их через переданный логгер.
 */
void logWorker(std::unique_ptr<ILogger> logger) {
  while (true) {
    std::unique_lock<std::mutex> lock(queueMutex);

    queueCV.wait(lock, ShouldWakeWorker);

    if (exitFlag.load() && logQueue.empty())
      break;

    auto logEntry = logQueue.front();
    logQueue.pop();
    lock.unlock();

    logger->log(logEntry.first, logEntry.second);
  }
}

/**
 * @brief Главная функция приложения
 * @param argc Количество аргументов командной строки
 * @param argv Массив аргументов командной строки
 * @return Код завершения программы
 * @retval 0 Успешное завершение
 * @retval 1 Ошибка создания логгера
 */
int main(int argc, char *argv[]) {
  std::string logFile = "app.log";
  LogLevel defaultLogLevel = LogLevel::INFO;

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

  auto logger = createFileLogger(logFile, defaultLogLevel);
  if (!logger) {
    std::cerr << "Ошибка: не удалось создать логгер" << std::endl;
    return 1;
  }

  std::thread worker(logWorker, std::move(logger));
  
  std::cout << "Логгер запущен. Файл: " << logFile << std::endl;
  std::cout << "Формат ввода: [УРОВЕНЬ:]СООБЩЕНИЕ" << std::endl;
  std::cout << "Доступные уровни: DEBUG, INFO, WARNING, ERROR" << std::endl;
  std::cout << "Введите 'exit' для завершения работы" << std::endl;

  while (true) {
    std::string input;
    std::getline(std::cin, input);

    if (input == "exit") {
      exitFlag.store(true);
      queueCV.notify_one();
      break;
    }

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

/** @} */ // end of async_logger group
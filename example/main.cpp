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
 * @brief Проверяет, должен ли рабочий поток проснуться
 * @return true если есть сообщения или запрошено завершение работы
 */
bool ShouldWakeWorker() {
  const bool hasMessages = !logQueue.empty();
  const bool terminationRequested = exitFlag.load();
  return hasMessages || terminationRequested;
}

/**
 * @brief Функция рабочего потока для асинхронной записи логов
 * @param logger Указатель на объект логгера
 */
void logWorker(std::unique_ptr<ILogger> logger) {
  while (true) {
    std::unique_lock<std::mutex> lock(queueMutex);

    // Ожидаем сообщения или сигнала завершения
    queueCV.wait(lock, ShouldWakeWorker);

    // Проверяем условие завершения работы
    if (exitFlag.load() && logQueue.empty())
      break;

    // Извлекаем сообщение из очереди
    auto logEntry = logQueue.front();
    logQueue.pop();
    lock.unlock();  // Освобождаем мьютекс перед записью

    // Записываем сообщение в лог
    logger->log(logEntry.first, logEntry.second);
  }
}

int main(int argc, char *argv[]) {
  // Параметры по умолчанию
  std::string logFile = "app.log";
  LogLevel defaultLogLevel = LogLevel::INFO;

  // Разбор аргументов командной строки
  for (int i = 0; i < argc; i++) {
    std::string arg = argv[i];
    if (arg == "--logfile" && i + 1 < argc) {
      logFile = argv[i++];  // Получаем имя файла для логов
    } else if (arg == "--loglevel" && i + 1 < argc) {
      std::string levelStr = argv[i++];  // Получаем уровень логирования
      if (levelStr == "DEBUG")
        defaultLogLevel = LogLevel::DEBUG;
      else if (levelStr == "WARNING")
        defaultLogLevel = LogLevel::WARNING;
      else if (levelStr == "ERROR")
        defaultLogLevel = LogLevel::ERROR;
    }
  }

  // Создаем логгер
  auto logger = createFileLogger(logFile, defaultLogLevel);
  if (!logger) {
    std::cerr << "Ошибка: не удалось создать логгер" << std::endl;
    return 1;
  }

  // Запускаем рабочий поток
  std::thread worker(logWorker, std::move(logger));
  
  // Выводим инструкции для пользователя
  std::cout << "Логгер запущен. Файл: " << logFile << std::endl;
  std::cout << "Формат ввода: [УРОВЕНЬ:]СООБЩЕНИЕ" << std::endl;
  std::cout << "Доступные уровни: DEBUG, INFO, WARNING, ERROR" << std::endl;
  std::cout << "Введите 'exit' для завершения работы" << std::endl;

  // Основной цикл обработки ввода пользователя
  while (true) {
    std::string input;
    std::getline(std::cin, input);

    // Проверяем команду выхода
    if (input == "exit") {
      exitFlag.store(true);    // Устанавливаем флаг завершения
      queueCV.notify_one();    // Будим рабочий поток
      break;
    }

    // Разбираем ввод пользователя
    LogLevel level = defaultLogLevel;
    size_t colonPos = input.find(':');
    
    // Если указан уровень логирования
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
      input = input.substr(colonPos + 1);  // Извлекаем текст сообщения
    }

    // Добавляем сообщение в очередь
    {
      std::lock_guard<std::mutex> lock(queueMutex);
      logQueue.push({input, level});
    }
    queueCV.notify_one();  // Уведомляем рабочий поток о новом сообщении
  }

  // Завершаем работу
  worker.join();  // Ожидаем завершения рабочего потока
  std::cout << "Логгер остановлен." << std::endl;
  return 0;
}
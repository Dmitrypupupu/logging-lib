#include <iostream>
#include "logger.h"

int main() {
    try {
        // Создаем логгер
        auto logger = createFileLogger("application.log", LogLevel::DEBUG);
        
        // Логируем сообщения разных уровней
        logger->log("Приложение запущено");
        logger->log("Отладочная информация", LogLevel::DEBUG);
        logger->log("Предупреждение: мало памяти", LogLevel::WARNING);
        logger->log("Ошибка: файл не найден", LogLevel::ERROR);
        
        // Меняем уровень логирования
        logger->setLogLevel(LogLevel::WARNING);
        logger->log("Это сообщение не будет записано", LogLevel::DEBUG);
        
        std::cout << "Логирование завершено. Проверьте файл application.log" << std::endl;
    } catch (const std::exception& e) {
        std::cerr << "Ошибка: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
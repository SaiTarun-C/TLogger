<<<<<<< HEAD
#include "Logger.h"

int main() {
    Logger& logger = Logger::getInstance();
    logger.setLogFile("output.log");
    logger.setLogLevel(LogLevel::DEBUG);
    logger.enableJsonOutput(false);
    logger.enableConsoleColor(true);

    LOG_INFO("This is an info message.");
    LOG_WARN("This is a warning message.");
    LOG_ERROR("This is an error message.");
    LOG_DEBUG("This is a debug message.");

    return 0;
}
=======
#include "Logger.h"

int main() {
    Logger& logger = Logger::getInstance();
    logger.setLogFile("output.log");
    logger.setLogLevel(LogLevel::DEBUG);
    logger.enableJsonOutput(false);
    logger.enableConsoleColor(true);

    LOG_INFO("This is an info message.");
    LOG_WARN("This is a warning message.");
    LOG_ERROR("This is an error message.");
    LOG_DEBUG("This is a debug message.");

    return 0;
}
>>>>>>> 8aa60b95cd7ef250696d06e67e82c5621805fc45

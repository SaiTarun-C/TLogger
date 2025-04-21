#ifndef LOGGER_H
#define LOGGER_H

#include <string>
#include <fstream>
#include <mutex>
#include <queue>
#include <thread>
#include <condition_variable>
#include <atomic>
#include <iostream>
#include <filesystem>
#include <vector>

enum class LogLevel {
    INFO,
    WARNING,
    LOG_ERROR,
    DEBUG
};

class Logger {
public:
    static Logger& getInstance();

    void setLogFile(const std::string& filename);
    void log(LogLevel level, const std::string& message, const std::string& file, int line, const std::string& function = "", bool encryptMessage = false);
    void setLogLevel(LogLevel level);
    void enableJsonOutput(bool enable);
    void enableConsoleColor(bool enable);

    // Function entry and exit logs
    void logFunctionEntry(const std::string& function, const std::string& file, int line);
    void logFunctionExit(const std::string& function, const std::string& file, int line);

    // Print log header
    void printLogHeader();

private:
    Logger();
    ~Logger();

    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    void processQueue();
    void rotateLogsIfNeeded();
    void compressOldLogs(const std::string& filename);
    std::string getTimestamp();
    std::string logLevelToString(LogLevel level);
    std::string encrypt(const std::string& text);
    std::string toJson(LogLevel level, const std::string& message, const std::string& file, int line, const std::string& function);
    std::string getColorCode(LogLevel level);

    std::ofstream logFile;
    std::mutex logMutex;
    std::condition_variable cv;
    std::queue<std::string> logQueue;
    std::thread logThread;
    std::atomic<bool> running;
    std::string currentLogFile;
    LogLevel currentLevel;
    bool jsonOutput;
    bool colorConsole;
    static constexpr size_t maxFileSize = 1024 * 1024 * 1024;  // 1GB
};

// Logging macros with file, line, and function context
#define LOG_INFO(msg)  Logger::getInstance().log(LogLevel::INFO, msg, __FILE__, __LINE__, __FUNCTION__)
#define LOG_WARN(msg)  Logger::getInstance().log(LogLevel::WARNING, msg, __FILE__, __LINE__, __FUNCTION__)
#define LOG_ERROR(msg) Logger::getInstance().log(LogLevel::LOG_ERROR, msg, __FILE__, __LINE__, __FUNCTION__)
#define LOG_DEBUG(msg) Logger::getInstance().log(LogLevel::DEBUG, msg, __FILE__, __LINE__, __FUNCTION__)

#define LOG_FUNCTION_ENTRY() Logger::getInstance().logFunctionEntry(__FUNCTION__, __FILE__, __LINE__)
#define LOG_FUNCTION_EXIT()  Logger::getInstance().logFunctionExit(__FUNCTION__, __FILE__, __LINE__)

#endif // LOGGER_H

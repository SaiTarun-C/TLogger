// Logger.h - No changes in header file needed.

#include "Logger.h"
#include <chrono>
#include <ctime>
#include <sstream>
#include <iomanip>
#include <iostream>
#include <filesystem>
#include <fstream>
#include <zlib.h>
#include <thread>
#include <queue>
#include <condition_variable>
#include <mutex>
#include <vector>

#ifdef _WIN32
#include <windows.h>
#endif

// ANSI color codes
#ifndef _WIN32
const std::string RESET_COLOR = "\033[0m";
const std::string INFO_COLOR = "\033[32m";
const std::string WARNING_COLOR = "\033[33m";
const std::string ERROR_COLOR = "\033[31m";
const std::string DEBUG_COLOR = "\033[36m";
#endif

#ifdef _WIN32
void setConsoleColor(int color) {
    SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
}
#else
void setConsoleColor(const std::string& color) {
    std::cout << color;
}
#endif

// Base64 Encoder
std::string base64Encode(const std::vector<unsigned char>& input) {
    static const char* base64_chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    std::string encoded;
    int val = 0;
    int valb = -6;
    for (unsigned char c : input) {
        val = (val << 8) + c;
        valb += 8;
        while (valb >= 0) {
            encoded.push_back(base64_chars[(val >> valb) & 0x3F]);
            valb -= 6;
        }
    }
    if (valb > -6) encoded.push_back(base64_chars[((val << 8) >> (valb + 8)) & 0x3F]);
    while (encoded.size() % 4) encoded.push_back('=');
    return encoded;
}

Logger::Logger() : running(true), currentLevel(LogLevel::DEBUG), jsonOutput(false), colorConsole(false) {
    // Call printLogHeader to print header when the logger is initialized
    printLogHeader();

    logThread = std::thread(&Logger::processQueue, this);
}

Logger::~Logger() {
    {
        std::lock_guard<std::mutex> lock(logMutex);
        running = false;
    }
    cv.notify_all();
    if (logThread.joinable()) {
        logThread.join();
    }
    if (logFile.is_open()) {
        logFile.close();
    }
}

Logger& Logger::getInstance() {
    static Logger instance;
    return instance;
}

void Logger::setLogFile(const std::string& filename) {
    std::lock_guard<std::mutex> lock(logMutex);
    if (logFile.is_open()) {
        logFile.close();
    }
    currentLogFile = filename;
    logFile.open(filename, std::ios::app);
    if (!logFile) {
        std::cerr << "Failed to open log file: " << filename << std::endl;
    }
}

void Logger::setLogLevel(LogLevel level) {
    currentLevel = level;
}

void Logger::enableJsonOutput(bool enable) {
    jsonOutput = enable;
}

void Logger::enableConsoleColor(bool enable) {
    colorConsole = enable;
}

void Logger::log(LogLevel level, const std::string& message, const std::string& file, int line, const std::string& function, bool encryptMessage) {
    if (level < currentLevel) return;

    std::ostringstream oss;
    oss << "[" << getTimestamp() << "] [" << logLevelToString(level) << "] [" 
        << file << ":" << line << "]";

    if (!function.empty()) {
        oss << " [Function: " << function << "]";
    }

    oss << " " << message;

    std::string logMessage = oss.str();

    // Encrypt if needed
    if (encryptMessage) {
        logMessage = encrypt(logMessage);
    }

    {
        std::lock_guard<std::mutex> lock(logMutex);
        logQueue.push(logMessage);
    }
    cv.notify_one();
}

void Logger::logFunctionEntry(const std::string& function, const std::string& file, int line) {
    log(LogLevel::DEBUG, "Entering function.", file, line, function);
}

void Logger::logFunctionExit(const std::string& function, const std::string& file, int line) {
    log(LogLevel::DEBUG, "Exiting function.", file, line, function);
}

void Logger::processQueue() {
    while (true) {
        std::unique_lock<std::mutex> lock(logMutex);
        cv.wait(lock, [this]() { return !logQueue.empty() || !running; });
        if (!running && logQueue.empty()) break;

        while (!logQueue.empty()) {
            rotateLogsIfNeeded();
            std::string entry = logQueue.front();

            if (logFile.is_open()) {
                logFile << entry << std::endl;
            }

            if (colorConsole) {
#ifndef _WIN32
                std::cout << getColorCode(currentLevel) << entry << RESET_COLOR << std::endl;
#else
                std::cout << entry << std::endl;
#endif
            } else {
                std::cout << entry << std::endl;
            }

            logQueue.pop();
        }
    }
}

void Logger::rotateLogsIfNeeded() {
    if (!currentLogFile.empty() && std::filesystem::exists(currentLogFile) &&
        std::filesystem::file_size(currentLogFile) > maxFileSize) {
        std::string rotatedName = currentLogFile + ".old";
        logFile.close();
        std::filesystem::rename(currentLogFile, rotatedName);
        compressOldLogs(rotatedName);
        logFile.open(currentLogFile, std::ios::app);
    }
}

std::string Logger::getTimestamp() {
    auto now = std::chrono::system_clock::now();
    std::time_t t = std::chrono::system_clock::to_time_t(now);
    std::tm tmBuf{};
#ifdef _WIN32
    localtime_s(&tmBuf, &t);
#else
    localtime_r(&t, &tmBuf);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tmBuf, "%Y-%m-%d %H:%M:%S");
    return oss.str();
}

std::string Logger::logLevelToString(LogLevel level) {
    switch (level) {
        case LogLevel::INFO: return "INFO";
        case LogLevel::WARNING: return "WARNING";
        case LogLevel::LOG_ERROR: return "ERROR";
        case LogLevel::DEBUG: return "DEBUG";
        default: return "UNKNOWN";
    }
}

std::string Logger::encrypt(const std::string& text) {
    std::string encrypted = text;
    for (char& c : encrypted) {
        c ^= 0xAA;  // XOR encryption (still valid)
    }

    // Convert the encrypted string to Base64 for safe logging
    std::vector<unsigned char> encryptedBytes(encrypted.begin(), encrypted.end());
    return base64Encode(encryptedBytes);
}

std::string Logger::toJson(LogLevel level, const std::string& message, const std::string& file, int line, const std::string& function) {
    std::ostringstream oss;
    oss << "{\"timestamp\": \"" << getTimestamp() << "\"," 
        << " \"level\": \"" << logLevelToString(level) << "\"," 
        << " \"file\": \"" << file << "\"," 
        << " \"line\": " << line << ","
        << " \"function\": \"" << function << "\"," 
        << " \"message\": \"" << message << "\"}";
    return oss.str();
}

std::string Logger::getColorCode(LogLevel level) {
#ifndef _WIN32
    switch (level) {
        case LogLevel::INFO: return INFO_COLOR;
        case LogLevel::WARNING: return WARNING_COLOR;
        case LogLevel::LOG_ERROR: return ERROR_COLOR;
        case LogLevel::DEBUG: return DEBUG_COLOR;
        default: return RESET_COLOR;
    }
#else
    return ""; // No color codes on Windows (unless using Console API)
#endif
}

void Logger::compressOldLogs(const std::string& filename) {
    std::ifstream inFile(filename, std::ios::binary);
    std::ofstream outFile(filename + ".gz", std::ios::binary);
    if (inFile && outFile) {
        std::string input((std::istreambuf_iterator<char>(inFile)), std::istreambuf_iterator<char>());
        uLongf compressedSize = compressBound(input.size());
        std::vector<char> compressed(compressedSize);
        if (compress((Bytef*)compressed.data(), &compressedSize,
                     (const Bytef*)input.data(), input.size()) == Z_OK) {
            outFile.write(compressed.data(), compressedSize);
            inFile.close();
            outFile.close();
            std::filesystem::remove(filename);
        }
    } else {
        std::cerr << "Failed to open file for compression: " << filename << std::endl;
    }
}

void Logger::printLogHeader() {
    std::cout << "You are using Tarun's Logger Library for checking logs" << std::endl;
    std::cout << "Logs Start from here" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------------------------------" << std::endl;
    std::cout << "---------------------------------------------- LOGS ------------------------------------------------------" << std::endl;
    std::cout << "-----------------------------------------------------------------------------------------------------------" << std::endl;
}

# TLogger
A high-performance, cross-platform logging library

# TLogger Library

**TLogger** is a high-performance, cross-platform logging library that provides flexible, customizable, and secure logging features. It supports log rotation, encryption, asynchronous logging, and more.
It is a C++ logging library designed to simplify logging for any project. It supports various advanced features such as log rotation, asynchronous logging, log level filtering, and more. This library is easy to integrate and highly customizable, making it suitable for both small and large-scale applications.

## Features

- **Cross-Platform**: Works on Windows, Linux, and macOS.
- **Asynchronous Logging**: Offloads log writing to a background thread to minimize performance overhead.
- **Custom Log Levels**: Support for INFO, WARNING, ERROR, and DEBUG log levels.
- **Log Rotation**: Automatically rotates logs when the size exceeds the set limit.
- **Encryption**: Encrypt log messages for secure logging.
- **JSON Output**: Option to output logs in JSON format.
- **Console Colors**: Colorized log output for easier debugging in the console.

--------------------------------------------
############## Installation ################
--------------------------------------------
#Prerequisites
Before using TLogger, you need to have the following installed:

C++ Compiler (GCC, MSVC, Clang)
CMake (for building the project)

Building the Library
#Clone the repository:
git clone https://github.com/SaiTarun-C/TLogger.git
cd TLogger

#Create a build directory:
mkdir build
cd build

#Run CMake to configure the project:
cmake ..

#Build the project:
cmake --build .

This will generate the necessary binaries in the build folder.

Usage
Integrating TLogger into Your Project

#Include the header file in your source code:
#include "Logger.h"

#Initialize the logger in your application:
Logger logger("logfile.log", Logger::LogLevel::DEBUG);

#Use the logger to log messages:
logger.log(Logger::LogLevel::INFO, "This is an informational message.");
logger.log(Logger::LogLevel::ERROR, "An error occurred.");

#To log function entries and exits:
#define LOG_FUNC_ENTRY  // Will log function entry
#define LOG_FUNC_EXIT   // Will log function exit

void someFunction() {
    LOG_FUNC_ENTRY;
    // Function logic
    LOG_FUNC_EXIT;
}

#Log Level Filtering
You can filter log messages based on the log level. For example, you can set the logger to only log messages with a severity level of INFO or higher:
logger.setLogLevel(Logger::LogLevel::INFO);

#Remote Logging
To enable remote logging, configure the logger with the server IP and port:
logger.setRemoteLogging("192.168.1.100", 12345);
This will send the logs to the specified remote server.

#Contributing
If you'd like to contribute to TLogger, feel free to fork the repository, make changes, and submit a pull request. All contributions are welcome!

Fork the repository
Create a new branch (git checkout -b feature-name)
Make your changes
Commit your changes (git commit -am 'Add new feature')
Push to the branch (git push origin feature-name)
Create a new Pull Request

#License
This project is licensed under the MIT License - see the LICENSE file for details.

<<<<<<< HEAD
cmake_minimum_required(VERSION 3.15)

project(LoggerLib)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(ZLIB REQUIRED)

include_directories(${ZLIB_INCLUDE_DIRS})

add_executable(logger_app main.cpp src/Logger.cpp)

target_link_libraries(logger_app PRIVATE ZLIB::ZLIB)
=======
cmake_minimum_required(VERSION 3.15)

project(LoggerLib)

set(CMAKE_CXX_STANDARD 17)

include_directories(include)

find_package(ZLIB REQUIRED)

include_directories(${ZLIB_INCLUDE_DIRS})

add_executable(logger_app main.cpp src/Logger.cpp)

target_link_libraries(logger_app PRIVATE ZLIB::ZLIB)
>>>>>>> 8aa60b95cd7ef250696d06e67e82c5621805fc45

CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra
LIBS = -lz   # Link against zlib

SRC = src/Logger.cpp main.cpp
TARGET = logger_app

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET) *.o *.log

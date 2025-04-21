<<<<<<< HEAD
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra
LIBS = -lz   # Link against zlib

SRC = src/Logger.cpp main.cpp
TARGET = logger_app

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET) *.o *.log
=======
CXX = g++
CXXFLAGS = -std=c++17 -Iinclude -Wall -Wextra
LIBS = -lz   # Link against zlib

SRC = src/Logger.cpp main.cpp
TARGET = logger_app

all:
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LIBS)

clean:
	rm -f $(TARGET) *.o *.log
>>>>>>> 8aa60b95cd7ef250696d06e67e82c5621805fc45

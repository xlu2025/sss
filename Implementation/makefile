# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -std=c++11 -Wall -g

# Target executable
TARGET = main

# Source files
SOURCES = main.cpp user.cpp event.cpp reservation.cpp facility_manager.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Header files
HEADERS = user.h event.h reservation.h facility_manager.h

# Default target
all: $(TARGET)

# Rule to link the target executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $@ $^

# Rule to compile source files into object files
%.o: %.cpp $(HEADERS)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Rebuild everything
rebuild: clean all

.PHONY: all clean rebuild

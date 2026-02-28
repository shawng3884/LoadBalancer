# Compiler and flags
CXX = g++
CXXFLAGS = -Wall -Werror -std=c++11

# Target executable
TARGET = loadbalancer

# Source files
SOURCES = main.cpp Request.cpp WebServer.cpp LoadBalancer.cpp

# Object files
OBJECTS = $(SOURCES:.cpp=.o)

# Default target
all: $(TARGET)

# Link object files to create executable
$(TARGET): $(OBJECTS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJECTS)

# Compile source files to object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean target to remove generated files
clean:
	rm -f $(TARGET) $(OBJECTS)

# Phony targets
.PHONY: all clean

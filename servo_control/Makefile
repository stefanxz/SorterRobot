# Makefile for servo_control using wiringPi

# Compiler to use
CXX = g++

# Compiler flags
CXXFLAGS = -Wall

# Linker flags
LDFLAGS = -lwiringPi

# Target executable
TARGET = main

# Source files
SRC = main.cpp

# Object files
OBJ = $(SRC:.cpp=.o)

# Build target
$(TARGET): $(OBJ)
	$(CXX) $(OBJ) -o $@ $(LDFLAGS)

# Rule to build object files
%.o: %.cpp
	$(CXX) -c $< -o $@ $(CXXFLAGS)

# Clean up
clean:
	rm -f $(OBJ) $(TARGET)

# Rule for 'make all'
all: $(TARGET)

# Run the program
run: $(TARGET)
	./$(TARGET)

# Phony targets
.PHONY: clean all run

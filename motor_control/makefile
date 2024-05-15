# Compiler settings
CXX=g++
CXXFLAGS=-I. -std=c++11 -Wall

# Linker settings
LDFLAGS=-lwiringPi -lpthread

# Source files in the src/ directory
SOURCES=main.cpp

# Object files
OBJECTS=$(SOURCES:.cpp=.o)

# Executable name
EXECUTABLE=MotorControl

all: $(SOURCES) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(OBJECTS) -o $@ $(LDFLAGS)

.cpp.o:
	$(CXX) -c $< -o $@ $(CXXFLAGS)

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: all clean run

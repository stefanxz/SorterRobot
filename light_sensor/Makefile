CC=g++
CFLAGS=-c -Wall
LDFLAGS=-lwiringPi
SOURCES=main.cpp ADCReader.cpp
OBJECTS=$(SOURCES:.cpp=.o)
EXECUTABLE=adc_test

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) $(LDFLAGS) -o $@

.cpp.o:
	$(CC) $(CFLAGS) $< -o $@

clean:
	rm -f $(OBJECTS) $(EXECUTABLE)

run: $(EXECUTABLE)
	./$(EXECUTABLE)

.PHONY: clean run

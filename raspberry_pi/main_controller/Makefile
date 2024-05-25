# Compiler settings - Can be customized.
CC = g++
CFLAGS = -I../util -lwiringPi

# Define the directories.
SRC_DIR = .
UTIL_DIR = ../util
DISPLAY_DIR = $(UTIL_DIR)/display_control
LIGHT_SENSOR_DIR = $(UTIL_DIR)/light_sensor
MOTOR_CONTROL_DIR = $(UTIL_DIR)/motor_control
SERVO_CONTROL_DIR = $(UTIL_DIR)/servo_control

# Define all object files.
OBJ = main.o \
      DisplayController.o \
      ADCReader.o \
      MotorController.o \
      Servo.o

# The build target executable:
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Main file
main.o: $(SRC_DIR)/main.cpp $(SERVO_CONTROL_DIR)/Servo.h
	$(CC) -c $< -o $@ $(CFLAGS)

# Display Controller
DisplayController.o: $(DISPLAY_DIR)/DisplayController.cpp $(DISPLAY_DIR)/DisplayController.h
	$(CC) -c $< -o $@ $(CFLAGS)

# ADC Reader
ADCReader.o: $(LIGHT_SENSOR_DIR)/ADCReader.cpp $(LIGHT_SENSOR_DIR)/ADCReader.h
	$(CC) -c $< -o $@ $(CFLAGS)

# Motor Controller
MotorController.o: $(MOTOR_CONTROL_DIR)/MotorControl.cpp $(MOTOR_CONTROL_DIR)/MotorController.h
	$(CC) -c $< -o $@ $(CFLAGS)

# Servo Controller
Servo.o: $(SERVO_CONTROL_DIR)/Servo.cpp $(SERVO_CONTROL_DIR)/Servo.h
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run
# Compiler settings - Can be customized.
CC = g++
CFLAGS = -Wall -Wextra -std=c++11 -lwiringPi

# Define the directories.
SRC_DIR = .
UTIL_DIR = ../util
DISPLAY_DIR = $(UTIL_DIR)/display_control
LIGHT_SENSOR_DIR = $(UTIL_DIR)/light_sensor
MOTOR_CONTROL_DIR = $(UTIL_DIR)/motor_control
SERVO_CONTROL_DIR = $(UTIL_DIR)/servo_control
LASER_SENSOR_DIR = $(UTIL_DIR)/laser_sensor

# Include directories
INCLUDES = -I$(UTIL_DIR) -I$(DISPLAY_DIR) -I$(LIGHT_SENSOR_DIR) -I$(MOTOR_CONTROL_DIR) -I$(SERVO_CONTROL_DIR) -I$(LASER_SENSOR_DIR)

# Define all object files.
OBJ = main.o \
      DisplayController.o \
      ADCReader.o \
      MotorController.o \
      Servo.o \
      LaserReceiver.o

# The build target executable:
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS)

# Main file
main.o: $(SRC_DIR)/main.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

# Display Controller
DisplayController.o: $(DISPLAY_DIR)/DisplayController.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

# ADC Reader
ADCReader.o: $(LIGHT_SENSOR_DIR)/ADCReader.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

# Motor Controller
MotorController.o: $(MOTOR_CONTROL_DIR)/MotorController.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

# Servo Controller
Servo.o: $(SERVO_CONTROL_DIR)/Servo.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

# Laser Receiver
LaserReceiver.o: $(LASER_SENSOR_DIR)/LaserReceiver.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(INCLUDES)

run: $(TARGET)
	./$(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run
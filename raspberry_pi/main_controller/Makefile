# Compiler settings - Can be customized.
CC = g++

CFLAGS = -I../../util -I../network -lwiringPi


# Define the directories relative to the Makefile location.
SRC_DIR = .
UTIL_DIR = ../util
NETWORK_DIR = ../network
DISPLAY_DIR = $(UTIL_DIR)/display_control
LIGHT_SENSOR_DIR = $(UTIL_DIR)/light_sensor
MOTOR_CONTROL_DIR = $(UTIL_DIR)/motor_control
SERVO_CONTROL_DIR = $(UTIL_DIR)/servo_control
LASER_SENSOR_DIR = $(UTIL_DIR)/laser_sensor
LASER_TRANSMITTER_DIR = $(UTIL_DIR)/laser_transmitter

# Include directories
INCLUDES = -I$(UTIL_DIR) -I$(DISPLAY_DIR) -I$(LIGHT_SENSOR_DIR) -I$(MOTOR_CONTROL_DIR) -I$(SERVO_CONTROL_DIR) -I$(LASER_SENSOR_DIR)

# Define all object files.
OBJ = main.o \
      SorterRobot.o \
      DisplayController.o \
      ADCReader.o \
      MotorController.o \
      Servo.o \
      LaserReceiver.o \
      LaserTransmitter.o \
      CarController.o

# The build target executable:
TARGET = main

all: $(TARGET)

$(TARGET): $(OBJ)
	@echo "Linking..."
	$(CC) -o $@ $^ $(CFLAGS) -lcurl

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
  
main.o: $(SRC_DIR)/main.cpp $(SRC_DIR)/SorterRobot.h $(SERVO_CONTROL_DIR)/ServoController.h $(NETWORK_DIR)/CarController.h
	@echo "Compiling main.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

# SorterRobot
SorterRobot.o: $(SRC_DIR)/SorterRobot.cpp $(SRC_DIR)/SorterRobot.h
	@echo "Compiling SorterRobot.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Display Controller
DisplayController.o: $(DISPLAY_DIR)/DisplayController.cpp $(DISPLAY_DIR)/DisplayController.h
	@echo "Compiling DisplayController.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

# ADC Reader
ADCReader.o: $(LIGHT_SENSOR_DIR)/ADCReader.cpp $(LIGHT_SENSOR_DIR)/ADCReader.h
	@echo "Compiling ADCReader.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Motor Controller
MotorController.o: $(MOTOR_CONTROL_DIR)/MotorController.cpp $(MOTOR_CONTROL_DIR)/MotorController.h
	@echo "Compiling MotorController.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Servo Controller
Servo.o: $(SERVO_CONTROL_DIR)/ServoController.cpp $(SERVO_CONTROL_DIR)/ServoController.h
	@echo "Compiling Servo.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Laser Receiver
LaserReceiver.o: $(LASER_SENSOR_DIR)/LaserReceiver.cpp $(LASER_SENSOR_DIR)/LaserReceiver.h
	@echo "Compiling LaserReceiver.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

# Laser Transmitter
LaserTransmitter.o: $(LASER_TRANSMITTER_DIR)/LaserTransmitter.cpp $(LASER_TRANSMITTER_DIR)/LaserTransmitter.h
	@echo "Compiling LaserTransmitter.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

# CarController
CarController.o: $(NETWORK_DIR)/CarController.cpp $(NETWORK_DIR)/CarController.h
	@echo "Compiling CarController.o..."
	$(CC) -c $< -o $@ $(CFLAGS)

run: $(TARGET)
	@echo "Running the application..."
	./$(TARGET)

clean:
	@echo "Cleaning up..."
	rm -f $(OBJ) $(TARGET)

.PHONY: all clean run
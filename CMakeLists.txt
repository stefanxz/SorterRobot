cmake_minimum_required(VERSION 3.28)
project(SorterRobot)

set(CMAKE_CXX_STANDARD 14)

include_directories(motor_control/src)
include_directories(servo_control)

add_executable(SorterRobot
        motor_control/src/main.cpp
        motor_control/src/MotorController.h
        servo_control/main.cpp
        servo_control/Servo.h)

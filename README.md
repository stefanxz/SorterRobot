# Sorting Robot & RC Car Project

## Course: CBL Embedded Systems

## Introduction

This repository hosts the collaborative university project for the CBL Embedded Systems course. It involves a sorting robot and an RC car designed to simulate an airport baggage handling system, utilizing a Raspberry Pi and Arduino Uno to handle respective tasks.

## Team Members

- Stefan Avram
- Lia Banuta
- Razvan Craciun
- Madalina Diaconescu-Carbunescu
- Ilinca Gurgu
- Kleon Oshafi

## Project Description

This project combines mechanical design, electronics, and programming to create an automated system that sorts and delivers disks based on size and color criteria. The sorting robot uses various sensors and actuators managed by a Raspberry Pi, while the RC car, controlled by an Arduino Uno, transports sorted disks to designated gates.

## System Components

### Sorting Robot

- **Raspberry Pi**: Manages sorting logic and sensor data processing.
- **Conveyor Belts**: Move disks through the sorting stages powered by electric motors.
- **Sensors**:
    - **Laser Sensors**: Detect blockages and ensure proper sorting.
    - **Light Sensor**: Determines disk color for sorting purposes.
- **Piston Mechanism**: Ejects disks onto the RC car conveyor based on timing sequences.
- **Display**: Shows the disk's color and the gate it needs to be delivered to.

### Automated RC Car

- **Arduino Uno**: Controls the car's navigation and pickup mechanisms.
- **Laser Sensors**: Confirm the car's alignment with the pickup zone.
- **Servo Motors**: Provide movement capabilities for disk pickup and delivery.

## Learning Objectives

- Implement embedded systems combining both hardware components and software programming.
- Develop an understanding of system integration involving real-time sensors and actuators.
- Apply practical skills in programming microcontrollers in C++ within a project-based learning environment.
- Enhance problem-solving skills through troubleshooting and optimizing automated systems.

## System Operation

1. **Sorting Phase**: Disks are sorted by the robot based on pre-defined criteria using height, width, and color.
2. **Delivery Phase**: The RC car automatically picks up and delivers disks to designated gates, identified by color.

## Setup Instructions

- **Initialization**: Power up systems and ensure the Raspberry Pi and Arduino Uno are correctly configured and operational.
- **Software Launch**: Start the control programs for both the sorting robot and the RC car.
- **Operation Monitoring**: Observe the sorting process and ensure smooth operation through the system’s display outputs.


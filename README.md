# Autonomous Line-Following Robot with Wireless Self-Charging Dock

An Arduino-based robotics project that combines **autonomous line following, battery-aware control, docking behavior, and wireless charging** into a single embedded robotic platform.

## Project Overview
This project focuses on the design and implementation of an **autonomous line-following robot** capable of:
- tracking a predefined path using an **IR sensor array**
- monitoring battery voltage in real time
- triggering return-to-dock behavior when battery level falls below a threshold
- docking with a **Qi-based wireless charging system**
- resuming operation after charging

The system was developed to demonstrate how robotics, embedded systems, and power management can be integrated to create a more **self-sustaining and autonomous mobile robot**.

## Key Features
- Autonomous line following using IR-based path sensing
- Centroid-based tracking for smoother path correction
- Battery voltage monitoring using Arduino ADC
- Return-to-dock logic triggered by low battery condition
- Wireless charging through Qi inductive charging
- State-based control for navigation, docking, charging, and resume operation

## Team Contributions
This project was completed as a collaborative team effort.

### Software Development
- **Manmeet Kaur**
- **Kalu Orji**

### Hardware Development
- **Dhvani Parikh**
- **Abhirami Chandran**

All team members also contributed to testing, calibration, documentation, and presentation work.

## My Contribution
My contribution focused on the **software development** side of the project in collaboration with **Kalu Orji**.

The software work was completed collaboratively on the same codebase through iterative development, debugging, testing, and refinement rather than through fully separate software modules.

My involvement included:
- Arduino programming for robot behavior
- line-following control logic
- battery-aware decision logic
- docking and charging-related software behavior
- testing and debugging of robot movement and transitions
- integration of control logic with the overall robotic system

## System Architecture
The robot system is built around three major subsystems:

### 1. Sensing Subsystem
- IR sensor array for line detection
- battery voltage sensing through resistor divider and Arduino ADC

### 2. Control Subsystem
- Arduino UNO as the main controller
- state-based logic for navigation, docking, charging, and resume operation

### 3. Actuation and Power Subsystem
- L298N motor driver
- DC geared motors and chassis
- 7.4V Li-ion battery pack
- Qi wireless charging dock

## Technologies Used
- Arduino / Embedded C++
- IR line sensors
- Arduino ADC for battery monitoring
- L298N motor driver
- Qi wireless charging integration
- state-machine-based control logic

## Repository Structure
```text
Autonomous-Line-Following-Robot/
├── README.md
├── arduino_code/
│   └── main.ino
├── docs/
│   ├── system_overview.md
│   └── dependencies.md
├── results/
│   ├── demo_notes.md
│   └── demo screenshots / result visuals
└── data/
    └── supporting data notes if included

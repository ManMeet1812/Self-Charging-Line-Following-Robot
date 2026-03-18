# System Overview

## Project Summary
This project is an Arduino-based autonomous line-following robot integrated with a self-charging docking system. The robot is designed to follow a predefined path, monitor its battery or charging condition, return to the docking station when required, and resume operation after charging.

## Objective
The goal of the system is to demonstrate how an autonomous mobile robot can combine navigation, embedded control, and battery-aware behavior in a single integrated platform.

## Main Functions
- line following using sensor-based path detection
- autonomous movement control
- battery-aware or charging-aware decision making
- docking behavior for charging
- resume operation after charging
- continuous embedded control through Arduino

## System Components
### Hardware
- Arduino-based controller
- line detection sensors
- motor driver and DC motors
- chassis and wheel assembly
- docking station
- charging setup
- supporting power and wiring components

### Software
- Arduino control logic
- sensor reading and processing
- motor control commands
- line-following behavior
- docking and charging logic
- decision flow for switching between movement and charging states

## Software Contribution
The software development was completed collaboratively by **Manmeet Kaur** and **Kalu Orji**. The codebase was developed through shared implementation, debugging, testing, and iterative refinement rather than fully separate software modules.

## Hardware Contribution
The hardware side of the project was completed by **Dhvani Parikh** and **Abhirami Chandran**, including the physical robot and docking station setup.

## Control Workflow
A simplified workflow of the system is:

1. Initialize sensors, motors, and control logic
2. Detect the line and perform path tracking
3. Continuously monitor robot state
4. If charging or low-power condition is triggered, switch to docking behavior
5. Align with the docking station
6. Charge or remain docked as required
7. Resume movement when the system is ready

## Engineering Focus
This project demonstrates concepts in:
- robotics
- embedded systems
- autonomous control
- hardware-software integration
- battery-aware robotic behavior

## Repository Purpose
This repository documents the Arduino code, project structure, and supporting materials related to the autonomous line-following robot and self-charging docking system.

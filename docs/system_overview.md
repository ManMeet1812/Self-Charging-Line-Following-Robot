# System Overview

## Project Summary
This project is an **Arduino-based autonomous line-following robot with wireless self-charging capability**. The system was designed to follow a predefined path using an **IR line sensor array** and a **centroid-based tracking approach**, monitor battery voltage in real time, return to a docking station when battery level falls below a threshold, charge through a **Qi-based wireless charging system**, and then resume operation without human intervention.

## Objective
The main objective of the system is to demonstrate how an autonomous mobile robot can integrate:
- accurate line-following navigation
- battery-aware decision making
- automated return-to-dock behavior
- wireless charging
- task resumption after charging

The project was developed to show how robotics, embedded control, and intelligent power management can be combined into a practical self-sustaining robotic platform. :contentReference[oaicite:0]{index=0}

## System Architecture
The robot is built around three main subsystems:

### 1. Sensing Subsystem
The sensing subsystem provides the information required for navigation and battery-aware control. It includes:
- **IR line sensor array** for line detection and path tracking
- **battery voltage monitoring circuit** using an ADC voltage divider
- supporting calibration and testing of sensors during integration

The IR sensor array continuously detects the floor path, while battery voltage sensing allows the software to classify the battery state and trigger return-to-dock behavior when needed. :contentReference[oaicite:1]{index=1}

### 2. Control Subsystem
The control subsystem is centered on the **Arduino UNO**, which acts as the main processing and decision-making unit. It reads sensor inputs, executes navigation logic, manages charging-related behavior, and coordinates transitions between robot operating states. The report describes the use of **state-machine-based control** to manage navigation, docking, charging, and task resumption automatically. 

### 3. Actuation and Power Subsystem
This subsystem handles movement and charging. It includes:
- **L298N motor driver**
- **DC geared motors and chassis**
- **7.4V Li-ion battery pack**
- **Qi wireless charging dock**

The motor driver enables speed and direction control of the robot, while the charging system supports contactless power transfer once the robot reaches the docking station. 

## Key Hardware Components
- **Arduino UNO** — main microcontroller for robot control and coordination
- **IR line sensor array** — detects the path and supports line following
- **battery voltage divider + ADC** — measures battery voltage safely for control logic
- **L298N motor driver** — controls motor speed and direction
- **DC motors and chassis** — provide robot motion and mobility
- **Qi wireless charging system** — enables docking-based inductive charging 

## Control Logic
The robot control logic is designed around autonomous state transitions. In general, the workflow is:

1. Initialize sensors, motors, and control states
2. Follow the predefined path using IR-based line tracking
3. Continuously monitor battery voltage during operation
4. Trigger return-to-dock behavior when battery voltage falls below threshold
5. Dock with the charging station and begin Qi wireless charging
6. Resume the interrupted task after charging is complete

The report identifies **state-machine control** as a core part of the design and also notes that the robot resumes its previous task after charging. 

## Navigation Method
The robot uses an **infrared line sensor array** and a **centroid-based tracking algorithm** to achieve smoother and more accurate path tracking than simple on-off control. This supports stable line following, accurate turning, and improved path-tracking performance during autonomous operation. 

## Battery Monitoring and Docking
Battery voltage is monitored using a resistor-divider-based sensing circuit connected to an Arduino analog input. Because the Li-ion battery voltage exceeds the Arduino’s direct analog input range, the voltage is scaled before being read by the ADC. The software then interprets the reading to determine battery condition and trigger docking when voltage falls below the defined limit. :contentReference[oaicite:7]{index=7}

## Charging System
The robot uses a **Qi-based wireless inductive charging system** to eliminate the need for physical charging contacts. This improves safety and reduces wear from repeated physical connection cycles. The report also notes that coil alignment is important, and that misalignment can reduce charging efficiency or prevent successful power transfer. 

## Team Contributions
This project was completed collaboratively.

### Software Development
- **Manmeet Kaur**
- **Kalu Orji**

Software responsibilities included:
- Arduino code development
- motor control logic
- sensor interfacing
- navigation logic
- auto-charging trigger logic
- debugging and integration of sensors and charging behavior 

### Hardware Development
- **Dhvani Parikh**
- **Abhirami Chandran**

Hardware responsibilities included:
- robot chassis assembly
- wiring and circuit connections
- battery, motor driver, and Qi charger integration
- safe and stable power system setup
- cable management and soldering 

### Shared Team Work
All team members contributed to:
- documentation and report writing
- presentation preparation
- testing, calibration, and optimization of sensors and system behavior 

## Safety and Reliability Notes
The robot operates using a **7.4V Li-ion battery pack**, so safe charging and voltage handling were important considerations. The report notes that performance depends on factors such as:
- floor texture
- ambient lighting
- path contrast quality
- wheel alignment
- docking coil alignment

These factors influence line-following reliability, battery detection accuracy, and charging success. Regular calibration and alignment checks improve consistency. :contentReference[oaicite:12]{index=12}

## Engineering Significance
This project demonstrates how **embedded systems, sensing, motor control, state-machine logic, and wireless power transfer** can be combined to create a practical autonomous robot. It also highlights the importance of hardware-software integration in achieving reliable, energy-aware robotic behavior for indoor automation applications such as delivery, laboratory transport, and warehouse use. :contentReference[oaicite:13]{index=13}

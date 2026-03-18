# Demo Notes

## Demonstration Overview
This document summarizes the main observations from the demonstration and testing of the **autonomous line-following robot with wireless self-charging capability**.

## Demonstrated Functions
The system demonstration focused on validating the following behaviors:

- line following using the IR sensor array
- centroid-based path tracking during movement
- motor response based on embedded control logic
- battery-aware system behavior
- transition from normal operation to docking mode
- integration of wireless charging through the docking station
- resume-operation concept after charging

## Navigation Performance
During testing, the robot followed the predefined path using infrared line sensing and control logic implemented on the Arduino UNO. The centroid-based tracking method supported smoother path correction compared to simple threshold-based switching.

## Battery Monitoring and Docking Behavior
The system continuously monitored battery voltage through the analog sensing circuit. When battery level fell below the programmed threshold, the control system was designed to trigger return-to-dock behavior for charging.

## Charging Demonstration
The robot was integrated with a **Qi wireless charging dock**. The demonstration emphasized the importance of proper alignment between the robot’s charging receiver and the docking transmitter, since charging performance depends strongly on coil positioning.

## System Integration Notes
The project demonstrated successful integration of:
- sensor input
- embedded decision logic
- motor actuation
- docking logic
- wireless charging hardware
- overall hardware-software coordination

## Observed Challenges
During implementation and testing, system performance could be affected by:
- floor texture and lighting conditions
- line contrast and sensor calibration
- wheel alignment
- docking precision
- charging coil alignment

These factors were important in evaluating system reliability and repeatability.

## Team Contribution Note
This project was completed collaboratively.

### Software Development
- **Manmeet Kaur**
- **Kalu Orji**

### Hardware Development
- **Dhvani Parikh**
- **Abhirami Chandran**

All team members also contributed to testing, calibration, documentation, and presentation preparation.

## Engineering Outcome
The project demonstrated a working example of how **embedded control, autonomous navigation, battery-aware behavior, and wireless charging** can be combined into a practical robotic platform. It also highlighted the importance of hardware-software integration for improving robotic autonomy and operational reliability.

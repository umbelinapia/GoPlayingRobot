# GoPlayingRobot

## Project aim 
This report aims to build an autonomous robot that can master the ancient game of Go. This report focuses on designing, building, and validating a 3-DOF mechatronic robotic arm pick-and-place subsystem capable of reliably moving standard Go stones between specified board locations, using a defined coordinate interface and motion control.

## Project Objectives
1. Design and build a complete 3-DOF robotic arm electro-mechanical assembly, including CAD, actuator selection, mounting and joint structures capable of reaching all positions within the defined planar workspace.
2. Design and build a Go-stone gripper including its actuator, mounting interface to the arm, and required electrical connections capable of reliably picking up and placing Go stones.
3. Implement inverse kinematics to convert board coordinates into joint angles, enabling the end-effector to reach target positions within a positional accuracy of approximately 2 mm.
4. Develop and implement an interactive graphical user interface that converts user input into structured commands, transmitted to the motion controller under a second for execution in real-time.

## Project Navigation
1. Clone the repository
2. Navigate to the python scipts Inside the software folder
3. Install dependencies listed in requirements.txt
4. Run the main code for the interface

## Motion controller
Upload `software/Arduino sketch/routines.ino` to your Arduino board using the Arduino IDE.

NOTE
Arduino Cloud sketches can also be found here:

🔗 \[Arduino Cloud Sketch](https://app.arduino.cc/sketches/9923f9a1-adf1-4cbb-a925-ab8f35f5f6f3?view-mode=preview)


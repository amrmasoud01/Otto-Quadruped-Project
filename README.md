# 🕷️ Quadruped Obstacle-Avoiding Robot

## 📌 Overview
An Arduino-based 4-legged (quadruped) robot designed for smooth walking and autonomous obstacle avoidance. The project utilizes 8 servo motors to control the hips and knees of the robot, paired with an HC-SR04 ultrasonic sensor to detect walls or objects in its path and alter its course automatically.

## 🚀 Features
* **Smooth Kinematics:** Custom interpolation functions (`smoothMove`, `smoothMovePair`, `smoothMoveQuad`) to ensure the servos move fluidly without sudden, jerky motions.
* **Autonomous Navigation:** The robot continuously scans the environment and moves forward as long as the path is clear.
* **Obstacle Evasion:** If an obstacle is detected within 20 cm, the robot executes a predefined sequence of right turns to find a clear path.
* **Modular Gaits:** Pre-calibrated angles and functions for standing, walking forward, turning left, and turning right.

## 🛠️ Hardware Components
* 1x Arduino (Nano / Uno)
* 8x Servo Motors (SG90 or MG996R)
* 1x HC-SR04 Ultrasonic Sensor
* 1x Additional Servo for the Ultrasonic Sensor (Head)
* 1x Sensor Shield / Expansion Board
* Power Supply Module (e.g., XL4015 Buck Converter)
* 18650 Li-ion Batteries

## 🔌 Pin Configuration

| Component | Arduino Pin |
| :--- | :--- |
| Leg 1 Hip | D7 |
| Leg 1 Knee | D11 |
| Leg 2 Hip | D5 |
| Leg 2 Knee | D8 |
| Leg 3 Hip | D9 |
| Leg 3 Knee | D4 |
| Leg 4 Hip | D10 |
| Leg 4 Knee | D6 |
| Ultrasonic Sensor TRIG | D3 |
| Ultrasonic Sensor ECHO | D2 |
| Ultrasonic Head Servo | D12 |

## ⚠️ Crucial Power Supply Warning
Running 8 servos simultaneously draws a significant amount of current. Powering the servos directly from the Arduino's 5V pin will cause voltage drops (Brownouts), leading to erratic servo movements and false readings (e.g., reading 999 or 0 cm) from the Ultrasonic sensor.

**Recommended Power Routing:**
* Use a robust Buck Converter (like XL4015) set to exactly 5V to power the **Servos only**.
* Power the **Arduino and Ultrasonic Sensor** separately (e.g., connecting the battery directly to the `VIN` pin to use the Arduino's internal linear regulator for clean power).
* Always ensure that the Ground (`GND`) of the Buck Converter and the Arduino are connected together (Common Ground).

## 💻 Installation & Usage
1. Assemble the 3D-printed or acrylic quadruped chassis.
2. Connect the servos and sensors according to the Pin Configuration table.
3. Ensure your power distribution isolates the servo noise from the Arduino.
4. Open the `quadruped_robot.ino` file in the Arduino IDE.
5. The project uses the standard `<Servo.h>` library, so no external downloads are required.
6. Upload the code to your Arduino board.
7. Disconnect the USB, power on the battery switch, and watch it walk!

# ESP32 & MPU6050 Buttonless Air Mouse

A wireless, buttonless air mouse that uses an ESP32 and an MPU6050 sensor to control your computer's cursor. This project translates physical movement into cursor motion and uses intuitive gestures for clicks, providing a seamless and minimalist control experience over Bluetooth.

This implementation uses the accelerometer for movement, meaning the cursor responds to you pushing the device through the air, rather than just tilting it.

---

## Features

* **Wireless Control**: Uses the ESP32's built-in Bluetooth to emulate a standard HID mouse.
* **No Drivers Needed**: Connects to any computer (Windows, macOS, Linux) just like a commercial Bluetooth mouse.
* **Movement-Based Tracking**: The cursor moves when you accelerate the device, offering an intuitive "push-the-cursor" feel.
* **Buttonless Operation**: All actions are controlled by gestures:
    * **Cursor Movement**: Move the device left, right, up, or down.
    * **Left-Click**: A sharp "flick" or "tap" gesture.
    * **Right-Click**: Hold the device perfectly still for a moment.
* **Low-Cost & DIY**: Built with common and affordable electronic components.
* **Customizable**: Easily tune sensitivity, thresholds, and timings in the code to match your preference.

---

## Hardware Requirements

* 1 x ESP32 Development Board (e.g., ESP32-DevKitC, NodeMCU-32S)
* 1 x MPU6050 6-Axis Gyroscope + Accelerometer Module
* Jumper Wires
* (Optional) A small case or enclosure to hold the components.

---

## Wiring

Connect the MPU6050 module to your ESP32 board using the I2C pins. The standard pins are GPIO 22 (SCL) and GPIO 21 (SDA).

| MPU6050 Pin | ESP32 Pin | Description |
| :---------- | :-------- | :---------- |
| VCC         | 3V3       | 3.3V Power  |
| GND         | GND       | Ground      |
| SCL         | GPIO 22   | I2C Clock   |
| SDA         | GPIO 21   | I2C Data    |

---

## Software & Setup

### 1. Arduino IDE

Make sure you have the Arduino IDE installed and have the ESP32 board manager set up. If not, follow a guide to add ESP32 boards to the Arduino IDE.

### 2. Required Libraries

This project requires three libraries. Install them using the Arduino IDE's Library Manager (`Sketch` -> `Include Library` -> `Manage Libraries...`).

* `Adafruit MPU6050`
* `Adafruit Unified Sensor` (a dependency for the MPU6050 library)
* `ESP32-BLE-Mouse` by T-vK

### 3. Upload the Code

1.  Open the project's `.ino` sketch file in the Arduino IDE.
2.  Connect your ESP32 to your computer via USB.
3.  Select your ESP32 board and the correct COM port from the `Tools` menu.
4.  Click the `Upload` button.

---

## How to Use

1.  **Power On**: After uploading, power your ESP32. It will immediately start broadcasting as a Bluetooth device.
2.  **Pairing**: On your computer, open Bluetooth settings. Search for a new device named "ESP32 Air Mouse" and connect to it.
3.  **Control**: You are now ready to control the cursor!

---

## Gesture Guide

* **Move Cursor**: Push the device through the air. The cursor will follow the direction of acceleration.
* **Left Click**: Give the device a short, sharp flick or tap. The motion should be primarily along the Z-axis (up and down if the chip is flat).
* **Right Click**: Hold the device perfectly still for about 1.5 seconds.

---

## Customization

The mouse's behavior can be fine-tuned by adjusting the constant values at the top of the `.ino` sketch file.

```cpp
// Adjust this value to make the cursor move faster or slower based on acceleration.
const float SENSITIVITY = 7.0;

// Acceleration values below this will be ignored. Prevents cursor drift when idle.
const float DEAD_ZONE = 0.5;

// How "still" the device must be to trigger a dwell. Lower is stricter.
const float STILL_THRESHOLD = 0.8;

// The Z-axis acceleration required to trigger a "flick" for a left-click.
const float FLICK_THRESHOLD = 20.0;

// Cooldown period (in milliseconds) after a flick to prevent multiple clicks.
const unsigned long FLICK_COOLDOWN = 400;

// Time (in milliseconds) to hold the mouse still to trigger a "dwell" for a right-click.
const unsigned long DWELL_TIME = 1500;
```
| Constant          | Description                                                               |
| ----------------- | ------------------------------------------------------------------------- |
| `SENSITIVITY`     | Controls cursor speed. Increase for faster, decrease for precise control. |
| `DEAD_ZONE`       | Ignores small movements to prevent drift.                                 |
| `STILL_THRESHOLD` | Sensitivity for right-click stillness.                                    |
| `FLICK_THRESHOLD` | Sensitivity for left-click flick detection.                               |
| `DWELL_TIME`      | Time required to register a right-click dwell.                            |

##License
This project is open-source and available under the MIT License

/*
 * Buttonless Air Mouse with ESP32 and MPU6050 (ACCELERATION-BASED MOVEMENT)
 * * Translates linear hand movement into cursor movement and uses gestures for clicks.
 * - Cursor Motion: Controlled by accelerating the device left/right/up/down.
 * - Left-Click: A sharp "flick" or "tap" gesture.
 * - Right-Click: Holding the device steady ("dwelling") for 1.5 seconds.
 * * Required Libraries:
 * - Adafruit_MPU6050
 * - Adafruit_Unified_Sensor
 * - ESP32-BLE-Mouse
 */

// Core Libraries
#include <Wire.h>
#include <BleMouse.h>

// Sensor Libraries
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

Adafruit_MPU6050 mpu;
BleMouse bleMouse("ESP32 Air Mouse", "Yax", 100);

// --- CUSTOMIZABLE SETTINGS ---

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

// --- GESTURE TRACKING VARIABLES ---
unsigned long lastFlickTime = 0;
unsigned long dwellStartTime = 0;
bool isDwelling = false;
bool rightClickSent = false;


void setup() {
  Serial.begin(115200);

  // Initialize MPU6050 Sensor
  if (!mpu.begin()) {
    Serial.println("Error: Failed to find MPU6050 chip");
    while (1) {
      delay(10);
    }
  }
  Serial.println("MPU6050 Found!");

  // Configure sensor settings for optimal performance
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DPS);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  
  // Start the Bluetooth Mouse
  Serial.println("Starting BLE Mouse...");
  bleMouse.begin();
}


void loop() {
  if (bleMouse.isConnected()) {
    sensors_event_t accel, gyro, temp;
    mpu.getEvent(&accel, &gyro, &temp);

    handleCursorMovement(accel);
    handleFlickGesture(accel);
    handleDwellGesture(accel);
  }
}

/**
 * @brief Reads accelerometer data and moves the mouse cursor.
 * @param a The sensor event data for the accelerometer.
 */
void handleCursorMovement(sensors_event_t a) {
  // Note: Acceleration values are in m/s^2.
  // You might need to change the signs or axes depending on your sensor's orientation.
  float accelX = a.acceleration.x;
  float accelY = a.acceleration.y;
  
  int moveX = 0;
  int moveY = 0;

  if (abs(accelX) > DEAD_ZONE) {
    moveX = -accelX * SENSITIVITY; // Negative sign might be needed to reverse direction
  }
  if (abs(accelY) > DEAD_ZONE) {
    moveY = accelY * SENSITIVITY;
  }

  if (moveX != 0 || moveY != 0) {
    bleMouse.move(moveX, moveY, 0, 0); // (x, y, wheel, hwheel)
  }
}

/**
 * @brief Detects a sharp flick on the Z-axis to perform a left-click.
 * @param a The sensor event data for the accelerometer.
 */
void handleFlickGesture(sensors_event_t a) {
  if (abs(a.acceleration.z) > FLICK_THRESHOLD && (millis() - lastFlickTime > FLICK_COOLDOWN)) {
    Serial.println("Flick detected! --> Left Click");
    bleMouse.click(MOUSE_LEFT);
    lastFlickTime = millis();
  }
}

/**
 * @brief Detects when the device is held stationary to perform a right-click.
 * @param a The sensor event data for the accelerometer.
 */
void handleDwellGesture(sensors_event_t a) {
  if (abs(a.acceleration.x) < STILL_THRESHOLD && abs(a.acceleration.y) < STILL_THRESHOLD) {
    if (!isDwelling) {
      isDwelling = true;
      dwellStartTime = millis();
      rightClickSent = false;
    }
  } else {
    isDwelling = false;
  }

  if (isDwelling && !rightClickSent && (millis() - dwellStartTime > DWELL_TIME)) {
    Serial.println("Dwell detected! --> Right Click");
    bleMouse.click(MOUSE_RIGHT);
    rightClickSent = true;
    isDwelling = false;
  }
}

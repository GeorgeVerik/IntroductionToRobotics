## 🧩 Homework #1 – RGB LED Control Using 3 Potentiometers

**Author:** Georgios Verykakis
**Course:** Introduction to Robotics (2025–2026)  
**Professor:** Andrei Dumitriu  
**Deadline:** Week of October 24–30, 2025  

---

### 🎯 Task Overview
This project controls an RGB LED using **three potentiometers**, one for each color channel: Red, Green, and Blue.  
Each potentiometer adjusts the brightness of its corresponding color through analog input readings and PWM output signals.

The goal is to understand how analog-to-digital conversion and PWM control work together in digital electronics.
---

### Youtube link : https://youtu.be/UlzIzNuqJCE?si=w-k8M7VIFlCsZf04
---
### ⚙️ Components Used

| Component | Quantity | Description |
|------------|-----------|-------------|
| Arduino Uno | 1 | Main microcontroller |
| RGB LED | 1 | Common cathode/anode (specify type) |
| Potentiometers (10kΩ) | 3 | One for each color channel |
| Resistors (220Ω) | 3 | For LED current limiting |
| Jumper wires | Several | For connections |
| Breadboard | 1 | For prototyping |

---

### 🧰 Circuit Connections

| Component | Arduino Pin | Description |
|------------|--------------|-------------|
| Potentiometer 1 | A0 | Controls Red intensity |
| Potentiometer 2 | A1 | Controls Green intensity |
| Potentiometer 3 | A2 | Controls Blue intensity |
| RGB LED Red | D3 | PWM output |
| RGB LED Green | D5 | PWM output |
| RGB LED Blue | D6 | PWM output |
| GND | Common Ground | Shared between all components |
---
### CODE WITH COMMENTS:

// Define the pins connected to the potentiometers
const int POTENTIOMETER_RED_PIN   = A0;  // Potentiometer for red color
const int POTENTIOMETER_GREEN_PIN = A1;  // Potentiometer for green color
const int POTENTIOMETER_BLUE_PIN  = A2;  // Potentiometer for blue color

// Define the PWM pins connected to the RGB LED
const int LED_RED_PIN   = 3;  // Red LED (PWM output)
const int LED_GREEN_PIN = 5;  // Green LED (PWM output)
const int LED_BLUE_PIN  = 6;  // Blue LED (PWM output)

// Constants for mapping ranges
const int MAX_ANALOG_VALUE = 1023; // Maximum value from analogRead() (10-bit ADC)
const int MAX_PWM_VALUE    = 255;  // Maximum value for analogWrite() (8-bit PWM)

void setup() {
  // Set potentiometer pins as inputs
  pinMode(POTENTIOMETER_RED_PIN, INPUT);
  pinMode(POTENTIOMETER_GREEN_PIN, INPUT);
  pinMode(POTENTIOMETER_BLUE_PIN, INPUT);

  // Set LED pins as outputs
  pinMode(LED_RED_PIN, OUTPUT);
  pinMode(LED_GREEN_PIN, OUTPUT);
  pinMode(LED_BLUE_PIN, OUTPUT);
}

void loop() {
  // Read analog values from each potentiometer (range: 0–1023)
  int redInput   = analogRead(POTENTIOMETER_RED_PIN);
  int greenInput = analogRead(POTENTIOMETER_GREEN_PIN);
  int blueInput  = analogRead(POTENTIOMETER_BLUE_PIN);

  // Map analog input (0–1023) to PWM output (0–255)
  int redPWM   = map(redInput, 0, MAX_ANALOG_VALUE, 0, MAX_PWM_VALUE);
  int greenPWM = map(greenInput, 0, MAX_ANALOG_VALUE, 0, MAX_PWM_VALUE);
  int bluePWM  = map(blueInput, 0, MAX_ANALOG_VALUE, 0, MAX_PWM_VALUE);

  // Write PWM values to the RGB LED pins to adjust brightness
  analogWrite(LED_RED_PIN, redPWM);
  analogWrite(LED_GREEN_PIN, greenPWM);
  analogWrite(LED_BLUE_PIN, bluePWM);

  // Small delay to stabilize readings
  delay(10);
}




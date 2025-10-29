## 🧩 Homework #2 – Traffic Lights System

**Author:** Georgios Verykakis

**Course:** Introduction to Robotics (2025–2026)  

**Professor:** Andrei Dumitriu  

**Deadline:** Week of October 27 – November 2, 2025  

---

### 🎯 Task Overview
A pedestrian crosswalk traffic light controller with:
- Cars: three LEDs (green, yellow, red)
- Pedestrians: two LEDs (red, green)
- 7-segment display showing countdown in states 2–4
- Buzzer with two beep rates (slow and fast)
- Push button triggers sequence (handled with interrupt)

Sequence of states:
1. **STATE 1 – IDLE**: Cars green, pedestrians red, buzzer off – waits for button.  
2. **STATE 2 – WAIT**: After a valid button press, wait 8 seconds before transition (countdown shown).  
3. **STATE 3 – YELLOW → PED_GO**: Cars yellow for 3 s (countdown), then cars red & pedestrians green for 8 s (buzzer slow).  
4. **STATE 4 – WARNING**: Pedestrian green blinks, buzzer beeps faster for 4 s, then returns to IDLE.  

Button presses in non-IDLE states have no effect.

---

### ⚙️ Components
- Arduino UNO  
- 5 × LEDs (2 pedestrian, 3 car)  
- 7-segment display (1-digit, common cathode)  
- 1 × push button  
- 1 × buzzer  
- Resistors (220 – 330 Ω) and jumper wires  
- Breadboard  

---

### 🔌 Wiring Summary (example pinout)
- **BUTTON** → D2 (`INPUT_PULLUP`, button to GND)  
- **PED GREEN** → D3  
- **PED RED** → D4  
- **BUZZER** → D6  
- **CAR GREEN** → D8  
- **CAR YELLOW** → D9  
- **CAR RED** → D10  
- **7-Segment A** → D7  
- **7-Segment B** → A3  
- **7-Segment C** → D11  
- **7-Segment D** → D12  
- **7-Segment E** → D13  
- **7-Segment F** → A0  
- **7-Segment G** → A1  
- **7-Segment DP** → A2  

---
  <img width="649" height="308" alt="image" src="https://github.com/user-attachments/assets/1f387770-8fb2-4374-a831-275f04a38a66" />


---
### 💻 Code


  Description:
    Pedestrian crosswalk traffic lights using Arduino UNO.

    - 3 LEDs for cars (Green, Yellow, Red)
    - 2 LEDs for pedestrians (Red, Green)
    - 1 buzzer for audio signal
    - 1 push button with interrupt and INPUT_PULLUP
    - 1 seven-segment display (common cathode) for countdown

    States:
      1. STATE_IDLE   → Cars green, pedestrians red, buzzer off (waits for button)
      2. STATE_WAIT   → 8 s wait after button press (cars green, pedestrians red)
      3. STATE_YELLOW → Cars yellow for 3 s
      4. STATE_PED_GO → Pedestrians green for 8 s, buzzer slow
      5. STATE_WARNING→ Pedestrians blinking green for 4 s, buzzer fast, then back to idle


#include <Arduino.h>

// ---------------- Pin definitions ----------------
// Pedestrian LEDs
const byte PED_GREEN_PIN  = 3;
const byte PED_RED_PIN    = 4;

// Button and buzzer
const byte BUTTON_PIN     = 2;    // interrupt pin (active LOW)
const byte BUZZER_PIN     = 6;

// Car traffic LEDs
const byte CAR_GREEN_PIN  = 8;
const byte CAR_YELLOW_PIN = 9;
const byte CAR_RED_PIN    = 10;

// 7-segment display pins (Common Cathode)
const byte SEG_A  = 7;
const byte SEG_B  = A3;   // moved here to avoid conflict with PED_RED_PIN
const byte SEG_C  = 11;
const byte SEG_D  = 12;
const byte SEG_E  = 13;
const byte SEG_F  = A0;
const byte SEG_G  = A1;
const byte SEG_DP = A2;

// ---------------- Timing configuration ----------------
const unsigned long WAIT_AFTER_PRESS = 8000UL;  // 8 s delay before yellow
const unsigned long YELLOW_DURATION  = 3000UL;  // 3 s yellow
const unsigned long PED_GO_DURATION  = 8000UL;  // 8 s pedestrian green
const unsigned long WARNING_DURATION = 4000UL;  // 4 s blink warning

const unsigned long DEBOUNCE_DELAY   = 200UL;   // debounce for interrupt
const unsigned long BLINK_INTERVAL   = 400UL;   // pedestrian blink interval
const unsigned long BUZZER_SLOW      = 500UL;   // slow beep (PED_GO)
const unsigned long BUZZER_FAST      = 150UL;   // fast beep (WARNING)

// ---------------- Globals ----------------
volatile bool buttonRequested = false;  // set by ISR
unsigned long lastInterruptTime = 0;

unsigned long stateStart = 0;
unsigned long requestedAt = 0;

unsigned long lastBlink = 0;
unsigned long lastBuzzerToggle = 0;

bool blinkState = false;
bool buzzerState = false;

// ---------------- State machine ----------------
enum State {
  STATE_IDLE,
  STATE_WAIT,
  STATE_YELLOW,
  STATE_PED_GO,
  STATE_WARNING
};
State currentState = STATE_IDLE;

// ---------------- 7-seg digits (common cathode) ----------------
// Bit order (LSB → MSB): A, B, C, D, E, F, G
const byte NUMBERS[] = {
  0b00111111, // 0
  0b00000110, // 1
  0b01011011, // 2
  0b01001111, // 3
  0b01100110, // 4
  0b01101101, // 5
  0b01111101, // 6
  0b00000111, // 7
  0b01111111, // 8
  0b01101111  // 9
};

// ---------------- Function declarations ----------------
void onButtonPress();
void setCarsGreen();
void setCarsYellow();
void setCarsRed();
void setPedRed();
void setPedGreen();
void clearDisplay();
void displayNumber(int num);
void handleBuzzer(unsigned long now, unsigned long interval);
void handleBlink(unsigned long now);

// ---------------- Setup ----------------
void setup() {
  Serial.begin(9600);

  pinMode(PED_GREEN_PIN, OUTPUT);
  pinMode(PED_RED_PIN, OUTPUT);

  pinMode(CAR_GREEN_PIN, OUTPUT);
  pinMode(CAR_YELLOW_PIN, OUTPUT);
  pinMode(CAR_RED_PIN, OUTPUT);

  pinMode(BUZZER_PIN, OUTPUT);

  // 7-segment setup
  pinMode(SEG_A, OUTPUT);
  pinMode(SEG_B, OUTPUT);
  pinMode(SEG_C, OUTPUT);
  pinMode(SEG_D, OUTPUT);
  pinMode(SEG_E, OUTPUT);
  pinMode(SEG_F, OUTPUT);
  pinMode(SEG_G, OUTPUT);
  pinMode(SEG_DP, OUTPUT);

  // Button input with internal pull-up (active LOW)
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Interrupt setup
  attachInterrupt(digitalPinToInterrupt(BUTTON_PIN), onButtonPress, FALLING);

  // Initial state
  setCarsGreen();
  setPedRed();
  clearDisplay();

  Serial.println("Traffic system initialized: STATE_IDLE");
}

// ---------------- Loop ----------------
void loop() {
  unsigned long now = millis();

  switch (currentState) {

    case STATE_IDLE:
      if (buttonRequested) {
        buttonRequested = false;
        requestedAt = now;
        currentState = STATE_WAIT;
        stateStart = now;
        Serial.println("Button pressed -> STATE_WAIT (8s)");
        setCarsGreen();
        setPedRed();
      }
      break;

    case STATE_WAIT: {
      unsigned long elapsed = now - requestedAt;
      int remaining = (WAIT_AFTER_PRESS - elapsed + 999) / 1000;
      if (remaining < 0) remaining = 0;
      displayNumber(remaining > 9 ? 9 : remaining);

      if (elapsed >= WAIT_AFTER_PRESS) {
        currentState = STATE_YELLOW;
        stateStart = now;
        Serial.println("STATE_YELLOW started");
        setCarsYellow();
        setPedRed();
      }
      break;
    }

    case STATE_YELLOW: {
      unsigned long elapsed = now - stateStart;
      int remaining = (YELLOW_DURATION - elapsed + 999) / 1000;
      if (remaining < 0) remaining = 0;
      displayNumber(remaining > 9 ? 9 : remaining);

      if (elapsed >= YELLOW_DURATION) {
        currentState = STATE_PED_GO;
        stateStart = now;
        Serial.println("STATE_PED_GO started");
        setCarsRed();
        setPedGreen();
        lastBuzzerToggle = now;
        buzzerState = false;
      }
      break;
    }

    case STATE_PED_GO: {
      unsigned long elapsed = now - stateStart;
      int remaining = (PED_GO_DURATION - elapsed + 999) / 1000;
      if (remaining < 0) remaining = 0;
      displayNumber(remaining > 9 ? 9 : remaining);

      handleBuzzer(now, BUZZER_SLOW);

      if (elapsed >= PED_GO_DURATION) {
        currentState = STATE_WARNING;
        stateStart = now;
        Serial.println("STATE_WARNING started");
        lastBlink = now;
        lastBuzzerToggle = now;
      }
      break;
    }

    case STATE_WARNING: {
      unsigned long elapsed = now - stateStart;
      int remaining = (WARNING_DURATION - elapsed + 999) / 1000;
      if (remaining < 0) remaining = 0;
      displayNumber(remaining > 9 ? 9 : remaining);

      handleBuzzer(now, BUZZER_FAST);
      handleBlink(now);

      if (elapsed >= WARNING_DURATION) {
        currentState = STATE_IDLE;
        Serial.println("Returning to STATE_IDLE");
        setCarsGreen();
        setPedRed();
        clearDisplay();
        digitalWrite(BUZZER_PIN, LOW);
        buzzerState = false;
        blinkState = false;
      }
      break;
    }
  }
}

// ---------------- Interrupt Handler ----------------
void onButtonPress() {
  unsigned long now = millis();
  if (now - lastInterruptTime < DEBOUNCE_DELAY) return;
  lastInterruptTime = now;

  if (currentState == STATE_IDLE) {
    buttonRequested = true;
  }
}

// ---------------- Helper Functions ----------------
void setCarsGreen() {
  digitalWrite(CAR_GREEN_PIN, HIGH);
  digitalWrite(CAR_YELLOW_PIN, LOW);
  digitalWrite(CAR_RED_PIN, LOW);
}
void setCarsYellow() {
  digitalWrite(CAR_GREEN_PIN, LOW);
  digitalWrite(CAR_YELLOW_PIN, HIGH);
  digitalWrite(CAR_RED_PIN, LOW);
}
void setCarsRed() {
  digitalWrite(CAR_GREEN_PIN, LOW);
  digitalWrite(CAR_YELLOW_PIN, LOW);
  digitalWrite(CAR_RED_PIN, HIGH);
}
void setPedRed() {
  digitalWrite(PED_RED_PIN, HIGH);
  digitalWrite(PED_GREEN_PIN, LOW);
}
void setPedGreen() {
  digitalWrite(PED_RED_PIN, LOW);
  digitalWrite(PED_GREEN_PIN, HIGH);
}

void handleBuzzer(unsigned long now, unsigned long interval) {
  if (now - lastBuzzerToggle >= interval) {
    lastBuzzerToggle = now;
    buzzerState = !buzzerState;
    digitalWrite(BUZZER_PIN, buzzerState);
  }
}

void handleBlink(unsigned long now) {
  if (now - lastBlink >= BLINK_INTERVAL) {
    lastBlink = now;
    blinkState = !blinkState;
    digitalWrite(PED_GREEN_PIN, blinkState);
  }
}

void displayNumber(int num) {
  if (num < 0 || num > 9) return;
  byte segs = NUMBERS[num];
  digitalWrite(SEG_A, bitRead(segs, 0));
  digitalWrite(SEG_B, bitRead(segs, 1));
  digitalWrite(SEG_C, bitRead(segs, 2));
  digitalWrite(SEG_D, bitRead(segs, 3));
  digitalWrite(SEG_E, bitRead(segs, 4));
  digitalWrite(SEG_F, bitRead(segs, 5));
  digitalWrite(SEG_G, bitRead(segs, 6));
  digitalWrite(SEG_DP, LOW);
}

void clearDisplay() {
  digitalWrite(SEG_A, LOW);
  digitalWrite(SEG_B, LOW);
  digitalWrite(SEG_C, LOW);
  digitalWrite(SEG_D, LOW);
  digitalWrite(SEG_E, LOW);
  digitalWrite(SEG_F, LOW);
  digitalWrite(SEG_G, LOW);
  digitalWrite(SEG_DP, LOW);
}


  

---

### 🎥 Demo Video
[YouTube Demo Link](https://youtube.com/your-video-link)



---

### 🧾 Notes
- Button uses internal pull-up (`INPUT_PULLUP`) and is active LOW.  
- ISR only sets a flag – main loop handles debounce and logic.  
- All LEDs and 7-segment segments use 220–330 Ω resistors.  
- Common cathode of 7-segment connected to GND.  
- Serial Monitor (9600 baud) prints debug messages.  

--- 


#include <Arduino.h>

// ------------------ Pin definitions ------------------
const int trigPin = 9;
const int echoPin = 10;
const int ldrPin = A0;
const int buzzerPin = 5;
const int redLedPin = 3;
const int greenLedPin = 4;

// Joystick pins
const int joyX = A1;
const int joyY = A2;
const int joyButton = 2;

// ------------------ Constants ------------------
float triggerDistanceChange = 5.0; // cm — now more sensitive
const int lightThresholdDefault = 400;
const unsigned long armDelay = 3000;
const unsigned long alarmDelay = 3000;
const int passwordDefault = 1234;

// ------------------ Globals ------------------
bool systemArmed = false;
bool alarmActive = false;
int userPassword = passwordDefault;
int lightThreshold = lightThresholdDefault;

float baselineDistance = 0;
float currentDistance = 0;
int lightValue = 0;

unsigned long previousMillis = 0;
unsigned long blinkInterval = 400;
bool ledState = false;
unsigned long startMillis;

// Joystick menu navigation
int menuSelection = 1;
int lastSelection = -1;
unsigned long lastMoveTime = 0;
const unsigned long moveDelay = 300;

// ------------------ Melody for buzzer ------------------
int melody[] = {262, 294, 330, 349, 392, 440, 494, 523}; // C D E F G A B C
int noteDurations[] = {4, 8, 8, 4, 4, 4, 4, 4};           // rhythm

// ------------------ Function declarations ------------------
void showMainMenu();
void handleMainMenuInput(int choice);
void showSettingsMenu();
void handleSettingsMenuInput(int choice);
void showBonusMenu();
void handleBonusMenuInput(int choice);

void checkSensors();
void updateAlarm();
void armSystem();
void disarmSystem();
bool verifyPassword();
float readUltrasonic();
void blinkRedLed();
void resetLeds();
void logEvent(const String &event);
void displayTimeSinceStart();
void joystickNavigateMenu();
void playAlarmMelody();

// ==========================================================
void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(redLedPin, OUTPUT);
  pinMode(greenLedPin, OUTPUT);
  pinMode(ldrPin, INPUT);

  pinMode(joyX, INPUT);
  pinMode(joyY, INPUT);
  pinMode(joyButton, INPUT_PULLUP);

  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, HIGH);

  startMillis = millis();

  Serial.println("============================================");
  Serial.println("   HOME ALARM SYSTEM – INITIALIZING...      ");
  Serial.println("============================================");

  // Baseline calibration
  Serial.println("Calibrating baseline distance...");
  float sum = 0;
  for (int i = 0; i < 10; i++) {
    float d = readUltrasonic();
    sum += d;
    Serial.print(d); Serial.print(" ");
    delay(100);
  }
  Serial.println();
  baselineDistance = sum / 10.0;
  Serial.print(" Baseline distance: ");
  Serial.print(baselineDistance);
  Serial.println(" cm");

  showMainMenu();
}

// ==========================================================
void loop() {
  checkSensors();
  updateAlarm();

  // Serial input
  if (Serial.available() > 0) {
    int option = Serial.parseInt();
    handleMainMenuInput(option);
    showMainMenu();
  }

  // Joystick navigation
  joystickNavigateMenu();
}

// ==========================================================
// ------------------ Menu Functions ------------------------
void showMainMenu() {
  Serial.println();
  Serial.println("===== HOME ALARM SYSTEM =====");
  Serial.print("Status: ");
  Serial.println(systemArmed ? "ARMED" : "DISARMED");
  Serial.println("-----------------------------");
  if (!systemArmed) {
    Serial.println("1. Arm System");
    Serial.println("2. Test Alarm");
    Serial.println("3. Settings");
    Serial.println("4. Bonus Features ");
  } else {
    Serial.println("1. Disarm System (requires password)");
  }
  Serial.println("-----------------------------");
  Serial.println("Use joystick ↑ ↓ to navigate, press button to select.");
  Serial.print("Current option: ");
  Serial.println(menuSelection);
}

// ==========================================================
void handleMainMenuInput(int choice) {
  if (!systemArmed) {
    switch (choice) {
      case 1: armSystem(); break;
      case 2: alarmActive = true; Serial.println("Manual alarm test triggered!"); break;
      case 3: showSettingsMenu(); break;
      case 4: showBonusMenu(); break;
      default: Serial.println("Invalid choice!"); break;
    }
  } else {
    if (choice == 1) {
      if (verifyPassword()) disarmSystem();
      else Serial.println(" Incorrect password!");
    }
  }
}

// ==========================================================
void showSettingsMenu() {
  Serial.println("\n----- SETTINGS -----");
  Serial.println("1. Set Ultrasonic Sensitivity");
  Serial.println("2. Set LDR Light Threshold");
  Serial.println("3. Change Password");
  Serial.println("0. Return");
  Serial.print("Enter option: ");

  while (!Serial.available()) {}
  int choice = Serial.parseInt();
  handleSettingsMenuInput(choice);
}

// ==========================================================
void handleSettingsMenuInput(int choice) {
  switch (choice) {
    case 1:
      Serial.println("Set new sensitivity (cm): ");
      while (!Serial.available()) {}
      triggerDistanceChange = Serial.parseFloat();
      Serial.print(" Updated to: "); Serial.println(triggerDistanceChange);
      break;
    case 2:
      Serial.println("Set new LDR threshold (0–1023): ");
      while (!Serial.available()) {}
      lightThreshold = Serial.parseInt();
      Serial.print(" Updated to: "); Serial.println(lightThreshold);
      break;
    case 3:
      Serial.println("Enter current password: ");
      if (verifyPassword()) {
        Serial.println("Enter new password: ");
        while (!Serial.available()) {}
        userPassword = Serial.parseInt();
        Serial.println(" Password changed successfully!");
      } else Serial.println(" Incorrect password.");
      break;
    default:
      Serial.println("Returning...");
      break;
  }
}

// ==========================================================
void showBonusMenu() {
  Serial.println("\n===== BONUS FEATURES =====");
  Serial.println("1. Show Event Log Example");
  Serial.println("2. Display Time Since Start");
  Serial.println("3. Joystick Menu Navigation Demo");
  Serial.println("0. Return");
  Serial.print("Enter option: ");
  while (!Serial.available()) {}
  int choice = Serial.parseInt();
  handleBonusMenuInput(choice);
}

void handleBonusMenuInput(int choice) {
  switch (choice) {
    case 1: logEvent("System armed for demo log."); break;
    case 2: displayTimeSinceStart(); break;
    case 3: joystickNavigateMenu(); break;
    default: Serial.println("Returning to main menu..."); break;
  }
}

// ==========================================================
// ------------------ Core Logic ----------------------------
void armSystem() {
  Serial.println("Arming system in 3 seconds...");
  unsigned long start = millis();
  while (millis() - start < armDelay) {
    Serial.print(".");
    delay(500);
  }
  Serial.println("\n System armed!");
  systemArmed = true;
  digitalWrite(greenLedPin, LOW);
  digitalWrite(redLedPin, HIGH);
  logEvent("System armed.");
}

void disarmSystem() {
  systemArmed = false;
  alarmActive = false;
  resetLeds();
  digitalWrite(greenLedPin, HIGH);
  noTone(buzzerPin);
  Serial.println(" System disarmed.");
  logEvent("System disarmed.");
}

// ==========================================================
bool verifyPassword() {
  Serial.print("Enter password: ");
  while (!Serial.available()) {}
  String line = Serial.readStringUntil('\n');
  line.trim();
  int input = line.toInt();
  return (input == userPassword);
}

// ==========================================================
void checkSensors() {
  if (!systemArmed) {
    lightValue = analogRead(ldrPin);
    if (lightValue < lightThreshold) {
      Serial.println(" Darkness detected – Auto arming...");
      armSystem();
    }
    return;
  }

  currentDistance = readUltrasonic();
  Serial.print("Distance: "); Serial.print(currentDistance);
  Serial.print(" cm | Baseline: "); Serial.println(baselineDistance);

  if (abs(currentDistance - baselineDistance) > triggerDistanceChange) {
    Serial.println(" Movement detected!");
    alarmActive = true;
    logEvent("Intrusion detected!");
  }
}

// ==========================================================
void playAlarmMelody() {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = 1000 / noteDurations[thisNote];
    tone(buzzerPin, melody[thisNote], noteDuration);
    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    noTone(buzzerPin);
  }
}

void updateAlarm() {
  if (!alarmActive) return;

  static unsigned long alarmStart = millis();
  if (millis() - alarmStart < alarmDelay) return;

  blinkRedLed();
  playAlarmMelody();  // play your melody instead of constant tone
}

// ==========================================================
void blinkRedLed() {
  if (millis() - previousMillis >= blinkInterval) {
    previousMillis = millis();
    ledState = !ledState;
    digitalWrite(redLedPin, ledState);
  }
}

void resetLeds() {
  digitalWrite(redLedPin, LOW);
  digitalWrite(greenLedPin, LOW);
}

// ==========================================================
float readUltrasonic() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  long duration = pulseIn(echoPin, HIGH, 30000); // 30ms timeout
  if (duration == 0) return baselineDistance;    // ignore invalid reading
  float distance = duration * 0.034 / 2.0;
  return distance;
}

// ==========================================================
void logEvent(const String &event) {
  unsigned long now = (millis() - startMillis) / 1000;
  unsigned long minutes = now / 60;
  unsigned long seconds = now % 60;
  Serial.print("[LOG "); Serial.print(minutes); Serial.print("m ");
  Serial.print(seconds); Serial.print("s] ");
  Serial.println(event);
}

void displayTimeSinceStart() {
  unsigned long now = (millis() - startMillis) / 1000;
  unsigned long minutes = now / 60;
  unsigned long seconds = now % 60;
  Serial.print(" System has been running for ");
  Serial.print(minutes); Serial.print("m "); Serial.print(seconds); Serial.println("s.");
}

// ==========================================================
// ------------------ Joystick Menu Navigation --------------
void joystickNavigateMenu() {
  int yVal = analogRead(joyY);
  int button = digitalRead(joyButton);
  unsigned long now = millis();

  // Move selection (Up/Down)
  if (now - lastMoveTime > moveDelay) {
    if (yVal > 800) { // Up
      menuSelection--;
      if (menuSelection < 1) menuSelection = systemArmed ? 1 : 4; // wrap
      lastMoveTime = now;
    } else if (yVal < 200) { // Down
      menuSelection++;
      if ((!systemArmed && menuSelection > 4) || (systemArmed && menuSelection > 1))
        menuSelection = 1;
      lastMoveTime = now;
    }
  }

  // Display current selection if changed
  if (menuSelection != lastSelection) {
    Serial.print("\n> Selected option: ");
    Serial.println(menuSelection);
    lastSelection = menuSelection;
  }

  // Select option on button press
  static bool prevButton = HIGH;
  if (prevButton == HIGH && button == LOW) {
    Serial.print(" Joystick selected option ");
    Serial.println(menuSelection);
    handleMainMenuInput(menuSelection);
    showMainMenu();
  }
  prevButton = button;
}


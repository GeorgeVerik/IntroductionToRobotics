# IntroductionToRobotics
Projects and homework for the Introduction to Robotics course (2025–2026)
My name is George Verykakis iam an Erasmus student from Greece 
<img width="919" height="985" alt="Screenshot 2025-10-18 131100" src="https://github.com/user-attachments/assets/c681f428-a54d-40ad-a2e4-1dac39dd611f" />

# 🏠 Homework #3 – Home Alarm System

**Author:** Georgios Verykakis
**Course:** Introduction to Robotics (2025–2026)  
**Professor:** Andrei Dumitriu  
**Deadline:** Week of November 3 – November 9, 2025  

---
<img width="1201" height="896" alt="Screenshot 2025-11-06 204517" src="https://github.com/user-attachments/assets/5460549b-ebbd-4abe-8fcd-1e5f846f8228" />
<img width="1200" height="894" alt="Screenshot 2025-11-06 204502" src="https://github.com/user-attachments/assets/cd39b41b-7bdc-4d91-bcfd-64391d854e7a" />



---

## 🎯 Task Overview

Design and build a **smart home alarm system** using Arduino components.  
The system can be **armed/disarmed** manually or automatically, detects **motion** and **light level changes**, and triggers an audible **buzzer alarm** with a visual LED indicator.  
User interaction occurs via the **Serial Monitor menu**, and an optional **joystick** allows menu navigation.

---

## ⚙️ Components

| Component | Qty | Description |
|------------|-----|-------------|
| Arduino UNO | 1 | Main controller |
| Ultrasonic Sensor (HC-SR04) | 1 | Motion / distance detection |
| Photoresistor (LDR) | 1 | Light sensing (auto-arm) |
| Buzzer | 1 | Audible alarm |
| Red LED | 1 | Alarm / armed indicator |
| Green LED | 1 | Disarmed indicator |
| Joystick (optional) | 1 | Navigate menu |
| Resistors (220–10 kΩ) | — | For LEDs & LDR voltage divider |
| Breadboard & Wires | — | Circuit assembly |

---

## 🧠 System Logic

### **Main States**

1. **Disarmed**  
   Green LED ON · Buzzer OFF · Waits for “Arm System”  
2. **Arming Delay**  
   3 s countdown before activation (Serial feedback shown)  
3. **Armed**  
   Red LED ON · Reads Ultrasonic and LDR continuously  
   - Ultrasonic: detect distance change > threshold → trigger alarm  
   - LDR: auto-arms if light < threshold (“night mode”)  
4. **Alarm Triggered**  
   - Buzzer plays melody  
   - Red LED blinks  
   - Serial Monitor prints “⚠️ Intrusion detected!”  
   - Await password entry to disarm  
5. **Disarmed (Post-Alarm)**  
   - Green LED ON, Buzzer OFF, System reset  

---

---

### 🔧 Technical Requirements
- All timing must use `millis()` (no `delay()`).
- Modular structure:
  - `showMenu()`
  - `checkSensors()`
  - `updateAlarm()`
  - `handlePassword()`
  - `manageSettings()`
- Password-protected disarming.
- Friendly Serial interface (rejects invalid input safely).
- Real-time state shown on Serial Monitor (e.g., `[Armed]` / `[Disarmed]`).

---



### 💡 Implementation Tips
- Record a **baseline ultrasonic distance** for a few seconds at startup to detect *changes* in environment.
- Define clear **tolerance thresholds** for both sensors.
- Use **non-blocking loops** (`millis()`-based) for flashing LEDs and buzzer patterns.
- Test ultrasonic and LDR separately before combining.

---

### 🌙 Bonus Ideas (Optional)
- 🕹️ Control menu with a joystick.  

---

### 💻 Code
[File](https://github.com/GeorgeVerik/IntroductionToRobotics/tree/9f4dd884d04f2a9fff899b2aa95391b9167dc8bb/homework3)

---

### 🎥 Demo Video
[YouTube Video](https://youtu.be/si9VusCJaq8)  


---

### 🧾 Notes
- Password and settings can be stored in memory or redefined each startup.  
- System startup message includes the system name (configurable).  
- Default password: `1234` (changeable through menu).  
- Serial Monitor Baud Rate: **9600**.  
- Works with **Arduino UNO**, **Nano**, or **Mega**.

---




# 🛸 Homework 5 Starship LCD Game

**Author:** Georgios Verykakis  
**Course:** Introduction to Robotics (2025–2026)  
**Professor:** Andrei Dumitriu  
**Homework #5**  

---

![IMG_9392](https://github.com/user-attachments/assets/7a88845b-780a-477e-8f78-1c593d18bca1)

---
## 🎯 Overview

This project implements a **Starship shooter game** on an Arduino with a 16x2 LCD.  
The player controls a starship that moves up and down and fires bullets at incoming enemies.  
The system uses a **joystick** for movement and firing and a **buzzer** for sound effects.  
Scores and high scores are tracked using **EEPROM**, and the game features a **menu** with multiple options.

---

## ⚙️ Components

| Component | Qty | Description |
|------------|-----|-------------|
| Arduino UNO | 1 | Main controller |
| 16x2 LCD (I2C or standard pins) | 1 | Display the game |
| Joystick Module | 1 | Move starship and fire bullets |
| Push Button | 1 | Pause / menu selection |
| Buzzer | 1 | Sound effects |
| Resistors | — | For button pull-ups |
| Breadboard & Wires | — | Circuit assembly |

---

## 🧠 Game Logic

### States

1. **Menu**  
   - Options: Play, High Scores, Reset High Score  
   - Navigate with joystick, select with fire button  

2. **Playing**  
   - Starship moves up/down with joystick  
   - Fire bullets with button  
   - Enemies spawn randomly on the right and move left  
   - Collisions:  
     - Bullet hits enemy → score increases, sound plays  
     - Enemy hits starship → game over  

3. **Game Over**  
   - Displays score and high score  
   - EEPROM updated if new high score  

4. **High Scores**  
   - Displays top 3 scores  

5. **Reset High Score**  
   - Clears stored scores  

6. **Pause (optional)**  
   - Button pauses the game  
   - Resume or exit to menu  

---

### Scoring

- Enemy destroyed: **+10 points**  
- Level completion / flag (optional): **+50 points**  

---

## 🔧 Features

- Non-blocking loops with `millis()`  
- Modular code structure:
  - `drawMenu()`  
  - `updateStarship()`  
  - `spawnEnemy()`  
  - `moveBullet()`  
  - `checkCollisions()`  
  - `updateScore()`  
- Friendly LCD interface with custom characters  
- Audio feedback via buzzer  
- High scores persist in EEPROM  

---

## 💡 Implementation Tips

- Use arrays for bullets and enemies  
- Custom LCD characters for ship, bullets, enemies  
- Test joystick analog values for accurate movement  
- Random enemy spawn positions increase gameplay variety  
- Non-blocking timing ensures smooth animations  

---

## 🔊 Optional / Bonus

- Pause menu  
- Animated starfield background  
- Multiple enemy types  
- Sound effects for shooting, collisions, and game over  

---

### 💻 Code
[File](https://github.com/GeorgeVerik/IntroductionToRobotics/blob/cc27908d8b5b1d7961d5c453e115bb321392dbb2/homwork5/homework5.ino.ino)

---

### 🎥 Demo Video
[YouTube Video](https://youtube.com/shorts/xr5KAANP-Vc?feature=share)  


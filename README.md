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
![IMG_9088](https://github.com/user-attachments/assets/b7c221d1-cdc1-4f8f-8624-a041a0c72f5c)
![IMG_9089](https://github.com/user-attachments/assets/8649ca16-4a9e-4a81-9e04-636350e084fa)


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
File: `home_alarm_system.ino`  
Implements modular functions for sensor readings, menu navigation, and alarm handling.

---

### 🎥 Demo Video
[YouTube Demo Link](https://youtube.com/your-video-link)  


---

### 🧾 Notes
- Password and settings can be stored in memory or redefined each startup.  
- System startup message includes the system name (configurable).  
- Default password: `1234` (changeable through menu).  
- Serial Monitor Baud Rate: **9600**.  
- Works with **Arduino UNO**, **Nano**, or **Mega**.

---

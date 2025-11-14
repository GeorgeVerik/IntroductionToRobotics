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
[FILE](https://github.com/GeorgeVerik/IntroductionToRobotics/tree/e17df9f341ec9c4609e731c559f052a570331a94/homework_2)




  

---

### 🎥 Demo Video
[My YT Video](https://youtu.be/wlG6SmsvlOk)



---

### 🧾 Notes
- Button uses internal pull-up (`INPUT_PULLUP`) and is active LOW.  
- ISR only sets a flag – main loop handles debounce and logic.  
- All LEDs and 7-segment segments use 220–330 Ω resistors.  
- Common cathode of 7-segment connected to GND.  
- Serial Monitor (9600 baud) prints debug messages.  

--- 


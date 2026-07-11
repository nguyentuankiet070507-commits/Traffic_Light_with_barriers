# 🚦 Traffic Light with Barriers — Smart Intersection Control System

An ESP32-based smart traffic light controller with barrier system simulation. Demonstrates finite state machine (FSM) design, GPIO multiplexing, and real-world IoT control patterns.

## 🎯 Project Overview

This project simulates an intelligent traffic management system with:
- **Traffic light control** (Red → Yellow → Green cycle)
- **Barrier management** (gates open/close based on light state)
- **Finite State Machine (FSM)** architecture for traffic coordination
- **Real-time state monitoring** via serial and OLED display

**Use case**: Smart city infrastructure — foundation for automated traffic control, parking systems, and toll gates.

## 🎥 Demo

Watch the system in action: [https://youtu.be/Dt5z1WxUfRQ](https://youtu.be/Dt5z1WxUfRQ)

## 🛠️ Hardware Used

| Component | Quantity | GPIO Pin | Purpose |
|-----------|----------|----------|---------|
| ESP32 DevKit V1 | 1 | — | Microcontroller (dual-core, real-time capable) |
| LED Red | 1 | GPIO 12 | Traffic light RED signal (stop) |
| LED Yellow | 1 | GPIO 14 | Traffic light YELLOW signal (caution) |
| LED Green | 1 | GPIO 13 | Traffic light GREEN signal (go) |
| Servo Motor (SG90) | 1 | GPIO 26 | Barrier gate actuator (0° = closed, 90° = open) |
| OLED Display (0.96") | 1 | GPIO 21 (SDA), GPIO 22 (SCL) | Real-time state display |
| Push Button | 1 | GPIO 27 | Manual trigger (optional) |
| Breadboard | 1 | — | Prototyping |
| Jumper wires | 15+ | — | Connections |
| USB cable (Micro-USB) | 1 | — | Power & programming |

## 📌 Wiring Diagram

```
ESP32 DevKit V1
┌────────────────────────────────┐
│ GPIO 12 ────→ [Red LED] ─┐     │
│ GPIO 14 ────→ [Yellow LED] ├─→ [Common GND]
│ GPIO 13 ────→ [Green LED] ─┘     │
│                                   │
│ GPIO 26 ────→ [Servo Signal]     │
│ 5V ──────────→ [Servo VCC]       │
│ GND ──────────→ [Servo GND]      │
│                                   │
│ GPIO 21 (SDA) ──→ [OLED SDA]    │
│ GPIO 22 (SCL) ──→ [OLED SCL]    │
│ 3.3V ─────────→ [OLED VCC]      │
│ GND ──────────→ [OLED GND]      │
│                                   │
│ GPIO 27 ──→ [Push Button] ─→ [GND]
└────────────────────────────────┘

LED Circuit (typical):
GPIO ──→ [220Ω Resistor] ──→ [LED Anode] ──→ [GND]
                 (longer leg)
```

**Pin Summary**:
| Component | ESP32 Pin | Signal Type |
|-----------|-----------|-------------|
| Red LED | GPIO 12 | Digital Output |
| Yellow LED | GPIO 14 | Digital Output |
| Green LED | GPIO 13 | Digital Output |
| Servo Motor | GPIO 26 | PWM Output |
| OLED SDA | GPIO 21 | I2C Data |
| OLED SCL | GPIO 22 | I2C Clock |
| Button | GPIO 27 | Digital Input |

## 🚀 Installation & Setup

### Prerequisites
- VS Code with PlatformIO extension
- ESP32 DevKit V1
- Servo motor (SG90 recommended)
- USB Micro-B cable
- Arduino framework knowledge

### Step-by-Step Setup

1. **Clone the repository**
   ```bash
   git clone https://github.com/nguyentuankiet070507-commits/Traffic_Light_with_barriers.git
   cd Traffic_Light_with_barriers
   ```

2. **Open in PlatformIO**
   - Launch VS Code → Open folder
   - PlatformIO auto-detects `platformio.ini`

3. **Verify board configuration** (check `platformio.ini`)
   ```ini
   platform = espressif32
   board = esp32doit-devkit-v1
   framework = arduino
   monitor_speed = 115200
   lib_deps = 
       adafruit/Adafruit SSD1306 @ ^2.5.7
       paulstoffregen/Servo @ ^1.2.2
   ```

4. **Connect hardware**
   - Wire LEDs to GPIO 12, 14, 13 (with 220Ω resistors)
   - Wire servo to GPIO 26 (PWM), 5V, GND
   - Wire OLED to GPIO 21 (SDA), GPIO 22 (SCL)
   - Connect all GND pins together

5. **Compile & upload**
   ```bash
   # Build
   Ctrl+Alt+B
   
   # Upload to ESP32
   Ctrl+Alt+U
   ```

6. **Monitor output**
   - Open Serial Monitor (Ctrl+Shift+A)
   - Set baud rate to **115200**
   - Expected output:
     ```
     Traffic Light FSM Started
     State: RED (Stop) - Barrier: CLOSED
     State: YELLOW (Caution) - Barrier: OPENING...
     State: GREEN (Go) - Barrier: OPEN
     ```

7. **Test**
   - Watch LEDs cycle: RED → YELLOW → GREEN → RED
   - Servo should rotate: 0° (closed) → 90° (open) → 0° (closed)
   - OLED displays current state in real-time

## 🔄 Finite State Machine (FSM) Architecture

This project implements a **3-state traffic control FSM**:

```
┌─────────────────────────────────────────────┐
│         TRAFFIC LIGHT FSM (T = 5s/state)   │
└─────────────────────────────────────────────┘

       ┌──────────────┐
       │     RED      │
       │ (5 seconds)  │◄──────────────┐
       │  Barrier: ❌ │              │
       └──────┬───────┘              │
              │                      │
              │ 5s elapsed           │ 5s elapsed
              ▼                      │
       ┌──────────────┐              │
       │   YELLOW     │              │
       │ (1-2 second) │──────────────┘
       │  Barrier: 🔄 │ (servo midpoint)
       └──────┬───────┘
              │
              │ 1.5s elapsed
              ▼
       ┌──────────────┐
       │    GREEN     │
       │ (5 seconds)  │
       │  Barrier: ✅ │
       └──────┬───────┘
              │ 5s elapsed
              └──────────────┬

Action per state:
─ RED:    digitalWrite(RED_PIN, HIGH);     servo.write(0°);   // Closed
─ YELLOW: digitalWrite(YELLOW_PIN, HIGH);  servo.write(45°);  // Opening
─ GREEN:  digitalWrite(GREEN_PIN, HIGH);   servo.write(90°);  // Open
```

**State Transitions**:
- **RED → YELLOW** (after 5 seconds)
- **YELLOW → GREEN** (after 1.5 seconds)
- **GREEN → RED** (after 5 seconds, loops)

## 💻 Code Structure

```
Traffic_Light_with_barriers/
├── platformio.ini          # Build configuration
├── src/
│   └── main.cpp           # FSM logic & main loop
├── include/
│   ├── traffic_fsm.h      # FSM state definitions
│   └── config.h           # Pin definitions & timing
├── lib/                   # External libraries (Adafruit, Servo)
└── README.md             # This file
```

### Key Code Snippet (FSM Loop)

```cpp
#include <Arduino.h>
#include <Servo.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define RED_PIN 12
#define YELLOW_PIN 14
#define GREEN_PIN 13
#define SERVO_PIN 26
#define OLED_SDA 21
#define OLED_SCL 22

// State machine
enum TrafficState { RED, YELLOW, GREEN };
TrafficState currentState = RED;
unsigned long stateStartTime = 0;

Servo barrier;
Adafruit_SSD1306 display(128, 64, &Wire, -1);

void setup() {
  Serial.begin(115200);
  
  // Initialize pins
  pinMode(RED_PIN, OUTPUT);
  pinMode(YELLOW_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  
  // Initialize servo (barrier gate)
  barrier.attach(SERVO_PIN);
  barrier.write(0);  // Start closed
  
  // Initialize OLED
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  
  Serial.println("Traffic Light FSM Started");
  stateStartTime = millis();
}

void loop() {
  unsigned long elapsed = millis() - stateStartTime;
  
  // FSM state transitions
  switch (currentState) {
    case RED:
      digitalWrite(RED_PIN, HIGH);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, LOW);
      barrier.write(0);  // Closed
      
      if (elapsed >= 5000) {  // Stay RED for 5 seconds
        currentState = YELLOW;
        stateStartTime = millis();
      }
      break;
      
    case YELLOW:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, HIGH);
      digitalWrite(GREEN_PIN, LOW);
      barrier.write(45);  // Opening...
      
      if (elapsed >= 1500) {  // YELLOW for 1.5 seconds
        currentState = GREEN;
        stateStartTime = millis();
      }
      break;
      
    case GREEN:
      digitalWrite(RED_PIN, LOW);
      digitalWrite(YELLOW_PIN, LOW);
      digitalWrite(GREEN_PIN, HIGH);
      barrier.write(90);  // Open
      
      if (elapsed >= 5000) {  // Stay GREEN for 5 seconds
        currentState = RED;
        stateStartTime = millis();
      }
      break;
  }
  
  // Update OLED display
  updateDisplay(currentState, elapsed);
  
  // Log to serial
  Serial.printf("State: %s | Elapsed: %lu ms\n", 
    getStateName(currentState), elapsed);
  
  delay(100);
}

const char* getStateName(TrafficState state) {
  switch (state) {
    case RED: return "RED (Stop)";
    case YELLOW: return "YELLOW (Caution)";
    case GREEN: return "GREEN (Go)";
  }
  return "UNKNOWN";
}

void updateDisplay(TrafficState state, unsigned long elapsed) {
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  
  display.setCursor(0, 0);
  display.println(getStateName(state));
  
  display.setTextSize(1);
  display.printf("Time: %lu ms\n", elapsed);
  
  int barrierAngle = (state == GREEN) ? 90 : (state == YELLOW) ? 45 : 0;
  display.printf("Barrier: %d°\n", barrierAngle);
  
  display.display();
}
```

## 🔧 Advanced Configuration

### Adjust Timing

Edit state durations in `include/config.h`:
```cpp
#define RED_DURATION 5000     // 5 seconds (ms)
#define YELLOW_DURATION 1500  // 1.5 seconds
#define GREEN_DURATION 5000   // 5 seconds
```

### Servo Angle Mapping

Calibrate servo if it doesn't close/open fully:
```cpp
barrier.write(0);    // Full close (adjust if needed: try 10, 20, etc.)
barrier.write(45);   // Mid-position (opening)
barrier.write(90);   // Full open (adjust if needed: try 85, 95, etc.)
```

### Extend with Button Override

Add manual control via push button:
```cpp
#define BUTTON_PIN 27

if (digitalRead(BUTTON_PIN) == LOW) {
  // Force GREEN state
  currentState = GREEN;
  stateStartTime = millis();
  delay(50);  // Debounce
}
```

## 🐛 Troubleshooting

| Issue | Solution |
|-------|----------|
| **LEDs not lighting** | Check GPIO pins; verify 220Ω resistors; test with multimeter |
| **Servo doesn't move** | Verify GPIO 26 PWM support; check 5V power to servo; test with `barrier.write(90)` |
| **OLED shows nothing** | Check I2C address (0x3C vs 0x3D); verify SDA/SCL on GPIO 21/22; install Adafruit library |
| **State stuck on RED** | Check `millis()` overflow (rare on ESP32); verify timer logic |
| **Serial garbage output** | Set baud rate to **115200**; check USB cable |
| **Servo jitter** | Add 100µF capacitor across servo power; move servo away from radio noise |

## 📚 Learning Outcomes

By completing this project, you'll master:
- ✅ **Finite State Machine (FSM) design** — core pattern for embedded systems
- ✅ **PWM control** — servo actuation & LED dimming
- ✅ **I2C communication** — OLED display integration
- ✅ **Real-time scheduling** — multi-millisecond precision timing
- ✅ **GPIO multiplexing** — managing 7+ I/O pins efficiently
- ✅ **State persistence** — debugging & monitoring FSM behavior

## 🚀 Next Steps & Extensions

1. **Add pedestrian button** → Override traffic light for foot traffic
2. **Sensor integration** → Motion/proximity detection to adjust timing
3. **MQTT cloud reporting** → Send traffic state to IoT platform
4. **Adaptive timing** → Change durations based on vehicle queue (using sensors)
5. **Multi-intersection sync** → Coordinate multiple traffic lights via ESP-NOW
6. **Real-time web dashboard** → Monitor live traffic state via WebSocket

## 📋 Dependencies

- **Arduino Framework** (built-in PlatformIO)
- **Adafruit SSD1306** (OLED driver) — auto-installed
- **Servo.h** (PWM control) — Arduino standard library
- **Wire.h** (I2C) — Arduino standard library

## 💡 Hardware Tips

- **Servo power**: Use separate 5V supply if servo jitters (USB 5V may brown-out)
- **Servo type**: SG90 is budget-friendly; MG90S is more precise
- **LED brightness**: 220Ω resistor works for 3.3V logic + 5V LED; adjust for dimmer effect
- **OLED contrast**: If display is faint, adjust `display.setContrast(50-255)` in code
- **Timing precision**: `millis()` on ESP32 is accurate to ~1ms; use `micros()` if sub-millisecond needed

## 🤝 Author

**Tuấn Kiệt Nguyễn** ([nguyentuankiet070507@gmail.com](mailto:nguyentuankiet070507@gmail.com))

Explores real-world smart city infrastructure patterns — finite state machines, multi-device coordination, and IoT system design.

## 📝 License

Open source — fork, modify, and use for educational or commercial projects.

## 🔗 Related Projects

- **RGB_LED_Make_it_colorful** — WiFi-controlled RGB LED (wireless IoT)
- **Counting_with_FreeRTOS** — Multi-task sensor system (real-time OS)
- **Led_control** — GPIO basics (foundation for traffic light)

---

**Last updated**: 2026  
**Board**: ESP32 DevKit V1  
**Framework**: Arduino + PlatformIO  
**Key concepts**: FSM, PWM, I2C, real-time scheduling

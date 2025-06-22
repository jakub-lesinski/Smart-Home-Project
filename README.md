# Smart Home Project

An embedded smart home system built on the **STM32 Nucleo** platform, communicating with a **Kotlin-based Android application**. The system enables modular control of room lighting and secure door access using sensors and RFID, connected via serial or wireless protocols.

---

## ✨ Features

- 🔒 **RFID-based access control** – scan tag to unlock doors
- 💡 **Lighting control** – room lights adapt to ambient conditions
- 📱 **Mobile app (Kotlin)** – control system components and monitor events
- 📊 Sensor integration – light sensors and optional motion detection
- 🧠 Embedded logic on STM32 Nucleo board

---

## 🧩 System Architecture

```
[ Kotlin App ] ⇄ [ Serial/Wireless ] ⇄ [ STM32 Nucleo ]
                                         ├─ Light Sensor
                                         ├─ LEDs (Room lighting)
                                         └─ RFID Reader (e.g. MFRC522)
```

---

## 🛠️ Hardware Requirements

- STM32 **Nucleo** development board (e.g., Nucleo-F401RE)
- RFID reader (e.g., MFRC522)
- LEDs + resistors
- LDR (light sensor)
- Jumper wires, breadboard
- USB cable (for programming and communication)

---

## ⚙️ Software Stack

- Embedded: **STM32 HAL / LL drivers**, written in C
- Mobile: **Android app in Kotlin**
- Communication: UART / Serial / USB (depending on setup)
- IDE: STM32CubeIDE for embedded code, Android Studio for app

---

## 📦 How to Set Up

### 🔧 STM32 Nucleo Firmware

1. Open STM32CubeIDE
2. Load firmware project from `/firmware/` folder
3. Flash to the Nucleo board
4. Ensure correct pin configuration for sensors and RFID

### 📱 Kotlin Android App

1. Open Android project in Android Studio (`/mobile-app/`)
2. Connect physical or virtual Android device
3. Build & run the app
4. Configure connection method (e.g., USB serial, BLE)

---

## 🧭 Project Structure

```
Smart-Home-Project/
├── firmware/           # STM32 project (C files, HAL/LL drivers)
├── mobile-app/         # Kotlin Android application
├── schematics/         # (Optional) wiring diagrams, hardware setup
└── README.md           # This file
```

---

## 🔁 How It Works

1. User interacts with the **Android app**
2. Commands sent to the **Nucleo board** (e.g., open door, toggle lights)
3. Nucleo reads sensors and RFID tags to make decisions
4. Feedback is sent back to the app and displayed in real time

---

# GY-521 Gesture Engine

Real-time gesture classifier running on a RISC-V MCU (ESP32-C) using a GY-521 MPU-6050 IMU

## Hardware
- ESP32-C3 DevKit (RISC_V core)
- GY-521 MPU-6050 IMU (I2C, 400kHz)

## Stack
- ESP-IDF 6.2 /FreeRTOS
C++17
TFLite Micro (coming soon)

## Wiring
| GY-521 | ESP32-C3 |
|--------|----------|
| VCC    | 3V3      |
| GND    | GND      |
| SCL    | GPIO7    |
| SDA    | GPIO6    |
| AD0    | GND      |

## Build
```bash
source ~/esp/esp-idf/export.sh
idf.py build
idf.py flash monitor
```

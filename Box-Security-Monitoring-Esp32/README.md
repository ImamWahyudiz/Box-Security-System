# ESP32 Version

## Hardware Requirements
- **ESP32 Development Board** (ESP32-WROOM-32 or similar)
- KY-003 Hall Effect Sensor
- KY-027 Mercury Switch Sensor
- Magnet for Hall sensor

## Pin Configuration
```
ESP32 Pin 32  →  KY-003 Hall Sensor Signal
ESP32 Pin 33  →  KY-027 Tilt Sensor Signal
ESP32 3.3V    →  Sensors VCC
ESP32 GND     →  Sensors GND
```

## Installation

### 1. Arduino IDE Setup
```bash
# Add ESP32 Board Support
File → Preferences → Additional Board Manager URLs:
https://dl.espressif.com/dl/package_esp32_index.json

# Install ESP32
Tools → Board → Boards Manager → Search "ESP32" → Install
```

### 2. Required Libraries
- **UniversalTelegramBot** by Brian Lough (v1.3.0+)
- **ArduinoJson** by Benoit Blanchon (v6.x)

### 3. Configuration
Edit `config.h`:
```cpp
const char* WIFI_SSID = "Your_WiFi_SSID";
const char* WIFI_PASSWORD = "Your_WiFi_Password";
#define TELEGRAM_BOT_TOKEN "YOUR_BOT_TOKEN"
#define TELEGRAM_CHAT_ID "YOUR_CHAT_ID"
```

### 4. Upload
1. Open `Box-Security-Monitoring.ino`
2. Select Board: **ESP32 Dev Module**
3. Select Port
4. Click Upload
5. Monitor via Serial (115200 baud)

## Features
- ✅ WiFi with auto-reconnect
- ✅ Telegram bot integration with SSL/TLS
- ✅ Remote arm/disarm control
- ✅ Sleep mode for power saving
- ✅ Continuous alerts when box is open
- ✅ Motion detection with stabilization

## Power Consumption
- **Active mode:** ~80-150mA
- **Sleep mode:** ~20-30mA (WiFi still active for commands)
- **Deep sleep:** < 1mA (requires code modification)

## Notes
- Uses `WiFiClientSecure` with certificate validation
- 520KB RAM available for program
- Dual-core processor for multitasking

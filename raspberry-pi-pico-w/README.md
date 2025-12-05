# Raspberry Pi Pico W Version

## Hardware Requirements
- **Raspberry Pi Pico W** (with wireless connectivity)
- KY-003 Hall Effect Sensor
- KY-027 Mercury Switch Sensor
- Magnet for Hall sensor
- Micro USB cable

## Pin Configuration
```
Pico W GPIO16  →  KY-003 Hall Sensor Signal
Pico W GPIO17  →  KY-027 Tilt Sensor Signal
Pico W 3.3V    →  Sensors VCC
Pico W GND     →  Sensors GND
```

## Installation

### 1. Arduino IDE Setup
```bash
# Add Raspberry Pi Pico Board Support
File → Preferences → Additional Board Manager URLs:
https://github.com/earlephilhower/arduino-pico/releases/download/global/package_rp2040_index.json

# Install RP2040 Boards
Tools → Board → Boards Manager → Search "pico" → Install "Raspberry Pi Pico/RP2040"
```

### 2. Required Libraries
- **UniversalTelegramBot** by Brian Lough (v1.3.0+)
- **ArduinoJson** by Benoit Blanchon (v6.x)

Libraries are available via Library Manager (same as ESP32)

### 3. Configuration
Edit `config.h`:
```cpp
const char* WIFI_SSID = "Your_WiFi_SSID";
const char* WIFI_PASSWORD = "Your_WiFi_Password";
#define TELEGRAM_BOT_TOKEN "YOUR_BOT_TOKEN"
#define TELEGRAM_CHAT_ID "YOUR_CHAT_ID"
```

### 4. Board Settings
```
Board: "Raspberry Pi Pico W"
Flash Size: "2MB (Sketch: 1MB, FS: 1MB)"
CPU Speed: "133 MHz"
Port: Select your Pico W port
```

### 5. Upload
1. Open `raspberry-pi-pico-w.ino`
2. Hold **BOOTSEL** button while connecting USB (first time only)
3. Select Board: **Raspberry Pi Pico W**
4. Select Port
5. Click Upload
6. Monitor via Serial (115200 baud)

## Features
- ✅ WiFi with auto-reconnect
- ✅ Telegram bot integration (using setInsecure())
- ✅ Remote arm/disarm control
- ✅ Sleep mode for power saving
- ✅ Continuous alerts when box is open
- ✅ Motion detection with stabilization
- ✅ Affordable alternative to ESP32

## Key Differences from ESP32

| Feature | Pico W | ESP32 |
|---------|--------|-------|
| **Processor** | RP2040 Dual ARM Cortex-M0+ @ 133MHz | Xtensa Dual-Core @ 240MHz |
| **RAM** | 264KB | 520KB |
| **Flash** | 2MB | 4MB (typical) |
| **WiFi** | 2.4GHz (Infineon CYW43439) | 2.4GHz (Espressif) |
| **Price** | ~$6 USD | ~$4-10 USD |
| **SSL/TLS** | Uses `setInsecure()` | Full certificate validation |
| **Power** | Lower consumption | Higher consumption |

## Power Consumption
- **Active mode:** ~60-100mA
- **Sleep mode:** ~15-25mA (WiFi active for commands)
- **Deep sleep:** ~1.3mA (requires DORMANT mode)

## Important Notes

### SSL/TLS Certificate
Pico W uses `secured_client.setInsecure()` instead of certificate validation:
```cpp
// In setup()
secured_client.setInsecure();  // Skip certificate validation
```

**⚠️ Security Note:** This makes the connection less secure but necessary due to RP2040's limited resources. Only use on trusted networks.

### Performance
- Slightly slower than ESP32 due to lower clock speed
- Sufficient for this application
- May have slightly longer response times for Telegram

### Memory Management
- Be mindful of RAM usage (264KB total)
- Avoid large String concatenations
- Use `F()` macro for constant strings

## Troubleshooting

### Pico W Not Recognized
- Hold BOOTSEL button while plugging in USB
- Should appear as mass storage device (RPI-RP2)
- Install board support package

### Upload Errors
- Make sure correct board is selected
- Try entering BOOTSEL mode manually
- Check USB cable (must support data)

### WiFi Connection Issues
- Ensure 2.4GHz WiFi (Pico W doesn't support 5GHz)
- Check SSID and password in config.h
- Some special characters in SSID may cause issues

### Telegram SSL Errors
- Code uses `setInsecure()` - this is normal for Pico W
- Ensure internet connection is stable
- Check Bot Token and Chat ID

## Advantages of Pico W
- ✅ **Lower cost** - More affordable than ESP32
- ✅ **Better availability** - Easier to find in stock
- ✅ **Lower power** - Better for battery operation
- ✅ **Great community** - MicroPython and C/C++ support
- ✅ **Raspberry Pi ecosystem** - Excellent documentation

## Recommended Use Cases
- ✅ Budget-conscious projects
- ✅ Battery-powered applications
- ✅ Learning IoT development
- ✅ When ESP32 is out of stock
- ✅ Projects requiring lower power consumption

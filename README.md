# Sistem Monitoring Keamanan Kotak
## Security Box Monitoring System

![Version](https://img.shields.io/badge/version-1.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)

Sistem monitoring keamanan real-time untuk kotak/peti penyimpanan menggunakan ESP32 dengan notifikasi Telegram. Mendeteksi pembukaan kotak dan gerakan mencurigakan.

## ✨ Features

- 🚨 **Instant Alert** - Notifikasi real-time saat kotak dibuka atau digerakkan
- 🧲 **Hall Effect Sensor** - Deteksi pembukaan kotak dengan sensor magnetik
- 📐 **Motion Detection** - Deteksi gerakan dan kemiringan kotak
- 🤖 **Telegram Bot** - Notifikasi langsung ke smartphone Anda
- 📊 **Periodic Reports** - Status report otomatis setiap 30 detik
- 🔋 **Low Power** - Efisien untuk operasi 24/7
- 🛡️ **Smart Detection** - Anti false alarm dengan stabilization time

## 📦 Hardware Requirements

| Component | Specification |
|-----------|--------------|
| **Microcontroller** | ESP32 Development Board |
| **Sensor 1** | KY-003 Hall Effect Sensor |
| **Sensor 2** | KY-027 Mercury Switch |
| **Power** | USB 5V / Power Bank / Adaptor |
| **Accessories** | Jumper wires, Magnet |

## 🔧 Quick Start

### 1. Install Arduino IDE & Libraries

```bash
# Install ESP32 Board Support
Board Manager URL: https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json

# Required Libraries:
- UniversalTelegramBot (v1.3.0+)
- ArduinoJson (v6.x)
```

### 2. Hardware Setup

```
ESP32 Pin 32  →  KY-003 Signal
ESP32 Pin 33  →  KY-027 Signal
ESP32 3.3V    →  Sensors VCC
ESP32 GND     →  Sensors GND
```

### 3. Configure

Edit `config.h`:
```cpp
const char* WIFI_SSID = "Your_WiFi_SSID";
const char* WIFI_PASSWORD = "Your_WiFi_Password";
#define TELEGRAM_BOT_TOKEN "123456789:ABC..."
#define TELEGRAM_CHAT_ID "987654321"
```

### 4. Upload & Run

1. Open `security_monitoring_system_v2.ino`
2. Select Board: ESP32 Dev Module
3. Select Port
4. Click Upload
5. Monitor via Serial Monitor (115200 baud)

## 📂 Project Structure

```
sketch_nov17b/
├── security_monitoring_system_v2.ino  # Main program
├── config.h                            # Configuration file
├── src/
│   ├── wifi_manager.h                  # WiFi connection handler
│   ├── telegram_manager.h              # Telegram bot manager
│   ├── sensor_handler.h                # Sensor logic
│   └── utils.h                         # Utility functions
├── docs/
│   ├── INSTALLATION.md                 # Installation guide
│   └── USER_GUIDE.md                   # User manual
└── README.md                           # This file
```

## 🎯 How It Works

### Detection Logic

**🚨 Box Opened Detection:**
- Hall sensor reads: `LOW` = Magnet close (Box closed)
- Hall sensor reads: `HIGH` = Magnet away (Box opened)
- **Alert:** Instant Telegram notification

**⚠️ Motion Detection:**
- Tilt sensor state change (0→1 or 1→0) = Motion detected
- **Alert:** First motion alert sent immediately
- **Continuous motion:** Additional alerts every 5 seconds
- **Stabilization:** No change for 3 seconds = Motion stopped
- **Auto-recovery:** System resets and ready for next detection

## 📱 Telegram Notifications

### Alert Types

| Icon | Type | Trigger | Priority |
|------|------|---------|----------|
| 🚨 | Box Opened | Magnet moved away | High |
| ⚠️ | Motion Detected | Box moved/tilted | High |
| ✅ | Box Closed | Magnet detected back | Normal |
| ✅ | Motion Stopped | Stable for 3 seconds | Normal |
| 📊 | Status Report | Every 30 seconds | Low |

### Example Notifications

```
🚨 ALERT: KOTAK DIBUKA!
━━━━━━━━━━━━━━━━━━━━━
🧲 Sensor Hall: Magnet menjauh
📦 Status: KOTAK TERBUKA
━━━━━━━━━━━━━━━━━━━━━
⏰ Waktu: 14:32:15
```

## ⚙️ Configuration

### Timing Settings (`config.h`)

```cpp
#define REPORT_INTERVAL 30000         // Periodic report (30s)
#define MOTION_STABLE_TIME 3000       // Stable time threshold (3s)
#define MOTION_ALERT_COOLDOWN 5000    // Motion alert cooldown (5s)
#define SENSOR_READ_DELAY 50          // Sensor polling rate (50ms)
```

### Pin Configuration

```cpp
#define PIN_HALL_SENSOR 32    // KY-003 Hall Sensor
#define PIN_TILT_SENSOR 33    // KY-027 Tilt Sensor
```

## 🐛 Troubleshooting

| Problem | Solution |
|---------|----------|
| WiFi not connecting | Check SSID/password, ensure 2.4GHz WiFi |
| No Telegram messages | Verify Bot Token and Chat ID, send /start to bot |
| Sensor not responding | Check wiring, verify 3.3V power supply |
| False motion alerts | Place on stable surface, adjust MOTION_STABLE_TIME |
| Compile errors | Install required libraries, use ArduinoJson v6.x |

For detailed troubleshooting, see [INSTALLATION.md](docs/INSTALLATION.md)

## 📖 Documentation

- **[Installation Guide](docs/INSTALLATION.md)** - Detailed setup instructions
- **[User Guide](docs/USER_GUIDE.md)** - How to use the system
- **[API Reference](src/)** - Code documentation

## 🔐 Security Notes

- ⚠️ **Never share** your Bot Token publicly
- 🔒 Use **private WiFi** with strong password
- 🛡️ Keep **config.h** secure (add to .gitignore)
- 🔄 **Update firmware** regularly for security patches

## 📊 System Requirements

- **ESP32** with at least 520KB RAM
- **WiFi** 2.4GHz network
- **Telegram** account for notifications
- **Arduino IDE** 1.8.x or newer

## 🤝 Contributing

Contributions are welcome! To contribute:

1. Fork the repository
2. Create feature branch (`git checkout -b feature/amazing-feature`)
3. Commit changes (`git commit -m 'Add amazing feature'`)
4. Push to branch (`git push origin feature/amazing-feature`)
5. Open Pull Request

## 📝 Changelog

### Version 1.0 (Current)
- ✨ Initial release
- 🚨 Box opening detection
- ⚠️ Motion detection with stabilization
- 🤖 Telegram bot integration
- 📊 Periodic status reports
- 🔋 Low power optimization

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👥 Authors

- **IoT Security Team** - Initial work

## 🙏 Acknowledgments

- UniversalTelegramBot library by Brian Lough
- ArduinoJson library by Benoit Blanchon
- ESP32 Arduino Core by Espressif Systems

## 📞 Support

For issues, questions, or suggestions:
- 📧 Open an issue in GitHub repository
- 💬 Check [User Guide](docs/USER_GUIDE.md) for common questions
- 📖 Read [Installation Guide](docs/INSTALLATION.md) for setup help

---

**⭐ If this project helps you, please consider giving it a star!**

Made with ❤️ by IoT Security Team

# Sistem Monitoring Keamanan Kotak
## Security Box Monitoring System

> 📚 **Academic Project Notice**  
> Proyek ini dibuat sebagai **Tugas Akhir Semester 3** untuk mata kuliah **Organisasi Komputer**

![Version](https://img.shields.io/badge/version-1.0-blue.svg)
![Platform](https://img.shields.io/badge/platform-ESP32-green.svg)
![License](https://img.shields.io/badge/license-MIT-orange.svg)

Sistem monitoring keamanan real-time untuk kotak/peti penyimpanan menggunakan ESP32 dengan notifikasi Telegram. Mendeteksi pembukaan kotak dan gerakan mencurigakan.

## ✨ Features

- 🚨 **Instant Alert** - Notifikasi real-time saat kotak dibuka atau digerakkan
- 🧲 **Hall Effect Sensor** - Deteksi pembukaan kotak dengan sensor magnetik
- 📐 **Motion Detection** - Deteksi gerakan dan kemiringan kotak
- 🤖 **Telegram Bot** - Notifikasi langsung ke smartphone Anda
- 🔐 **Remote Control** - Arm/Disarm sistem via command Telegram (/arm, /disarm, /sleep)
- 💤 **Sleep Mode** - Mode hemat energi dengan monitoring dinonaktifkan
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

### 🔄 Alternative Sensors (Recommended Upgrades)

Untuk hasil yang lebih akurat dan reliable, pertimbangkan upgrade ke sensor berikut:

#### **1. Door/Box Opening Detection:**

**🔵 Reed Switch (Recommended)**
- **Keunggulan:**
  - Lebih sensitif dan konsisten dari Hall Effect
  - Ukuran lebih kecil, mudah dipasang pada pintu/tutup kotak
  - Harga terjangkau (~Rp 2.000-5.000)
  - Kontak fisik = akurasi 100%
- **Contoh:** Reed Switch MC-38 / Door Magnetic Sensor
- **Wiring:** 2 pin (NO/NC), sama seperti switch biasa
- **Code Change:** Minimal, hanya ubah logika HIGH/LOW jika perlu

**⚡ Alternatif:**
- **Limit Switch** - Untuk tutup kotak yang menekan sensor
- **Optical Sensor** (IR Transmitter/Receiver) - Tanpa kontak fisik

#### **2. Motion/Tilt Detection:**

**🌀 MPU6050 Gyroscope + Accelerometer (Highly Recommended)**
- **Keunggulan:**
  - Data gerakan 6-axis (3-axis gyro + 3-axis accel)
  - Deteksi rotasi, getaran, dan orientasi yang presisi
  - Bisa tentukan sudut kemiringan exact
  - Komunikasi I2C, mudah diprogram
  - Harga terjangkau (~Rp 15.000-30.000)
- **Library:** MPU6050 by Electronic Cats
- **Use Case:** Deteksi kotak diangkat, dimiringkan, diguncang

**⚙️ Alternatif:**
- **ADXL345** - Accelerometer 3-axis (lebih murah)
- **Vibration Sensor SW-420** - Deteksi getaran sederhana
- **Tilt Switch SW-520D** - Upgrade dari mercury switch (lebih aman)

#### **💡 Configuration Tips:**

Jika menggunakan sensor alternatif, ubah di `config.h`:
```cpp
// Contoh untuk Reed Switch (aktif LOW saat tertutup)
#define HALL_SENSOR_INVERTED false

// Untuk MPU6050, tambahkan threshold
#define MOTION_THRESHOLD 5.0  // Sensitivity level
```

Untuk implementasi MPU6050, lihat folder `examples/mpu6050_integration/`

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
#define TELEGRAM_BOT_TOKEN "xxxxxxx:xxxxxxxxxxxxxxx"
#define TELEGRAM_CHAT_ID "xxxxxxxxxx"
```

### 4. Upload & Run

1. Open `Box-Security-Monitoring.ino`
2. Select Board: ESP32 Dev Module
3. Select Port
4. Click Upload
5. Monitor via Serial Monitor (115200 baud)

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

### 🎮 Remote Control Commands

Kontrol sistem dari Telegram dengan commands berikut:

| Command | Function | Description |
|---------|----------|-------------|
| `/start` | Help Menu | Tampilkan daftar command |
| `/help` | Help Menu | Tampilkan daftar command |
| `/status` | System Status | Cek status sistem (armed/sleep, uptime, WiFi) |
| `/arm` | Activate | Aktifkan monitoring (sensor dibaca, alert aktif) |
| `/disarm` | Deactivate | Nonaktifkan monitoring (sleep mode) |
| `/sleep` | Sleep Mode | Mode hemat energi (sensor tidak dibaca) |

**💡 Tips:**
- Gunakan `/sleep` saat kotak tidak perlu dipantau (hemat baterai)
- Kirim `/arm` untuk aktifkan kembali monitoring
- `/status` untuk cek apakah sistem armed atau sleep

### 🚨 Alert Types

| Icon | Type | Trigger | Priority |
|------|------|---------|----------|
| 🚨 | Box Opened | Magnet moved away | High |
| ⚠️ | Motion Detected | Box moved/tilted | High |
| ✅ | Box Closed | Magnet detected back | Normal |
| ✅ | Motion Stopped | Stable for 3 seconds | Normal |
| 📊 | Status Report | Every 30 seconds | Low |

### Example Notifications

```
🚨 ALERT: KOTAK TERBUKA!
━━━━━━━━━━━━━━━━━━━━━
📬 Status: TERBUKA
━━━━━━━━━━━━━━━━━━━━━
⏰ Waktu: 14:32:15
```

**Sleep Mode Response:**
```
💤 Sistem DISARMED

🔓 Mode sleep - hemat energi
🔇 Sensor tidak dibaca
⚡ Konsumsi daya minimal

Ketik /arm untuk aktifkan kembali
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


## 📝 Changelog

### Version 1.0 (Current)
- ✨ Initial release
- 🚨 Box opening detection
- ⚠️ Motion detection with stabilization
- 🤖 Telegram bot integration
- 🔐 Remote arm/disarm control via Telegram
- 💤 Sleep mode for energy saving
- 📊 Periodic status reports
- 🔋 Low power optimization

## 📄 License

This project is licensed under the MIT License - see the LICENSE file for details.

## 👥 Authors

- **Apachersa** - Initial work

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

Made with ❤️ by Apachersa

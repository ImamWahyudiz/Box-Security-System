# LAPORAN PROJECT: SISTEM MONITORING KEAMANAN KOTAK
## Box Security Monitoring System

---

## 📋 INFORMASI PROJECT

**Nama Project:** Box Security Monitoring System  
**Tanggal:** Desember 2025  
**Platform:** ESP32 & Raspberry Pi Pico 2 W  
**Bahasa:** C++ (Arduino) & MicroPython  
**Status:** Completed ✅

---

## 🎯 TUJUAN PROJECT

Mengembangkan sistem monitoring keamanan kotak berbasis IoT yang dapat:
1. Mendeteksi pembukaan kotak secara real-time
2. Mendeteksi pergerakan/perpindahan kotak
3. Mengirim notifikasi instant ke Telegram
4. Dapat dikendalikan remote melalui Telegram Bot
5. Efisien energi dengan mode sleep

---

## 🔧 KOMPONEN HARDWARE

### Versi ESP32
| Komponen | Spesifikasi | Fungsi |
|----------|-------------|--------|
| ESP32 Development Board | Dual-core 240MHz, WiFi/BT | Kontroler utama |
| KY-003 Hall Effect Sensor | GPIO 16 | Deteksi buka/tutup kotak |
| KY-027 Mercury Switch | GPIO 17 | Deteksi gerakan/kemiringan |
| Power Supply | 5V USB/Battery | Catu daya |

### Versi Raspberry Pi Pico 2 W
| Komponen | Spesifikasi | Fungsi |
|----------|-------------|--------|
| Raspberry Pi Pico 2 W | RP2350, 264KB RAM, WiFi | Kontroler utama |
| KY-027 Mercury Switch | GPIO 17 | Deteksi gerakan/kemiringan |
| Power Supply | 5V USB/Battery | Catu daya |

**Catatan:** Versi Pico 2 W hanya menggunakan tilt sensor karena keterbatasan GPIO compatibility dengan Hall sensor.

---

## 💻 SOFTWARE & TEKNOLOGI

### Framework & Library
- **ESP32:**
  - Arduino Framework
  - WiFi.h - Koneksi WiFi
  - WiFiClientSecure.h - HTTPS communication
  - UniversalTelegramBot.h - Telegram Bot API

- **Raspberry Pi Pico 2 W:**
  - MicroPython
  - network module - WiFi management
  - urequests - HTTP requests
  - ujson - JSON parsing
  - machine - GPIO control

### Development Tools
- Arduino IDE (ESP32)
- Thonny IDE (Pico 2 W)
- Git/GitHub (Version control)
- Telegram Bot API

---

## ⚙️ FITUR SISTEM

### 1. Monitoring Real-time
- ✅ Deteksi pembukaan kotak (Hall sensor - ESP32)
- ✅ Deteksi gerakan/perubahan posisi (Tilt sensor)
- ✅ Status sensor real-time
- ✅ Motion stability detection

### 2. Notifikasi Telegram
- ✅ Alert instant saat kotak dibuka
- ✅ Alert saat kotak digerakkan
- ✅ Security breach alert (persistent)
- ✅ Notifikasi perubahan status

### 3. Remote Control via Telegram
| Command | Fungsi |
|---------|--------|
| `/status` | Tampilkan status sistem lengkap |
| `/arm` | Aktifkan monitoring |
| `/disarm` atau `/sleep` | Mode hemat energi |
| `/safe` | Reset security breach |
| `/help` | Tampilkan menu command |

### 4. Keamanan
- ✅ Chat ID validation
- ✅ Unauthorized access detection
- ✅ Persistent breach flag (sampai konfirmasi `/safe`)
- ✅ HTTPS communication

### 5. Efisiensi Energi
- ✅ Mode sleep (disarm)
- ✅ Sensor tidak dibaca saat sleep
- ✅ Konsumsi daya minimal
- ✅ Delay optimized

---

## 📊 ARSITEKTUR SISTEM

### Struktur File ESP32
```
Box-Security-Monitoring-Esp32/
├── Box-Security-Monitoring-Esp32.ino  # Main program
├── config.h                           # Konfigurasi user
├── wifi_manager.h                     # WiFi management
├── sensor_handler.h                   # Sensor logic
├── telegram_manager.h                 # Telegram integration
└── utils.h                            # Helper functions
```

### Struktur File Raspberry Pi Pico 2 W
```
raspberry-pi-pico-2w-python/
├── main.py         # Program utama
├── config.py       # Konfigurasi user
└── README.md       # Dokumentasi
```

---

## 🔄 ALUR KERJA SISTEM

### 1. Startup
```
Power ON → WiFi Connect → Telegram Bot Init → System Ready
                ↓
        Send Startup Message
```

### 2. Mode Armed (Monitoring Aktif)
```
Read Sensors → Detect Changes → Send Alert (if needed)
     ↓              ↓                    ↓
  Hall/Tilt    Motion/Open         Telegram
  Sensors      Detection           Notification
```

### 3. Alert Flow
```
Event Detected → Validate → Send Alert → Update Status
     ↓              ↓            ↓           ↓
  Box Opened   Check Armed   Telegram    Set Flags
  or Moved     Mode Active   Message     (breach/motion)
```

### 4. Security Breach
```
Box Opened (Armed) → Set breach=true → Repeat Alert
                          ↓
                    User sends /safe
                          ↓
                    Reset breach=false
```

---

## 📈 HASIL PENGUJIAN

### Test Case 1: Deteksi Pembukaan Kotak (ESP32)
- **Status:** ✅ PASS
- **Hasil:** Hall sensor mendeteksi perubahan status dalam <100ms
- **Alert:** Terkirim ke Telegram dalam 1-2 detik

### Test Case 2: Deteksi Gerakan
- **Status:** ✅ PASS (ESP32 & Pico 2 W)
- **Hasil:** Tilt sensor mendeteksi perubahan posisi akurat
- **Stability:** Motion berhenti setelah 3 detik tanpa perubahan

### Test Case 3: Telegram Commands
- **Status:** ✅ PASS
- **Hasil:** Semua command (`/status`, `/arm`, `/disarm`, `/safe`) berfungsi
- **Response Time:** <2 detik

### Test Case 4: Security Breach Persistence
- **Status:** ✅ PASS
- **Hasil:** Breach flag tetap aktif sampai `/safe` dikirim
- **Alert Interval:** Berulang setiap 30 detik

### Test Case 5: Mode Sleep
- **Status:** ✅ PASS
- **Hasil:** Sensor tidak dibaca, konsumsi daya menurun
- **Wake Up:** Instant response saat `/arm`

### Test Case 6: WiFi Reconnection
- **Status:** ✅ PASS
- **Hasil:** Auto-reconnect setelah koneksi terputus
- **Recovery Time:** 5-10 detik

---

## 🔍 ANALISIS PERFORMA

### ESP32 Version
| Metric | Value |
|--------|-------|
| Sensor Read Interval | 100ms |
| Alert Response Time | 1-2 detik |
| WiFi Connection Time | 3-5 detik |
| RAM Usage | ~40KB / 520KB |
| Power Consumption (Active) | ~150-200mA |
| Power Consumption (Sleep) | ~30-50mA |

### Raspberry Pi Pico 2 W Version
| Metric | Value |
|--------|-------|
| Sensor Read Interval | 100ms |
| Alert Response Time | 2-3 detik |
| WiFi Connection Time | 5-8 detik |
| RAM Usage | ~80KB / 264KB |
| Power Consumption (Active) | ~100-150mA |
| Power Consumption (Sleep) | ~20-30mA |

---

## 🚧 TANTANGAN & SOLUSI

### 1. Hall Sensor GPIO Compatibility
**Problem:** Hall sensor (KY-003) tidak berfungsi di Raspberry Pi Pico 2 W (stuck di value 1)

**Analisis:**
- Sensor bekerja normal di ESP32
- RP2350 chip memiliki GPIO handling berbeda
- GPIO 16/17 kemungkinan konflik dengan SPI

**Solusi:**
- Versi Pico 2 W: Hapus Hall sensor, gunakan Tilt sensor only
- Sistem tetap fungsional dengan single sensor
- Simplifikasi meningkatkan reliability

### 2. urequests Parameter Limitation
**Problem:** MicroPython `urequests.get()` tidak support parameter `params`

**Error:**
```python
response = urequests.get(url, params=params)
# TypeError: unexpected keyword argument 'params'
```

**Solusi:**
```python
# Build query string manual
url = f"https://api.telegram.org/bot{TOKEN}/getUpdates?offset={offset}&timeout=1"
response = urequests.get(url)
```

### 3. Persistent Breach Alert
**Problem:** Alert kotak terbuka hanya sekali, user bisa miss notification

**Solusi:**
- Implementasi `securityBreached` flag
- Alert berulang setiap 30 detik sampai user konfirmasi
- Command `/safe` untuk reset breach status

### 4. Mode Sleep Efficiency
**Problem:** Konsumsi daya tetap tinggi saat tidak digunakan

**Solusi:**
- Implementasi mode disarm/sleep
- Sensor tidak dibaca saat sleep
- Delay diperpanjang untuk hemat energi
- WiFi tetap connected untuk remote wake-up

---

## 📱 CONTOH OUTPUT SISTEM

### Status Command Output
```
📊 STATUS SISTEM LENGKAP

━━━━━━━━━━━━━━━━━━━━━
🔒 Mode: ARMED ✓
⚠️ Security Breach: Clear ✓

📡 SENSOR STATUS:
━━━━━━━━━━━━━━━━━━━━━
📬 Hall Sensor (KY-003):
  • Pin: GPIO 16
  • Value: 1
  • Status: Tertutup ✓

📐 Tilt Sensor (KY-027):
  • Pin: GPIO 17
  • Value: 1
  • Motion: Diam ✓

⚙️ SYSTEM INFO:
━━━━━━━━━━━━━━━━━━━━━
⏰ Uptime: 00:15:23
🕐 Time: 14:30:45

📶 WiFi: Connected
  • SSID: MyWiFi
  • IP: 192.168.1.100
  • RSSI: -45 dBm

🖥️ PLATFORM:
━━━━━━━━━━━━━━━━━━━━━
• ESP32 Development Board
• Hall + Tilt Sensors
• Arduino Framework
━━━━━━━━━━━━━━━━━━━━━
```

### Alert Examples
```
🚨 ALERT: KOTAK TERBUKA!

━━━━━━━━━━━━━━━━━━━━━
📬 Status: TERBUKA
🕐 14:31:02
```

```
⚠️ ALERT: KOTAK DIGERAKKAN!

📐 Sensor Tilt: Perubahan posisi
📦 Status: KOTAK BERGERAK
🕐 14:32:15
```

```
⚠️ PELANGGARAN KEAMANAN!

🔓 KOTAK TELAH DIBUKA
📢 Alert akan terus dikirim
✅ Gunakan /safe untuk konfirmasi aman
🕐 14:33:20
```

---

## 📚 DOKUMENTASI TEKNIS

### Konfigurasi Sensor

**Hall Sensor (KY-003) - ESP32:**
```cpp
#define PIN_HALL_SENSOR 16
#define HALL_SENSOR_INVERTED true  // HIGH = tertutup
pinMode(PIN_HALL_SENSOR, INPUT_PULLUP);
```

**Tilt Sensor (KY-027):**
```cpp
// ESP32
#define PIN_TILT_SENSOR 17
pinMode(PIN_TILT_SENSOR, INPUT_PULLUP);

// Pico 2 W
PIN_TILT_SENSOR = 17
tilt_sensor = machine.Pin(17, machine.Pin.IN, machine.Pin.PULL_UP)
```

### WiFi Configuration
```cpp
// config.h
#define WIFI_SSID "your_wifi_name"
#define WIFI_PASSWORD "your_wifi_password"
```

### Telegram Bot Setup
```cpp
#define TELEGRAM_BOT_TOKEN "1234567890:ABCdefGHIjklMNOpqrsTUVwxyz"
#define TELEGRAM_CHAT_ID "123456789"
```

### Timing Configuration
```cpp
#define SENSOR_READ_DELAY 100           // 100ms
#define BOT_CHECK_INTERVAL 1000         // 1 detik
#define MOTION_STABLE_TIME 3000         // 3 detik
#define BOX_OPEN_ALERT_INTERVAL 30000   // 30 detik
```

---

## 🎓 PEMBELAJARAN & INSIGHT

### Technical Skills
1. ✅ IoT system design & implementation
2. ✅ Multi-platform development (ESP32 & Pico 2 W)
3. ✅ Dual-language programming (C++ & Python)
4. ✅ Sensor integration & calibration
5. ✅ API integration (Telegram Bot)
6. ✅ HTTPS communication & security
7. ✅ Error handling & debugging
8. ✅ Power optimization

### Problem Solving
1. ✅ Hardware compatibility analysis
2. ✅ GPIO troubleshooting
3. ✅ Library limitation workarounds
4. ✅ Code optimization & refactoring

### Best Practices
1. ✅ Modular code structure
2. ✅ Configuration separation
3. ✅ Comprehensive error handling
4. ✅ User-friendly notifications
5. ✅ Security considerations
6. ✅ Documentation

---

## 🚀 PENGEMBANGAN MASA DEPAN

### Short Term
- [ ] Web dashboard untuk monitoring
- [ ] Data logging ke SD card
- [ ] Battery level monitoring
- [ ] Multiple box support

### Long Term
- [ ] Machine learning untuk anomaly detection
- [ ] GPS tracking integration
- [ ] Cloud database integration
- [ ] Mobile app development
- [ ] Multi-user support

---

## 📝 KESIMPULAN

Project **Box Security Monitoring System** berhasil dikembangkan dengan fitur:
- ✅ Dual-platform support (ESP32 & Pico 2 W)
- ✅ Real-time monitoring & instant alerts
- ✅ Remote control via Telegram
- ✅ Persistent security breach detection
- ✅ Energy-efficient sleep mode
- ✅ Reliable & responsive system

Sistem ini membuktikan bahwa IoT security monitoring dapat diimplementasikan dengan:
- Hardware affordable (~Rp 150.000)
- Platform open-source (Arduino & MicroPython)
- Free communication (Telegram Bot API)
- Mudah dikembangkan & di-maintain

Project ini memberikan pengalaman praktis dalam:
- Embedded systems programming
- IoT architecture design
- API integration
- Hardware troubleshooting
- Multi-platform development

---

## 👨‍💻 INFORMASI DEVELOPER

**Repository:** [GitHub - Box-Security-System](https://github.com/ImamWahyudiz/Box-Security-System)  
**Documentation:** README.md, USER_GUIDE.md, INSTALLATION.md  
**License:** Open Source

---

## 📎 LAMPIRAN

### A. Wiring Diagram
```
ESP32:
  Hall Sensor (KY-003)    Tilt Sensor (KY-027)
  VCC → 3.3V              VCC → 3.3V
  GND → GND               GND → GND
  DO  → GPIO 16           DO  → GPIO 17

Raspberry Pi Pico 2 W:
  Tilt Sensor (KY-027)
  VCC → 3.3V (Pin 36)
  GND → GND (Pin 38)
  DO  → GPIO 17 (Pin 22)
```

### B. Serial Monitor Output Sample
```
========================================
  BOX SECURITY MONITORING SYSTEM
========================================
✓ WiFi connected
✓ IP: 192.168.1.100
✓ RSSI: -45 dBm
✓ Telegram connected
✓ Bot: @MySecurityBot

[ARMED MODE] Monitoring active...

⏰ 14:30:45 | 📬 Hall: 1 (Tertutup) | 📐 Tilt: 1 (Diam)
⏰ 14:30:46 | 📬 Hall: 1 (Tertutup) | 📐 Tilt: 1 (Diam)
⚠️ KOTAK TERBUKA!
📤 Alert terkirim ke Telegram
⏰ 14:30:47 | 📬 Hall: 0 (Terbuka) | 📐 Tilt: 1 (Diam)
```

### C. Telegram Bot Command List
| Command | Description | Response Time |
|---------|-------------|---------------|
| `/start` | Mulai bot | <1s |
| `/help` | Menu bantuan | <1s |
| `/status` | Status lengkap | 1-2s |
| `/arm` | Aktifkan monitor | <1s |
| `/disarm` | Mode sleep | <1s |
| `/safe` | Reset breach | <1s |

---

**Tanggal Laporan:** 11 Desember 2025  
**Versi Dokumen:** 1.0  
**Status Project:** ✅ COMPLETED & TESTED

---

*Laporan ini dapat digunakan untuk keperluan dokumentasi project, presentasi, atau laporan akademik.*

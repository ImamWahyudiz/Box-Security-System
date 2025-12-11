  # Sistem Monitoring Keamanan Kotak
## Raspberry Pi Pico 2 W - MicroPython Version (Tilt Sensor Only)

> 🐍 **MicroPython Version**  
> Versi Python untuk Raspberry Pi Pico 2 W menggunakan Thonny IDE  
> ⚠️ **TILT SENSOR ONLY** - Hall sensor dihapus karena kompatibilitas

![Platform](https://img.shields.io/badge/platform-Pico%202%20W-green.svg)
![Language](https://img.shields.io/badge/language-MicroPython-blue.svg)
![IDE](https://img.shields.io/badge/IDE-Thonny-orange.svg)

## 🎯 Overview

Sistem monitoring keamanan kotak dengan Telegram notification menggunakan **MicroPython** di **Raspberry Pi Pico 2 W**. Versi ini hanya pakai **KY-027 Tilt Sensor** untuk deteksi gerakan kotak.

## ✨ Features

- 🚨 **Real-time Alert** - Notifikasi instant via Telegram
- 📐 **Tilt Detection** - Deteksi gerakan dan kemiringan kotak
- 🔐 **Remote Control** - Arm/disarm via Telegram commands
- 💤 **Sleep Mode** - Hemat energi saat tidak monitoring
- ⚠️ **Persistent Breach Alert** - Alert terus sampai konfirmasi /safe
- 🐍 **Pure Python** - Mudah dipahami dan dimodifikasi

## 📦 Hardware Requirements

| Component | Specification |
|-----------|--------------|
| **Microcontroller** | Raspberry Pi Pico 2 W |
| **Sensor** | KY-027 Mercury Switch / Tilt Sensor |
| **Power** | Micro USB 5V |
| **Accessories** | Jumper wires |

**Note:** Hall sensor (KY-003) tidak digunakan karena masalah kompatibilitas GPIO di Pico 2 W.

## 🔌 Wiring Diagram

```
Raspberry Pi Pico 2 W      KY-027 Tilt Sensor
━━━━━━━━━━━━━━━━━━━━━    ━━━━━━━━━━━━━━━━━
GPIO 17 ─────────────────── DO (Digital Out)
3V3 ─────────────────────── VCC
GND ─────────────────────── GND
```

**Note:** Hall sensor tidak digunakan di versi ini.

## 🚀 Installation

### 1. Install MicroPython di Pico 2 W

1. **Download MicroPython firmware:**
   - Kunjungi: https://micropython.org/download/RPI_PICO2/
   - Download file `.uf2` terbaru untuk Pico 2 W

2. **Upload firmware:**
   - Tahan tombol **BOOTSEL** di Pico 2 W
   - Sambungkan ke komputer via USB (tetap tahan BOOTSEL)
   - Pico akan muncul sebagai USB drive `RPI-RP2`
   - Drag & drop file `.uf2` ke drive tersebut
   - Pico akan restart otomatis

### 2. Install Thonny IDE

1. Download Thonny dari: https://thonny.org/
2. Install sesuai OS Anda (Windows/Mac/Linux)
3. Buka Thonny

### 3. Setup Thonny untuk Pico

1. Di Thonny, klik **Run → Select Interpreter**
2. Pilih **MicroPython (Raspberry Pi Pico)**
3. Pilih port COM Pico Anda
4. Klik **OK**

### 4. Install Library yang Diperlukan

**Library `urequests`** (untuk HTTP requests):

1. Di Thonny, klik **Tools → Manage packages**
2. Search: `micropython-urequests`
3. Klik **Install**

**Atau install manual via Shell:**
```python
import upip
upip.install('micropython-urequests')
```

### 5. Konfigurasi

Edit file `config.py`:

```python
# WiFi Configuration
WIFI_SSID = "YourWiFiName"
WIFI_PASSWORD = "YourWiFiPassword"

# Telegram Configuration
TELEGRAM_BOT_TOKEN = "123456789:ABCdefGHIjklMNOpqrsTUVwxyz"
TELEGRAM_CHAT_ID = "987654321"

# Pin Configuration (sesuaikan jika perlu)
PIN_TILT_SENSOR = 17  # GPIO 17
```

**Cara mendapatkan Telegram Token & Chat ID:** Sama seperti versi Arduino (lihat README utama)

**Note:** `PIN_HALL_SENSOR` tidak ada karena Hall sensor tidak digunakan.

### 6. Upload File ke Pico

1. **Buka file di Thonny:**
   - File → Open
   - Pilih `config.py`
   
2. **Save ke Pico:**
   - File → Save As
   - Pilih **Raspberry Pi Pico** (bukan This Computer)
   - Save sebagai `config.py`

3. **Ulangi untuk `main.py`:**
   - Open `main.py`
   - File → Save As → Raspberry Pi Pico
   - Save sebagai `main.py`

### 7. Jalankan Program

**Opsi 1: Run Manual**
- Klik tombol ▶️ **Run** di Thonny
- Program akan jalan dan tampil output di Shell

**Opsi 2: Auto-run saat Boot**
- Rename `main.py` menjadi `boot.py` atau biarkan sebagai `main.py`
- MicroPython akan otomatis jalankan `main.py` saat boot
- Disconnect USB dan reconnect untuk test

## 📱 Telegram Commands

| Command | Function |
|---------|----------|
| `/start` atau `/help` | Tampilkan menu command |
| `/status` | Cek status sistem (armed/sleep, uptime, breach) |
| `/arm` | Aktifkan monitoring |
| `/disarm` atau `/sleep` | Mode hemat energi |
| `/safe` | Reset breach flag, stop alert berulang |

## 🎮 How to Use

### Pertama Kali:

1. Upload `config.py` dan `main.py` ke Pico
2. Konfigurasi WiFi dan Telegram di `config.py`
3. Pasang sensor sesuai wiring
4. Power on Pico → akan auto-connect WiFi
5. Bot kirim notif "SISTEM MONITORING AKTIF"

### Monitoring:

- Sistem default **ARMED** (monitoring aktif)
- Miringkan/gerakkan kotak → Alert dikirim ke Telegram
- Alert terus berulang sampai kirim `/safe`
- Gerakan terdeteksi via Tilt sensor

### Remote Control:

- Kirim `/sleep` → Matikan monitoring (hemat baterai)
- Kirim `/arm` → Aktifkan kembali monitoring
- Kirim `/status` → Cek kondisi sistem

## 🔧 Troubleshooting

### WiFi Tidak Connect

```python
# Cek di Shell Thonny:
import network
wlan = network.WLAN(network.STA_IF)
wlan.active(True)
wlan.scan()  # Lihat WiFi yang terdeteksi
```

**Solusi:**
- Pastikan WiFi 2.4GHz (Pico W tidak support 5GHz)
- Cek SSID dan password di `config.py`
- Restart Pico (unplug & replug USB)

### Sensor Tidak Respond

**Hall Sensor Selalu Terbaca Sama (Tertutup/Terbuka):**

```python
# Test sensor di Shell Thonny:
from machine import Pin

# PENTING: KY-003 di ESP32 pakai INPUT (tanpa pull resistor)
# Sama di Pico, coba tanpa pull dulu:
hall = Pin(16, Pin.IN)  # Tanpa PULL_UP atau PULL_DOWN
print(f"Hall value: {hall.value()}")

# Coba dekatkan/jauhkan magnet, nilai harus berubah!
# Test loop continuous:
while True:
    h = hall.value()
    print(f"Hall: {h} {'<-- MAGNET!' if h == 1 else ''}")
    time.sleep(0.2)
```

**Solusi Step-by-Step:**

1. **Cek Wiring Dulu:**
   ```
   KY-003 Hall Sensor → Pico 2 W
   S (Signal)         → GPIO 16
   + (VCC/Middle)     → 3V3
   - (GND)            → GND
   ```

2. **Konfigurasi Pin Sama dengan ESP32:**
   
   Di `main.py`, Hall sensor TANPA pull resistor:
   ```python
   # Seperti pinMode(PIN, INPUT) di ESP32
   hall_sensor = machine.Pin(PIN_HALL_SENSOR, machine.Pin.IN)
   ```
   
   Sudah diupdate otomatis! ✓

3. **Flip Sensor Logic di config.py jika perlu:**
   ```python
   # Coba toggle ini:
   HALL_SENSOR_INVERTED = False  # atau True
   ```

4. **Test dengan `check_sensor.py`:**
   - Upload dan run file test
   - Dekatkan/jauhkan magnet
   - Lihat nilai berubah atau tidak

**Tilt Sensor (Sudah Benar):**

```python
# Test tilt sensor di Shell:
from machine import Pin
tilt = Pin(17, Pin.IN, Pin.PULL_UP)

# Loop test:
while True:
    print(f"Tilt: {tilt.value()}")
    time.sleep(0.5)
```

**Solusi:**
- Cek wiring (VCC, GND, Signal)
- Pastikan sensor dapat power 3.3V
- Untuk Hall: Gunakan `PULL_DOWN` bukan `PULL_UP`
- Coba setting `HALL_SENSOR_INVERTED` di config

### Telegram Tidak Kirim Pesan

```python
# Test manual di Shell:
import urequests
url = "https://api.telegram.org/bot<TOKEN>/getMe"
response = urequests.get(url)
print(response.text)  # Harus return bot info
```

**Solusi:**
- Cek Bot Token dan Chat ID
- Pastikan WiFi connected
- Test dengan `/start` di Telegram

### Program Crash / Error

- Lihat error message di Shell Thonny
- Common errors:
  - `ImportError: no module named 'urequests'` → Install library
  - `OSError: [Errno 110]` → WiFi timeout, cek koneksi
  - `MemoryError` → Pico kehabisan RAM, reboot

## 📊 Serial Monitor Output

Saat program berjalan, Anda akan lihat output seperti:

```
=============================================
   SISTEM MONITORING KEAMANAN KOTAK
   Raspberry Pi Pico 2 W - MicroPython
=============================================

✓ Pin sensor diinisialisasi
  - Hall Sensor: GPIO 16
  - Tilt Sensor: GPIO 17

Menghubungkan ke WiFi: MyWiFi
.....
✓ WiFi terhubung!
  IP Address: 192.168.1.100
  RSSI: -45 dBm

Mengirim notifikasi startup...
✓ Notifikasi startup terkirim

=============================================
  ✓ Sistem siap beroperasi
=============================================

🔄 Memulai monitoring...

[10s] 🔒 ARMED | 📬 TERTUTUP ✓ | 📐 Diam ○
[11s] 🔒 ARMED | 📬 TERTUTUP ✓ | 📐 Diam ○
[12s] 🔒 ARMED | 📬 TERBUKA ⚠ | 📐 Diam ○

===================================
  🚨 SECURITY BREACHED!
     KOTAK TERBUKA!
===================================

📨 Pesan diterima: /safe
✓ Security breach flag RESET via /safe
```

## 🔄 Perbedaan dengan Versi Arduino

| Aspect | Arduino (C++) | MicroPython |
|--------|---------------|-------------|
| **Bahasa** | C/C++ | Python |
| **IDE** | Arduino IDE | Thonny |
| **Learning Curve** | Medium | Easy |
| **Performance** | Lebih cepat | Sedikit lebih lambat |
| **Code Readability** | Verbose | Lebih ringkas |
| **Debugging** | Serial Monitor | Thonny Shell (interaktif) |
| **Library** | Arduino libraries | MicroPython modules |
| **Memory** | Lebih efisien | Overhead Python |

## 💡 Tips & Best Practices

1. **Hemat Baterai:**
   - Gunakan `/sleep` saat tidak perlu monitoring
   - Turunkan `SENSOR_READ_DELAY` jika perlu response lebih cepat

2. **Debugging:**
   - Gunakan Thonny Shell untuk test kode interaktif
   - Tambahkan `print()` untuk debug

3. **Modifikasi:**
   - Semua config ada di `config.py` - mudah diubah
   - Tambah sensor baru? Tinggal import `machine.Pin`

4. **Auto-run:**
   - File `main.py` otomatis dijalankan saat boot
   - Untuk disable auto-run, rename file

## 📁 File Structure

```
raspberry-pi-pico-2w-python/
├── main.py                # Program utama
├── config.py              # Konfigurasi sistem
├── test_hall_sensor.py    # Test troubleshooting Hall sensor
└── README.md              # Dokumentasi ini
```

## 🧪 Testing & Debugging

### Quick Test Hall Sensor

Jika Hall sensor tidak terbaca benar, jalankan test script:

1. **Upload `test_hall_sensor.py` ke Pico**
2. **Run di Thonny**
3. **Dekatkan/jauhkan magnet** saat test berjalan
4. **Lihat nilai berubah atau tidak**

Script akan test kedua mode:
- ✅ PULL_DOWN (recommended untuk KY-003)
- ⚠️ PULL_UP (kadang tidak cocok)

Gunakan mode yang nilai-nya **BERUBAH** saat magnet didekatkan!

## 🆚 Perbandingan: Arduino vs MicroPython

**Pilih Arduino C++ jika:**
- ✅ Butuh performance maksimal
- ✅ Familiar dengan C/C++
- ✅ Ingin memory efficiency optimal
- ✅ Banyak library Arduino tersedia

**Pilih MicroPython jika:**
- ✅ Familiar dengan Python
- ✅ Ingin rapid prototyping
- ✅ Lebih suka syntax yang simple
- ✅ Butuh debugging interaktif (REPL)

## 📝 Changelog

### Version 1.0
- ✨ Initial MicroPython version
- 🚨 Security breach alert system
- 🔐 Remote arm/disarm via Telegram
- 💤 Sleep mode support
- ⚠️ Persistent breach alerts
- 🐍 Full Python implementation

## 📄 License

MIT License - Free to use and modify

## 🙏 Credits

- **MicroPython** - https://micropython.org/
- **Thonny IDE** - https://thonny.org/
- **Telegram Bot API** - https://core.telegram.org/bots/api

---

**⭐ Enjoy coding dengan Python di Pico 2 W!**

Made with 🐍 Python & ❤️

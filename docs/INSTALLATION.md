# Panduan Instalasi dan Setup

## 📋 Requirements

### Hardware
- ESP32 Development Board
- KY-003 Hall Effect Sensor
- KY-027 Mercury Switch Sensor
- Kabel jumper
- USB Cable untuk programming
- Power supply 3.3V

### Software
- Arduino IDE (versi 1.8.x atau lebih baru)
- ESP32 Board Package
- Library yang diperlukan (lihat bagian Library)

## 📚 Instalasi Library

Buka Arduino IDE → Tools → Manage Libraries, lalu install:

1. **UniversalTelegramBot** by Brian Lough
   - Versi: 1.3.0 atau lebih baru
   
2. **ArduinoJson** by Benoit Blanchon
   - Versi: 6.x (PENTING: jangan gunakan versi 7.x)

## 🔧 Setup ESP32 di Arduino IDE

1. Buka Arduino IDE
2. File → Preferences
3. Pada "Additional Board Manager URLs", tambahkan:
   ```
   https://raw.githubusercontent.com/espressif/arduino-esp32/gh-pages/package_esp32_index.json
   ```
4. Tools → Board → Boards Manager
5. Cari "ESP32" dan install "esp32 by Espressif Systems"
6. Tools → Board → ESP32 Arduino → pilih "ESP32 Dev Module"

## 🔌 Koneksi Hardware

### KY-003 Hall Effect Sensor
```
S (Signal) → ESP32 Pin 32
+ (VCC)    → 3.3V
- (GND)    → GND
```

### KY-027 Mercury Switch
```
S (Signal) → ESP32 Pin 33
+ (VCC)    → 3.3V
- (GND)    → GND
```

### Diagram Koneksi
```
ESP32                KY-003 (Hall)
┌────────┐          ┌──────────┐
│        │          │          │
│  Pin32 ├──────────┤ S        │
│  3.3V  ├──────────┤ +        │
│  GND   ├──────────┤ -        │
│        │          └──────────┘
│        │
│        │          KY-027 (Tilt)
│        │          ┌──────────┐
│  Pin33 ├──────────┤ S        │
│  3.3V  ├──────────┤ +        │
│  GND   ├──────────┤ -        │
└────────┘          └──────────┘
```

## 🤖 Setup Telegram Bot

### Langkah 1: Buat Bot Baru
1. Buka aplikasi Telegram
2. Cari dan chat dengan `@BotFather`
3. Kirim perintah: `/newbot`
4. Ikuti instruksi:
   - Masukkan nama bot (contoh: "Security Box Monitor")
   - Masukkan username bot (harus diakhiri dengan 'bot', contoh: "securitybox_bot")
5. **Simpan Bot Token** yang diberikan (format: `123456789:ABCdefGHIjklMNOpqrsTUVwxyz`)

### Langkah 2: Dapatkan Chat ID
1. Cari dan chat dengan bot `@userinfobot` di Telegram
2. Bot akan membalas dengan informasi Anda
3. **Simpan Chat ID** (format: angka seperti `987654321`)

### Langkah 3: Start Bot Anda
1. Cari bot yang baru Anda buat
2. Klik "Start" atau kirim `/start`

## ⚙️ Konfigurasi Software

### Edit file `config.h`

```cpp
// WiFi Credentials
const char* WIFI_SSID = "Nama_WiFi_Anda";        // ← Ganti dengan SSID WiFi
const char* WIFI_PASSWORD = "Password_WiFi";     // ← Ganti dengan password WiFi

// Telegram Bot
#define TELEGRAM_BOT_TOKEN "123456:ABC..."       // ← Paste Bot Token
#define TELEGRAM_CHAT_ID "987654321"             // ← Paste Chat ID
```

### Kustomisasi Timing (Optional)

```cpp
// Dalam file config.h
#define REPORT_INTERVAL 30000         // Report setiap 30 detik
#define MOTION_STABLE_TIME 3000       // Diam 3 detik = berhenti
#define MOTION_ALERT_COOLDOWN 5000    // Cooldown alert 5 detik
#define SENSOR_READ_DELAY 50          // Delay baca sensor 50ms
```

## 📤 Upload ke ESP32

1. Hubungkan ESP32 ke komputer via USB
2. Buka file `security_monitoring_system_v2.ino`
3. Tools → Board → "ESP32 Dev Module"
4. Tools → Port → Pilih port COM yang sesuai
5. Tools → Upload Speed → "115200"
6. Klik tombol **Upload** (panah →)
7. Tunggu hingga selesai
8. Buka Serial Monitor (Ctrl+Shift+M) untuk melihat log

## 🔍 Testing

### Test 1: WiFi Connection
- Buka Serial Monitor
- Cek apakah muncul "✓ WiFi terhubung!"
- Cek IP address yang diberikan

### Test 2: Telegram Notification
- Seharusnya bot mengirim pesan "🤖 SISTEM MONITORING AKTIF"
- Jika tidak terkirim, cek Bot Token dan Chat ID

### Test 3: Hall Sensor
- Dekatkan magnet ke sensor KY-003
- Jauhkan magnet dari sensor
- Bot seharusnya kirim alert "Kotak Dibuka"

### Test 4: Tilt Sensor
- Gerakkan/miringkan board/sensor KY-027
- Bot seharusnya kirim alert "Kotak Digerakkan"
- Diamkan selama 3 detik
- Bot seharusnya kirim "Kotak Berhenti Bergerak"

## 🐛 Troubleshooting

### WiFi tidak terhubung
**Masalah:** "✗ Gagal terhubung ke WiFi!"

**Solusi:**
- Cek SSID dan password di `config.h`
- Pastikan WiFi 2.4GHz (ESP32 tidak support 5GHz)
- Pastikan ESP32 dalam jangkauan WiFi
- Restart ESP32

### Telegram tidak menerima pesan
**Masalah:** Tidak ada notifikasi di Telegram

**Solusi:**
- Verifikasi Bot Token dan Chat ID
- Pastikan sudah /start bot di Telegram
- Cek koneksi internet
- Cek Serial Monitor untuk error message

### Sensor tidak terdeteksi
**Masalah:** Sensor tidak memberikan respons

**Solusi:**
- Cek koneksi kabel (pastikan tidak longgar)
- Pastikan pin sesuai dengan konfigurasi (32 & 33)
- Test dengan multimeter (cek tegangan 3.3V)
- Coba ganti sensor jika rusak

### Error saat compile
**Masalah:** "Error: ... not found"

**Solusi:**
- Install semua library yang diperlukan
- Pastikan menggunakan ArduinoJson versi 6.x
- Update ESP32 board package ke versi terbaru
- Restart Arduino IDE

### Serial Monitor tidak menampilkan apa-apa
**Masalah:** Layar Serial Monitor kosong

**Solusi:**
- Pastikan baud rate di Serial Monitor = 115200
- Pilih "Both NL & CR" di Serial Monitor
- Tekan tombol EN/Reset di ESP32
- Coba port USB yang berbeda

## 📞 Support

Jika masih ada masalah, cek:
- Serial Monitor untuk error log detail
- Pastikan semua file ada di folder yang sama
- Pastikan tidak ada typo di `config.h`

## 🔄 Update Firmware

Untuk update kode:
1. Edit file yang diperlukan
2. Upload ulang ke ESP32
3. Monitor via Serial Monitor
4. Test semua fitur kembali

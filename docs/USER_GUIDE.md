# User Guide - Sistem Monitoring Keamanan Kotak

## 📖 Pendahuluan

Sistem ini dirancang untuk memantau keamanan kotak/peti penyimpanan dengan mendeteksi:
- **Pembukaan kotak** (menggunakan sensor Hall Effect)
- **Gerakan kotak** (menggunakan sensor Mercury Switch)

Notifikasi real-time akan dikirim ke Telegram Anda.

## 🎯 Fitur Utama

### 1. Deteksi Pembukaan Kotak 🚨
- **Cara kerja:** Magnet dipasang di pintu kotak, sensor Hall Effect di badan kotak
- **Trigger:** Saat pintu dibuka, magnet menjauh dari sensor
- **Notifikasi:** "🚨 ALERT: KOTAK DIBUKA!"
- **Auto-recovery:** Saat pintu ditutup, kirim notif "✅ Kotak Ditutup"

### 2. Deteksi Gerakan Kotak ⚠️
- **Cara kerja:** Sensor tilt mendeteksi perubahan posisi/kemiringan
- **Trigger:** Kotak diangkat, digerakkan, atau dimiringkan
- **Notifikasi:** "⚠️ ALERT: KOTAK DIGERAKKAN!"
- **Smart detection:** Jika diam selama 3 detik, kirim "✅ Kotak Berhenti Bergerak"
- **No spam:** Cooldown 5 detik antar alert (jika masih bergerak terus)

### 3. Report Berkala 📊
- **Interval:** Setiap 30 detik
- **Isi report:**
  - Status Hall Sensor (kotak tertutup/terbuka)
  - Status Tilt Sensor (diam/bergerak)
  - Uptime sistem
  - Kekuatan sinyal WiFi

### 4. Serial Monitor Logging 📝
- **Real-time status:** Tampilan status sensor setiap detik
- **Event logging:** Log detail setiap event yang terjadi
- **Debug info:** Informasi teknis untuk troubleshooting

## 🔔 Jenis Notifikasi Telegram

### Alert Prioritas Tinggi (Immediate)
1. **🚨 Kotak Dibuka**
   ```
   🚨 ALERT: KOTAK DIBUKA!
   ━━━━━━━━━━━━━━━━━━━━━
   🧲 Sensor Hall: Magnet menjauh
   📦 Status: KOTAK TERBUKA
   ━━━━━━━━━━━━━━━━━━━━━
   ⏰ Waktu: 01:23:45
   ```

2. **⚠️ Kotak Digerakkan**
   ```
   ⚠️ ALERT: KOTAK DIGERAKKAN!
   ━━━━━━━━━━━━━━━━━━━━━
   📐 Sensor Tilt: Perubahan posisi
   📦 Status: KOTAK BERGERAK
   ━━━━━━━━━━━━━━━━━━━━━
   ⏰ Waktu: 01:23:50
   ```

### Notifikasi Info (Normal)
3. **✅ Kotak Ditutup**
   ```
   ✅ Kotak Ditutup
   
   🧲 Magnet terdeteksi kembali
   📦 Status: KOTAK TERTUTUP
   ```

4. **✅ Gerakan Berhenti**
   ```
   ✅ Kotak Berhenti Bergerak
   
   📐 Posisi stabil selama 3 detik
   📦 Status: KOTAK DIAM
   ```

### Report Rutin
5. **📊 Status Report** (Setiap 30 detik)
   ```
   📊 STATUS REPORT
   ━━━━━━━━━━━━━━━━━━━━━
   🧲 Hall Sensor: Kotak tertutup ✓
   📐 Tilt Sensor: Posisi stabil ✓
   ━━━━━━━━━━━━━━━━━━━━━
   ⏰ Uptime: 1h 23m 45s
   📶 WiFi: -56 dBm
   ```

## 🎬 Skenario Penggunaan

### Skenario 1: Pencurian Kotak
```
[User meletakkan kotak di tempat aman]
✓ Kotak tertutup, tidak bergerak

[Pencuri mencoba membuka kotak]
🚨 ALERT: KOTAK DIBUKA!
⚠️ ALERT: KOTAK DIGERAKKAN!

[Pencuri membawa kabur kotak]
⚠️ ALERT: KOTAK DIGERAKKAN! (setiap 5 detik jika masih bergerak)

[User dapat langsung mengambil tindakan]
```

### Skenario 2: Perpindahan Normal
```
[User ingin memindahkan kotak]
⚠️ ALERT: KOTAK DIGERAKKAN!

[User meletakkan di tempat baru dan diam]
[Tunggu 3 detik...]
✅ Kotak Berhenti Bergerak

[Sistem kembali normal]
```

### Skenario 3: Pemeriksaan Isi Kotak
```
[User membuka kotak]
🚨 ALERT: KOTAK DIBUKA!

[User mengecek isi, kotak tidak bergerak]
[Tidak ada alert gerakan]

[User menutup kembali]
✅ Kotak Ditutup

[Sistem kembali normal]
```

## 📱 Tips Penggunaan

### Instalasi Fisik

1. **Posisi Magnet:**
   - Pasang magnet di pintu/tutup kotak
   - Sensor Hall Effect di badan kotak
   - Jarak ideal: < 2cm saat tertutup

2. **Posisi Sensor Tilt:**
   - Pasang horizontal/vertikal (pilih salah satu)
   - Pastikan stabil, tidak goyang
   - Akan sensitif terhadap perubahan kemiringan

3. **Power Supply:**
   - Gunakan power bank atau adaptor 5V
   - Pastikan stabil untuk operasi 24/7
   - Cek daya baterai secara berkala

### Konfigurasi Optimal

**Untuk keamanan tinggi:**
```cpp
#define REPORT_INTERVAL 60000      // Report setiap 1 menit
#define MOTION_STABLE_TIME 2000    // Lebih sensitif (2 detik)
#define MOTION_ALERT_COOLDOWN 3000 // Alert lebih sering (3 detik)
```

**Untuk mengurangi notifikasi:**
```cpp
#define REPORT_INTERVAL 300000     // Report setiap 5 menit
#define MOTION_STABLE_TIME 5000    // Kurang sensitif (5 detik)
#define MOTION_ALERT_COOLDOWN 10000 // Jarang alert (10 detik)
```

### Maintenance

**Checklist Harian:**
- ✓ Cek apakah masih menerima report berkala
- ✓ Test manual sensor dengan membuka kotak
- ✓ Cek baterai/power supply

**Checklist Mingguan:**
- ✓ Bersihkan sensor dari debu
- ✓ Cek koneksi kabel (tidak longgar)
- ✓ Restart sistem untuk refresh memory

**Checklist Bulanan:**
- ✓ Update firmware jika ada versi baru
- ✓ Backup konfigurasi
- ✓ Test disaster recovery

## ⚠️ Limitasi Sistem

1. **WiFi Dependency:** Sistem butuh WiFi aktif untuk notifikasi Telegram
2. **Power Dependency:** Butuh power supply stabil
3. **Sensor Limitation:** 
   - Hall sensor hanya efektif dalam jarak dekat (< 3cm)
   - Tilt sensor bisa false positive jika ada getaran eksternal
4. **Telegram Rate Limit:** Maksimal 30 pesan per detik per bot

## 🔒 Security Best Practices

1. **Jangan share Bot Token** ke siapa pun
2. **Gunakan WiFi private**, bukan WiFi publik
3. **Ganti password WiFi** secara berkala
4. **Backup config.h** di tempat aman
5. **Monitor log** untuk deteksi anomali
6. **Update firmware** untuk security patch

## 📞 Emergency Response

Jika menerima alert:

1. **Cek Serial Monitor** (jika akses tersedia)
2. **Lihat timestamp** di notifikasi Telegram
3. **Cek CCTV** (jika ada)
4. **Hubungi security** atau ke lokasi
5. **Dokumentasi** untuk bukti

## 🎓 FAQ

**Q: Kenapa sering false alarm gerakan?**
A: Sensor tilt sangat sensitif. Pastikan kotak di permukaan stabil, tidak ada getaran eksternal.

**Q: Notifikasi delay berapa lama?**
A: Instant (< 1 detik) jika WiFi stabil. Delay bisa terjadi jika koneksi buruk.

**Q: Bisa untuk outdoor?**
A: Tidak recommended. ESP32 dan sensor tidak waterproof. Gunakan enclosure tahan air jika diperlukan.

**Q: Berapa lama baterai power bank bertahan?**
A: Tergantung kapasitas. Power bank 10000mAh bisa ~24-48 jam. Untuk 24/7 gunakan adaptor.

**Q: Bisa monitoring dari jauh?**
A: Ya! Telegram bisa diakses dari mana saja selama ada internet.

**Q: Aman dari hacker?**
A: Telegram menggunakan enkripsi. Bot Token jangan di-share. Gunakan WiFi aman dengan password kuat.

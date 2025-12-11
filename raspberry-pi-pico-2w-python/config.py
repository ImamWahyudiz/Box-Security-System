"""
╔═══════════════════════════════════════════╗
║         KONFIGURASI SISTEM                ║
║    Raspberry Pi Pico 2 W - MicroPython    ║
╚═══════════════════════════════════════════╝
"""

# ====================================
# KONFIGURASI WiFi
# ====================================
WIFI_SSID = "Your_WiFi_SSID"        # Ganti dengan SSID WiFi Anda
WIFI_PASSWORD = "Your_WiFi_Password" # Ganti dengan password WiFi Anda

# ====================================
# KONFIGURASI TELEGRAM BOT
# ====================================
TELEGRAM_BOT_TOKEN = "YOUR_BOT_TOKEN_HERE"  # Token dari @BotFather
TELEGRAM_CHAT_ID = "YOUR_CHAT_ID_HERE"      # Chat ID Anda

# ====================================
# PIN CONFIGURATION (GPIO)
# ====================================
# PIN_HALL_SENSOR = 16  # DIHAPUS - Hall sensor tidak dipakai
PIN_TILT_SENSOR = 17  # GPIO 17 untuk KY-027 Tilt Sensor

# ====================================
# SENSOR CONFIGURATION
# ====================================
# Hall sensor configuration dihapus - hanya pakai Tilt sensor
# HALL_SENSOR_INVERTED = True  # TIDAK DIPAKAI

# ====================================
# TIMING CONFIGURATION (milliseconds)
# ====================================
BOX_OPEN_ALERT_INTERVAL = 3000    # Interval alert kotak terbuka (3 detik)
MOTION_STABLE_TIME = 3000         # Waktu stabil untuk deteksi gerakan berhenti (3 detik)
MOTION_ALERT_COOLDOWN = 5000      # Cooldown antara alert gerakan (5 detik)
SENSOR_READ_DELAY = 50            # Delay pembacaan sensor (50ms)
BOT_CHECK_INTERVAL = 1000         # Interval cek Telegram saat armed (1 detik)
SLEEP_BOT_CHECK_INTERVAL = 30000  # Interval cek Telegram saat sleep (30 detik)

# ====================================
# CATATAN PENTING
# ====================================
# 1. Pastikan WiFi 2.4GHz (Pico W tidak support 5GHz)
# 2. Token bot dapat dari @BotFather di Telegram
# 3. Chat ID dari https://api.telegram.org/bot<TOKEN>/getUpdates
# 4. Sesuaikan HALL_SENSOR_INVERTED dengan sensor Anda

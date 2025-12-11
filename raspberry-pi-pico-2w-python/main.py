"""
╔═══════════════════════════════════════════╗
║                                           ║
║   SISTEM MONITORING KEAMANAN KOTAK        ║
║   Security Box Monitoring System          ║
║                                           ║
║   Hardware:                               ║
║   - Raspberry Pi Pico 2 W                 ║
║   - KY-027: Mercury Switch Sensor (Tilt)  ║
║                                           ║
║   Language: MicroPython                   ║
║   IDE: Thonny                             ║
║   Version: 1.0 (Tilt Only)                ║
║                                           ║
╚═══════════════════════════════════════════╝
"""

import machine
import network
import urequests
import ujson
import time
from config import *

# ====================================
# GLOBAL VARIABLES
# ====================================
# Status sensor
last_tilt_state = 1

# Tracking gerakan
last_tilt_change_time = 0
last_motion_alert_time = 0
is_motion_active = False
motion_alert_sent = False

# Variabel status keamanan
security_breached = False  # FLAG: Kotak digerakkan
last_breach_alert_time = 0

# Sistem arm/disarm
is_system_armed = True  # Default: sistem aktif

# Timing
last_bot_check = 0
last_offset = 0

# WiFi object
wlan = None

# ====================================
# SETUP PIN SENSOR
# ====================================
print("\n" + "="*45)
print("   SISTEM MONITORING KEAMANAN KOTAK")
print("   Raspberry Pi Pico 2 W - MicroPython")
print("   TILT SENSOR ONLY")
print("="*45 + "\n")

# Setup GPIO pin - hanya Tilt sensor
tilt_sensor = machine.Pin(PIN_TILT_SENSOR, machine.Pin.IN, machine.Pin.PULL_UP)

print("✓ Pin sensor diinisialisasi")
print(f"  - Tilt Sensor: GPIO {PIN_TILT_SENSOR} (PULL_UP)")

# Test sensor value
print("\n🔍 TEST SENSOR:")
print(f"  Tilt: {tilt_sensor.value()} (Miringkan kotak untuk test)")
print()

# ====================================
# FUNGSI: KONEKSI WiFi
# ====================================
def connect_wifi():
    global wlan
    wlan = network.WLAN(network.STA_IF)
    wlan.active(True)
    
    if not wlan.isconnected():
        print(f"Menghubungkan ke WiFi: {WIFI_SSID}")
        wlan.connect(WIFI_SSID, WIFI_PASSWORD)
        
        timeout = 20
        while not wlan.isconnected() and timeout > 0:
            print(".", end="")
            time.sleep(0.5)
            timeout -= 0.5
        
        print()
        
        if wlan.isconnected():
            print(f"✓ WiFi terhubung!")
            print(f"  IP Address: {wlan.ifconfig()[0]}")
            print(f"  RSSI: {wlan.status('rssi')} dBm\n")
            return True
        else:
            print("✗ Gagal terhubung ke WiFi\n")
            return False
    else:
        print("✓ WiFi sudah terhubung\n")
        return True

# ====================================
# FUNGSI: CEK WiFi CONNECTED
# ====================================
def is_wifi_connected():
    return wlan and wlan.isconnected()

# ====================================
# FUNGSI: KIRIM PESAN TELEGRAM
# ====================================
def send_telegram_message(text):
    if not is_wifi_connected():
        print("✗ WiFi tidak terhubung, tidak bisa kirim pesan")
        return False
    
    try:
        url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/sendMessage"
        data = {
            "chat_id": TELEGRAM_CHAT_ID,
            "text": text
        }
        
        response = urequests.post(url, json=data)
        success = response.status_code == 200
        response.close()
        
        return success
    except Exception as e:
        print(f"✗ Error kirim pesan: {e}")
        return False

# ====================================
# FUNGSI: GET FORMATTED TIME
# ====================================
def get_formatted_time():
    current = time.localtime()
    return f"{current[3]:02d}:{current[4]:02d}:{current[5]:02d}"

# ====================================
# FUNGSI: GET FORMATTED UPTIME
# ====================================
def get_formatted_uptime():
    uptime_sec = time.ticks_ms() // 1000
    hours = uptime_sec // 3600
    minutes = (uptime_sec % 3600) // 60
    seconds = uptime_sec % 60
    return f"{hours}h {minutes}m {seconds}s"

# ====================================
# FUNGSI: SEND STARTUP MESSAGE
# ====================================
def send_startup_message():
    print("Mengirim notifikasi startup...")
    
    message = "🤖 SISTEM MONITORING AKTIF\n\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "📡 SENSOR TERPASANG:\n"
    message += f"• KY-027 Tilt Sensor (GPIO {PIN_TILT_SENSOR})\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "⚙️ PLATFORM:\n"
    message += "• Raspberry Pi Pico 2 W\n"
    message += "• MicroPython + Thonny\n"
    message += "• Tilt Detection Only\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "✅ Sistem siap memantau\n\n"
    message += "💬 Ketik /help untuk melihat commands"
    
    if send_telegram_message(message):
        print("✓ Notifikasi startup terkirim\n")
    else:
        print("✗ Gagal mengirim notifikasi\n")

# ====================================
# FUNGSI: SEND MOTION ALERT
# ====================================
def send_motion_alert():
    message = "⚠️ ALERT: KOTAK DIGERAKKAN!\n\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "📐 Sensor Tilt: Perubahan posisi\n"
    message += "📦 Status: KOTAK BERGERAK\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += f"⏰ {get_formatted_time()}"
    
    send_telegram_message(message)

# ====================================
# FUNGSI: SEND SECURITY BREACH ALERT
# ====================================
def send_security_breach_alert():
    global last_breach_alert_time
    
    message = "⚠️ PELANGGARAN KEAMANAN!\n\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "📦 KOTAK TELAH DIGERAKKAN\n"
    message += "📢 Alert akan terus dikirim\n"
    message += "✅ Gunakan /safe untuk konfirmasi aman\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += f"⏰ {get_formatted_time()}"
    
    send_telegram_message(message)
    last_breach_alert_time = time.ticks_ms()

# ====================================
# FUNGSI: SEND HELP MENU
# ====================================
def send_help_menu():
    message = "🤖 COMMAND MENU\n\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "📋 Available Commands:\n\n"
    message += "/status - Cek status sistem\n"
    message += "/arm - Aktifkan monitoring\n"
    message += "/disarm - Nonaktifkan (sleep)\n"
    message += "/sleep - Mode hemat energi\n"
    message += "/safe - Konfirmasi situasi aman\n"
    message += "/help - Tampilkan menu ini\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "📐 Tilt Sensor Only:\n"
    message += "• Deteksi gerakan kotak\n"
    message += "• Alert saat dimiringkan\n"
    message += "• Gunakan /safe untuk reset"
    
    send_telegram_message(message)

# ====================================
# FUNGSI: SEND SYSTEM STATUS
# ====================================
def send_system_status():
    global is_system_armed, security_breached, is_motion_active
    global last_tilt_state
    
    message = "📊 STATUS SISTEM LENGKAP\n\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    
    # System Mode
    message += "🔒 Mode: "
    message += "ARMED ✓" if is_system_armed else "DISARMED (Sleep) 💤"
    
    # Security Breach Status
    message += "\n⚠️ Security Breach: "
    message += "ACTIVE! 🚨" if security_breached else "Clear ✓"
    
    # Sensor Status
    message += "\n\n📡 SENSOR STATUS:\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "📐 Tilt Sensor:\n"
    message += f"  • Pin: GPIO {PIN_TILT_SENSOR}\n"
    message += f"  • Value: {tilt_sensor.value()}\n"
    message += f"  • Motion: {'AKTIF ⚠️' if is_motion_active else 'Diam ✓'}"
    
    # System Info
    message += "\n\n⚙️ SYSTEM INFO:\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += f"⏰ Uptime: {get_formatted_uptime()}\n"
    message += f"🕐 Time: {get_formatted_time()}"
    
    # WiFi Info
    if is_wifi_connected():
        message += f"\n\n📶 WiFi: Connected\n"
        message += f"  • SSID: {WIFI_SSID}\n"
        message += f"  • IP: {wlan.ifconfig()[0]}\n"
        message += f"  • RSSI: {wlan.status('rssi')} dBm"
    else:
        message += f"\n\n📶 WiFi: Disconnected ❌"
    
    # Platform Info
    message += "\n\n🖥️ PLATFORM:\n"
    message += "━━━━━━━━━━━━━━━━━━━━━\n"
    message += "• Raspberry Pi Pico 2 W\n"
    message += "• MicroPython\n"
    message += "• Tilt Sensor Only"
    
    message += "\n━━━━━━━━━━━━━━━━━━━━━"
    
    send_telegram_message(message)
    
    message += "\n━━━━━━━━━━━━━━━━━━━━━"
    
    send_telegram_message(message)

# ====================================
# FUNGSI: HANDLE TELEGRAM MESSAGES
# ====================================
def handle_telegram_messages():
    global last_offset, is_system_armed, security_breached
    
    if not is_wifi_connected():
        return
    
    try:
        # Build URL dengan query string manual (urequests tidak support params)
        url = f"https://api.telegram.org/bot{TELEGRAM_BOT_TOKEN}/getUpdates?offset={last_offset}&timeout=1"
        
        response = urequests.get(url)
        data = ujson.loads(response.text)
        response.close()
        
        if data.get("ok") and data.get("result"):
            for update in data["result"]:
                last_offset = update["update_id"] + 1
                
                if "message" not in update:
                    continue
                
                message = update["message"]
                chat_id = str(message["chat"]["id"])
                text = message.get("text", "")
                
                # Validasi chat ID
                if chat_id != TELEGRAM_CHAT_ID:
                    print(f"⚠️ Unauthorized access from: {chat_id}")
                    continue
                
                print(f"📨 Pesan diterima: {text}")
                
                # Process commands
                if text in ["/start", "/help"]:
                    send_help_menu()
                    
                elif text == "/status":
                    send_system_status()
                    
                elif text == "/arm":
                    is_system_armed = True
                    send_telegram_message("✅ Sistem ARMED\n\n🔒 Monitoring aktif")
                    print("✓ Sistem ARMED via Telegram")
                    
                elif text in ["/disarm", "/sleep"]:
                    is_system_armed = False
                    send_telegram_message("💤 Sistem DISARMED\n\n🔓 Mode sleep - hemat energi\n🔇 Sensor tidak dibaca\n⚡ Konsumsi daya minimal\n\nKetik /arm untuk aktifkan kembali")
                    print("✓ Sistem DISARMED via Telegram")
                    
                elif text == "/safe":
                    if security_breached:
                        security_breached = False
                        send_telegram_message("✅ SITUASI DIKONFIRMASI AMAN\n\n🔒 Flag breach telah direset\n📢 Alert berulang dihentikan\n\n⚠️ Monitoring tetap aktif")
                        print("✓ Security breach flag RESET via /safe")
                    else:
                        send_telegram_message("ℹ️ Tidak ada breach aktif\n\nSistem dalam kondisi normal")
                        
                else:
                    send_telegram_message("❓ Command tidak dikenal\n\nKetik /help untuk melihat daftar command")
                    
    except Exception as e:
        print(f"✗ Error handle messages: {e}")

# ====================================
# FUNGSI: CHECK BOX MOVEMENT
# ====================================
def check_box_movement(tilt_value):
    global last_tilt_state, last_tilt_change_time
    global is_motion_active, motion_alert_sent, last_motion_alert_time
    global security_breached, last_breach_alert_time
    
    current_time = time.ticks_ms()
    
    # Deteksi perubahan state
    if tilt_value != last_tilt_state:
        last_tilt_change_time = current_time
        last_tilt_state = tilt_value
        
        # Set gerakan aktif DAN security breached
        if not is_motion_active:
            is_motion_active = True
            motion_alert_sent = False
            security_breached = True  # SET FLAG saat pertama kali bergerak
            
            print("\n" + "="*35)
            print("  ⚠️ GERAKAN TERDETEKSI!")
            print("  🚨 SECURITY BREACHED!")
            print("="*35 + "\n")
            print("="*35 + "\n")
        
        # Kirim alert jika belum atau sudah lewat cooldown
        if not motion_alert_sent or (time.ticks_diff(current_time, last_motion_alert_time) > MOTION_ALERT_COOLDOWN):
            send_motion_alert()
            last_motion_alert_time = current_time
            motion_alert_sent = True
    
    # Cek stabilisasi
    if is_motion_active:
        time_since_last = time.ticks_diff(current_time, last_tilt_change_time)
        
        if time_since_last > MOTION_STABLE_TIME:
            is_motion_active = False
            motion_alert_sent = False
            print("\n✓ Gerakan berhenti - kotak diam\n")

# ====================================
# SETUP - KONEKSI WiFi & STARTUP
# ====================================
if connect_wifi():
    send_startup_message()

print("="*45)
print("  ✓ Sistem siap beroperasi")
print("="*45 + "\n")

# ====================================
# MAIN LOOP
# ====================================
print("🔄 Memulai monitoring...\n")

while True:
    try:
        # Cek pesan Telegram
        bot_check_interval = BOT_CHECK_INTERVAL if is_system_armed else SLEEP_BOT_CHECK_INTERVAL
        
        if is_wifi_connected() and (time.ticks_diff(time.ticks_ms(), last_bot_check) > bot_check_interval):
            handle_telegram_messages()
            last_bot_check = time.ticks_ms()
        
        # HANYA BACA SENSOR JIKA ARMED
        if is_system_armed:
            # Baca sensor tilt
            tilt_value = tilt_sensor.value()
            
            # Display status
            uptime = time.ticks_ms() // 1000
            mode = "ARMED" if is_system_armed else "SLEEP"
            motion_status = "AKTIF ⚠" if is_motion_active else "Diam ○"
            
            print(f"[{uptime}s] 🔒 {mode} | 📐 Tilt: {motion_status}")
            
            # Monitoring
            if is_wifi_connected():
                check_box_movement(tilt_value)
                
                # Kirim breach alert jika ada gerakan
                if security_breached and (time.ticks_diff(time.ticks_ms(), last_breach_alert_time) > MOTION_ALERT_COOLDOWN):
                    send_security_breach_alert()
            
            time.sleep_ms(SENSOR_READ_DELAY)
            
        else:
            # SLEEP MODE
            print("💤 [SLEEP MODE] Sistem dalam mode hemat energi...")
            time.sleep(10)
            
    except KeyboardInterrupt:
        print("\n\n⚠️ Program dihentikan oleh user")
        break
    except Exception as e:
        print(f"\n✗ Error: {e}")
        time.sleep(5)

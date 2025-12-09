/*
 * ╔═══════════════════════════════════════════╗
 * ║                                           ║
 * ║   SISTEM MONITORING KEAMANAN KOTAK        ║
 * ║   Security Box Monitoring System          ║
 * ║                                           ║
 * ║   Hardware:                               ║
 * ║   - Raspberry Pi Pico W                   ║
 * ║   - KY-003: Hall Effect Sensor            ║
 * ║   - KY-027: Mercury Switch Sensor         ║
 * ║                                           ║
 * ║   Features:                               ║
 * ║   - Real-time box monitoring              ║
 * ║   - Telegram instant notifications        ║
 * ║   - Motion detection with stability       ║
 * ║   - Periodic status reports               ║
 * ║   - Remote arm/disarm control             ║
 * ║   - Sleep mode for power saving           ║
 * ║                                           ║
 * ║   Version: 1.0                            ║
 * ║   Platform: Raspberry Pi Pico W           ║
 * ║                                           ║
 * ╚═══════════════════════════════════════════╝
 */

// ====================================
// LIBRARY
// ====================================
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// ====================================
// PROJECT FILES
// ====================================
#include "config.h"
#include "utils.h"
#include "wifi_manager.h"
#include "sensor_handler.h"
#include "telegram_manager.h"

// ====================================
// GLOBAL OBJECTS
// ====================================
WiFiClientSecure secured_client;
UniversalTelegramBot bot(TELEGRAM_BOT_TOKEN, secured_client);

// ====================================
// GLOBAL VARIABLES
// ====================================
// Status sensor
int lastTiltState = HIGH;

// Inisialisasi berdasarkan konfigurasi sensor
#if HALL_SENSOR_INVERTED
  int lastHallState = HIGH;  // Sensor inverted: HIGH = tertutup
#else
  int lastHallState = LOW;   // Sensor normal: LOW = tertutup
#endif

// Tracking gerakan
unsigned long lastTiltChangeTime = 0;
unsigned long lastMotionAlertTime = 0;
bool isMotionActive = false;
bool motionAlertSent = false;

// Tracking kotak terbuka
bool isBoxOpen = false;
unsigned long lastBoxOpenAlertTime = 0;

// Variabel status keamanan
bool securityBreached = false; // FLAG: Kotak pernah dibuka
unsigned long lastBreachAlertTime = 0;

// Sistem arm/disarm
bool isSystemArmed = true;  // Default: sistem aktif

// Timing
unsigned long lastReportTime = 0;
unsigned long lastBotCheck = 0;

// ====================================
// SETUP - INISIALISASI SISTEM
// ====================================
void setup() {
  // Inisialisasi Serial Monitor
  Serial.begin(SERIAL_BAUD_RATE);
  delay(1000);
  
  // Tampilkan header
  printHeader();
  
  // Inisialisasi Pin Sensor
  pinMode(PIN_HALL_SENSOR, INPUT);
  pinMode(PIN_TILT_SENSOR, INPUT_PULLUP);
  Serial.println("✓ Pin sensor diinisialisasi");
  Serial.print("  - Hall Sensor: GPIO ");
  Serial.println(PIN_HALL_SENSOR);
  Serial.print("  - Tilt Sensor: GPIO ");
  Serial.println(PIN_TILT_SENSOR);
  Serial.println();
  
  // Koneksi ke WiFi
  connectWiFi();
  
  // Setup Telegram
  if (isWiFiConnected()) {
    secured_client.setInsecure();  // Pico W menggunakan setInsecure() bukan setCACert()
    sendStartupMessage();
  }
  
  // Sistem siap
  Serial.println();
  printSeparator();
  Serial.println("✓ Sistem siap beroperasi");
  printSeparator();
  Serial.println();
  
  delay(1000);
}

// ====================================
// LOOP UTAMA
// ====================================
void loop() {
  // Cek koneksi WiFi
  static unsigned long lastWiFiCheck = 0;
  if (millis() - lastWiFiCheck > 30000) {  // Cek setiap 30 detik
    reconnectWiFi();
    lastWiFiCheck = millis();
  }
  
  // Cek pesan Telegram dengan interval berbeda tergantung mode
  unsigned long botCheckInterval = isSystemArmed ? BOT_CHECK_INTERVAL : SLEEP_BOT_CHECK_INTERVAL;
  if (isWiFiConnected() && (millis() - lastBotCheck > botCheckInterval)) {
    handleTelegramMessages();
    lastBotCheck = millis();
  }
  
  // HANYA BACA SENSOR JIKA SISTEM ARMED
  if (isSystemArmed) {
    // Baca status sensor
    int hallValue = digitalRead(PIN_HALL_SENSOR);
    int tiltValue = digitalRead(PIN_TILT_SENSOR);

    // Tampilkan status di Serial Monitor
    displaySerialStatus(hallValue, tiltValue);
  
    // Monitoring kotak dibuka
    if (isWiFiConnected()) {
      checkBoxOpened(hallValue);
      
      // PRIORITAS: Kirim alert breach jika keamanan dilanggar
      if (securityBreached && (millis() - lastBreachAlertTime > BOX_OPEN_ALERT_INTERVAL)) {
        sendSecurityBreachAlert();
      }
      // Kirim alert biasa jika kotak terbuka (tapi belum breach)
      else if (isBoxOpen && (millis() - lastBoxOpenAlertTime > BOX_OPEN_ALERT_INTERVAL)) {
        sendBoxOpenedAlert();
        lastBoxOpenAlertTime = millis();
      }
      
      // Monitoring gerakan kotak (perubahan tilt)
      checkBoxMovement(tiltValue);
      
      // Report berkala DIHAPUS - hanya kirim saat ada bahaya
    }
    
    delay(SENSOR_READ_DELAY);
  } else {
    // MODE SLEEP - hemat energi, tidak baca sensor
    static unsigned long lastSleepLog = 0;
    if (millis() - lastSleepLog > 10000) {  // Log setiap 10 detik
      Serial.println("💤 [SLEEP MODE] Sistem dalam mode hemat energi...");
      lastSleepLog = millis();
    }
    delay(1000);  // Delay lebih lama untuk hemat energi
  }
}

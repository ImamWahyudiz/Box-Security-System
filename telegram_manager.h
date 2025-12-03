/*
 * ====================================
 * TELEGRAM MANAGER
 * ====================================
 * Fungsi untuk mengelola komunikasi Telegram
 * ====================================
 */

#ifndef TELEGRAM_MANAGER_H
#define TELEGRAM_MANAGER_H

#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "config.h"
#include "utils.h"

extern WiFiClientSecure secured_client;
extern UniversalTelegramBot bot;
extern bool isSystemArmed;

// ====================================
// FUNGSI: KIRIM MENU HELP
// ====================================
void sendHelpMenu() {
  String message = "🤖 COMMAND MENU\n\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "📋 Available Commands:\n\n";
  message += "/status - Cek status sistem\n";
  message += "/arm - Aktifkan monitoring\n";
  message += "/disarm - Nonaktifkan (sleep)\n";
  message += "/sleep - Mode hemat energi\n";
  message += "/help - Tampilkan menu ini\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "💡 Sleep Mode:\n";
  message += "• Sensor tidak dibaca\n";
  message += "• Hemat energi\n";
  message += "• Tetap bisa di-arm kembali";
  
  bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
}

// ====================================
// FUNGSI: KIRIM STATUS SISTEM
// ====================================
void sendSystemStatus() {
  String message = "📊 STATUS SISTEM\n\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "🔒 Mode: ";
  message += isSystemArmed ? "ARMED ✓" : "DISARMED (Sleep) 💤";
  message += "\n⏰ Uptime: " + getFormattedUptime();
  message += "\n📶 WiFi: " + String(WiFi.RSSI()) + " dBm";
  message += "\n━━━━━━━━━━━━━━━━━━━━━";
  
  bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
}

// ====================================
// FUNGSI: HANDLE PESAN TELEGRAM
// ====================================
void handleTelegramMessages() {
  int numNewMessages = bot.getUpdates(bot.last_message_received + 1);
  
  for (int i = 0; i < numNewMessages; i++) {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;
    
    // Validasi chat ID untuk keamanan
    if (chat_id != TELEGRAM_CHAT_ID) {
      bot.sendMessage(chat_id, "⛔ Unauthorized access!", "");
      Serial.println("⚠️ Unauthorized access attempt from: " + chat_id);
      continue;
    }
    
    Serial.println("📨 Pesan diterima: " + text);
    
    // Process commands
    if (text == "/start" || text == "/help") {
      sendHelpMenu();
    }
    else if (text == "/status") {
      sendSystemStatus();
    }
    else if (text == "/arm") {
      isSystemArmed = true;
      bot.sendMessage(TELEGRAM_CHAT_ID, "✅ Sistem ARMED\n\n🔒 Monitoring aktif", "");
      Serial.println("✓ Sistem ARMED via Telegram");
    }
    else if (text == "/disarm" || text == "/sleep") {
      isSystemArmed = false;
      bot.sendMessage(TELEGRAM_CHAT_ID, "💤 Sistem DISARMED\n\n🔓 Mode sleep - hemat energi\n🔇 Sensor tidak dibaca\n⚡ Konsumsi daya minimal\n\nKetik /arm untuk aktifkan kembali", "");
      Serial.println("✓ Sistem DISARMED via Telegram - masuk mode hemat energi");
    }
      Serial.println("✓ Sistem DISARMED via Telegram - masuk mode hemat energi");
    }
    else {
      bot.sendMessage(TELEGRAM_CHAT_ID, "❓ Command tidak dikenal\n\nKetik /help untuk melihat daftar command", "");
    }
  }
}

// ====================================
// FUNGSI: KIRIM PESAN STARTUP
// ====================================
void sendStartupMessage() {
  Serial.println("Mengirim notifikasi startup...");
  
  String message = "🤖 SISTEM MONITORING AKTIF\n\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "📡 SENSOR TERPASANG:\n";
  message += "• KY-003 Hall Sensor (Pin " + String(PIN_HALL_SENSOR) + ")\n";
  message += "• KY-027 Tilt Sensor (Pin " + String(PIN_TILT_SENSOR) + ")\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "⚙️ KONFIGURASI:\n";
  message += "• Report interval: " + String(REPORT_INTERVAL / 1000) + "s\n";
  message += "• Motion stable time: " + String(MOTION_STABLE_TIME / 1000) + "s\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "✅ Sistem siap memantau\n\n";
  message += "💬 Ketik /help untuk melihat commands";
  
  bool sent = bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
  
  if (sent) {
    Serial.println("✓ Notifikasi startup terkirim");
  } else {
    Serial.println("✗ Gagal mengirim notifikasi");
  }
}

// ====================================
// FUNGSI: KIRIM ALERT KOTAK DIBUKA
// ====================================
void sendBoxOpenedAlert() {
  String message = "🚨 ALERT: KOTAK TERBUKA!\n\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "📬 Status: TERBUKA\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "⏰ " + getFormattedTime();
  
  bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
}

// ====================================
// FUNGSI: KIRIM NOTIF KOTAK DITUTUP
// ====================================
void sendBoxClosedNotif() {
  String message = "✅ Kotak Tertutup\n\n";
  message += "📬 Status: TERTUTUP";
  
  bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
}

// ====================================
// FUNGSI: KIRIM ALERT GERAKAN
// ====================================
void sendMotionAlert() {
  String message = "⚠️ ALERT: KOTAK DIGERAKKAN!\n\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "📐 Sensor Tilt: Perubahan posisi\n";
  message += "📦 Status: KOTAK BERGERAK\n";
  message += "━━━━━━━━━━━━━━━━━━━━━\n";
  message += "⏰ " + getFormattedTime();
  
  bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
}

// ====================================
// FUNGSI: KIRIM NOTIF GERAKAN BERHENTI
// ====================================
void sendMotionStoppedNotif() {
  String message = "✅ Kotak Berhenti Bergerak\n\n";
  message += "📐 Posisi stabil selama " + String(MOTION_STABLE_TIME / 1000) + " detik\n";
  message += "📦 Status: KOTAK DIAM";
  
  bot.sendMessage(TELEGRAM_CHAT_ID, message, "");
}

// ====================================
// FUNGSI: KIRIM REPORT BERKALA
// ====================================
void sendPeriodicReport(int hallValue, int tiltValue, bool isMotionActive) {
  String report = "📊 STATUS REPORT\n\n";
  report += "━━━━━━━━━━━━━━━━━━━━━\n";
  report += "📬 Kotak: ";
  
  // Status sesuai konfigurasi sensor
  #if HALL_SENSOR_INVERTED
    report += (hallValue == HIGH) ? "Tertutup ✓" : "Terbuka ⚠️";
  #else
    report += (hallValue == LOW) ? "Tertutup ✓" : "Terbuka ⚠️";
  #endif
  
  report += "\n📐 Gerakan: ";
  report += isMotionActive ? "Bergerak ⚠️" : "Posisi stabil ✓";
  report += "\n━━━━━━━━━━━━━━━━━━━━━\n";
  report += "⏰ Uptime: " + getFormattedUptime() + "\n";
  report += "📶 WiFi: " + String(WiFi.RSSI()) + " dBm";
  
  bot.sendMessage(TELEGRAM_CHAT_ID, report, "");
  Serial.println("\n✓ Report berkala terkirim ke Telegram\n");
}

#endif

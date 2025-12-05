/*
 * ====================================
 * SENSOR HANDLER
 * ====================================
 * Fungsi untuk menangani logika sensor
 * ====================================
 */

#ifndef SENSOR_HANDLER_H
#define SENSOR_HANDLER_H

#include "config.h"
#include "telegram_manager.h"

// Variabel eksternal yang digunakan
extern int lastTiltState;
extern int lastHallState;
extern unsigned long lastTiltChangeTime;
extern unsigned long lastMotionAlertTime;
extern bool isMotionActive;
extern bool motionAlertSent;
extern bool isBoxOpen;
extern unsigned long lastBoxOpenAlertTime;
extern bool isSystemArmed;

// ====================================
// FUNGSI: CEK KOTAK DIBUKA
// ====================================
void checkBoxOpened(int hallValue) {
  // Tentukan state berdasarkan konfigurasi sensor
  bool currentBoxState;
  
  #if HALL_SENSOR_INVERTED
    // Sensor terbalik: HIGH = tertutup, LOW = terbuka
    currentBoxState = (hallValue == LOW);
  #else
    // Sensor normal: LOW = tertutup, HIGH = terbuka
    currentBoxState = (hallValue == HIGH);
  #endif
  
  // Deteksi perubahan status
  if (hallValue != lastHallState) {
    if (currentBoxState) {
      // Kotak baru saja dibuka!
      isBoxOpen = true;
      
      Serial.println("\n╔═══════════════════════════════════╗");
      Serial.println("║  🚨 ALERT: KOTAK TERBUKA!         ║");
      Serial.println("╚═══════════════════════════════════╝\n");
      
      // Kirim alert pertama kali
      sendBoxOpenedAlert();
      lastBoxOpenAlertTime = millis();
      
    } else {
      // Kotak ditutup kembali
      isBoxOpen = false;
      
      Serial.println("\n✓ Kotak tertutup kembali\n");
      
      sendBoxClosedNotif();
    }
    
    lastHallState = hallValue;
  }
}

// ====================================
// FUNGSI: CEK GERAKAN KOTAK
// ====================================
void checkBoxMovement(int tiltValue) {
  unsigned long currentTime = millis();
  
  // Deteksi perubahan state tilt (gerakan)
  if (tiltValue != lastTiltState) {
    // Ada perubahan posisi - kotak bergerak!
    lastTiltChangeTime = currentTime;
    lastTiltState = tiltValue;
    
    // Set gerakan aktif
    if (!isMotionActive) {
      isMotionActive = true;
      motionAlertSent = false;
      
      Serial.println("\n╔═══════════════════════════════════╗");
      Serial.println("║  ⚠️  GERAKAN TERDETEKSI!          ║");
      Serial.println("╚═══════════════════════════════════╝\n");
    }
    
    // Kirim alert jika belum dikirim atau sudah lewat cooldown
    if (!motionAlertSent || (currentTime - lastMotionAlertTime > MOTION_ALERT_COOLDOWN)) {
      sendMotionAlert();
      lastMotionAlertTime = currentTime;
      motionAlertSent = true;
    }
  }
  
  // Cek apakah gerakan sudah stagnan (berhenti)
  if (isMotionActive) {
    unsigned long timeSinceLastChange = currentTime - lastTiltChangeTime;
    
    if (timeSinceLastChange > MOTION_STABLE_TIME) {
      // Kotak sudah diam selama MOTION_STABLE_TIME
      isMotionActive = false;
      motionAlertSent = false;
      
      Serial.println("\n✓ Gerakan berhenti - kotak diam\n");
      
      sendMotionStoppedNotif();
    }
  }
}

// ====================================
// FUNGSI: TAMPILKAN STATUS DI SERIAL
// ====================================
void displaySerialStatus(int hallValue, int tiltValue) {
  Serial.print("[");
  Serial.print(millis() / 1000);
  Serial.print("s] ");
  
  // Status sistem
  Serial.print("🔒 Mode: ");
  Serial.print(isSystemArmed ? "ARMED" : "SLEEP");
  Serial.print(" | ");
  
  Serial.print("📬 Kotak: ");
  
  // Tampilkan status berdasarkan konfigurasi sensor
  #if HALL_SENSOR_INVERTED
    Serial.print(hallValue == HIGH ? "TERTUTUP ✓" : "TERBUKA ⚠");
  #else
    Serial.print(hallValue == LOW ? "TERTUTUP ✓" : "TERBUKA ⚠");
  #endif
  
  Serial.print(" | 📐 Gerakan: ");
  if (isMotionActive) {
    Serial.print("AKTIF ⚠");
  } else {
    Serial.print("Diam ○");
  }
  
  Serial.println();
}

#endif

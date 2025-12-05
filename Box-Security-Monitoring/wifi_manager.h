/*
 * ====================================
 * WIFI MANAGER
 * ====================================
 * Fungsi untuk mengelola koneksi WiFi
 * ====================================
 */

#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include "config.h"

// ====================================
// FUNGSI: KONEKSI WIFI
// ====================================
void connectWiFi() {
  Serial.println("\n=== Menghubungkan ke WiFi ===");
  Serial.print("SSID: ");
  Serial.println(WIFI_SSID);
  
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  
  int attempts = 0;
  while (WiFi.status() != WL_CONNECTED && attempts < 30) {
    Serial.print(".");
    delay(500);
    attempts++;
  }
  
  if (WiFi.status() == WL_CONNECTED) {
    Serial.println("\n✓ WiFi terhubung!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    Serial.print("Signal Strength: ");
    Serial.print(WiFi.RSSI());
    Serial.println(" dBm");
  } else {
    Serial.println("\n✗ Gagal terhubung ke WiFi!");
    Serial.println("Sistem akan tetap berjalan tanpa notifikasi Telegram");
  }
}

// ====================================
// FUNGSI: CEK STATUS WIFI
// ====================================
bool isWiFiConnected() {
  return WiFi.status() == WL_CONNECTED;
}

// ====================================
// FUNGSI: RECONNECT WIFI
// ====================================
void reconnectWiFi() {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("\n⚠️ WiFi terputus, mencoba koneksi ulang...");
    WiFi.disconnect();
    WiFi.reconnect();
    
    int attempts = 0;
    while (WiFi.status() != WL_CONNECTED && attempts < 10) {
      delay(500);
      attempts++;
    }
    
    if (WiFi.status() == WL_CONNECTED) {
      Serial.println("✓ WiFi terhubung kembali!");
    }
  }
}

#endif

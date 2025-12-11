 /*
 * ====================================
 * UTILITIES
 * ====================================
 * Fungsi-fungsi utility umum
 * ====================================
 */

#ifndef UTILS_H
#define UTILS_H

#include <Arduino.h>

// ====================================
// FUNGSI: FORMAT WAKTU
// ====================================
String getFormattedTime() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  
  seconds = seconds % 60;
  minutes = minutes % 60;
  
  String time = "Waktu: ";
  if (hours < 10) time += "0";
  time += String(hours) + ":";
  if (minutes < 10) time += "0";
  time += String(minutes) + ":";
  if (seconds < 10) time += "0";
  time += String(seconds);
  
  return time;
}

// ====================================
// FUNGSI: FORMAT UPTIME
// ====================================
String getFormattedUptime() {
  unsigned long seconds = millis() / 1000;
  unsigned long minutes = seconds / 60;
  unsigned long hours = minutes / 60;
  unsigned long days = hours / 24;
  
  hours = hours % 24;
  minutes = minutes % 60;
  seconds = seconds % 60;
  
  String uptime = "";
  if (days > 0) uptime += String(days) + "d ";
  if (hours > 0) uptime += String(hours) + "h ";
  if (minutes > 0) uptime += String(minutes) + "m ";
  uptime += String(seconds) + "s";
  
  return uptime;
}

// ====================================
// FUNGSI: PRINT HEADER
// ====================================
void printHeader() {
  Serial.println("\n\n");
  Serial.println("╔═══════════════════════════════════════════╗");
  Serial.println("║                                           ║");
  Serial.println("║   SISTEM MONITORING KEAMANAN KOTAK        ║");
  Serial.println("║   Security Box Monitoring System          ║");
  Serial.println("║                                           ║");
  Serial.println("║   Version: 1.0                            ║");
  Serial.println("║   Author: IoT Security Team               ║");
  Serial.println("║                                           ║");
  Serial.println("╚═══════════════════════════════════════════╝");
  Serial.println();
}

// ====================================
// FUNGSI: PRINT SEPARATOR
// ====================================
void printSeparator() {
  Serial.println("═══════════════════════════════════════════");
}

#endif

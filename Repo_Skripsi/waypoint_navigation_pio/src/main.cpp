#include <Arduino.h>
#include <WiFi.h>
#include <Wire.h>

#include "config.h"
#include "globals.h"
#include "motors.h"
#include "odometry.h"
#include "sensors.h"
#include "navigation.h"
#include "commands.h"
#include "web_server.h"
#include "nvs_storage.h"

void setup() {
  Serial.begin(115200);
  
  // --- Inisialisasi I2C & Wire (SDA: 32, SCL: 33) ---
  Wire.begin(I2C_SDA_PIN, I2C_SCL_PIN);
  Wire.setClock(400000);

  // --- Inisialisasi MPU9250 ---
  mpuReady = initMpu9250();
  
  // --- Inisialisasi OLED ---
  oledReady = initOled();

  loadStoredParameters();

  // --- Inisialisasi LEDC PWM untuk 4 Pin Motor (20kHz, Ultra-Hening) ---
  ledcAttach(L_RPWM, PWM_FREQ, PWM_RES);
  ledcAttach(L_LPWM, PWM_FREQ, PWM_RES);
  ledcAttach(R_RPWM, PWM_FREQ, PWM_RES);
  ledcAttach(R_LPWM, PWM_FREQ, PWM_RES);

  // --- Inisialisasi Pin Enable BTS7960 ---
  pinMode(L_EN, OUTPUT);
  pinMode(R_EN, OUTPUT);
  enableDrivers();

  // --- Inisialisasi 8 Sensor HY-SRF05 (Mode 1-Pin) ---
  for (int i = 0; i < 8; i++) {
    pinMode(SENSOR_PINS[i], INPUT);
  }

  // --- Inisialisasi Encoder ---
  pinMode(ENCA_L, INPUT);
  pinMode(ENCB_L, INPUT);
  pinMode(ENCA_R, INPUT);
  pinMode(ENCB_R, INPUT);

  // --- Pasang Interupsi Encoder ---
  attachInterrupt(digitalPinToInterrupt(ENCA_L), isrLA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCB_L), isrLB, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCA_R), isrRA, CHANGE);
  attachInterrupt(digitalPinToInterrupt(ENCB_R), isrRB, CHANGE);

  stopMotor();

  // --- Inisialisasi Wi-Fi Access Point ---
  WiFi.setSleep(false); // Matikan WiFi sleep mode agar koneksi tidak mudah putus
  WiFi.setTxPower(WIFI_POWER_19_5dBm); // Set TX power maksimal
  WiFi.softAP(ssid, password, 6); // Gunakan channel 6 agar tidak terlalu padat
  Serial.println("\n============================================");
  Serial.println(">>> Wi-Fi AP 'OKTO-BOT-AP' Dimulai!");
  Serial.print(">>> Password: "); Serial.println(password);
  Serial.print(">>> IP Address: "); Serial.println(WiFi.softAPIP());
  Serial.println("============================================");

  // --- Inisialisasi HTTP Web Server ---
  setupWebServer();
  Serial.println(">>> HTTP Web Server Port 80 Berhasil Dimulai!");
  Serial.println("============================================");

  printHelpMenu();
}

void loop() {
  server.handleClient();
  handleSerial();

  unsigned long now = millis();

  if (now - lastLoopTime < LOOP_MS)
    return;

  float dt = (now - lastLoopTime) / 1000.0;
  lastLoopTime = now;

  // 1. Baca IMU MPU9250 jika aktif
  if (mpuReady) {
    readMPU9250();
  }

  // 2. Baca 1 sensor ultrasonik secara Round-Robin (Non-Blocking)
  readNextSensorRoundRobin();

  // 3. Update odometri & kecepatan terfilter LPF
  updateOdometry(dt);

  // 4. State Machine Navigasi & Inner Loop PI Kecepatan
  runNavigationStateMachine(dt);

  // 5. Update layar OLED secara berkala (setiap 200ms)
  static unsigned long lastOledTime = 0;
  if (oledReady && (now - lastOledTime >= 200)) {
    lastOledTime = now;
    updateOledDisplay();
  }
}

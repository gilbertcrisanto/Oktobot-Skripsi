#include "sensors.h"
#include "globals.h"
#include "nvs_storage.h"
#include <Wire.h>
#include <WiFi.h>

void readNextSensorRoundRobin() {
  static int currentSensor = 0;
  int pin = SENSOR_PINS[currentSensor];

  pinMode(pin, OUTPUT);
  digitalWrite(pin, LOW);
  delayMicroseconds(2);
  digitalWrite(pin, HIGH);
  delayMicroseconds(10);
  digitalWrite(pin, LOW);

  pinMode(pin, INPUT);
  long duration = pulseIn(pin, HIGH, 4000); // 4ms timeout

  if (duration == 0) {
    sensorDist[currentSensor] = 999.0;
  } else {
    sensorDist[currentSensor] = (duration / 2.0) * 0.0343;
  }

  currentSensor = (currentSensor + 1) % 8;
}

// --- I2C Helper Functions ---
bool i2cWrByte(uint8_t addr, uint8_t reg, uint8_t val) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  Wire.write(val);
  return Wire.endTransmission() == 0;
}

bool i2cRdBytes(uint8_t addr, uint8_t reg, uint8_t n, uint8_t* dst) {
  Wire.beginTransmission(addr);
  Wire.write(reg);
  if (Wire.endTransmission(false) != 0) return false;
  if (Wire.requestFrom(addr, n) != n) return false;
  for (uint8_t i = 0; i < n; i++) dst[i] = Wire.read();
  return true;
}

int16_t mk16(uint8_t h, uint8_t l) {
  return (int16_t)((h << 8) | l);
}

// --- MPU9250 ---
bool initMpu9250() {
  uint8_t who = 0;
  if (i2cRdBytes(MPU_ADDR_PRI, MPU_WHO_AM_I, 1, &who)) {
    mpuAddr = MPU_ADDR_PRI;
  } else if (i2cRdBytes(MPU_ADDR_SEC, MPU_WHO_AM_I, 1, &who)) {
    mpuAddr = MPU_ADDR_SEC;
  } else {
    Serial.println(">>> IMU: MPU9250 tidak terdeteksi!");
    return false;
  }

  Serial.printf(">>> IMU: MPU9250 ditemukan di 0x%02X, WHO_AM_I = 0x%02X\n", mpuAddr, who);

  // Power management 1: wake up
  i2cWrByte(mpuAddr, MPU_PWR1, 0x00);
  delay(100);

  // Configure bypass mode for AK8963 magnetometer (if present)
  i2cWrByte(mpuAddr, 0x6A, 0x00); // disable master mode
  i2cWrByte(mpuAddr, 0x37, 0x02); // enable bypass
  delay(50);

  uint8_t akWho = 0;
  if (i2cRdBytes(AK_ADDR, AK_WIA, 1, &akWho) && akWho == 0x48) {
    Serial.println(">>> IMU: AK8963 Magnetometer OK!");
    i2cWrByte(AK_ADDR, AK_CNTL1, 0x00); delay(10);
    i2cWrByte(AK_ADDR, AK_CNTL1, 0x0F); delay(10); // Fuse ROM mode
    uint8_t asa[3];
    if (i2cRdBytes(AK_ADDR, AK_ASAX, 3, asa)) {
      magCal[0] = (float)(asa[0] - 128) / 256.0f + 1.0f;
      magCal[1] = (float)(asa[1] - 128) / 256.0f + 1.0f;
      magCal[2] = (float)(asa[2] - 128) / 256.0f + 1.0f;
    }
    i2cWrByte(AK_ADDR, AK_CNTL1, 0x00); delay(10);
    i2cWrByte(AK_ADDR, AK_CNTL1, 0x16); delay(10); // Continuous measurement 100Hz, 16-bit
    akReady = true;
  } else {
    Serial.println(">>> IMU: AK8963 Magnetometer tidak ditemukan (opsional).");
  }

  return true;
}

bool readMPU9250() {
  uint8_t raw[14];
  if (!i2cRdBytes(mpuAddr, MPU_ACCEL_H, 14, raw)) return false;

  lastAx = mk16(raw[0], raw[1]) / 16384.0f;
  lastAy = mk16(raw[2], raw[3]) / 16384.0f;
  lastAz = mk16(raw[4], raw[5]) / 16384.0f;
  lastTemp = (mk16(raw[6], raw[7]) / 333.87f) + 21.0f;
  lastGx = (mk16(raw[8], raw[9]) / 131.0f) - gyroBias[0];
  lastGy = (mk16(raw[10], raw[11]) / 131.0f) - gyroBias[1];
  lastGz = (mk16(raw[12], raw[13]) / 131.0f) - gyroBias[2];

  unsigned long now = millis();
  if (lastImuT == 0) lastImuT = now;
  float dt = (now - lastImuT) / 1000.0f;
  lastImuT = now;

  // Integrate Z-axis gyro values to get yaw (in degrees)
  imuYaw += lastGz * dt;

  // Normalize imuYaw to [-180, 180] degrees
  while (imuYaw > 180.0f)  imuYaw -= 360.0f;
  while (imuYaw < -180.0f) imuYaw += 360.0f;

  lastMagOk = false;
  if (akReady) {
    uint8_t st1 = 0;
    if (i2cRdBytes(AK_ADDR, AK_ST1, 1, &st1) && (st1 & 0x01)) {
      uint8_t mr[7];
      if (i2cRdBytes(AK_ADDR, AK_HXL, 7, mr) && !(mr[6] & 0x08)) {
        lastMx = mk16(mr[1], mr[0]) * 0.15f * magCal[0];
        lastMy = mk16(mr[3], mr[2]) * 0.15f * magCal[1];
        lastMz = mk16(mr[5], mr[4]) * 0.15f * magCal[2];
        lastMagOk = true;
      }
    }
  }

  imuDataOk = true;
  return true;
}

void calGyro() {
  const int N = 200;
  if (!mpuReady) {
    mpuReady = initMpu9250();
    if (!mpuReady) return;
  }

  Serial.println("\n=== KALIBRASI GYRO BIAS — Jangan gerakkan robot! ===");
  if (oledReady) {
    display.clearDisplay();
    display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
    display.setCursor(10, 10);
    display.println("KALIBRASI IMU...");
    display.setCursor(10, 25);
    display.println("Jangan gerakkan");
    display.setCursor(10, 35);
    display.println("robot!");
    display.display();
  }

  delay(2000);
  float sx = 0, sy = 0, sz = 0;
  int cnt = 0;

  for (int i = 0; i < N; i++) {
    uint8_t r[14];
    if (i2cRdBytes(mpuAddr, MPU_ACCEL_H, 14, r)) {
      sx += mk16(r[8], r[9]) / 131.0f;
      sy += mk16(r[10], r[11]) / 131.0f;
      sz += mk16(r[12], r[13]) / 131.0f;
      cnt++;
    }
    if (i % 20 == 0 && oledReady) {
      display.clearDisplay();
      display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
      display.setCursor(10, 10);
      display.println("KALIBRASI IMU...");
      display.drawRect(10, 30, 108, 10, SSD1306_WHITE);
      display.fillRect(10, 30, (i * 108) / N, 10, SSD1306_WHITE);
      display.setCursor(10, 48);
      display.printf("Progress: %d%%", (i * 100) / N);
      display.display();
    }
    delay(5);
  }

  if (cnt > 0) {
    gyroBias[0] = sx / cnt;
    gyroBias[1] = sy / cnt;
    gyroBias[2] = sz / cnt;
    gyroCalibrated = true;
    saveAllParameters(); // save to NVS
    Serial.printf("Gyro Bias Berhasil Diperoleh -> X:%+.4f Y:%+.4f Z:%+.4f\n", gyroBias[0], gyroBias[1], gyroBias[2]);

    if (oledReady) {
      display.clearDisplay();
      display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
      display.setCursor(10, 15);
      display.println("KALIBRASI SELESAI!");
      display.setCursor(10, 35);
      display.printf("Bias Z: %+.4f", gyroBias[2]);
      display.display();
      delay(2000);
    }
  }
}

// --- OLED ---
bool initOled() {
  if (display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    oledAddr = 0x3C;
  } else if (display.begin(SSD1306_SWITCHCAPVCC, 0x3D)) {
    oledAddr = 0x3D;
  } else {
    Serial.println(">>> OLED: Layar OLED tidak terdeteksi!");
    return false;
  }

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);
  display.setCursor(15, 10);
  display.println("OKTO-BOT AP");
  display.setCursor(15, 25);
  display.println("SDA:32 SCL:33");
  display.display();

  return true;
}

void updateOledDisplay() {
  if (!oledReady) return;
  oledCnt++;

  display.clearDisplay();
  display.setTextColor(SSD1306_WHITE);
  display.setTextSize(1);

  // Draw border
  display.drawRect(0, 0, 128, 64, SSD1306_WHITE);

  // Title bar
  display.setCursor(8, 4);
  String title = "OKTO-BOT";
  if (useIMUHeading) title += " [IMU]";
  else title += " [ENC]";
  display.print(title);

  display.drawLine(0, 14, 127, 14, SSD1306_WHITE);

  // Position X, Y
  display.setCursor(4, 17);
  display.printf("X: %6.1f cm", posX);
  display.setCursor(4, 27);
  display.printf("Y: %6.1f cm", posY);

  // Heading & Waypoint
  display.setCursor(4, 37);
  display.printf("Th: %5.1f deg", degrees(theta));
  
  String stateStr = "IDLE";
  switch (robotState) {
    case IDLE:              stateStr = "IDLE"; break;
    case ROTATE_TO_TARGET:  stateStr = "ROT"; break;
    case WAIT_AFTER_ROTATE: stateStr = "WAIT"; break;
    case MOVE_TO_TARGET:    stateStr = "MOVE"; break;
    case FINISHED:          stateStr = "DONE"; break;
    case MANUAL_CONTROL:    stateStr = "MAN"; break;
    case PAUSED_MEASURE:    stateStr = "PAUS"; break;
  }
  display.setCursor(4, 47);
  display.printf("WP: %d/%d  St: %s", currentWaypoint, totalWaypoints, stateStr.c_str());

  // Bottom line & IP
  display.drawLine(0, 56, 127, 56, SSD1306_WHITE);
  display.setCursor(4, 57);
  display.print(WiFi.softAPIP().toString());

  display.display();
}


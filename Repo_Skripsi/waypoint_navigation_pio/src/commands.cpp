#include "commands.h"
#include "globals.h"
#include "nvs_storage.h"
#include "motors.h"
#include "sensors.h"


void processCommand(String input) {
  String cmd = input;
  cmd.toUpperCase();

  if (cmd.equals("CLEAR") || cmd.equals("C")) {
    totalWaypoints = 0;
    currentWaypoint = 0;
    robotState = IDLE;
    latchedEscapeDir = 0.0;
    frontClearCount = 0;
    Serial.println(">>> WAYPOINTS CLEARED. State: IDLE.");
  }
  else if (cmd.equals("START") || cmd.equals("S")) {
    if (totalWaypoints == 0) {
      Serial.println(">>> ERROR: Tidak ada waypoint!");
      return;
    }
    currentWaypoint = 0;
    prevEncL = encL; prevEncR = encR;
    robotState = ROTATE_TO_TARGET;
    latchedEscapeDir = 0.0;
    frontClearCount = 0;
    enableDrivers();
    Serial.print(">>> MEMULAI NAVIGASI MENUJU ");
    Serial.print(totalWaypoints);
    Serial.println(" WAYPOINTS...");
  }
  else if (cmd.equals("STOP")) {
    robotState = IDLE;
    stopMotor();
    disableDrivers();
    latchedEscapeDir = 0.0;
    frontClearCount = 0;
    Serial.println(">>> DARURAT: Robot dihentikan!");
  }
  else if (cmd.equals("STATUS") || cmd.equals("P")) {
    printStatus();
  }
  else if (cmd.equals("SENSOR")) {
    printSensorReadings();
  }
  else if (cmd.equals("APF ON")) {
    useAPF = true;
    saveAllParameters();
    Serial.println(">>> APF (Obstacle Avoidance): ON (Aktif)");
  }
  else if (cmd.equals("APF OFF")) {
    useAPF = false;
    saveAllParameters();
    Serial.println(">>> APF (Obstacle Avoidance): OFF (Bypass)");
  }
  else if (cmd.equals("APF")) {
    useAPF = !useAPF;
    saveAllParameters();
    Serial.print(">>> APF (Obstacle Avoidance) diubah ke: ");
    Serial.println(useAPF ? "ON" : "OFF");
  }
  else if (cmd.startsWith("SPEED ")) {
    float s = cmd.substring(6).toFloat();
    if (s >= 8.0 && s <= 50.0) {
      MAX_MOVE_VEL = s;
      saveAllParameters();
      Serial.print(">>> MAX_MOVE_VEL diubah ke: ");
      Serial.print(MAX_MOVE_VEL);
      Serial.println(" cm/s");
    } else {
      Serial.println(">>> ERROR: Kecepatan tidak valid (8.0 - 50.0 cm/s)!");
    }
  }
  else if (cmd.startsWith("KREP ")) {
    float k = cmd.substring(5).toFloat();
    if (k >= 1.0 && k <= 40.0) {
      K_REP = k;
      saveAllParameters();
      Serial.print(">>> K_REP diubah ke: ");
      Serial.println(K_REP);
    } else {
      Serial.println(">>> ERROR: K_REP tidak valid (1.0 - 40.0)!");
    }
  }
  else if (cmd.startsWith("MANUAL ")) {
    // Parser MANUAL L_SPEED R_SPEED
    int firstSpace = cmd.indexOf(' ');
    int secondSpace = cmd.indexOf(' ', firstSpace + 1);
    if (firstSpace != -1) {
      float spdL, spdR;
      if (secondSpace != -1) {
        spdL = cmd.substring(firstSpace + 1, secondSpace).toFloat();
        spdR = cmd.substring(secondSpace + 1).toFloat();
      } else {
        spdL = cmd.substring(firstSpace + 1).toFloat();
        spdR = spdL;
      }
      robotState = MANUAL_CONTROL;
      enableDrivers();
      targetVelL = constrain(spdL, -50.0, 50.0);
      targetVelR = constrain(spdR, -50.0, 50.0);
      lastManualCmdTime = millis();
    }
  }
  else if (cmd.startsWith("SETPIDHEADING ")) {
    int firstSpace = cmd.indexOf(' ');
    int secondSpace = cmd.indexOf(' ', firstSpace + 1);
    int thirdSpace = cmd.indexOf(' ', secondSpace + 1);
    if (firstSpace != -1 && secondSpace != -1 && thirdSpace != -1) {
      Kp_move = cmd.substring(firstSpace + 1, secondSpace).toFloat();
      Ki_move = cmd.substring(secondSpace + 1, thirdSpace).toFloat();
      Kd_move = cmd.substring(thirdSpace + 1).toFloat();
      saveAllParameters();
      Serial.printf(">>> PID Heading diubah -> Kp: %.4f | Ki: %.4f | Kd: %.4f\n", Kp_move, Ki_move, Kd_move);
    }
  }
  else if (cmd.startsWith("SETPIDSPEED ")) {
    int firstSpace = cmd.indexOf(' ');
    int secondSpace = cmd.indexOf(' ', firstSpace + 1);
    if (firstSpace != -1 && secondSpace != -1) {
      Kp_vel = cmd.substring(firstSpace + 1, secondSpace).toFloat();
      Ki_vel = cmd.substring(secondSpace + 1).toFloat();
      saveAllParameters();
      Serial.printf(">>> PI Kecepatan diubah -> Kp: %.4f | Ki: %.4f\n", Kp_vel, Ki_vel);
    }
  }
  else if (cmd.equals("RESETPOSE")) {
    posX = 0;
    posY = 0;
    theta = PI / 2.0;
    prevEncL = encL;
    prevEncR = encR;
    latchedEscapeDir = 0.0;
    frontClearCount = 0;
    Serial.println(">>> Pose direset ke Home (X=0, Y=0, Th=90 deg)");
  }
  else if (cmd.startsWith("SETPOSE ")) {
    int firstSpace = cmd.indexOf(' ');
    int secondSpace = cmd.indexOf(' ', firstSpace + 1);
    int thirdSpace = cmd.indexOf(' ', secondSpace + 1);
    if (firstSpace != -1 && secondSpace != -1 && thirdSpace != -1) {
      float x = cmd.substring(firstSpace + 1, secondSpace).toFloat();
      float y = cmd.substring(secondSpace + 1, thirdSpace).toFloat();
      float th = cmd.substring(thirdSpace + 1).toFloat();
      posX = x;
      posY = y;
      theta = radians(th);
      prevEncL = encL;
      prevEncR = encR;
      latchedEscapeDir = 0.0;
      frontClearCount = 0;
      Serial.printf(">>> Pose diatur ke: X=%.2f, Y=%.2f, Th=%.2f deg\n", posX, posY, th);
    }
  }
  else if (cmd.equals("CALIMU")) {
    calGyro();
  }
  else if (cmd.equals("IMUHEADING ON")) {
    useIMUHeading = true;
    saveAllParameters();
    Serial.println(">>> IMU Heading ON");
  }
  else if (cmd.equals("IMUHEADING OFF")) {
    useIMUHeading = false;
    saveAllParameters();
    Serial.println(">>> IMU Heading OFF");
  }
  else if (cmd.equals("RESETNVS")) {
    preferences.begin("oktobot", false);
    preferences.clear();
    preferences.end();
    Serial.println(">>> NVS parameters cleared! Restarting ESP32 to apply default values...");
    delay(1000);
    ESP.restart();
  }
  else if (cmd.startsWith("ADD ") || cmd.startsWith("A ")) {
    if (totalWaypoints >= MAX_WAYPOINTS) {
      Serial.println(">>> ERROR: Array waypoint penuh!");
      return;
    }

    int firstSpace = input.indexOf(' ');
    int secondSpace = input.indexOf(' ', firstSpace + 1);
    int comma = input.indexOf(',', firstSpace + 1);

    String xStr = "";
    String yStr = "";

    if (secondSpace != -1) {
      xStr = input.substring(firstSpace + 1, secondSpace);
      yStr = input.substring(secondSpace + 1);
    } else if (comma != -1) {
      xStr = input.substring(firstSpace + 1, comma);
      yStr = input.substring(comma + 1);
    } else {
      Serial.println(">>> ERROR: Format salah! Gunakan: ADD X Y atau ADD X,Y");
      return;
    }

    xStr.trim();
    yStr.trim();

    if (xStr.length() > 0 && yStr.length() > 0) {
      float x = xStr.toFloat();
      float y = yStr.toFloat();

      path[totalWaypoints].x = x;
      path[totalWaypoints].y = y;

      Serial.printf(">>> WAYPOINT [%d] DITAMBAHKAN: (%.1f, %.1f)\n", totalWaypoints, x, y);
      totalWaypoints++;
    } else {
      Serial.println(">>> ERROR: Gagal parsing koordinat.");
    }
  }
  else if (cmd.equals("STEPMODE ON")) {
    stepModeActive = true;
    Serial.println(">>> STEPMODE: ON");
  }
  else if (cmd.equals("STEPMODE OFF")) {
    stepModeActive = false;
    Serial.println(">>> STEPMODE: OFF");
  }
  else if (cmd.equals("RESUME")) {
    if (robotState == PAUSED_MEASURE) {
      robotState = nextStateAfterPause;
      if (robotState == WAIT_AFTER_ROTATE) {
        waitStartTime = millis(); // Mulai ulang timer tunggu saat resume
      }
      Serial.println(">>> STEPMODE: Resuming navigation...");
    } else {
      Serial.println(">>> ERROR: Robot tidak dalam kondisi pause!");
    }
  }
  else if (cmd.equals("HELP") || cmd.equals("H")) {
    printHelpMenu();
  }
  else {
    Serial.println(">>> ERROR: Perintah tidak dikenal!");
    printHelpMenu();
  }
}

void handleSerial() {
  if (Serial.available() > 0) {
    String input = Serial.readStringUntil('\n');
    input.trim();
    if (input.length() > 0) {
      processCommand(input);
    }
  }
}

void printHelpMenu() {
  Serial.println("====================================================");
  Serial.println("KONTROL NAVIGASI VIA SERIAL/WEB (ESP32):");
  Serial.println("  ADD X Y     : Menambahkan waypoint baru (ADD -50 100)");
  Serial.println("  CLEAR (C)   : Menghapus semua daftar waypoint");
  Serial.println("  START (S)   : Memulai navigasi robot");
  Serial.println("  STOP        : Menghentikan robot (Emergency)");
  Serial.println("  STATUS (P)  : Melihat status & daftar waypoint");
  Serial.println("  SENSOR      : Melihat jarak 8 ultrasonik");
  Serial.println("  APF ON/OFF  : Mengaktifkan/menonaktifkan sensor APF");
  Serial.println("  APF         : Toggle sensor halangan");
  Serial.println("  MANUAL L R  : Mengemudi manual (L, R dalam cm/s)");
  Serial.println("  RESETPOSE   : Mengeset posisi robot kembali ke awal (0,0,90)");
  Serial.println("  SETPOSE X Y Th : Mengatur posisi robot kustom");
  Serial.println("  SETPIDHEADING P I D : Mengubah parameter PID Heading");
  Serial.println("  SETPIDSPEED P I     : Mengubah parameter PI Kecepatan Roda");
  Serial.println("  CALIMU      : Kalibrasi IMU");
  Serial.println("  IMUHEADING ON/OFF   : Mengaktifkan/menonaktifkan heading berbasis IMU");
  Serial.println("  RESETNVS    : Reset seluruh parameter NVS ke default bawaan kode");
  Serial.println("  STEPMODE ON/OFF : Mengaktifkan/menonaktifkan mode ukur fisik");
  Serial.println("  RESUME          : Melanjutkan navigasi setelah pause ukur fisik");
  Serial.println("  HELP (H)    : Menampilkan menu bantuan ini");
  Serial.println("====================================================");
}

void printStatus() {
  Serial.println("\n--- STATUS ROBOT (ESP32 3-PID + 360° APF) ---");
  Serial.print("APF    : ");
  Serial.println(useAPF ? "ON (Aktif)" : "OFF (Bypass)");
  Serial.print("Speed  : "); Serial.print(MAX_MOVE_VEL); Serial.println(" cm/s");
  Serial.print("K_REP  : "); Serial.println(K_REP);
  Serial.printf("Heading PID -> Kp: %.3f | Ki: %.3f | Kd: %.3f\n", Kp_move, Ki_move, Kd_move);
  Serial.printf("Wheel Speed PI -> Kp: %.3f | Ki: %.3f\n", Kp_vel, Ki_vel);
  Serial.print("State  : ");
  switch (robotState) {
    case IDLE:              Serial.println("IDLE"); break;
    case ROTATE_TO_TARGET:  Serial.println("ROTATE_TO_TARGET"); break;
    case WAIT_AFTER_ROTATE: Serial.println("WAIT_AFTER_ROTATE"); break;
    case MOVE_TO_TARGET:    Serial.println("MOVE_TO_TARGET"); break;
    case FINISHED:          Serial.println("FINISHED"); break;
    case MANUAL_CONTROL:    Serial.println("MANUAL"); break;
    case PAUSED_MEASURE:    Serial.println("PAUSED_MEASURE"); break;
  }
  Serial.printf("Posisi : X=%.2f cm, Y=%.2f cm, TH=%.2f deg\n", posX, posY, degrees(theta));
  Serial.printf("Vel Akt: L=%.2f cm/s, R=%.2f cm/s\n", velL_act, velR_act);
  Serial.printf("Heading: %s (IMU:%s, Cal:%s, Yaw:%.2f deg)\n", useIMUHeading ? "IMU" : "ENC", mpuReady ? "OK" : "NO", gyroCalibrated ? "YES" : "NO", imuYaw);

  printSensorReadings();

  Serial.printf("Waypoints (Total: %d):\n", totalWaypoints);
  for (int i = 0; i < totalWaypoints; i++) {
    Serial.printf("  [%d] (%.1f, %.1f)", i, path[i].x, path[i].y);
    if (i == currentWaypoint && robotState != IDLE && robotState != FINISHED) {
      Serial.println(" <-- AKTIF");
    } else {
      Serial.println();
    }
  }
  Serial.println("----------------------------------------------\n");
}

void printSensorReadings() {
  Serial.println("Sensor 360° (HY-SRF05):");
  for (int i = 0; i < 8; i++) {
    Serial.printf("  [%s]: ", SENSOR_NAMES[i]);
    if (sensorDist[i] > 900) {
      Serial.println("Aman (>68cm)");
    } else {
      Serial.printf("%.1f cm\n", sensorDist[i]);
    }
  }
}

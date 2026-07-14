#include "navigation.h"
#include "globals.h"
#include "motors.h"
#include "commands.h"
#include <math.h>

float normalizeDeg(float a) {
  while (a > 180.0)  a -= 360.0;
  while (a < -180.0) a += 360.0;
  return a;
}

float mapFloat(float x, float in_min, float in_max, float out_min, float out_max) {
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

void runNavigationStateMachine(float dt) {
  switch (robotState) {

    case IDLE: {
      targetVelL = 0;
      targetVelR = 0;
      break;
    }

    // ========================================================
    // KENDALI MANUAL KENDARAAN (WATCHDOG SAFETY)
    // ========================================================
    case MANUAL_CONTROL: {
      // Watchdog: jika tidak ada perintah manual baru selama 500ms, stop motor
      if (millis() - lastManualCmdTime > 500) {
        targetVelL = 0;
        targetVelR = 0;
      }
      break;
    }

    // ========================================================
    // ROTATE
    // ========================================================
    case ROTATE_TO_TARGET: {
      float targetX = path[currentWaypoint].x;
      float targetY = path[currentWaypoint].y;

      float dx = targetX - posX;
      float dy = targetY - posY;

      // [Persamaan 2.12] Perhitungan bearing (sudut target) menuju waypoint tujuan
      float targetTheta = atan2(dy, dx);
      // [Persamaan 2.13] Perhitungan heading error (selisih sudut target dengan heading robot)
      float errDeg = normalizeDeg(degrees(targetTheta - theta));
      float absErr = abs(errDeg);

      if (absErr < ROTATE_DONE_DEG) {
        activeBrake();
        if (stepModeActive) {
          robotState = PAUSED_MEASURE;
          nextStateAfterPause = WAIT_AFTER_ROTATE;
          Serial.println("STEPMODE: PAUSED AFTER ROTATION");
        } else {
          robotState = WAIT_AFTER_ROTATE;
          waitStartTime = millis();
        }
        Serial.print("ROTATION COMPLETE WP:");
        Serial.println(currentWaypoint);
        break;
      }

      if (absErr < ROTATE_DEAD_DEG) {
        targetVelL = 0;
        targetVelR = 0;
        break;
      }

      float targetRotVel = Kp_rot_vel * absErr;

      if (absErr < ROTATE_BRAKE_DEG) {
        targetRotVel *= (absErr / ROTATE_BRAKE_DEG);
      }

      targetRotVel = constrain(targetRotVel, MIN_ROT_VEL, MAX_ROT_VEL);

      if (errDeg > 0) {
        targetVelL = -targetRotVel;
        targetVelR = +targetRotVel;
      } else {
        targetVelL = +targetRotVel;
        targetVelR = -targetRotVel;
      }

      static unsigned long lastPrintRotTime = 0;
      if (millis() - lastPrintRotTime >= 200) {
        lastPrintRotTime = millis();
        Serial.print("ROTATE WP:");
        Serial.print(currentWaypoint);
        Serial.print(" ERR:");
        Serial.print(errDeg);
        Serial.print(" TargetRotVel:");
        Serial.println(targetRotVel);
      }

      break;
    }

    // ========================================================
    // WAIT
    // ========================================================
    case WAIT_AFTER_ROTATE: {
      targetVelL = 0;
      targetVelR = 0;

      if (millis() - waitStartTime >= WAIT_MS) {
        headingIntegral = 0;
        prevHeadingErr  = 0;
        moveStartTime   = millis();
        robotState = MOVE_TO_TARGET;
        Serial.print("MOVE TO WP:");
        Serial.println(currentWaypoint);
      }
      break;
    }

    // ========================================================
    // MOVE DENGAN 360° ARTIFICIAL POTENTIAL FIELD (APF)
    // ========================================================
    case MOVE_TO_TARGET: {
      float targetX = path[currentWaypoint].x;
      float targetY = path[currentWaypoint].y;

      float dx = targetX - posX;
      float dy = targetY - posY;
      // [Persamaan 2.14 / 2.16] Perhitungan jarak Euclidean menuju target waypoint
      float dist = sqrt(dx * dx + dy * dy);

      // Waypoint Tercapai
      if (dist < MOVE_DONE_CM) {
        activeBrake();
        Serial.print("WAYPOINT ");
        Serial.print(currentWaypoint);
        Serial.println(" REACHED");
        currentWaypoint++;
        latchedEscapeDir = 0.0; // Reset escape direction latch
        frontClearCount = 0;

        if (currentWaypoint >= totalWaypoints) {
          robotState = FINISHED;
          disableDrivers();
          Serial.println("ALL WAYPOINT FINISHED");
          printStatus();
        } else {
          if (stepModeActive) {
            robotState = PAUSED_MEASURE;
            nextStateAfterPause = ROTATE_TO_TARGET;
            Serial.println("STEPMODE: PAUSED AFTER WAYPOINT");
          } else {
            robotState = ROTATE_TO_TARGET;
          }
        }
        break;
      }

      // ============================================================
      // ARTIFICIAL POTENTIAL FIELD — Pers. 2.15 ~ 2.22
      // ============================================================

      // [Persamaan 2.17] Arah relatif menuju target waypoint (dalam radian)
      float beta_goal = atan2(dy, dx);
      float F_att_x = 0;
      float F_att_y = 0;

      // [Persamaan 2.15] Perhitungan komponen gaya atraktif (F_att_x, F_att_y) berdasarkan zona jarak target
      if (dist < R_GOAL) {
        // Zona 1 (d < r): Sudah di dalam radius goal → gaya = 0
        F_att_x = 0;
        F_att_y = 0;
      } else if (dist <= S_GOAL + R_GOAL) {
        // Zona 2 (r <= d <= s+r): Linear ramp — gaya naik proporsional jarak
        float mag = K_ATT * (dist - R_GOAL) / S_GOAL;
        F_att_x = mag * cos(beta_goal);
        F_att_y = mag * sin(beta_goal);
      } else {
        // Zona 3 (d > s+r): Gaya atraktif konstan maksimum
        F_att_x = K_ATT * cos(beta_goal);
        F_att_y = K_ATT * sin(beta_goal);
      }

      // [Persamaan 2.18] Gaya Repulsif dari 8 Sensor Ultrasonik
      float F_rep_x = 0;
      float F_rep_y = 0;

      if (useAPF) {
        for (int i = 0; i < 8; i++) {
          float Dn = sensorDist[i];
          if (Dn > 900.0 || Dn < 1.0) continue;  // Sensor invalid

          // [Persamaan 2.21] Proyeksi posisi obstacle ke koordinat global berdasarkan heading dan posisi robot
          float obs_x = posX + Dn * cos(SENSOR_ANGLES[i] + theta);
          float obs_y = posY + Dn * sin(SENSOR_ANGLES[i] + theta);

          // [Persamaan 2.19] Perhitungan jarak Euclidean antara robot dengan obstacle
          float dx_obs = obs_x - posX;
          float dy_obs = obs_y - posY;
          float d_obs = sqrt(dx_obs * dx_obs + dy_obs * dy_obs);
          if (d_obs < 1.0) continue;  // Hindari div-by-zero

          // [Persamaan 2.20] Arah relatif menuju obstacle (dalam radian)
          float beta_obs = atan2(dy_obs, dx_obs);
          // Arah tolak = berlawanan arah obstacle (beta_obs + PI)
          float repel_angle = beta_obs + PI;

          float mag = 0;
          if (d_obs < R_OBS) {
            // Zona 1 (d < r): Bahaya! Gaya tolak maksimum
            mag = K_REP;
          } else if (d_obs <= S_OBS + R_OBS) {
            // Zona 2 (r <= d <= s+r): Linear decay (Pers. 2.18)
            mag = K_REP * (S_OBS + R_OBS - d_obs) / S_OBS;
          }
          // Zona 3 (d > s+r): mag = 0, tidak ada pengaruh

          F_rep_x += mag * cos(repel_angle);
          F_rep_y += mag * sin(repel_angle);
        }
      }

      // C. Anti Local-Minimum: Dekomposisi & Koreksi Gaya Repulsif
      float F_rep_mag = sqrt(F_rep_x * F_rep_x + F_rep_y * F_rep_y);
      float F_att_mag = sqrt(F_att_x * F_att_x + F_att_y * F_att_y);

      // Reset escape direction jika tidak ada halangan di depan maupun di samping (sesuai arah escape)
      bool frontObstaclePresent = (sensorDist[0] < OBSTACLE_THRESHOLD || 
                                   sensorDist[1] < OBSTACLE_THRESHOLD || 
                                   sensorDist[7] < OBSTACLE_THRESHOLD);
      bool obstacleOnTrack = frontObstaclePresent;
      
      // Proteksi Turn-In Dini: Paksa latch tetap aktif minimal sejauh 45 cm untuk melewati panjang rintangan
      if (latchedEscapeDir != 0.0) {
        float dx_esc = posX - escapeStartX;
        float dy_esc = posY - escapeStartY;
        float esc_dist = sqrt(dx_esc * dx_esc + dy_esc * dy_esc);
        if (esc_dist < 30.0) {
          obstacleOnTrack = true;
        }
        
        // R1: Proteksi posisi obstacle — jangan lepas latch sampai robot melewati posisi Y obstacle
        //     Proyeksikan posisi robot ke arah goal dan bandingkan dengan posisi obstacle
        float goalDirX = path[currentWaypoint].x - escapeStartX;
        float goalDirY = path[currentWaypoint].y - escapeStartY;
        float goalDirMag = sqrt(goalDirX * goalDirX + goalDirY * goalDirY);
        if (goalDirMag > 1.0) {
          float guX = goalDirX / goalDirMag;
          float guY = goalDirY / goalDirMag;
          // Proyeksi posisi robot dan posisi obstacle ke sumbu arah goal
          float robotProj = (posX - escapeStartX) * guX + (posY - escapeStartY) * guY;
          float obstProj  = (obstacleEstX - escapeStartX) * guX + (obstacleEstY - escapeStartY) * guY;
          float SAFETY_MARGIN = 25.0; // cm melewati obstacle baru boleh lepas latch
          if (robotProj < obstProj + SAFETY_MARGIN) {
            obstacleOnTrack = true;
          }
        }
      }
      
      // Jika sedang menghindar, pastikan halangan benar-benar sudah dilewati di samping sebelum melepas latch
      if (latchedEscapeDir > 0.0) { // Sedang melipir ke kiri -> halangan ada di kanan
        bool rightObstaclePresent = (sensorDist[1] < OBSTACLE_THRESHOLD || 
                                     sensorDist[2] < OBSTACLE_THRESHOLD || 
                                     sensorDist[3] < OBSTACLE_THRESHOLD);
        if (rightObstaclePresent) obstacleOnTrack = true;
      }
      else if (latchedEscapeDir < 0.0) { // Sedang melipir ke kanan -> halangan ada di kiri
        bool leftObstaclePresent = (sensorDist[7] < OBSTACLE_THRESHOLD || 
                                    sensorDist[6] < OBSTACLE_THRESHOLD || 
                                    sensorDist[5] < OBSTACLE_THRESHOLD);
        if (leftObstaclePresent) obstacleOnTrack = true;
      }

      if (obstacleOnTrack) {
        frontClearCount = 0;
      } else {
        frontClearCount++;
        if (frontClearCount >= 30) { // 30 loop * 20ms = 600ms
          latchedEscapeDir = 0.0;
          escapeCooldown = 100; // R2: Blokir RE-ROTATE selama 100 loop (~2 detik) setelah latch lepas
        }
      }

      if ((F_rep_mag > 0.01 || latchedEscapeDir != 0.0) && F_att_mag > 0.01) {
        // Unit vector arah goal (dari gaya atraktif)
        float ux = F_att_x / F_att_mag;
        float uy = F_att_y / F_att_mag;

        // Dekomposisi F_rep: komponen parallel & perpendicular terhadap arah goal
        float par = F_rep_x * ux + F_rep_y * uy; // negatif = melawan goal
        
        // Komponen perpendicular diproyeksikan ke sumbu kiri-goal (-uy, ux)
        // Positif = gaya mengarah ke KIRI goal, Negatif = gaya mengarah ke KANAN goal
        float perp_proj = F_rep_x * (-uy) + F_rep_y * ux;

        // 1. BATASI komponen anti-parallel: maks 70% dari F_att
        //    Sehingga resultan SELALU punya komponen maju (minimal 30%)
        if (par < -F_att_mag * 0.7) {
          par = -F_att_mag * 0.7;
        }

        // 2. TANGENTIAL ESCAPE: Jika obstacle tepat di depan
        //    (anti-parallel kuat TAPI perpendicular lemah → local minimum, ATAU ada escape yang dikunci)
        float perp_mag = abs(perp_proj);
        if (latchedEscapeDir != 0.0 || (par < -F_att_mag * 0.3 && perp_mag < F_att_mag * 0.5)) {
          // Jika belum ada arah escape yang dikunci, pilih dan kunci sekarang
          if (latchedEscapeDir == 0.0) {
            float clearLeft  = sensorDist[6] + sensorDist[7]; // KIR + D-KIR
            float clearRight = sensorDist[1] + sensorDist[2]; // D-KAN + KAN
            latchedEscapeDir = (clearLeft >= clearRight) ? 1.0 : -1.0;
            
            // Catat koordinat awal mula menghindar (latching start point)
            escapeStartX = posX;
            escapeStartY = posY;
            
            // R1: Estimasi posisi obstacle di koordinat global
            // Gunakan sensor depan (sensor 0) sebagai referensi utama
            float estDist = sensorDist[0];
            if (estDist > 900.0 || estDist < 1.0) estDist = 40.0; // Fallback jika sensor 0 invalid
            obstacleEstX = posX + estDist * cos(theta);
            obstacleEstY = posY + estDist * sin(theta);
          }

          // SUPRESI gaya repulsif perpendicular yang MELAWAN arah escape:
          // Jika escape ke kiri (latchedEscapeDir=+1), perp_proj harus >= 0 (dorong ke kiri)
          // Jika escape ke kanan (latchedEscapeDir=-1), perp_proj harus <= 0 (dorong ke kanan)
          // Jika berlawanan, clamp ke 0 agar tidak membatalkan manuver hindar
          if (perp_proj * latchedEscapeDir < 0.0f) {
            perp_proj = 0.0f;
          }

          // Tambahkan gaya hindar tangensial (tegak lurus arah goal) menggunakan arah yang dikunci
          float escape_mag = F_att_mag * 1.5;
          perp_proj += latchedEscapeDir * escape_mag;
        }

        // Rekonstruksi F_rep terkoreksi dari skalar (par, perp_proj) ke vektor (x, y)
        F_rep_x = par * ux + perp_proj * (-uy);
        F_rep_y = par * uy + perp_proj * ux;
        F_rep_mag = sqrt(F_rep_x * F_rep_x + F_rep_y * F_rep_y);
      }

      // Cap total gaya repulsif
      float maxRepMag = max(F_att_mag * 3.0f, K_ATT * 2.0f);
      if (F_rep_mag > maxRepMag) {
        float scale = maxRepMag / F_rep_mag;
        F_rep_x *= scale;
        F_rep_y *= scale;
        F_rep_mag = maxRepMag;
      }

      // [Persamaan 2.22] Resultan gaya total (penggabungan gaya atraktif dan repulsif)
      float F_tot_x = F_att_x + F_rep_x;
      float F_tot_y = F_att_y + F_rep_y;

      // Target Arah Hadap dari APF (sudut resultan gaya total)
      float targetThetaAPF = atan2(F_tot_y, F_tot_x);

      float headingErr = normalizeDeg(degrees(targetThetaAPF - theta));

      // Re-rotate HANYA jika TIDAK ADA obstacle terdekat DAN jarak masih jauh (>25 cm)
      // Saat menghindari obstacle atau sudah dekat goal, PID heading yang mengarahkan
      bool hasRepulsive = (F_rep_mag > 0.01);
      
      // R2: Decrement cooldown counter
      if (escapeCooldown > 0) escapeCooldown--;
      
      // RE-ROTATE diblokir jika: ada gaya repulsif, escape latch aktif, ATAU masih dalam cooldown
      if (!hasRepulsive && latchedEscapeDir == 0.0 && escapeCooldown == 0 && abs(headingErr) > 40 && dist > 25.0) {
        targetVelL = 0;
        targetVelR = 0;
        robotState = ROTATE_TO_TARGET;
        Serial.println("RE-ROTATE (no obstacle, heading off)");
        break;
      }

      // Kurangi kecepatan saat heading error besar (obstacle avoidance)
      float headingFactor = 1.0;
      if (abs(headingErr) > 15.0) {
        headingFactor = constrain(1.0 - (abs(headingErr) - 15.0) / 90.0, 0.25, 1.0);
      }

      // ======================================================
      // PID HEADING (OUTER LOOP)
      // ======================================================
      // [Persamaan 2.24] Pendekatan integral diskrit (akumulasi error x sampling time dt)
      headingIntegral += headingErr * dt;
      headingIntegral = constrain(headingIntegral, -MAX_HEADING_INTEGRAL, MAX_HEADING_INTEGRAL);

      static float filteredDerivative = 0.0;
      // [Persamaan 2.25] Pendekatan turunan diskrit (selisih error dibagi sampling time dt)
      float rawDerivative = (headingErr - prevHeadingErr) / dt;
      filteredDerivative = 0.15f * rawDerivative + 0.85f * filteredDerivative;
      float derivative = filteredDerivative;
      prevHeadingErr = headingErr;

      // [Persamaan 2.26] Sinyal kendali digital PID Heading (mengoreksi kesalahan arah)
      float correction =
        (Kp_move * headingErr) +
        (Ki_move * headingIntegral) +
        (Kd_move * derivative);
      correction = constrain(correction, -18.0, 18.0); // Batasi kecepatan koreksi belok agar tidak berputar di tempat (chattering prevention)

      // ======================================================
      // DYNAMIC SPEED PROFILE
      // ======================================================
      float targetVel;
      if (dist > SLOWDOWN_DIST) {
        targetVel = MAX_MOVE_VEL;
      } else {
        targetVel = mapFloat(dist, 0, SLOWDOWN_DIST, MIN_MOVE_VEL, MAX_MOVE_VEL);
      }

      // Ramping akselerasi
      unsigned long elapsed = millis() - moveStartTime;
      float rampFactor = 1.0;
      if (elapsed < RAMP_UP_MS) {
        rampFactor = (float)elapsed / RAMP_UP_MS;
      }

      float baseVel = MIN_MOVE_VEL + (targetVel - MIN_MOVE_VEL) * rampFactor * headingFactor;

      // Target kecepatan fisik roda (cm/s)
      targetVelL = constrain(baseVel - correction, -50.0, 50.0);
      targetVelR = constrain(baseVel + correction, -50.0, 50.0);

      // Debug APF periodik (setiap 200ms)
      static unsigned long lastPrintMoveTime = 0;
      if (millis() - lastPrintMoveTime >= 200) {
        lastPrintMoveTime = millis();
        // R4: Diagnostik lengkap dengan status escape
        char escChar = (latchedEscapeDir > 0.0) ? 'L' : (latchedEscapeDir < 0.0) ? 'R' : '-';
        Serial.printf("MOVE WP:%d d:%.1f hErr:%.1f Fatt:(%.2f,%.2f) Frep:(%.2f,%.2f) hF:%.2f VL:%.1f VR:%.1f ESC:%c CD:%d\n",
          currentWaypoint, dist, headingErr, F_att_x, F_att_y, F_rep_x, F_rep_y, headingFactor, targetVelL, targetVelR, escChar, escapeCooldown);
      }

      break;
    }

    case FINISHED: {
      targetVelL = 0;
      targetVelR = 0;
      break;
    }

    case PAUSED_MEASURE: {
      targetVelL = 0;
      targetVelR = 0;
      break;
    }
  }

  // ============================================================
  // INNER LOOP: VELOCITY PI CONTROL (PID 2 & 3)
  // ============================================================
  if (robotState == IDLE || robotState == FINISHED || robotState == PAUSED_MEASURE) {
    stopMotor();
    velIntegralL = 0;
    velIntegralR = 0;
    prevVelErrL = 0;
    prevVelErrR = 0;
  } else {
    // PI Roda Kiri
    float velErrL = targetVelL - velL_act;
    velIntegralL += velErrL * dt;
    velIntegralL = constrain(velIntegralL, -200.0 / Ki_vel, 200.0 / Ki_vel);
    float velDerivL = (velErrL - prevVelErrL) / dt;
    prevVelErrL = velErrL;

    float pwmL = (Kp_vel * velErrL) + (Ki_vel * velIntegralL) + (Kd_vel * velDerivL);

    // PI Roda Kanan
    float velErrR = targetVelR - velR_act;
    velIntegralR += velErrR * dt;
    velIntegralR = constrain(velIntegralR, -200.0 / Ki_vel, 200.0 / Ki_vel);
    float velDerivR = (velErrR - prevVelErrR) / dt;
    prevVelErrR = velErrR;

    float pwmR = (Kp_vel * velErrR) + (Ki_vel * velIntegralR) + (Kd_vel * velDerivR);

    setMotorPWM(pwmL, pwmR);
  }
}

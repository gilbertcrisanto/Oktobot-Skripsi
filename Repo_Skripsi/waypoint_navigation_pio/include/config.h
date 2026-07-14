#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>

// ============================================================
// PIN ALOKASI ESP32 DevKit V1
// ============================================================

// --- Motor Driver BTS7960 ---
#define L_RPWM  16  // PWM Maju  Kiri
#define L_LPWM  17  // PWM Mundur Kiri
#define L_EN     5  // Enable Kiri  (L_REN + L_LEN)
#define R_RPWM  18  // PWM Maju  Kanan
#define R_LPWM  19  // PWM Mundur Kanan
#define R_EN    21  // Enable Kanan (R_REN + R_LEN)

// --- Encoder JGA25 ---
#define ENCA_L  34
#define ENCB_L  35
#define ENCA_R  36
#define ENCB_R  39

// --- 8× HY-SRF05 Mode 1-Pin ---
extern const int SENSOR_PINS[8];

// ============================================================
// ESP32 LEDC PWM (20 kHz — Motor Hening)
// ============================================================
const int PWM_FREQ = 20000;  // 20 kHz
const int PWM_RES  = 8;      // 8-bit resolusi (0–255)

// ============================================================
// SENSOR ULTRASONIK 360° (8 ARAH OKTAGONAL)
// ============================================================
extern const float SENSOR_ANGLES[8];
extern const char* SENSOR_NAMES[8];

// ============================================================
// WAYPOINT & STATE MACHINE
// ============================================================
struct Waypoint { float x, y; };

const int MAX_WAYPOINTS = 30;

enum State {
  IDLE,
  ROTATE_TO_TARGET,
  WAIT_AFTER_ROTATE,
  MOVE_TO_TARGET,
  FINISHED,
  MANUAL_CONTROL,  // Kendali manual via Joystick/WASD
  PAUSED_MEASURE   // Jeda untuk pengukuran fisik (step mode)
};

// ============================================================
// CONSTANTS
// ============================================================
const float MAX_HEADING_INTEGRAL = 1000.0;
const float RAMP_UP_MS = 1000;

// ============================================================
// I2C, MPU9250 & OLED CONFIGURATION
// ============================================================
#define I2C_SDA_PIN 32
#define I2C_SCL_PIN 33

#define SCR_W 128
#define SCR_H 64

#define MPU_ADDR_PRI  0x68
#define MPU_ADDR_SEC  0x69
#define MPU_WHO_AM_I  0x75
#define MPU_PWR1      0x6B
#define MPU_ACCEL_H   0x3B

#define AK_ADDR   0x0C
#define AK_WIA    0x00
#define AK_ST1    0x02
#define AK_HXL    0x03
#define AK_CNTL1  0x0A
#define AK_ASAX   0x10

#endif // CONFIG_H


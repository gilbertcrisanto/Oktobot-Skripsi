#ifndef GLOBALS_H
#define GLOBALS_H

#include "config.h"
#include <WebServer.h>
#include <Preferences.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>


// ============================================================
// WIFI & WEB SERVER
// ============================================================
extern const char* ssid;
extern const char* password;
extern WebServer server;
extern Preferences preferences;

// ============================================================
// ENCODER & MUTEX
// ============================================================
extern portMUX_TYPE encoderMux;
extern volatile long encL;
extern volatile long encR;

// ============================================================
// SENSOR ULTRASONIK
// ============================================================
extern float sensorDist[8];

// ============================================================
// APF PARAMETERS
// ============================================================
extern float K_ATT;
extern float R_GOAL;
extern float S_GOAL;

extern float K_REP;
extern float R_OBS;
extern float S_OBS;
extern float OBSTACLE_THRESHOLD;

extern bool  useAPF;
extern float latchedEscapeDir;
extern int   frontClearCount;
extern float escapeStartX;
extern float escapeStartY;
extern float obstacleEstX;
extern float obstacleEstY;
extern int   escapeCooldown;

// ============================================================
// PHYSICAL PARAMETERS
// ============================================================
extern float PPR_L;
extern float PPR_R;
extern float WHEEL_D;
extern float WHEEL_CIRC;
extern float WHEEL_BASE;

// ============================================================
// ODOMETRY & VELOCITY
// ============================================================
extern float posX;
extern float posY;
extern float theta;

extern long  prevEncL;
extern long  prevEncR;

extern float velL_act;
extern float velR_act;
extern float VEL_FILTER_ALPHA;

extern float targetVelL;
extern float targetVelR;

// ============================================================
// WAYPOINTS & STATE
// ============================================================
extern Waypoint path[MAX_WAYPOINTS];
extern int totalWaypoints;
extern int currentWaypoint;

extern State robotState;
extern bool  stepModeActive;
extern State nextStateAfterPause;

// ============================================================
// ROTATION CONTROL
// ============================================================
extern float Kp_rot_vel;
extern float MIN_ROT_VEL;
extern float MAX_ROT_VEL;
extern float ROTATE_DONE_DEG;
extern float ROTATE_DEAD_DEG;
extern float ROTATE_BRAKE_DEG;

// ============================================================
// HEADING PID
// ============================================================
extern float Kp_move;
extern float Ki_move;
extern float Kd_move;

extern float headingIntegral;
extern float prevHeadingErr;

// ============================================================
// VELOCITY PI
// ============================================================
extern float Kp_vel;
extern float Ki_vel;
extern float Kd_vel;

extern float velIntegralL;
extern float prevVelErrL;
extern float velIntegralR;
extern float prevVelErrR;

// ============================================================
// SPEED PROFILE
// ============================================================
extern float MIN_MOVE_VEL;
extern float MAX_MOVE_VEL;
extern float SLOWDOWN_DIST;
extern float MOVE_DONE_CM;

// ============================================================
// TIMING
// ============================================================
extern unsigned long lastLoopTime;
extern unsigned long LOOP_MS;
extern unsigned long waitStartTime;
extern unsigned long WAIT_MS;
extern unsigned long moveStartTime;
extern unsigned long lastManualCmdTime;

// ============================================================
// I2C / MPU9250 / OLED
// ============================================================
extern Adafruit_SSD1306 display;
extern bool mpuReady;
extern uint8_t mpuAddr;
extern bool akReady;
extern float magCal[3];
extern float imuYaw;
extern unsigned long lastImuT;
extern float gyroBias[3];
extern bool gyroCalibrated;
extern float lastAx, lastAy, lastAz;
extern float lastGx, lastGy, lastGz;
extern float lastMx, lastMy, lastMz, lastTemp;
extern bool lastMagOk;
extern bool imuDataOk;

extern bool oledReady;
extern uint8_t oledAddr;
extern unsigned long oledCnt;

extern bool useIMUHeading;
extern float COMP_FILTER_ALPHA;

#endif // GLOBALS_H


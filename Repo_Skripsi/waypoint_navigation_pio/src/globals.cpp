#include "globals.h"

// ============================================================
// WIFI & WEB SERVER
// ============================================================
const char* ssid     = "OKTO-BOT-AP";
const char* password = "password123";  // Minimal 8 karakter
WebServer server(80);
Preferences preferences;

// ============================================================
// ENCODER & MUTEX
// ============================================================
portMUX_TYPE encoderMux = portMUX_INITIALIZER_UNLOCKED;
volatile long encL = 0;
volatile long encR = 0;

// ============================================================
// SENSOR ULTRASONIK
// ============================================================
float sensorDist[8] = {999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0, 999.0};

const float SENSOR_ANGLES[8] = {
   0.0,              //  0: DEPAN          (  0°)
  -PI / 4.0,         //  1: DEPAN-KANAN    (-45°)
  -PI / 2.0,         //  2: KANAN          (-90°)
  -3.0 * PI / 4.0,   //  3: BELAKANG-KANAN (-135°)
   PI,               //  4: BELAKANG       (180°)
   3.0 * PI / 4.0,   //  5: BELAKANG-KIRI  (135°)
   PI / 2.0,         //  6: KIRI           ( 90°)
   PI / 4.0          //  7: DEPAN-KIRI     ( 45°)
};

const char* SENSOR_NAMES[8] = {
  "DEP", "D-KAN", "KAN", "B-KAN", "BEL", "B-KIR", "KIR", "D-KIR"
};

const int SENSOR_PINS[8] = {22, 23, 25, 26, 4, 13, 14, 15};

// ============================================================
// APF PARAMETERS
// ============================================================
float K_ATT  = 1.5;   // α: konstanta penguat medan atraktif
float R_GOAL = 2.0;   // r: jari-jari zona tujuan (cm)
float S_GOAL = 20.0;  // s: faktor penyebaran tujuan (cm)

float K_REP  = 15.0;  // β: konstanta penguat gaya repulsif
float R_OBS  = 10.0;  // r: radius pengaruh obstacle (cm)
float S_OBS  = 30.0;  // s: penyebaran gaya repulsif (cm)
float OBSTACLE_THRESHOLD = R_OBS + S_OBS;  // Total jarak pengaruh (40.0 cm)

bool  useAPF          = true;   // Status aktif APF (ubah via Serial/Web)
float latchedEscapeDir = 0.0;   // 0.0: none | +1.0: kiri | -1.0: kanan (histeresis)
int   frontClearCount  = 0;     // Debounce counter untuk reset latch escape
float escapeStartX     = 0.0;   // Posisi X awal manuver hindar (cegah turn-in dini)
float escapeStartY     = 0.0;   // Posisi Y awal manuver hindar
float obstacleEstX     = 0.0;   // [R1] Estimasi posisi X obstacle saat pertama terdeteksi
float obstacleEstY     = 0.0;   // [R1] Estimasi posisi Y obstacle saat pertama terdeteksi
int   escapeCooldown   = 0;     // [R2] Cooldown setelah latch lepas (blokir RE-ROTATE)

// ============================================================
// PHYSICAL PARAMETERS
// ============================================================
float PPR_L      = 940.0;          // Pulses Per Revolution roda kiri
float PPR_R      = 925.0;          // Pulses Per Revolution roda kanan
float WHEEL_D    = 6.8;            // Diameter roda (cm)
float WHEEL_CIRC = PI * WHEEL_D;   // Keliling roda — di-update saat WHEEL_D berubah
float WHEEL_BASE = 27.4;           // Jarak antar roda (cm)

// ============================================================
// ODOMETRY & VELOCITY
// ============================================================
float posX  = 0;
float posY  = 0;
float theta = PI / 2.0;  // Orientasi awal: menghadap +Y (90°)

long  prevEncL = 0;
long  prevEncR = 0;

float velL_act = 0;
float velR_act = 0;
float VEL_FILTER_ALPHA = 0.25;  // Konstanta LPF untuk kecepatan — tunable

float targetVelL = 0;
float targetVelR = 0;

// ============================================================
// WAYPOINTS & STATE
// ============================================================
Waypoint path[MAX_WAYPOINTS];
int totalWaypoints  = 0;
int currentWaypoint = 0;

State robotState         = IDLE;
bool  stepModeActive     = false;
State nextStateAfterPause = IDLE;

// ============================================================
// ROTATION CONTROL
// ============================================================
float Kp_rot_vel      = 0.8;
float MIN_ROT_VEL     = 4.0;
float MAX_ROT_VEL     = 20.0;
float ROTATE_DONE_DEG  = 4.0;
float ROTATE_DEAD_DEG  = 2.0;
float ROTATE_BRAKE_DEG = 25.0;

// ============================================================
// HEADING PID
// ============================================================
float Kp_move = 0.35;
float Ki_move = 0.001;
float Kd_move = 0.01;

float headingIntegral = 0;
float prevHeadingErr  = 0;

// ============================================================
// VELOCITY PI
// ============================================================
float Kp_vel = 3.0;  // Kp kecepatan roda — tunable via Web
float Ki_vel = 0.8;  // Ki kecepatan roda — tunable via Web
float Kd_vel = 0.0;

float velIntegralL = 0, prevVelErrL = 0;
float velIntegralR = 0, prevVelErrR = 0;

// ============================================================
// SPEED PROFILE
// ============================================================
float MIN_MOVE_VEL  = 8.0;
float MAX_MOVE_VEL  = 22.0;  // Dapat diubah via Serial/Web
float SLOWDOWN_DIST = 20.0;
float MOVE_DONE_CM  = 2.0;

// ============================================================
// TIMING
// ============================================================
unsigned long lastLoopTime     = 0;
unsigned long LOOP_MS          = 20;
unsigned long waitStartTime    = 0;
unsigned long WAIT_MS          = 300;
unsigned long moveStartTime    = 0;
unsigned long lastManualCmdTime = 0;  // Watchdog kendali manual

// ============================================================
// I2C / MPU9250 / OLED
// ============================================================
Adafruit_SSD1306 display(SCR_W, SCR_H, &Wire, -1);
bool mpuReady = false;
uint8_t mpuAddr = MPU_ADDR_PRI;
bool akReady = false;
float magCal[3] = {1.0f, 1.0f, 1.0f};
float imuYaw = 0.0f;
unsigned long lastImuT = 0;
float gyroBias[3] = {0.0f, 0.0f, 0.0f};
bool gyroCalibrated = false;
float lastAx = 0.0f, lastAy = 0.0f, lastAz = 0.0f;
float lastGx = 0.0f, lastGy = 0.0f, lastGz = 0.0f;
float lastMx = 0.0f, lastMy = 0.0f, lastMz = 0.0f, lastTemp = 0.0f;
bool lastMagOk = false;
bool imuDataOk = false;

bool oledReady = false;
uint8_t oledAddr = 0x3C;
unsigned long oledCnt = 0;

bool useIMUHeading = false;
float COMP_FILTER_ALPHA = 0.98f;



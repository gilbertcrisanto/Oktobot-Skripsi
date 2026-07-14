#include "nvs_storage.h"
#include "globals.h"

void loadStoredParameters() {
  preferences.begin("oktobot", true);  // Namespace "oktobot", mode read-only

  // Parameter fisik
  PPR_L      = preferences.getFloat("ppr_l",     PPR_L);
  PPR_R      = preferences.getFloat("ppr_r",     PPR_R);
  WHEEL_D    = preferences.getFloat("wheel_d",   WHEEL_D);
  WHEEL_CIRC = PI * WHEEL_D;  // Hitung ulang keliling roda
  WHEEL_BASE = preferences.getFloat("wheel_base", WHEEL_BASE);

  // APF
  K_ATT  = preferences.getFloat("k_att",   K_ATT);
  K_REP  = preferences.getFloat("k_rep",   K_REP);
  R_GOAL = preferences.getFloat("r_goal",  R_GOAL);
  S_GOAL = preferences.getFloat("s_goal",  S_GOAL);
  R_OBS  = preferences.getFloat("r_obs",   R_OBS);
  S_OBS  = preferences.getFloat("s_obs",   S_OBS);
  OBSTACLE_THRESHOLD = R_OBS + S_OBS;  // Hitung ulang threshold
  useAPF = preferences.getBool("use_apf", useAPF);

  // Kontrol rotasi
  Kp_rot_vel      = preferences.getFloat("kp_rot",   Kp_rot_vel);
  MIN_ROT_VEL     = preferences.getFloat("min_rot",  MIN_ROT_VEL);
  MAX_ROT_VEL     = preferences.getFloat("max_rot",  MAX_ROT_VEL);
  ROTATE_DONE_DEG = preferences.getFloat("rot_done", ROTATE_DONE_DEG);
  ROTATE_DEAD_DEG = preferences.getFloat("rot_dead", ROTATE_DEAD_DEG);
  ROTATE_BRAKE_DEG= preferences.getFloat("rot_brake",ROTATE_BRAKE_DEG);

  // Heading PID
  Kp_move = preferences.getFloat("kp_move", Kp_move);
  Ki_move = preferences.getFloat("ki_move", Ki_move);
  Kd_move = preferences.getFloat("kd_move", Kd_move);

  // Velocity PI (inner loop)
  Kp_vel = preferences.getFloat("kp_vel", Kp_vel);
  Ki_vel = preferences.getFloat("ki_vel", Ki_vel);

  // Filter & batas kecepatan
  VEL_FILTER_ALPHA = preferences.getFloat("vel_alpha", VEL_FILTER_ALPHA);
  MIN_MOVE_VEL     = preferences.getFloat("min_vel",   MIN_MOVE_VEL);
  MAX_MOVE_VEL     = preferences.getFloat("max_vel",   MAX_MOVE_VEL);
  SLOWDOWN_DIST    = preferences.getFloat("slowdown",  SLOWDOWN_DIST);
  MOVE_DONE_CM     = preferences.getFloat("move_done", MOVE_DONE_CM);

  // Pewaktuan
  WAIT_MS = preferences.getUInt("wait_ms", WAIT_MS);
  LOOP_MS = preferences.getUInt("loop_ms", LOOP_MS);

  // IMU MPU9250
  useIMUHeading  = preferences.getBool("use_imu_th", useIMUHeading);
  COMP_FILTER_ALPHA = preferences.getFloat("comp_alpha", COMP_FILTER_ALPHA);
  gyroBias[0]    = preferences.getFloat("gyro_bias_x", gyroBias[0]);
  gyroBias[1]    = preferences.getFloat("gyro_bias_y", gyroBias[1]);
  gyroBias[2]    = preferences.getFloat("gyro_bias_z", gyroBias[2]);
  gyroCalibrated = preferences.getBool("gyro_cal", gyroCalibrated);

  preferences.end();
  Serial.println(">>> Stored parameters loaded successfully from NVS!");
}

void saveAllParameters() {
  preferences.begin("oktobot", false);  // Namespace "oktobot", mode read-write

  // Parameter fisik
  preferences.putFloat("ppr_l",      PPR_L);
  preferences.putFloat("ppr_r",      PPR_R);
  preferences.putFloat("wheel_d",    WHEEL_D);
  preferences.putFloat("wheel_base", WHEEL_BASE);

  // APF
  preferences.putFloat("k_att",   K_ATT);
  preferences.putFloat("k_rep",   K_REP);
  preferences.putFloat("r_goal",  R_GOAL);
  preferences.putFloat("s_goal",  S_GOAL);
  preferences.putFloat("r_obs",   R_OBS);
  preferences.putFloat("s_obs",   S_OBS);
  preferences.putBool("use_apf",  useAPF);

  // Kontrol rotasi
  preferences.putFloat("kp_rot",   Kp_rot_vel);
  preferences.putFloat("min_rot",  MIN_ROT_VEL);
  preferences.putFloat("max_rot",  MAX_ROT_VEL);
  preferences.putFloat("rot_done", ROTATE_DONE_DEG);
  preferences.putFloat("rot_dead", ROTATE_DEAD_DEG);
  preferences.putFloat("rot_brake",ROTATE_BRAKE_DEG);

  // Heading PID
  preferences.putFloat("kp_move", Kp_move);
  preferences.putFloat("ki_move", Ki_move);
  preferences.putFloat("kd_move", Kd_move);

  // Velocity PI (inner loop)
  preferences.putFloat("kp_vel", Kp_vel);
  preferences.putFloat("ki_vel", Ki_vel);

  // Filter & batas kecepatan
  preferences.putFloat("vel_alpha", VEL_FILTER_ALPHA);
  preferences.putFloat("min_vel",   MIN_MOVE_VEL);
  preferences.putFloat("max_vel",   MAX_MOVE_VEL);
  preferences.putFloat("slowdown",  SLOWDOWN_DIST);
  preferences.putFloat("move_done", MOVE_DONE_CM);

  // Pewaktuan
  preferences.putUInt("wait_ms", WAIT_MS);
  preferences.putUInt("loop_ms", LOOP_MS);

  // IMU MPU9250
  preferences.putBool("use_imu_th", useIMUHeading);
  preferences.putFloat("comp_alpha", COMP_FILTER_ALPHA);
  preferences.putFloat("gyro_bias_x", gyroBias[0]);
  preferences.putFloat("gyro_bias_y", gyroBias[1]);
  preferences.putFloat("gyro_bias_z", gyroBias[2]);
  preferences.putBool("gyro_cal", gyroCalibrated);

  preferences.end();
  Serial.println(">>> Parameters saved permanently to NVS!");
}

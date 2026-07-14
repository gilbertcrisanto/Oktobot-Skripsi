#include "odometry.h"
#include "globals.h"
#include <math.h>

void updateOdometry(float dt) {
  long eL, eR;

  portENTER_CRITICAL(&encoderMux);
  eL = encL;
  eR = encR;
  portEXIT_CRITICAL(&encoderMux);

  long dL_tick = eL - prevEncL;
  long dR_tick = eR - prevEncR;

  prevEncL = eL;
  prevEncR = eR;

  // [Persamaan 2.8 & 2.9] Jarak tempuh roda kiri/kanan berdasarkan resolusi encoder dan keliling roda (Persamaan 2.7)
  float dL = (dL_tick / PPR_L) * WHEEL_CIRC;
  float dR = (dR_tick / PPR_R) * WHEEL_CIRC;

  float raw_velL = (dt > 0) ? dL / dt : 0;
  float raw_velR = (dt > 0) ? dR / dt : 0;

  velL_act = (VEL_FILTER_ALPHA * raw_velL) + ((1.0 - VEL_FILTER_ALPHA) * velL_act);
  velR_act = (VEL_FILTER_ALPHA * raw_velR) + ((1.0 - VEL_FILTER_ALPHA) * velR_act);

  // [Persamaan 2.2] Kecepatan linear rata-rata robot (dC mewakili delta linear distance)
  float dC = (dL + dR) / 2.0;
  // [Persamaan 2.6 / 2.3] Perubahan sudut (heading) berdasarkan selisih jarak roda dan wheelbase
  float dTheta = (dR - dL) / WHEEL_BASE;

  float thetaMid;

  if (mpuReady && gyroCalibrated && useIMUHeading) {
    float dTheta_gyro_z = radians(lastGz) * dt;
    
    // Complementary Filter: blend gyro Z rate and encoder rate
    float dTheta_fused = (COMP_FILTER_ALPHA * dTheta_gyro_z) + ((1.0f - COMP_FILTER_ALPHA) * dTheta);
    
    thetaMid = theta + (dTheta_fused / 2.0f);
    posX += dC * cos(thetaMid);
    posY += dC * sin(thetaMid);
    theta += dTheta_fused;
  } else {
    thetaMid = theta + (dTheta / 2.0f);
    posX += dC * cos(thetaMid);
    posY += dC * sin(thetaMid);
    theta += dTheta;
  }

  // [Persamaan 2.10] Normalisasi arah hadap (theta) agar berada di rentang [-PI, PI]
  while (theta > PI)  theta -= 2.0 * PI;
  while (theta < -PI) theta += 2.0 * PI;
}

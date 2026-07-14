#ifndef MOTORS_H
#define MOTORS_H

#include <Arduino.h>
#include "config.h"

// Deklarasi fungsi motor
void setMotorPWM(float left, float right);
void stopMotor();
void activeBrake();
void enableDrivers();
void disableDrivers();

// Deklarasi ISR Encoder
void IRAM_ATTR isrLA();
void IRAM_ATTR isrLB();
void IRAM_ATTR isrRA();
void IRAM_ATTR isrRB();

#endif // MOTORS_H

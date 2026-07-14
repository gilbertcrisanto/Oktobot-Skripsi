#ifndef SENSORS_H
#define SENSORS_H

#include <Arduino.h>

void readNextSensorRoundRobin();

// --- MPU9250 ---
bool initMpu9250();
bool readMPU9250();
void calGyro();

// --- OLED ---
bool initOled();
void updateOledDisplay();

#endif // SENSORS_H


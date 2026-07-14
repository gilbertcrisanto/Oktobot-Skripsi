#ifndef NAVIGATION_H
#define NAVIGATION_H

#include <Arduino.h>

void runNavigationStateMachine(float dt);
float normalizeDeg(float a);
float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);

#endif // NAVIGATION_H

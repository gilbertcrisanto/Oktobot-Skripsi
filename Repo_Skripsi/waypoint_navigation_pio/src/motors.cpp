#include "motors.h"
#include "globals.h"

void setMotorPWM(float left, float right) {
  left  = constrain(left,  -255, 255);
  right = constrain(right, -255, 255);

  if (left >= 0) {
    ledcWrite(L_RPWM, (int)left);
    ledcWrite(L_LPWM, 0);
  } else {
    ledcWrite(L_RPWM, 0);
    ledcWrite(L_LPWM, (int)(-left));
  }

  if (right >= 0) {
    ledcWrite(R_RPWM, (int)right);
    ledcWrite(R_LPWM, 0);
  } else {
    ledcWrite(R_RPWM, 0);
    ledcWrite(R_LPWM, (int)(-right));
  }
}

void stopMotor() {
  ledcWrite(L_RPWM, 0);
  ledcWrite(L_LPWM, 0);
  ledcWrite(R_RPWM, 0);
  ledcWrite(R_LPWM, 0);
}

void activeBrake() {
  targetVelL = 0;
  targetVelR = 0;
  velIntegralL = 0;
  velIntegralR = 0;
  prevVelErrL = 0;
  prevVelErrR = 0;

  setMotorPWM(-50, -50);
  delay(30);
  stopMotor();
}

void enableDrivers() {
  digitalWrite(L_EN, HIGH);
  digitalWrite(R_EN, HIGH);
}

void disableDrivers() {
  digitalWrite(L_EN, LOW);
  digitalWrite(R_EN, LOW);
}

void IRAM_ATTR isrLA() {
  bool A = digitalRead(ENCA_L);
  bool B = digitalRead(ENCB_L);
  portENTER_CRITICAL_ISR(&encoderMux);
  if (A == B) encL++; else encL--;
  portEXIT_CRITICAL_ISR(&encoderMux);
}

void IRAM_ATTR isrLB() {
  bool A = digitalRead(ENCA_L);
  bool B = digitalRead(ENCB_L);
  portENTER_CRITICAL_ISR(&encoderMux);
  if (A != B) encL++; else encL--;
  portEXIT_CRITICAL_ISR(&encoderMux);
}

void IRAM_ATTR isrRA() {
  bool A = digitalRead(ENCA_R);
  bool B = digitalRead(ENCB_R);
  portENTER_CRITICAL_ISR(&encoderMux);
  if (A == B) encR++; else encR--;
  portEXIT_CRITICAL_ISR(&encoderMux);
}

void IRAM_ATTR isrRB() {
  bool A = digitalRead(ENCA_R);
  bool B = digitalRead(ENCB_R);
  portENTER_CRITICAL_ISR(&encoderMux);
  if (A != B) encR++; else encR--;
  portEXIT_CRITICAL_ISR(&encoderMux);
}

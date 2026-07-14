#ifndef COMMANDS_H
#define COMMANDS_H

#include <Arduino.h>

void handleSerial();
void processCommand(String input);
void printStatus();
void printHelpMenu();
void printSensorReadings();

#endif // COMMANDS_H

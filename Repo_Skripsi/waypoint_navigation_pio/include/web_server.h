#ifndef WEB_SERVER_H
#define WEB_SERVER_H

#include <Arduino.h>

void setupWebServer();
void handleRoot();
String buildStatusJson();
void handleStatus();
void handleStatusLive();
void handleCmdEndpoint();
void handleGetParams();
void handleSetParam();
void handleNotFound();

#endif // WEB_SERVER_H

#include "web_server.h"
#include "globals.h"
#include "nvs_storage.h"
#include "commands.h"
#include "web_dashboard.h"

void handleRoot() {
  server.send_P(200, "text/html", index_html);
}

String buildStatusJson() {
  String stateStr = "IDLE";
  switch (robotState) {
    case IDLE:              stateStr = "IDLE"; break;
    case ROTATE_TO_TARGET:  stateStr = "ROTATE"; break;
    case WAIT_AFTER_ROTATE: stateStr = "WAIT"; break;
    case MOVE_TO_TARGET:    stateStr = "MOVE"; break;
    case FINISHED:          stateStr = "FINISHED"; break;
    case MANUAL_CONTROL:    stateStr = "MANUAL"; break;
    case PAUSED_MEASURE:    stateStr = "PAUSED"; break;
  }

  String json = "{";
  json += "\"state\":\"" + stateStr + "\",";
  json += "\"x\":" + String(posX, 2) + ",";
  json += "\"y\":" + String(posY, 2) + ",";
  json += "\"th\":" + String(degrees(theta), 2) + ",";
  json += "\"velL\":" + String(velL_act, 2) + ",";
  json += "\"velR\":" + String(velR_act, 2) + ",";
  json += "\"targetVelL\":" + String(targetVelL, 2) + ",";
  json += "\"targetVelR\":" + String(targetVelR, 2) + ",";
  json += "\"wp\":" + String(currentWaypoint) + ",";
  json += "\"total\":" + String(totalWaypoints) + ",";
  json += "\"apf\":" + String(useAPF ? "true" : "false") + ",";
  json += "\"krep\":" + String(K_REP, 2) + ",";
  json += "\"speedMax\":" + String(MAX_MOVE_VEL, 2) + ",";
  json += "\"millis\":" + String(millis()) + ",";
  json += "\"use_imu_th\":" + String(useIMUHeading ? "true" : "false") + ",";
  json += "\"comp_alpha\":" + String(COMP_FILTER_ALPHA, 4) + ",";
  json += "\"imuYaw\":" + String(imuYaw, 2) + ",";
  json += "\"imuReady\":" + String(mpuReady ? "true" : "false") + ",";
  json += "\"gyroCal\":" + String(gyroCalibrated ? "true" : "false") + ",";
  json += "\"sensors\":[";
  for (int i = 0; i < 8; i++) {
    json += String(sensorDist[i], 2);
    if (i < 7) json += ",";
  }
  json += "]";
  json += "}";

  return json;
}

void handleStatus() {
  server.sendHeader("Cache-Control", "no-store");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", buildStatusJson());
}

void handleStatusLive() {
  const char page[] PROGMEM = R"rawliteral(
<!doctype html>
<html lang="id">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width,initial-scale=1">
<title>OKTO-BOT Status Live</title>
<style>
body{margin:0;background:#0b1117;color:#dbeafe;font-family:Consolas,monospace;padding:18px}
h1{font-size:18px;margin:0 0 12px;color:#67e8f9}
.row{display:flex;gap:10px;flex-wrap:wrap;margin-bottom:12px}
.card{border:1px solid #1f3b4d;background:#101923;border-radius:6px;padding:10px 12px;min-width:120px}
.label{font-size:11px;color:#7dd3fc;text-transform:uppercase;letter-spacing:1px}.value{font-size:22px;color:#f8fafc}
pre{white-space:pre-wrap;border:1px solid #1f3b4d;background:#05080c;border-radius:6px;padding:12px;min-height:220px}
small{color:#94a3b8}
</style>
</head>
<body>
<h1>OKTO-BOT /status-live</h1>
<div class="row">
  <div class="card"><div class="label">State</div><div class="value" id="state">-</div></div>
  <div class="card"><div class="label">X</div><div class="value" id="x">-</div></div>
  <div class="card"><div class="label">Y</div><div class="value" id="y">-</div></div>
  <div class="card"><div class="label">Heading</div><div class="value" id="th">-</div></div>
  <div class="card"><div class="label">Update</div><div class="value" id="count">0</div></div>
</div>
<small>Auto update dari /status setiap 200 ms. Untuk data mentah gunakan http://192.168.4.1/status</small>
<pre id="raw">Menunggu data...</pre>
<script>
let n=0;
async function tick(){
  try{
    const res=await fetch('/status?ts='+Date.now(),{cache:'no-store'});
    const d=await res.json();
    n++;
    document.getElementById('state').textContent=d.state;
    document.getElementById('x').textContent=Number(d.x).toFixed(2);
    document.getElementById('y').textContent=Number(d.y).toFixed(2);
    document.getElementById('th').textContent=Number(d.th).toFixed(1);
    document.getElementById('count').textContent=n;
    document.getElementById('raw').textContent=JSON.stringify(d,null,2);
  }catch(e){
    document.getElementById('raw').textContent='Koneksi gagal: '+e;
  }
}
setInterval(tick,200);
tick();
</script>
</body>
</html>
)rawliteral";
  server.sendHeader("Cache-Control", "no-store");
  server.send_P(200, "text/html", page);
}

void handleCmdEndpoint() {
  if (server.hasArg("v")) {
    String cmd = server.arg("v");
    cmd.trim();
    if (cmd.length() > 0) {
      processCommand(cmd);
      server.sendHeader("Access-Control-Allow-Origin", "*");
      server.send(200, "text/plain", "OK");
      return;
    }
  }
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(400, "text/plain", "Bad Request");
}

void handleGetParams() {
  String json = "{";
  json += "\"PPR_L\":" + String(PPR_L, 2) + ",";
  json += "\"PPR_R\":" + String(PPR_R, 2) + ",";
  json += "\"WHEEL_D\":" + String(WHEEL_D, 3) + ",";
  json += "\"WHEEL_BASE\":" + String(WHEEL_BASE, 3) + ",";
  json += "\"apf\":" + String(useAPF ? "true" : "false") + ",";
  json += "\"K_ATT\":" + String(K_ATT, 3) + ",";
  json += "\"K_REP\":" + String(K_REP, 3) + ",";
  json += "\"R_GOAL\":" + String(R_GOAL, 3) + ",";
  json += "\"S_GOAL\":" + String(S_GOAL, 3) + ",";
  json += "\"R_OBS\":" + String(R_OBS, 3) + ",";
  json += "\"S_OBS\":" + String(S_OBS, 3) + ",";
  json += "\"Kp_rot_vel\":" + String(Kp_rot_vel, 3) + ",";
  json += "\"MIN_ROT_VEL\":" + String(MIN_ROT_VEL, 3) + ",";
  json += "\"MAX_ROT_VEL\":" + String(MAX_ROT_VEL, 3) + ",";
  json += "\"ROTATE_DONE_DEG\":" + String(ROTATE_DONE_DEG, 3) + ",";
  json += "\"ROTATE_DEAD_DEG\":" + String(ROTATE_DEAD_DEG, 3) + ",";
  json += "\"ROTATE_BRAKE_DEG\":" + String(ROTATE_BRAKE_DEG, 3) + ",";
  json += "\"Kp_move\":" + String(Kp_move, 3) + ",";
  json += "\"Ki_move\":" + String(Ki_move, 5) + ",";
  json += "\"Kd_move\":" + String(Kd_move, 5) + ",";
  json += "\"Kp_vel\":" + String(Kp_vel, 3) + ",";
  json += "\"Ki_vel\":" + String(Ki_vel, 3) + ",";
  json += "\"VEL_FILTER_ALPHA\":" + String(VEL_FILTER_ALPHA, 3) + ",";
  json += "\"MIN_MOVE_VEL\":" + String(MIN_MOVE_VEL, 3) + ",";
  json += "\"MAX_MOVE_VEL\":" + String(MAX_MOVE_VEL, 3) + ",";
  json += "\"SLOWDOWN_DIST\":" + String(SLOWDOWN_DIST, 3) + ",";
  json += "\"MOVE_DONE_CM\":" + String(MOVE_DONE_CM, 3) + ",";
  json += "\"WAIT_MS\":" + String(WAIT_MS) + ",";
  json += "\"LOOP_MS\":" + String(LOOP_MS) + ",";
  json += "\"use_imu_th\":" + String(useIMUHeading ? "true" : "false") + ",";
  json += "\"comp_alpha\":" + String(COMP_FILTER_ALPHA, 4);
  json += "}";

  server.sendHeader("Cache-Control", "no-store");
  server.sendHeader("Access-Control-Allow-Origin", "*");
  server.send(200, "application/json", json);
}

void handleSetParam() {
  if (!server.hasArg("name") || !server.hasArg("value")) {
    server.send(400, "application/json", "{\"error\":\"Missing name or value\"}");
    return;
  }
  
  String name  = server.arg("name");
  float  value  = server.arg("value").toFloat();
  bool   found  = false;
  
  // Physical Parameters
  if (name == "PPR_L") { PPR_L = constrain(value, 100.0, 2000.0); found = true; }
  else if (name == "PPR_R") { PPR_R = constrain(value, 100.0, 2000.0); found = true; }
  else if (name == "WHEEL_D") { WHEEL_D = constrain(value, 1.0, 20.0); WHEEL_CIRC = PI * WHEEL_D; found = true; }
  else if (name == "WHEEL_BASE") { WHEEL_BASE = constrain(value, 5.0, 100.0); found = true; }
  
  // APF Parameters
  else if (name == "K_ATT") { K_ATT = constrain(value, 0.1, 10.0); found = true; }
  else if (name == "K_REP") { K_REP = constrain(value, 0.1, 50.0); found = true; }
  else if (name == "R_GOAL") { R_GOAL = constrain(value, 0.1, 10.0); found = true; }
  else if (name == "S_GOAL") { S_GOAL = constrain(value, 0.1, 50.0); found = true; }
  else if (name == "R_OBS") { R_OBS = constrain(value, 0.1, 30.0); found = true; OBSTACLE_THRESHOLD = R_OBS + S_OBS; }
  else if (name == "S_OBS") { S_OBS = constrain(value, 0.1, 50.0); found = true; OBSTACLE_THRESHOLD = R_OBS + S_OBS; }
  else if (name == "apf") { useAPF = (value > 0.5); found = true; }
  
  // Rotation Parameters
  else if (name == "Kp_rot_vel") { Kp_rot_vel = constrain(value, 0.1, 5.0); found = true; }
  else if (name == "MIN_ROT_VEL") { MIN_ROT_VEL = constrain(value, 1.0, 10.0); found = true; }
  else if (name == "MAX_ROT_VEL") { MAX_ROT_VEL = constrain(value, 5.0, 50.0); found = true; }
  else if (name == "ROTATE_DONE_DEG") { ROTATE_DONE_DEG = constrain(value, 0.5, 10.0); found = true; }
  else if (name == "ROTATE_DEAD_DEG") { ROTATE_DEAD_DEG = constrain(value, 0.1, 5.0); found = true; }
  else if (name == "ROTATE_BRAKE_DEG") { ROTATE_BRAKE_DEG = constrain(value, 5.0, 90.0); found = true; }
  
  // Heading PID
  else if (name == "Kp_move") { Kp_move = constrain(value, 0.01, 2.0); found = true; }
  else if (name == "Ki_move") { Ki_move = constrain(value, 0.0001, 0.1); found = true; }
  else if (name == "Kd_move") { Kd_move = constrain(value, 0.0, 1.0); found = true; }
  
  // Velocity PI
  else if (name == "Kp_vel") { Kp_vel = constrain(value, 0.1, 20.0); found = true; }
  else if (name == "Ki_vel") { Ki_vel = constrain(value, 0.0, 5.0); found = true; }
  
  // Filter & Move
  else if (name == "VEL_FILTER_ALPHA") { VEL_FILTER_ALPHA = constrain(value, 0.01, 1.0); found = true; }
  else if (name == "MIN_MOVE_VEL") { MIN_MOVE_VEL = constrain(value, 1.0, 30.0); found = true; }
  else if (name == "MAX_MOVE_VEL") { MAX_MOVE_VEL = constrain(value, 5.0, 80.0); found = true; }
  else if (name == "SLOWDOWN_DIST") { SLOWDOWN_DIST = constrain(value, 5.0, 100.0); found = true; }
  else if (name == "MOVE_DONE_CM") { MOVE_DONE_CM = constrain(value, 0.5, 10.0); found = true; }
  else if (name == "WAIT_MS") { WAIT_MS = (unsigned long)constrain(value, 50, 2000); found = true; }
  else if (name == "LOOP_MS") { LOOP_MS = (unsigned long)constrain(value, 10, 100); found = true; }
  else if (name == "use_imu_th") { useIMUHeading = (value > 0.5); found = true; }
  else if (name == "comp_alpha") { COMP_FILTER_ALPHA = constrain(value, 0.0f, 1.0f); found = true; }
  
  if (found) {
    saveAllParameters();
    String response = "{\"status\":\"ok\",\"name\":\"" + name + "\",\"value\":" + String(value, 3) + "}";
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(200, "application/json", response);
  } else {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.send(400, "application/json", "{\"error\":\"Unknown parameter\"}");
  }
}

void handleNotFound() {
  server.send(404, "text/plain", "404 Not Found");
}

void setupWebServer() {
  server.on("/", handleRoot);
  server.on("/status", handleStatus);
  server.on("/status-live", handleStatusLive);
  server.on("/status-stream", handleStatusLive);
  server.on("/cmd", handleCmdEndpoint);
  server.on("/params", handleGetParams);
  server.on("/setparam", handleSetParam);
  server.onNotFound(handleNotFound);
  server.begin();
}

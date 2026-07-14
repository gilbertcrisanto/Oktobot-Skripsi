#ifndef WEB_DASHBOARD_H
#define WEB_DASHBOARD_H

#include <Arduino.h>


const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE html>
<html lang="id">
<head>
<meta charset="UTF-8">
<meta name="viewport" content="width=device-width, initial-scale=1.0">
<title>OKTO-BOT — Dashboard Simulasi & Kontrol</title>
<style>
@import url('https://fonts.googleapis.com/css2?family=Share+Tech+Mono&family=Rajdhani:wght@400;500;600;700&display=swap');

:root {
  --bg0: #080c10;
  --bg1: #0d1117;
  --bg2: #111820;
  --bg3: #162030;
  --accent: #00e5ff;
  --accent2: #00ff9d;
  --warn: #ffb300;
  --danger: #ff3d3d;
  --muted: #3a4a5a;
  --text: #c8dae8;
  --text2: #5f7a8a;
  --border: rgba(0,229,255,0.15);
  --border2: rgba(0,229,255,0.08);
}

* { box-sizing: border-box; margin: 0; padding: 0; }

body {
  background: var(--bg0);
  color: var(--text);
  font-family: 'Rajdhani', sans-serif;
  font-size: 15px;
  min-height: 100vh;
  overflow-x: hidden;
  display: block;
  visibility: visible;
  opacity: 1;
}

body::before {
  content: '';
  position: fixed; inset: 0;
  background: repeating-linear-gradient(0deg, transparent, transparent 2px, rgba(0,0,0,0.04) 2px, rgba(0,0,0,0.04) 4px);
  pointer-events: none; z-index: 9999;
}

body::after {
  content: '';
  position: fixed; inset: 0;
  background-image:
    linear-gradient(rgba(0,229,255,0.03) 1px, transparent 1px),
    linear-gradient(90deg, rgba(0,229,255,0.03) 1px, transparent 1px);
  background-size: 40px 40px;
  pointer-events: none; z-index: 0;
}

header {
  position: relative; z-index: 10;
  display: flex; align-items: center; justify-content: space-between;
  padding: 14px 24px;
  border-bottom: 1px solid var(--border);
  background: linear-gradient(180deg, rgba(0,229,255,0.05) 0%, transparent 100%);
}

.logo {
  display: flex; align-items: center; gap: 12px;
}

.logo-oct {
  width: 40px; height: 40px; position: relative;
}
.logo-oct svg { width: 100%; height: 100%; }

.logo-text h1 {
  font-size: 22px; font-weight: 700; letter-spacing: 4px;
  color: var(--accent);
  text-shadow: 0 0 20px rgba(0,229,255,0.5);
}
.logo-text p {
  font-size: 11px; letter-spacing: 3px; color: var(--text2);
  font-family: 'Share Tech Mono', monospace;
}

.header-right {
  display: flex; align-items: center; gap: 20px;
}

.status-pill {
  display: flex; align-items: center; gap: 8px;
  background: var(--bg2); border: 1px solid var(--border);
  border-radius: 20px; padding: 6px 14px;
  font-family: 'Share Tech Mono', monospace;
  font-size: 12px;
}
.status-dot {
  width: 8px; height: 8px; border-radius: 50%;
  background: var(--accent2);
  box-shadow: 0 0 8px var(--accent2);
  animation: pulse 2s ease-in-out infinite;
}
@keyframes pulse {
  0%,100% { opacity: 1; transform: scale(1); }
  50% { opacity: 0.5; transform: scale(0.85); }
}
.status-dot.warn { background: var(--warn); box-shadow: 0 0 8px var(--warn); }
.status-dot.danger { background: var(--danger); box-shadow: 0 0 8px var(--danger); animation: none; }
.status-dot.idle { background: var(--muted); box-shadow: none; animation: none; }

main {
  position: relative; z-index: 5;
  display: grid;
  grid-template-columns: 280px 1fr 280px;
  grid-template-rows: auto auto;
  gap: 12px;
  padding: 16px 20px;
  max-width: 1400px; margin: 0 auto;
}

.card {
  background: var(--bg1);
  border: 1px solid var(--border2);
  border-radius: 4px;
  padding: 16px;
  position: relative;
  overflow: hidden;
}
.card::before {
  content: '';
  position: absolute; top: 0; left: 0; right: 0; height: 1px;
  background: linear-gradient(90deg, transparent, var(--accent), transparent);
  opacity: 0.4;
}

.card-title {
  font-size: 11px; letter-spacing: 3px; font-weight: 600;
  color: var(--text2); text-transform: uppercase;
  display: flex; align-items: center; gap: 8px; margin-bottom: 14px;
}
.card-title::before {
  content: '';
  width: 3px; height: 12px;
  background: var(--accent);
  box-shadow: 0 0 6px var(--accent);
}

.col-left { grid-column: 1; grid-row: 1 / 3; display: flex; flex-direction: column; gap: 12px; }

.telem-grid {
  display: grid; grid-template-columns: 1fr 1fr; gap: 10px;
}
.telem-item {
  background: var(--bg2); border: 1px solid var(--border2);
  border-radius: 4px; padding: 10px 12px;
}
.telem-label {
  font-size: 10px; letter-spacing: 2px; color: var(--text2);
  font-family: 'Share Tech Mono', monospace; margin-bottom: 4px;
}
.telem-value {
  font-family: 'Share Tech Mono', monospace;
  font-size: 20px; color: var(--accent);
  text-shadow: 0 0 12px rgba(0,229,255,0.4);
  line-height: 1;
}
.telem-value.green { color: var(--accent2); text-shadow: 0 0 12px rgba(0,255,157,0.4); }
.telem-value.warn  { color: var(--warn);   text-shadow: 0 0 12px rgba(255,179,0,0.4); }
.telem-unit {
  font-size: 10px; color: var(--text2);
  font-family: 'Share Tech Mono', monospace;
}

.compass-wrap {
  display: flex; flex-direction: column; align-items: center; gap: 8px;
}
#compass-svg { filter: drop-shadow(0 0 10px rgba(0,229,255,0.2)); }

.sensor-ring {
  position: relative; width: 180px; height: 180px; margin: 0 auto;
}
.sensor-ring canvas { position: absolute; top: 0; left: 0; }
.sensor-center {
  position: absolute; top: 50%; left: 50%; transform: translate(-50%,-50%);
  text-align: center;
}
.sensor-center span {
  font-family: 'Share Tech Mono', monospace;
  font-size: 11px; color: var(--text2);
  display: block;
}

.col-center { grid-column: 2; grid-row: 1 / 3; display: flex; flex-direction: column; gap: 12px; }

.map-wrap {
  position: relative;
  width: 100%;
}
#map-canvas {
  width: 100%;
  aspect-ratio: 16 / 10;
  display: block;
  background: var(--bg2);
  border-radius: 2px;
  cursor: crosshair;
}
.map-overlay {
  position: absolute; top: 10px; left: 10px;
  font-family: 'Share Tech Mono', monospace; font-size: 10px;
  color: var(--text2); letter-spacing: 1px;
}
.map-toolbar {
  display: flex; gap: 6px; align-items: center; flex-wrap: wrap;
  padding: 6px 0 8px 0; border-bottom: 1px solid var(--border2); margin-bottom: 8px;
}
.map-toolbar button {
  background: var(--bg2); border: 1px solid var(--muted);
  color: var(--text2); font-family: 'Share Tech Mono', monospace;
  font-size: 10px; letter-spacing: 0.5px; padding: 4px 10px;
  border-radius: 3px; cursor: pointer; transition: all 0.15s;
  display: flex; align-items: center; gap: 4px;
}
.map-toolbar button:hover { border-color: var(--accent); color: var(--accent); }
.map-toolbar button.btn-dl {
  margin-left: auto; background: rgba(0,229,255,0.06);
  border-color: var(--accent); color: var(--accent); font-weight: 600;
}
.map-toolbar button.btn-dl:hover { background: rgba(0,229,255,0.15); }
.map-toolbar .sep { width: 1px; height: 18px; background: var(--border2); }

.wp-inputs {
  display: grid; grid-template-columns: 1fr 1fr auto; gap: 8px; align-items: end;
  margin-bottom: 12px;
}
.field label {
  display: block; font-size: 10px; letter-spacing: 2px;
  color: var(--text2); margin-bottom: 4px;
}
input[type=number] {
  width: 100%; background: var(--bg2);
  border: 1px solid var(--muted); border-radius: 4px;
  color: var(--accent); font-family: 'Share Tech Mono', monospace;
  font-size: 16px; padding: 8px 10px;
  outline: none; transition: border-color 0.2s, background 0.3s;
  -moz-appearance: textfield;
}
input[type=number]:disabled {
  opacity: 0.4;
  cursor: not-allowed;
  border-color: rgba(255, 61, 61, 0.3);
}
input[type=number]::-webkit-inner-spin-button { display: none; }
input[type=number]:focus { border-color: var(--accent); box-shadow: 0 0 0 2px rgba(0,229,255,0.1); }

.wp-table { width: 100%; border-collapse: collapse; }
.wp-table th {
  font-size: 10px; letter-spacing: 2px; color: var(--text2);
  padding: 4px 8px; text-align: left;
  border-bottom: 1px solid var(--border2);
}
.wp-table td {
  font-family: 'Share Tech Mono', monospace; font-size: 13px;
  color: var(--text); padding: 5px 8px;
  border-bottom: 1px solid var(--border2);
}
.wp-table tr.active td { color: var(--accent); background: rgba(0,229,255,0.04); }
.wp-table tr.done td { color: var(--muted); text-decoration: line-through; }
.wp-del {
  background: none; border: none; cursor: pointer;
  color: var(--muted); font-size: 14px; padding: 2px 6px;
  border-radius: 3px; transition: color 0.2s, background 0.2s;
}
.wp-del:hover { color: var(--danger); background: rgba(255,61,61,0.1); }

.col-right { grid-column: 3; grid-row: 1 / 3; display: flex; flex-direction: column; gap: 12px; }

.btn-group { display: flex; flex-direction: column; gap: 8px; }
.btn {
  width: 100%; padding: 12px 16px;
  border: 1px solid; border-radius: 4px;
  font-family: 'Rajdhani', sans-serif; font-size: 14px; font-weight: 600;
  letter-spacing: 2px; text-transform: uppercase;
  cursor: pointer; transition: all 0.15s;
  display: flex; align-items: center; gap: 10px;
}
.btn-start {
  background: rgba(0,255,157,0.08); border-color: var(--accent2);
  color: var(--accent2);
}
.btn-start:hover { background: rgba(0,255,157,0.18); box-shadow: 0 0 16px rgba(0,255,157,0.2); }
.btn-stop {
  background: rgba(255,61,61,0.08); border-color: var(--danger);
  color: var(--danger);
}
.btn-stop:hover { background: rgba(255,61,61,0.18); box-shadow: 0 0 16px rgba(255,61,61,0.2); }
.btn-primary {
  background: rgba(0,229,255,0.06); border-color: var(--accent);
  color: var(--accent);
}
.btn-primary:hover { background: rgba(0,229,255,0.14); }
.btn-neutral {
  background: var(--bg2); border-color: var(--muted);
  color: var(--text2);
}
.btn-neutral:hover { border-color: var(--text2); color: var(--text); }
.btn:disabled {
  opacity: 0.4;
  cursor: not-allowed;
  box-shadow: none !important;
}

.btn-icon { font-size: 18px; line-height: 1; }

.apf-row {
  display: flex; align-items: center; justify-content: space-between;
  padding: 10px 0;
  border-bottom: 1px solid var(--border2);
}
.apf-label {
  font-size: 12px; letter-spacing: 2px; font-weight: 600;
}
.toggle {
  width: 44px; height: 24px; background: var(--muted);
  border-radius: 12px; cursor: pointer; position: relative;
  transition: background 0.2s; border: none; outline: none;
}
.toggle::after {
  content: ''; position: absolute;
  top: 3px; left: 3px; width: 18px; height: 18px;
  background: var(--bg0); border-radius: 50%;
  transition: transform 0.2s;
}
.toggle.on { background: var(--accent2); }
.toggle.on::after { transform: translateX(20px); }

.slider-wrap label {
  display: flex; justify-content: space-between; align-items: center;
  font-size: 11px; letter-spacing: 2px; color: var(--text2);
  margin-bottom: 6px;
}
.slider-val {
  font-family: 'Share Tech Mono', monospace;
  color: var(--accent); font-size: 14px;
}
input[type=range] {
  width: 100%; -webkit-appearance: none;
  height: 4px; background: var(--muted);
  border-radius: 2px; outline: none;
}
input[type=range]::-webkit-slider-thumb {
  -webkit-appearance: none;
  width: 16px; height: 16px; border-radius: 50%;
  background: var(--accent); cursor: pointer;
  box-shadow: 0 0 8px rgba(0,229,255,0.5);
  transition: transform 0.1s;
}
input[type=range]::-webkit-slider-thumb:hover { transform: scale(1.2); }

#log {
  flex: 1; min-height: 120px; max-height: 200px;
  background: var(--bg0); border: 1px solid var(--border2);
  border-radius: 4px; padding: 10px;
  overflow-y: auto; font-family: 'Share Tech Mono', monospace;
  font-size: 11px; line-height: 1.7; color: var(--text2);
}
#log .log-ok   { color: var(--accent2); }
#log .log-cmd  { color: var(--accent); }
#log .log-err  { color: var(--danger); }
#log .log-warn { color: var(--warn); }

.tab-btn {
  background: none; border: none; color: var(--text2);
  font-family: 'Rajdhani', sans-serif; font-size: 11px; font-weight: 700;
  letter-spacing: 1px; cursor: pointer; padding: 6px 10px;
  border-radius: 3px; transition: all 0.2s;
}
.tab-btn.active {
  color: var(--accent); background: rgba(0,229,255,0.06);
  border: 1px solid rgba(0,229,255,0.15);
}

.param-btn {
  background: rgba(0,255,157,0.1); border: 1px solid var(--accent2);
  color: var(--accent2); font-size: 10px; font-weight: 600;
  padding: 2px 4px; border-radius: 2px; cursor: pointer;
  transition: all 0.2s; position: absolute; right: 2px; top: 50%;
  transform: translateY(-50%);
}
.param-btn:hover { background: rgba(0,255,157,0.2); }

.field {
  position: relative;
}
.field input[type=number] {
  padding-right: 30px;
}

::-webkit-scrollbar { width: 4px; height: 4px; }
::-webkit-scrollbar-track { background: transparent; }
::-webkit-scrollbar-thumb { background: var(--muted); border-radius: 2px; }
::-webkit-scrollbar-thumb:hover { background: var(--text2); }

@media (max-width: 900px) {
  main {
    grid-template-columns: 1fr;
    grid-template-rows: auto;
  }
  .col-left, .col-center, .col-right {
    grid-column: 1; grid-row: auto;
  }
}

.btn:active { transform: scale(0.97); }

.conn-badge {
  font-family: 'Share Tech Mono', monospace;
  font-size: 11px; color: var(--text2);
  display: flex; align-items: center; gap: 6px;
}

.card-corner {
  position: absolute; bottom: 0; right: 0;
  width: 20px; height: 20px;
  border-right: 1px solid var(--accent); border-bottom: 1px solid var(--accent);
  opacity: 0.3;
}
.card-corner-tl {
  position: absolute; top: 0; left: 0;
  width: 12px; height: 12px;
  border-left: 1px solid var(--accent); border-top: 1px solid var(--accent);
  opacity: 0.3;
}

/* A* Path Planner */
.astar-toolbar {
  display: flex; gap: 4px; flex-wrap: wrap; padding: 6px 0 8px 0;
  border-bottom: 1px solid var(--border2); margin-bottom: 8px;
}
.astar-btn {
  background: var(--bg2); border: 1px solid var(--muted); color: var(--text2);
  font-family: 'Rajdhani', sans-serif; font-size: 10px; font-weight: 600;
  letter-spacing: 0.5px; padding: 4px 8px; border-radius: 3px; cursor: pointer;
  transition: all 0.15s; white-space: nowrap;
}
.astar-btn:hover { border-color: var(--text2); color: var(--text); }
.astar-btn.active { background: rgba(0,229,255,0.1); border-color: var(--accent); color: var(--accent); }
.astar-btn.run { background: rgba(0,255,157,0.08); border-color: var(--accent2); color: var(--accent2); }
.astar-btn.run:hover { background: rgba(0,255,157,0.18); box-shadow: 0 0 8px rgba(0,255,157,0.2); }
.astar-btn.send { background: rgba(0,229,255,0.06); border-color: var(--accent); color: var(--accent); }
.astar-btn.send:hover { background: rgba(0,229,255,0.14); box-shadow: 0 0 8px rgba(0,229,255,0.2); }
#astar-canvas {
  width: 100%; aspect-ratio: 1 / 1; display: block;
  background: #0a0e14; border: 1px solid var(--border); border-radius: 3px;
  cursor: crosshair; image-rendering: pixelated;
}
.astar-info {
  display: flex; gap: 12px; flex-wrap: wrap; padding-top: 6px;
  font-family: 'Share Tech Mono', monospace; font-size: 10px; color: var(--text2);
}
.astar-info span { color: var(--accent); }
.astar-cfg {
  display: flex; gap: 8px; align-items: center; flex-wrap: wrap;
  font-size: 10px; color: var(--text2); margin-bottom: 6px;
}
.astar-cfg label { font-weight: 600; letter-spacing: 0.5px; }
.astar-cfg input {
  width: 50px; font-size: 10px; padding: 2px 4px;
  background: var(--bg0); border: 1px solid var(--muted); border-radius: 3px;
  color: var(--accent); text-align: center; font-family: 'Share Tech Mono', monospace;
}
</style>
</head>
<body>

<!-- MODAL POPUP KALIBRASI IMU -->
<div id="calibration-modal" style="display:none; position:fixed; inset:0; background:rgba(8,12,16,0.85); backdrop-filter:blur(4px); z-index:10000; align-items:center; justify-content:center;">
  <div style="background:var(--bg1); border:1px solid var(--accent); border-radius:8px; padding:24px; text-align:center; max-width:320px; box-shadow:0 0 30px rgba(0,229,255,0.25); position:relative;">
    <div style="font-size:32px; margin-bottom:12px; animation: pulse 1s infinite alternate;">🧭</div>
    <div style="font-size:18px; font-weight:700; color:var(--accent); margin-bottom:8px; letter-spacing:2px; text-transform:uppercase;">Kalibrasi Gyro IMU</div>
    <p style="font-size:13px; color:var(--text); margin-bottom:16px; line-height:1.5;">Sedang merekam bias sensor giroskop. Harap diamkan robot dan jangan disentuh!</p>
    <div style="font-family:'Share Tech Mono',monospace; font-size:28px; color:var(--warn); font-weight:bold;" id="cal-countdown">2.0s</div>
  </div>
</div>

<header>
  <div class="logo">
    <div class="logo-oct">
      <svg viewBox="0 0 40 40" fill="none" xmlns="http://www.w3.org/2000/svg">
        <polygon points="27,3 37,13 37,27 27,37 13,37 3,27 3,13 13,3" fill="none" stroke="#00e5ff" stroke-width="1.5" opacity="0.6"/>
        <polygon points="25,9 31,15 31,25 25,31 15,31 9,25 9,15 15,9" fill="rgba(0,229,255,0.08)" stroke="#00e5ff" stroke-width="1" opacity="0.4"/>
        <circle cx="20" cy="20" r="4" fill="#00e5ff" opacity="0.8"/>
        <line x1="20" y1="3" x2="20" y2="9" stroke="#00e5ff" stroke-width="1" opacity="0.5"/>
        <line x1="20" y1="31" x2="20" y2="37" stroke="#00e5ff" stroke-width="1" opacity="0.5"/>
        <line x1="3" y1="20" x2="9" y2="20" stroke="#00e5ff" stroke-width="1" opacity="0.5"/>
        <line x1="31" y1="20" x2="37" y2="20" stroke="#00e5ff" stroke-width="1" opacity="0.5"/>
      </svg>
    </div>
    <div class="logo-text">
      <h1>OKTO-BOT</h1>
      <p>NAVIGATION CONTROL SYSTEM v2.0</p>
    </div>
  </div>

  <div class="header-right">
    <div class="conn-badge">
      <span id="conn-dot" style="width:6px;height:6px;border-radius:50%;background:var(--muted);display:inline-block;transition:background 0.3s"></span>
      <span id="conn-text">DISCONNECTED</span>
    </div>
    <div class="status-pill">
      <div class="status-dot idle" id="state-dot"></div>
      <span id="state-text" style="font-family:'Share Tech Mono',monospace;font-size:12px;letter-spacing:2px">IDLE</span>
    </div>
  </div>
</header>

<main>
  <!-- LEFT COLUMN -->
  <div class="col-left">
    <div class="card">
      <div class="card-corner-tl"></div>
      <div class="card-corner"></div>
      <div class="card-title">Telemetri Posisi</div>
      <div class="telem-grid">
        <div class="telem-item">
          <div class="telem-label">POS X</div>
          <div class="telem-value" id="tel-x">0.0</div>
          <div class="telem-unit">cm</div>
        </div>
        <div class="telem-item">
          <div class="telem-label">POS Y</div>
          <div class="telem-value" id="tel-y">0.0</div>
          <div class="telem-unit">cm</div>
        </div>
        <div class="telem-item">
          <div class="telem-label">HEADING</div>
          <div class="telem-value green" id="tel-th">90.0</div>
          <div class="telem-unit">deg</div>
        </div>
        <div class="telem-item">
          <div class="telem-label">WAYPOINT</div>
          <div class="telem-value warn" id="tel-wp">0/0</div>
          <div class="telem-unit">target</div>
        </div>
      </div>
    </div>

    <div class="card">
      <div class="card-title">Kompas</div>
      <div class="compass-wrap">
        <svg id="compass-svg" width="140" height="140" viewBox="0 0 140 140">
          <circle cx="70" cy="70" r="64" fill="none" stroke="rgba(0,229,255,0.12)" stroke-width="1"/>
          <circle cx="70" cy="70" r="58" fill="none" stroke="rgba(0,229,255,0.06)" stroke-width="8"/>
          <g stroke="rgba(0,229,255,0.3)" stroke-width="1">
            <line x1="70" y1="8" x2="70" y2="16"/>
            <line x1="70" y1="124" x2="70" y2="132"/>
            <line x1="8" y1="70" x2="16" y2="70"/>
            <line x1="124" y1="70" x2="132" y2="70"/>
          </g>
          <g stroke="rgba(0,229,255,0.15)" stroke-width="0.5">
            <line x1="105.4" y1="14.7" x2="101.9" y2="20.8"/>
            <line x1="125.3" y1="34.6" x2="119.2" y2="38.1"/>
            <line x1="125.3" y1="105.4" x2="119.2" y2="101.9"/>
            <line x1="105.4" y1="125.3" x2="101.9" y2="119.2"/>
            <line x1="34.6" y1="125.3" x2="38.1" y2="119.2"/>
            <line x1="14.7" y1="105.4" x2="20.8" y2="101.9"/>
            <line x1="14.7" y1="34.6" x2="20.8" y2="38.1"/>
            <line x1="34.6" y1="14.7" x2="38.1" y2="20.8"/>
          </g>
          <text x="70" y="6" text-anchor="middle" fill="#00e5ff" font-family="Share Tech Mono" font-size="9" opacity="0.7">N</text>
          <text x="70" y="139" text-anchor="middle" fill="rgba(0,229,255,0.4)" font-family="Share Tech Mono" font-size="9">S</text>
          <text x="136" y="73" text-anchor="middle" fill="rgba(0,229,255,0.4)" font-family="Share Tech Mono" font-size="9">E</text>
          <text x="5" y="73" text-anchor="middle" fill="rgba(0,229,255,0.4)" font-family="Share Tech Mono" font-size="9">W</text>
          <circle cx="70" cy="70" r="5" fill="none" stroke="rgba(0,229,255,0.3)" stroke-width="1"/>
          <g id="compass-needle">
            <polygon points="70,22 73,70 70,72 67,70" fill="#00e5ff" opacity="0.9"/>
            <polygon points="70,118 73,70 70,68 67,70" fill="rgba(0,229,255,0.25)"/>
          </g>
        </svg>
        <div style="font-family:'Share Tech Mono',monospace;font-size:13px;color:var(--accent);letter-spacing:2px" id="compass-deg">090.0°</div>
      </div>
    </div>

    <div class="card" style="flex:1;display:flex;flex-direction:column; min-height: 200px;">
      <div class="card-title" style="margin-bottom:8px;">
        Log Komunikasi
        <button onclick="clearLog()" style="margin-left:auto;background:none;border:none;color:var(--muted);cursor:pointer;font-size:11px;font-family:'Share Tech Mono',monospace;">CLR</button>
      </div>
      <div id="log" style="flex:1; min-height: 120px;"></div>
    </div>
  </div>

  <!-- CENTER COLUMN -->
  <div class="col-center">
    <div class="card" style="flex:1;display:flex;flex-direction:column;padding:12px;">
      <div class="card-title">
        Peta Navigasi
        <span style="margin-left:auto;font-family:'Share Tech Mono',monospace;font-size:10px;color:var(--text2)" id="map-scale-label">1px = 0.50cm</span>
      </div>
      <div class="map-toolbar">
        <button onclick="zoomMap(0.25)" title="Perbesar peta">🔍 +</button>
        <button onclick="zoomMap(-0.25)" title="Perkecil peta">🔍 −</button>
        <button onclick="resetMapView()" title="Reset tampilan peta">⟳ Reset</button>
        <div class="sep"></div>
        <button onclick="clearTrails()" title="Hapus semua jalur">✕ Hapus Trail</button>
        <div class="sep"></div>
        <button class="btn-dl" onclick="downloadMap()" title="Simpan peta sebagai gambar PNG">⬇ Unduh Peta (PNG)</button>
      </div>
      <div class="map-wrap" style="flex:1;">
        <canvas id="map-canvas"></canvas>
        <div class="map-overlay" id="cursor-pos">X: — Y: —</div>
      </div>
    </div>

    <div class="card">
      <div class="card-title">Tambah Waypoint</div>
      <div class="wp-inputs">
        <div class="field">
          <label>X (cm)</label>
          <input type="number" id="wp-x" placeholder="0" step="5">
        </div>
        <div class="field">
          <label>Y (cm)</label>
          <input type="number" id="wp-y" placeholder="0" step="5">
        </div>
        <button class="btn btn-primary" onclick="addWaypoint()" style="height:38px;padding:0 16px;white-space:nowrap">
          + ADD
        </button>
      </div>

      <div style="overflow-x:auto;max-height:100px;overflow-y:auto;margin-bottom:8px;">
        <table class="wp-table">
          <thead>
            <tr>
              <th>#</th><th>X</th><th>Y</th><th>DIST</th><th></th>
            </tr>
          </thead>
          <tbody id="wp-tbody">
            <tr><td colspan="5" style="color:var(--muted);font-size:11px;padding:10px 8px">Belum ada waypoint. Klik pada peta atau isi koordinat di atas.</td></tr>
          </tbody>
        </table>
      </div>
    </div>

    <!-- A* PATH PLANNER CARD -->
    <div class="card" style="padding:12px;">
      <div class="card-title">A* Path Planner
        <span style="margin-left:auto;font-family:'Share Tech Mono',monospace;font-size:10px;color:var(--text2)" id="astar-grid-label">30×30 grid</span>
      </div>
      <div class="astar-cfg">
        <label>LEBAR</label><input type="number" id="astar-w" value="300" step="50" min="100" max="1000" onchange="initAstarGrid()">cm
        <label>TINGGI</label><input type="number" id="astar-h" value="300" step="50" min="100" max="1000" onchange="initAstarGrid()">cm
        <label>SEL</label><input type="number" id="astar-res" value="10" step="5" min="5" max="50" onchange="initAstarGrid()">cm
      </div>
      <div class="astar-toolbar">
        <button class="astar-btn active" id="astar-mode-wall" onclick="setAstarMode('wall')">🖊 DINDING</button>
        <button class="astar-btn" id="astar-mode-erase" onclick="setAstarMode('erase')">🧹 HAPUS</button>
        <button class="astar-btn" id="astar-mode-goal" onclick="setAstarMode('goal')">🎯 TUJUAN</button>
        <button class="astar-btn" onclick="resetAstarMap()">🔄 RESET</button>
        <button class="astar-btn run" onclick="runAstar()">▶ CARI JALUR</button>
        <button class="astar-btn send" id="astar-send-btn" onclick="sendAstarToRobot()" disabled>📤 KIRIM</button>
      </div>
      <canvas id="astar-canvas"></canvas>
      <div class="astar-info">
        <div>MODE: <span id="astar-mode-lbl">GAMBAR DINDING</span></div>
        <div>JALUR: <span id="astar-path-len">—</span></div>
        <div>WAYPOINTS: <span id="astar-wp-count">—</span></div>
      </div>
    </div>

    <!-- KENDALI MANUAL CARD -->
    <div class="card">
      <div class="card-title">Kendali Manual (Joystick / Keyboard WASD)</div>
      <div style="display:flex; justify-content:space-around; align-items:center; flex-wrap:wrap; gap:16px;">
        <div id="joystick-zone" style="position:relative; width:130px; height:130px; background:var(--bg2); border:1px solid var(--border); border-radius:50%; cursor:grab; touch-action:none;">
          <div id="joystick-handle" style="position:absolute; width:40px; height:40px; background:var(--accent); border-radius:50%; top:45px; left:45px; box-shadow:0 0 10px var(--accent); transition: transform 0.05s;"></div>
        </div>
        <div style="font-family:'Share Tech Mono',monospace; font-size:12px; color:var(--text2); line-height:1.6;">
          <span style="color:var(--accent)">Kontrol Keyboard:</span><br>
          · W: Maju | S: Mundur<br>
          · A: Putar Kiri | D: Putar Kanan<br>
          · Spacebar: Rem / Berhenti<br>
          <div style="margin-top:6px; font-size:13px;">
            Target Roda Kiri: <span id="manual-speed-l" style="color:var(--accent2)">0</span> cm/s<br>
            Target Roda Kanan: <span id="manual-speed-r" style="color:var(--accent2)">0</span> cm/s
          </div>
        </div>
      </div>
    </div>
  </div>

  <!-- RIGHT COLUMN -->
  <div class="col-right">
    <div class="card">
      <div class="card-title">Kendali Navigasi</div>
      <div class="btn-group">
        <button class="btn btn-start" onclick="sendCmd('START')">
          <span class="btn-icon">▶</span> START NAVIGASI
        </button>
        <button class="btn btn-stop" onclick="sendCmd('STOP')">
          <span class="btn-icon">■</span> EMERGENCY STOP
        </button>
        <button class="btn btn-primary" onclick="sendCmd('STATUS')">
          <span class="btn-icon" style="font-size:14px">⊙</span> REFRESH STATUS
        </button>
        <button class="btn btn-neutral" onclick="clearAll()">
          <span class="btn-icon" style="font-size:14px">✕</span> CLEAR WAYPOINTS
        </button>
        <div style="border-top:1px solid var(--border2); margin-top:8px; padding-top:8px; display:flex; flex-direction:column; gap:6px;">
          <div style="display:flex; justify-content:space-between; align-items:center;">
            <span style="font-size:11px; color:var(--text2); font-weight:600; letter-spacing:1px; text-transform:uppercase;">MODE UKUR FISIK (STEP)</span>
            <button class="toggle" id="step-mode-toggle" onclick="toggleStepMode()"></button>
          </div>
          <button class="btn btn-start" id="btn-resume-nav" onclick="sendCmd('RESUME')" style="background:rgba(0,255,157,0.05); border-color:var(--accent2); color:var(--accent2); justify-content:center; padding:8px; font-size:12px; height:32px;" disabled>
            ⏭ LANJUTKAN NAVIGASI
          </button>
        </div>
      </div>
    </div>

    <!-- PENGATURAN TABS CARD -->
    <div class="card" style="padding:12px;">
      <div class="card-title" style="margin-bottom:8px;">Pengontrol & Kalibrasi</div>
      
      <div style="display:flex; gap:2px; margin-bottom:12px; border-bottom:1px solid var(--border2); padding-bottom:4px;">
        <button class="tab-btn active" onclick="switchTab('tab-parameters')">PARAM & PID</button>
        <button class="tab-btn" onclick="switchTab('tab-sys')">SISTEM</button>
        <button class="tab-btn" onclick="switchTab('tab-log-csv')">LOGGER</button>
      </div>

      <!-- Parameters Tab (Unified Param & PID) -->
      <div id="tab-parameters" class="tab-content" style="display:block; max-height:480px; overflow-y:auto;">
        <div style="margin-bottom:8px;">
          <button class="btn btn-primary" onclick="loadParameters()" style="width:100%; padding:6px 8px; font-size:10px; height:auto; justify-content:center;">RELOAD ALL PARAMS</button>
        </div>

        <!-- Individual Parameters List -->
        <div style="font-size:9px; color:var(--text2); margin-bottom:6px; font-weight:600; letter-spacing:1px;">APF PARAMETERS</div>
        <div style="display:grid; grid-template-columns:1fr 1fr; gap:4px; margin-bottom:8px;">
          <div class="field"><label>K_ATT</label><input type="number" id="param-K_ATT" step="0.1" min="0.1" max="10.0"><button class="param-btn" onclick="setParam('K_ATT', 'param-K_ATT')">✓</button></div>
          <div class="field"><label>K_REP</label><input type="number" id="param-K_REP" step="0.5" min="0.1" max="50.0"><button class="param-btn" onclick="setParam('K_REP', 'param-K_REP')">✓</button></div>
          <div class="field"><label>R_GOAL</label><input type="number" id="param-R_GOAL" step="0.1" min="0.1" max="10.0"><button class="param-btn" onclick="setParam('R_GOAL', 'param-R_GOAL')">✓</button></div>
          <div class="field"><label>S_GOAL</label><input type="number" id="param-S_GOAL" step="0.5" min="0.1" max="50.0"><button class="param-btn" onclick="setParam('S_GOAL', 'param-S_GOAL')">✓</button></div>
          <div class="field"><label>R_OBS</label><input type="number" id="param-R_OBS" step="0.5" min="0.1" max="30.0"><button class="param-btn" onclick="setParam('R_OBS', 'param-R_OBS')">✓</button></div>
          <div class="field"><label>S_OBS</label><input type="number" id="param-S_OBS" step="0.5" min="0.1" max="50.0"><button class="param-btn" onclick="setParam('S_OBS', 'param-S_OBS')">✓</button></div>
        </div>

        <div style="font-size:9px; color:var(--text2); margin-bottom:6px; font-weight:600; letter-spacing:1px; margin-top:8px;">ROTATION PARAMETERS</div>
        <div style="display:grid; grid-template-columns:1fr 1fr; gap:4px; margin-bottom:8px;">
          <div class="field"><label>Kp_rot_vel</label><input type="number" id="param-Kp_rot_vel" step="0.05" min="0.1" max="5.0"><button class="param-btn" onclick="setParam('Kp_rot_vel', 'param-Kp_rot_vel')">✓</button></div>
          <div class="field"><label>MIN_ROT_VEL</label><input type="number" id="param-MIN_ROT_VEL" step="0.5" min="1.0" max="10.0"><button class="param-btn" onclick="setParam('MIN_ROT_VEL', 'param-MIN_ROT_VEL')">✓</button></div>
          <div class="field"><label>MAX_ROT_VEL</label><input type="number" id="param-MAX_ROT_VEL" step="0.5" min="5.0" max="50.0"><button class="param-btn" onclick="setParam('MAX_ROT_VEL', 'param-MAX_ROT_VEL')">✓</button></div>
          <div class="field"><label>ROTATE_DONE</label><input type="number" id="param-ROTATE_DONE_DEG" step="0.1" min="0.5" max="10.0"><button class="param-btn" onclick="setParam('ROTATE_DONE_DEG', 'param-ROTATE_DONE_DEG')">✓</button></div>
          <div class="field"><label>ROTATE_DEAD</label><input type="number" id="param-ROTATE_DEAD_DEG" step="0.1" min="0.1" max="5.0"><button class="param-btn" onclick="setParam('ROTATE_DEAD_DEG', 'param-ROTATE_DEAD_DEG')">✓</button></div>
          <div class="field"><label>ROTATE_BRAKE</label><input type="number" id="param-ROTATE_BRAKE_DEG" step="0.5" min="5.0" max="90.0"><button class="param-btn" onclick="setParam('ROTATE_BRAKE_DEG', 'param-ROTATE_BRAKE_DEG')">✓</button></div>
        </div>

        <div style="font-size:9px; color:var(--text2); margin-bottom:6px; font-weight:600; letter-spacing:1px; margin-top:8px;">HEADING PID</div>
        <div style="display:grid; grid-template-columns:1fr 1fr 1fr; gap:4px; margin-bottom:8px;">
          <div class="field"><label>Kp_move</label><input type="number" id="param-Kp_move" step="0.01" min="0.01" max="2.0"><button class="param-btn" onclick="setParam('Kp_move', 'param-Kp_move')">✓</button></div>
          <div class="field"><label>Ki_move</label><input type="number" id="param-Ki_move" step="0.0001" min="0.0001" max="0.1"><button class="param-btn" onclick="setParam('Ki_move', 'param-Ki_move')">✓</button></div>
          <div class="field"><label>Kd_move</label><input type="number" id="param-Kd_move" step="0.01" min="0.0" max="1.0"><button class="param-btn" onclick="setParam('Kd_move', 'param-Kd_move')">✓</button></div>
        </div>

        <div style="font-size:9px; color:var(--text2); margin-bottom:6px; font-weight:600; letter-spacing:1px; margin-top:8px;">VELOCITY CONTROL</div>
        <div style="display:grid; grid-template-columns:1fr 1fr; gap:4px; margin-bottom:8px;">
          <div class="field"><label>Kp_vel</label><input type="number" id="param-Kp_vel" step="0.1" min="0.1" max="20.0"><button class="param-btn" onclick="setParam('Kp_vel', 'param-Kp_vel')">✓</button></div>
          <div class="field"><label>Ki_vel</label><input type="number" id="param-Ki_vel" step="0.1" min="0.0" max="5.0"><button class="param-btn" onclick="setParam('Ki_vel', 'param-Ki_vel')">✓</button></div>
          <div class="field"><label>VEL_ALPHA</label><input type="number" id="param-VEL_FILTER_ALPHA" step="0.01" min="0.01" max="1.0"><button class="param-btn" onclick="setParam('VEL_FILTER_ALPHA', 'param-VEL_FILTER_ALPHA')">✓</button></div>
          <div class="field"><label>MIN_VEL</label><input type="number" id="param-MIN_MOVE_VEL" step="0.5" min="1.0" max="30.0"><button class="param-btn" onclick="setParam('MIN_MOVE_VEL', 'param-MIN_MOVE_VEL')">✓</button></div>
          <div class="field"><label>MAX_VEL</label><input type="number" id="param-MAX_MOVE_VEL" step="0.5" min="5.0" max="80.0"><button class="param-btn" onclick="setParam('MAX_MOVE_VEL', 'param-MAX_MOVE_VEL')">✓</button></div>
          <div class="field"><label>SLOWDOWN</label><input type="number" id="param-SLOWDOWN_DIST" step="0.5" min="5.0" max="100.0"><button class="param-btn" onclick="setParam('SLOWDOWN_DIST', 'param-SLOWDOWN_DIST')">✓</button></div>
        </div>

        <div style="font-size:9px; color:var(--text2); margin-bottom:6px; font-weight:600; letter-spacing:1px; margin-top:8px;">MOVEMENT & TIMING</div>
        <div style="display:grid; grid-template-columns:1fr 1fr; gap:4px; margin-bottom:8px;">
          <div class="field"><label>MOVE_DONE</label><input type="number" id="param-MOVE_DONE_CM" step="0.1" min="0.5" max="10.0"><button class="param-btn" onclick="setParam('MOVE_DONE_CM', 'param-MOVE_DONE_CM')">✓</button></div>
          <div class="field"><label>WAIT_MS</label><input type="number" id="param-WAIT_MS" step="10" min="50" max="2000"><button class="param-btn" onclick="setParam('WAIT_MS', 'param-WAIT_MS')">✓</button></div>
          <div class="field"><label>LOOP_MS</label><input type="number" id="param-LOOP_MS" step="1" min="10" max="100"><button class="param-btn" onclick="setParam('LOOP_MS', 'param-LOOP_MS')">✓</button></div>
        </div>

        <div style="font-size:9px; color:var(--text2); margin-bottom:6px; font-weight:600; letter-spacing:1px; margin-top:8px; border-top:1px solid var(--border2); padding-top:8px;">ROBOT PHYSICAL (Hardware)</div>
        <div style="display:grid; grid-template-columns:1fr 1fr; gap:4px; margin-bottom:8px;">
          <div class="field"><label>PPR_L</label><input type="number" id="param-PPR_L" step="1" min="100" max="2000"><button class="param-btn" onclick="setParam('PPR_L', 'param-PPR_L')">✓</button></div>
          <div class="field"><label>PPR_R</label><input type="number" id="param-PPR_R" step="1" min="100" max="2000"><button class="param-btn" onclick="setParam('PPR_R', 'param-PPR_R')">✓</button></div>
          <div class="field"><label>WHEEL_D</label><input type="number" id="param-WHEEL_D" step="0.1" min="1.0" max="20.0"><button class="param-btn" onclick="setParam('WHEEL_D', 'param-WHEEL_D')">✓</button></div>
          <div class="field"><label>WHEEL_BASE</label><input type="number" id="param-WHEEL_BASE" step="0.1" min="5.0" max="100.0"><button class="param-btn" onclick="setParam('WHEEL_BASE', 'param-WHEEL_BASE')">✓</button></div>
        </div>
      </div>

      <!-- System Tab (With Guided Calibration & Reset NVS) -->
      <div id="tab-sys" class="tab-content" style="display:none;">
        <!-- APF & Kecepatan -->
        <div style="background:var(--bg2); border:1px solid var(--border2); border-radius:4px; padding:10px; margin-bottom:12px;">
          <div style="font-size:10px; color:var(--text2); margin-bottom:6px; font-weight:600; letter-spacing:1px; text-transform:uppercase;">APF & Kecepatan</div>
          <div class="apf-row" style="display:flex; align-items:center; justify-content:space-between; padding:4px 0; border-bottom:1px solid var(--border2); margin-bottom:10px;">
            <div>
              <div class="apf-label" style="color:var(--text); font-size:11px; font-weight:600; letter-spacing:1px;">APF Obstacle</div>
              <div style="font-size:9px;color:var(--text2);font-family:'Share Tech Mono',monospace;margin-top:2px" id="apf-status-txt">AKTIF</div>
            </div>
            <button class="toggle on" id="apf-toggle" onclick="toggleAPF()"></button>
          </div>
          <div class="slider-wrap">
            <label style="display:flex; justify-content:space-between; align-items:center; font-size:11px; letter-spacing:2px; color:var(--text2); margin-bottom:6px;">
              <span>KECEPATAN MAX</span>
              <span class="slider-val" id="speed-val" style="font-family:'Share Tech Mono',monospace; color:var(--accent); font-size:14px;">22 cm/s</span>
            </label>
            <input type="range" id="speed-slider" min="8" max="50" value="22" oninput="updateSpeed(this.value)">
          </div>
        </div>

        <div style="font-size:10px; color:var(--text2); margin-bottom:4px; font-weight:600; letter-spacing:1px;">SET POSISI / ODOMETRI</div>
        <div style="display:grid; grid-template-columns:1fr 1fr 1fr; gap:4px; margin-bottom:8px;">
          <input type="number" id="sys-posx" placeholder="X (cm)">
          <input type="number" id="sys-posy" placeholder="Y (cm)">
          <input type="number" id="sys-posth" placeholder="Th (deg)">
        </div>
        <div style="display:flex; gap:4px; margin-bottom:10px;">
          <button class="btn btn-primary" onclick="setPose()" style="flex:1; padding:6px 4px; font-size:10px; height:auto; justify-content:center;">SET POSE</button>
          <button class="btn btn-neutral" onclick="resetPose()" style="flex:1; padding:6px 4px; font-size:10px; height:auto; justify-content:center;">RESET HOME</button>
        </div>
        
        <div style="border-top:1px solid var(--border2); padding-top:8px; display:flex; flex-direction:column; gap:6px;">
          <button class="btn btn-start" id="btn-cal-imu" onclick="runImuCalibration()" style="padding:6px 10px; font-size:11px; height:auto; background:rgba(0,229,255,0.04); border-color:var(--accent); color:var(--accent); width:100%; justify-content:center;">
            🧭 CALIBRATE GYRO IMU
          </button>
          <div style="display:flex; align-items:center; justify-content:space-between; margin-top:2px;">
            <span style="font-size:9px; color:var(--text2); font-weight:600; letter-spacing:1px;">GUNAKAN IMU UNTUK HEADING</span>
            <input type="checkbox" id="chk-use-imu-th" onchange="toggleImuHeading()" style="cursor:pointer; width:16px; height:16px;">
          </div>
          <div style="display:flex; align-items:center; justify-content:space-between;">
            <span style="font-size:9px; color:var(--text2); font-weight:600; letter-spacing:1px;">COMPLEMENTARY ALPHA</span>
            <input type="number" id="param-comp_alpha" step="0.01" min="0.0" max="1.0" onchange="setParam('comp_alpha', 'param-comp_alpha')" style="width:60px; font-size:10px; padding:2px 4px; background:var(--bg0); border:1px solid var(--muted); border-radius:3px; color:var(--accent); text-align:center;">
          </div>
        </div>

        <!-- Guided Calibration Panel -->
        <div style="border-top:1px solid var(--border2); padding-top:10px; margin-top:10px;">
          <div style="font-size:10px; color:var(--text2); margin-bottom:6px; font-weight:600; letter-spacing:1px;">ASISTEN KALIBRASI ODOMETRI</div>
          
          <!-- Kalibrasi Jarak Linier (PPR) -->
          <div style="background:var(--bg2); border:1px solid var(--border2); border-radius:4px; padding:8px; margin-bottom:8px;">
            <div style="font-size:10px; color:var(--accent); font-weight:600; margin-bottom:4px;">1. Kalibrasi Jarak (Linear)</div>
            <div style="display:flex; gap:4px; align-items:center; margin-bottom:6px;">
              <div class="field" style="flex:1; margin-bottom:0;"><label style="font-size:9px; margin-bottom:2px; letter-spacing:1px;">Target (cm)</label><input type="number" id="cal-linear-target" value="100" style="font-size:12px; padding:4px 6px; height:24px;"></div>
              <button class="btn btn-neutral" id="btn-cal-dist-test" onclick="runLinearCalTest()" style="padding:4px 8px; font-size:9px; height:24px; margin-top:13px; justify-content:center; white-space:nowrap; flex:1;">TEST JALAN</button>
            </div>
            <div style="display:flex; gap:4px; align-items:center;">
              <input type="number" id="cal-linear-dist" placeholder="Jarak fisik asli (cm)" style="font-size:12px; padding:6px 4px; background:var(--bg0); border:1px solid var(--muted); border-radius:3px; color:var(--accent); width:100%;">
              <button class="param-btn" onclick="applyLinearCal()" style="position:static; transform:none; padding:6px 8px; white-space:nowrap; background:rgba(0,255,157,0.08); border-color:var(--accent2); color:var(--accent2);">✓ Set PPR</button>
            </div>
          </div>
          
          <!-- Kalibrasi Sudut Rotasi (Wheelbase) -->
          <div style="background:var(--bg2); border:1px solid var(--border2); border-radius:4px; padding:8px;">
            <div style="font-size:10px; color:var(--accent); font-weight:600; margin-bottom:4px;">2. Kalibrasi Sudut (Wheelbase)</div>
            <div style="display:flex; gap:4px; align-items:center; margin-bottom:6px;">
              <div class="field" style="flex:1; margin-bottom:0;"><label style="font-size:9px; margin-bottom:2px; letter-spacing:1px;">Target Putar (°)</label><input type="number" id="cal-rot-target" value="360" style="font-size:12px; padding:4px 6px; height:24px;"></div>
              <button class="btn btn-neutral" id="btn-cal-rot-test" onclick="runRotationCalTest()" style="padding:4px 8px; font-size:9px; height:24px; margin-top:13px; justify-content:center; white-space:nowrap; flex:1;">TEST PUTAR</button>
            </div>
            <div style="display:flex; gap:4px; align-items:center;">
              <input type="number" id="cal-rot-deg" placeholder="Sudut fisik asli (°)" style="font-size:12px; padding:6px 4px; background:var(--bg0); border:1px solid var(--muted); border-radius:3px; color:var(--accent); width:100%;">
              <button class="param-btn" onclick="applyRotationCal()" style="position:static; transform:none; padding:6px 8px; white-space:nowrap; background:rgba(0,255,157,0.08); border-color:var(--accent2); color:var(--accent2);">✓ Set Base</button>
            </div>
          </div>
        </div>

        <div style="border-top:1px solid var(--border2); padding-top:8px; margin-top:8px;">
          <button class="btn btn-stop" onclick="resetNVS()" style="padding:6px 10px; font-size:11px; height:auto; background:rgba(255,61,61,0.04); border-color:var(--danger); color:var(--danger); width:100%; justify-content:center;">
            ⚠️ RESET PARAMETER NVS
          </button>
        </div>
      </div>

      <!-- LOGGER Tab -->
      <div id="tab-log-csv" class="tab-content" style="display:none;">
        <div style="font-size:10px; color:var(--text2); margin-bottom:4px; font-weight:600; letter-spacing:1px;">CSV DATA LOGGER</div>
        
        <div style="background:var(--bg2); border:1px solid var(--border2); border-radius:4px; padding:8px; margin-bottom:10px;">
          <div style="display:flex; justify-content:space-between; align-items:center; margin-bottom:6px;">
            <span style="font-size:11px; color:var(--text2);">Status Logger:</span>
            <span id="log-status-lbl" style="font-family:'Share Tech Mono',monospace; font-size:11px; color:var(--accent2);">SIAP</span>
          </div>
          <div style="display:flex; justify-content:space-between; align-items:center;">
            <span style="font-size:11px; color:var(--text2);">Sampel Tercatat:</span>
            <span id="log-sample-counter" style="font-family:'Share Tech Mono',monospace; font-size:14px; color:var(--warn); font-weight:bold;">0</span>
          </div>
        </div>

        <div style="margin-bottom:10px;">
          <label style="display:block; font-size:10px; letter-spacing:1px; color:var(--text2); margin-bottom:4px;">INTERVAL POLLING (ms)</label>
          <select id="log-interval-select" onchange="changeLogInterval(this.value)" style="width:100%; background:var(--bg2); border:1px solid var(--muted); border-radius:4px; color:var(--accent); font-family:'Share Tech Mono',monospace; font-size:14px; padding:6px;">
            <option value="100">100 ms (Sangat Cepat)</option>
            <option value="200">200 ms (Cepat)</option>
            <option value="500">500 ms (Sedang)</option>
            <option value="1000" selected>1000 ms (Standar)</option>
            <option value="2000">2000 ms (Lambat)</option>
          </select>
        </div>

        <div style="display:flex; flex-direction:column; gap:6px;">
          <button class="btn btn-start" id="btn-log-start" onclick="startLoggingJs()" style="padding:8px; font-size:11px; height:auto; justify-content:center;">
            START LOGGING
          </button>
          <button class="btn btn-stop" id="btn-log-stop" onclick="stopLoggingJs()" style="padding:8px; font-size:11px; height:auto; justify-content:center;" disabled>
            STOP & DOWNLOAD CSV
          </button>
          <div style="display:flex; gap:4px;">
            <button class="btn btn-primary" id="btn-log-auto" onclick="armAutoLoggingJs()" style="flex:1; padding:8px 4px; font-size:10px; height:auto; justify-content:center;">
              ARM AUTO
            </button>
            <button class="btn btn-neutral" id="btn-log-cancel" onclick="cancelAutoLoggingJs()" style="flex:1; padding:8px 4px; font-size:10px; height:auto; justify-content:center;" disabled>
              CANCEL AUTO
            </button>
          </div>
        </div>
      </div>
    </div>

    <div class="card">
      <div class="card-title">Sensor Detail</div>
      <div id="sensor-list" style="display:grid;grid-template-columns:1fr 1fr;gap:4px;">
        <!-- Filled by JS -->
      </div>
    </div>
  </div>
</main>

<script>
const DEMO = (window.location.protocol === 'file:' || window.location.hostname === 'localhost' || window.location.hostname === '127.0.0.1');
let isSimulator = DEMO;
let robotIp = (window.location.protocol === 'http:' || window.location.protocol === 'https:') ? window.location.hostname : '192.168.4.1';

const getRobotUrl = (path) => {
  if (!DEMO) {
    return path;
  }
  return `http://${robotIp}${path}`;
};

let waypoints = [];
let robotPos = { x: 0, y: 0, theta: Math.PI / 2 };
let robotState = 'IDLE';
let stepModeActive = false;
let nextStateAfterPause = 'IDLE';
let sensorData = [999, 999, 999, 999, 999, 999, 999, 999];
let apfOn = true;
let currentWP = 0;
let autoTrail = [];
let manualTrail = [];

// Parameter Default Robot untuk Keperluan Mode Demo
let PPR_L = 940.0;
let PPR_R = 925.0;
let WHEEL_D = 6.8;
let WHEEL_BASE = 27.4;
let K_ATT = 1.5;
let K_REP = 15.0;
let R_GOAL = 2.0;
let S_GOAL = 20.0;
let R_OBS = 10.0;
let S_OBS = 30.0;
let Kp_rot_vel = 0.8;
let MIN_ROT_VEL = 4.0;
let MAX_ROT_VEL = 20.0;
let ROTATE_DONE_DEG = 4.0;
let ROTATE_DEAD_DEG = 2.0;
let ROTATE_BRAKE_DEG = 25.0;
let Kp_move = 0.35;
let Ki_move = 0.001;
let Kd_move = 0.01;
let Kp_vel = 3.0;
let Ki_vel = 0.8;
let VEL_FILTER_ALPHA = 0.25;
let MIN_MOVE_VEL = 8.0;
let MAX_MOVE_VEL = 22.0;
let SLOWDOWN_DIST = 20.0;
let MOVE_DONE_CM = 2.0;
let WAIT_MS = 300;
let LOOP_MS = 20;

// Posisi Obstacle Statis untuk Simulasi Medan Potensial (Mode Demo)
const mockObstacles = [
  { x: 30, y: 60, r: 12 },
  { x: -40, y: 100, r: 15 }
];

const distPoints = (p2, p1) => {
  return Math.sqrt((p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y));
};

let isLogging = false;
let isAutoLogArmed = false;
let loggedData = [];
let logSampleCount = 0;
let lastRobotStateForLog = 'IDLE';
let fetchIntervalId = null;
let currentFetchInterval = 1000;

// ================================================================
// PARAMETER MANAGEMENT FUNCTIONS
// ================================================================
const loadParameters = async () => {
  if (isSimulator) {
    const mockParams = {
      PPR_L, PPR_R, WHEEL_D, WHEEL_BASE,
      K_ATT, K_REP, R_GOAL, S_GOAL, R_OBS, S_OBS,
      Kp_rot_vel, MIN_ROT_VEL, MAX_ROT_VEL, ROTATE_DONE_DEG, ROTATE_DEAD_DEG, ROTATE_BRAKE_DEG,
      Kp_move, Ki_move, Kd_move, Kp_vel, Ki_vel, VEL_FILTER_ALPHA,
      MIN_MOVE_VEL, MAX_MOVE_VEL, SLOWDOWN_DIST, MOVE_DONE_CM, WAIT_MS, LOOP_MS
    };
    fillParamFields(mockParams);
    addLog('Parameters loaded successfully (SIMULATOR MODE)', 'ok');
    return;
  }

  try {
    const res = await fetch(getRobotUrl('/params'));
    if (!res.ok) {
      console.warn('Failed to fetch params: HTTP ' + res.status);
      return;
    }
    const data = await res.json();
    fillParamFields(data);
    addLog('Parameters loaded successfully from ESP32', 'ok');
  } catch (e) {
    console.error('Load params error:', e);
  }
};

const fillParamFields = (data) => {
  document.getElementById('param-PPR_L').value = data.PPR_L || '';
  document.getElementById('param-PPR_R').value = data.PPR_R || '';
  document.getElementById('param-WHEEL_D').value = data.WHEEL_D || '';
  document.getElementById('param-WHEEL_BASE').value = data.WHEEL_BASE || '';
  
  document.getElementById('param-K_ATT').value = data.K_ATT || '';
  document.getElementById('param-K_REP').value = data.K_REP || '';
  document.getElementById('param-R_GOAL').value = data.R_GOAL || '';
  document.getElementById('param-S_GOAL').value = data.S_GOAL || '';
  document.getElementById('param-R_OBS').value = data.R_OBS || '';
  document.getElementById('param-S_OBS').value = data.S_OBS || '';
  
  document.getElementById('param-Kp_rot_vel').value = data.Kp_rot_vel || '';
  document.getElementById('param-MIN_ROT_VEL').value = data.MIN_ROT_VEL || '';
  document.getElementById('param-MAX_ROT_VEL').value = data.MAX_ROT_VEL || '';
  document.getElementById('param-ROTATE_DONE_DEG').value = data.ROTATE_DONE_DEG || '';
  document.getElementById('param-ROTATE_DEAD_DEG').value = data.ROTATE_DEAD_DEG || '';
  document.getElementById('param-ROTATE_BRAKE_DEG').value = data.ROTATE_BRAKE_DEG || '';
  
  document.getElementById('param-Kp_move').value = data.Kp_move || '';
  document.getElementById('param-Ki_move').value = data.Ki_move || '';
  document.getElementById('param-Kd_move').value = data.Kd_move || '';
  
  document.getElementById('param-Kp_vel').value = data.Kp_vel || '';
  document.getElementById('param-Ki_vel').value = data.Ki_vel || '';
  
  document.getElementById('param-VEL_FILTER_ALPHA').value = data.VEL_FILTER_ALPHA || '';
  
  document.getElementById('param-MIN_MOVE_VEL').value = data.MIN_MOVE_VEL || '';
  document.getElementById('param-MAX_MOVE_VEL').value = data.MAX_MOVE_VEL || '';
  document.getElementById('param-SLOWDOWN_DIST').value = data.SLOWDOWN_DIST || '';
  document.getElementById('param-MOVE_DONE_CM').value = data.MOVE_DONE_CM || '';
  
  document.getElementById('param-WAIT_MS').value = data.WAIT_MS || '';
  document.getElementById('param-LOOP_MS').value = data.LOOP_MS || '';
  
  const useImuTh = data.use_imu_th === true || data.use_imu_th === "true";
  const chk = document.getElementById('chk-use-imu-th');
  if (chk) chk.checked = useImuTh;

  const compAlpha = data.comp_alpha !== undefined ? data.comp_alpha : '';
  const compInput = document.getElementById('param-comp_alpha');
  if (compInput) compInput.value = compAlpha;
};

const setParam = async (paramName, fieldId) => {
  const element = document.getElementById(fieldId);
  if (!element) return;
  
  let value = parseFloat(element.value);
  if (isNaN(value)) {
    addLog('ERR: Nilai input tidak valid!', 'err');
    return;
  }

  const min = parseFloat(element.min);
  const max = parseFloat(element.max);
  if (!isNaN(min) && value < min) { value = min; element.value = min; }
  if (!isNaN(max) && value > max) { value = max; element.value = max; }
  
  if (isSimulator) {
    updateLocalParam(paramName, value);
    addLog(`✓ ${paramName} = ${value} (SIMULATION MODE)`, 'ok');
    element.style.background = 'rgba(0,255,157,0.1)';
    setTimeout(() => { element.style.background = 'var(--bg2)'; }, 300);
    return;
  }
  
  try {
    const res = await fetch(getRobotUrl('/setparam?name=' + paramName + '&value=' + value));
    const data = await res.json();
    
    if (data.status === 'ok') {
      addLog('✓ ' + paramName + ' = ' + value, 'ok');
      element.style.background = 'rgba(0,255,157,0.1)';
      setTimeout(() => { element.style.background = 'var(--bg2)'; }, 300);
    } else {
      addLog('ERR: ' + (data.error || 'Unknown error'), 'err');
    }
  } catch (e) {
    addLog('ERR: Gagal mengirim parameter: ' + e.message, 'err');
  }
};

const updateLocalParam = (name, value) => {
  if (name === "PPR_L") PPR_L = value;
  else if (name === "PPR_R") PPR_R = value;
  else if (name === "WHEEL_D") WHEEL_D = value;
  else if (name === "WHEEL_BASE") WHEEL_BASE = value;
  else if (name === "K_ATT") K_ATT = value;
  else if (name === "K_REP") K_REP = value;
  else if (name === "R_GOAL") R_GOAL = value;
  else if (name === "S_GOAL") S_GOAL = value;
  else if (name === "R_OBS") R_OBS = value;
  else if (name === "S_OBS") S_OBS = value;
  else if (name === "Kp_rot_vel") Kp_rot_vel = value;
  else if (name === "MIN_ROT_VEL") MIN_ROT_VEL = value;
  else if (name === "MAX_ROT_VEL") MAX_ROT_VEL = value;
  else if (name === "ROTATE_DONE_DEG") ROTATE_DONE_DEG = value;
  else if (name === "ROTATE_DEAD_DEG") ROTATE_DEAD_DEG = value;
  else if (name === "ROTATE_BRAKE_DEG") ROTATE_BRAKE_DEG = value;
  else if (name === "Kp_move") Kp_move = value;
  else if (name === "Ki_move") Ki_move = value;
  else if (name === "Kd_move") Kd_move = value;
  else if (name === "Kp_vel") Kp_vel = value;
  else if (name === "Ki_vel") Ki_vel = value;
  else if (name === "VEL_FILTER_ALPHA") VEL_FILTER_ALPHA = value;
  else if (name === "MIN_MOVE_VEL") MIN_MOVE_VEL = value;
  else if (name === "MAX_MOVE_VEL") MAX_MOVE_VEL = value;
  else if (name === "SLOWDOWN_DIST") SLOWDOWN_DIST = value;
  else if (name === "MOVE_DONE_CM") MOVE_DONE_CM = value;
  else if (name === "WAIT_MS") WAIT_MS = value;
  else if (name === "LOOP_MS") LOOP_MS = value;
};

// ================================================================
// PROTEKSI STATE-LOCKING (KUNCI PARAMETER SAAT BERGERAK)
// ================================================================
const updateStateLockUI = () => {
  const isMoving = (robotState === 'MOVE' || robotState === 'ROTATE' || robotState === 'WAIT' || robotState === 'PAUSED');
  
  const sensitiveInputs = [
    'param-PPR_L', 'param-PPR_R', 'param-WHEEL_D', 'param-WHEEL_BASE',
    'sys-posx', 'sys-posy', 'sys-posth', 'wp-x', 'wp-y',
    'cal-linear-target', 'cal-rot-target'
  ];
  
  sensitiveInputs.forEach(id => {
    const el = document.getElementById(id);
    if (el) el.disabled = isMoving;
  });

  const sensitiveButtons = ['btn-cal-imu', 'btn-cal-dist-test', 'btn-cal-rot-test'];
  sensitiveButtons.forEach(id => {
    const el = document.getElementById(id);
    if (el) el.disabled = isMoving;
  });
};

// ================================================================
// KALIBRASI IMU
// ================================================================
const runImuCalibration = async () => {
  if (robotState !== 'IDLE' && robotState !== 'FINISHED') {
    addLog('WARN: Kalibrasi IMU dibatalkan. Robot harus dalam keadaan IDLE/DIAM!', 'warn');
    return;
  }

  const modal = document.getElementById('calibration-modal');
  const countdownEl = document.getElementById('cal-countdown');
  modal.style.display = 'flex';
  
  let timer = 2.0;
  countdownEl.textContent = timer.toFixed(1) + 's';
  
  addLog('🧭 Kalibrasi giroskop IMU dimulai... Harap diamkan robot!', 'warn');
  
  if (!isSimulator) {
    sendCmd('CALIMU');
  }
  
  const interval = setInterval(() => {
    timer -= 0.1;
    if (timer <= 0) {
      clearInterval(interval);
      modal.style.display = 'none';
      addLog('✓ Kalibrasi IMU Selesai.', 'ok');
    } else {
      countdownEl.textContent = timer.toFixed(1) + 's';
    }
  }, 100);
};

const toggleImuHeading = async () => {
  const chk = document.getElementById('chk-use-imu-th');
  const value = chk.checked ? 1 : 0;
  if (isSimulator) {
    addLog(`IMU Heading set to: ${chk.checked ? "ON" : "OFF"} (SIMULATOR)`, 'ok');
    return;
  }
  try {
    const res = await fetch(getRobotUrl('/setparam?name=use_imu_th&value=' + value));
    const d = await res.json();
    if (d.status === "ok") {
      addLog(`IMU Heading set to: ${chk.checked ? "ON" : "OFF"}`, 'ok');
    }
  } catch (e) {
    addLog('ERR: Gagal mengubah setting IMU Heading: ' + e.message, 'err');
    chk.checked = !chk.checked;
  }
};


// ================================================================
// GUIDED CALIBRATION (ASISTEN KALIBRASI ODOMETRI)
// ================================================================
const runLinearCalTest = () => {
  const target = parseFloat(document.getElementById('cal-linear-target').value) || 100.0;
  addLog(`Menjalankan uji kalibrasi linier: Robot diperintahkan maju sejauh ${target} cm...`, 'warn');
  if (isSimulator) {
    waypoints = [{ x: 0, y: target }];
    currentWP = 0;
    robotState = 'ROTATE';
    addLog(`Simulasi Uji Jalan ${target}cm dimulai.`, 'ok');
    return;
  }
  
  sendCmd('CLEAR');
  setTimeout(() => sendCmd('SETPOSE 0 0 90'), 100);
  setTimeout(() => sendCmd(`ADD 0 ${target}`), 200);
  setTimeout(() => sendCmd('START'), 300);
};

const applyLinearCal = () => {
  const actualDist = parseFloat(document.getElementById('cal-linear-dist').value);
  if (isNaN(actualDist) || actualDist <= 10.0) {
    alert("Masukkan jarak fisik aktual yang valid (dalam cm)!");
    return;
  }

  const targetDist = parseFloat(document.getElementById('cal-linear-target').value) || 100.0;
  const currentPpr = parseFloat(document.getElementById('param-PPR_L').value) || PPR_L;
  
  const newPprL = Math.round(currentPpr * (actualDist / targetDist));
  const newPprR = Math.round((parseFloat(document.getElementById('param-PPR_R').value) || PPR_R) * (actualDist / targetDist));

  addLog(`Menghitung kalibrasi jarak: Jarak aktual = ${actualDist} cm (Target: ${targetDist} cm)`, 'ok');
  addLog(`PPR Baru hasil kalkulasi -> Kiri: ${newPprL} | Kanan: ${newPprR}`, 'warn');

  document.getElementById('param-PPR_L').value = newPprL;
  document.getElementById('param-PPR_R').value = newPprR;
  
  setParam('PPR_L', 'param-PPR_L');
  setTimeout(() => setParam('PPR_R', 'param-PPR_R'), 300);
  document.getElementById('cal-linear-dist').value = '';
};

const runRotationCalTest = () => {
  const target = parseFloat(document.getElementById('cal-rot-target').value) || 360.0;
  addLog(`Menjalankan uji kalibrasi rotasi: Robot diperintahkan berputar ${target} derajat...`, 'warn');
  if (isSimulator) {
    robotState = 'ROTATE';
    let startTh = robotPos.theta;
    let targetRad = target * Math.PI / 180;
    const rotInterval = setInterval(() => {
      robotPos.theta += 0.15;
      if (robotPos.theta >= startTh + targetRad) {
        clearInterval(rotInterval);
        robotState = 'IDLE';
        addLog(`Simulasi Uji Putar ${target}° Selesai.`, 'ok');
      }
    }, 50);
    return;
  }

  sendCmd('MANUAL -15 15');
  
  let startTheta = robotPos.theta;
  let accumulatedTurn = 0;
  let lastTh = startTheta;
  let targetRad = target * Math.PI / 180;
  let lastWatchdogFeedTime = Date.now();
  
  const checkTurn = setInterval(() => {
    let diff = robotPos.theta - lastTh;
    while (diff > Math.PI) diff -= Math.PI * 2;
    while (diff < -Math.PI) diff += Math.PI * 2;
    accumulatedTurn += Math.abs(diff);
    lastTh = robotPos.theta;
    
    // Feed the watchdog by sending the manual command periodically (every 200ms)
    let now = Date.now();
    if (now - lastWatchdogFeedTime > 200) {
      lastWatchdogFeedTime = now;
      sendCmd('MANUAL -15 15');
    }
    
    if (accumulatedTurn >= targetRad - (5 * Math.PI / 180)) {
      clearInterval(checkTurn);
      sendCmd('STOP');
      addLog('Putaran robot dihentikan. Silakan ukur sudut hadap fisik akhir robot!', 'ok');
    }
  }, 50);
};

const applyRotationCal = () => {
  const actualDeg = parseFloat(document.getElementById('cal-rot-deg').value);
  if (isNaN(actualDeg) || actualDeg <= 45) {
    alert("Masukkan sudut fisik aktual yang valid (dalam derajat)!");
    return;
  }

  const targetDeg = parseFloat(document.getElementById('cal-rot-target').value) || 360.0;
  const currentBase = parseFloat(document.getElementById('param-WHEEL_BASE').value) || WHEEL_BASE;
  
  const newBase = parseFloat((currentBase * (targetDeg / actualDeg)).toFixed(3));

  addLog(`Menghitung kalibrasi sudut: Sudut aktual = ${actualDeg}° (Target: ${targetDeg}°)`, 'ok');
  addLog(`Wheel Base Baru hasil kalkulasi: ${newBase} cm`, 'warn');

  document.getElementById('param-WHEEL_BASE').value = newBase;
  setParam('WHEEL_BASE', 'param-WHEEL_BASE');
  document.getElementById('cal-rot-deg').value = '';
};

// ================================================================
// RESET NVS
// ================================================================
const resetNVS = () => {
  if (confirm("Apakah Anda yakin ingin mereset seluruh parameter NVS ke default bawaan kode? Robot akan restart otomatis.")) {
    if (isSimulator) {
      addLog('⚠️ NVS parameters reset (SIMULATOR DEFAULT)', 'err');
      PPR_L = 940.0; PPR_R = 925.0; WHEEL_BASE = 27.4;
      K_ATT = 1.5; K_REP = 15.0; R_GOAL = 2.0;
      loadParameters();
      return;
    }
    sendCmd('RESETNVS');
  }
};

// ================================================================
// AUTO TRAIL & MANUAL LOGGING CSV
// ================================================================
const startFetchStatusLoop = (ms) => {
  if (fetchIntervalId) clearInterval(fetchIntervalId);
  currentFetchInterval = ms;
  fetchIntervalId = setInterval(fetchStatus, ms);
};

const changeLogInterval = (val) => {
  const ms = parseInt(val);
  startFetchStatusLoop(ms);
  addLog(`Interval polling diubah ke ${ms}ms.`, 'ok');
};

const updateLoggerStatusDisplay = (statusText, colorVar) => {
  const lbl = document.getElementById('log-status-lbl');
  if (lbl) {
    lbl.textContent = statusText;
    lbl.style.color = colorVar || 'var(--accent2)';
  }
};

const startLoggingJs = () => {
  if (isLogging) return;
  loggedData = [];
  logSampleCount = 0;
  document.getElementById('log-sample-counter').textContent = "0";
  isLogging = true;
  isAutoLogArmed = false;
  
  document.getElementById('btn-log-start').disabled = true;
  document.getElementById('btn-log-stop').disabled = false;
  document.getElementById('btn-log-auto').disabled = true;
  document.getElementById('btn-log-cancel').disabled = true;
  
  updateLoggerStatusDisplay("MENCATAT...", 'var(--accent)');
  addLog("Logging manual dimulai di web browser.", 'ok');
};

const stopLoggingJs = () => {
  if (!isLogging) return;
  isLogging = false;
  
  document.getElementById('btn-log-start').disabled = false;
  document.getElementById('btn-log-stop').disabled = true;
  document.getElementById('btn-log-auto').disabled = false;
  document.getElementById('btn-log-cancel').disabled = true;
  
  updateLoggerStatusDisplay("BERHENTI. MENGUNDUH...");
  addLog("Logging dihentikan. Mengunduh CSV...", 'ok');
  downloadCSV();
};

const armAutoLoggingJs = () => {
  loggedData = [];
  logSampleCount = 0;
  document.getElementById('log-sample-counter').textContent = "0";
  isLogging = false;
  isAutoLogArmed = true;
  
  document.getElementById('btn-log-start').disabled = true;
  document.getElementById('btn-log-stop').disabled = true;
  document.getElementById('btn-log-auto').disabled = true;
  document.getElementById('btn-log-cancel').disabled = false;
  
  updateLoggerStatusDisplay("AUTO ARMED (MENUNGGU)", 'var(--warn)');
  addLog("Auto Logging bersiap. Menunggu robot bergerak.", 'ok');
};

const cancelAutoLoggingJs = () => {
  isLogging = false;
  isAutoLogArmed = false;
  
  document.getElementById('btn-log-start').disabled = false;
  document.getElementById('btn-log-stop').disabled = true;
  document.getElementById('btn-log-auto').disabled = false;
  document.getElementById('btn-log-cancel').disabled = true;
  
  updateLoggerStatusDisplay("SIAP");
  addLog("Auto Logging dibatalkan.", 'ok');
};

const recordLogSample = (d) => {
  const now = new Date().toISOString();
  const sensors = d.sensors || [999,999,999,999,999,999,999,999];
  const row = [
    now,
    d.millis || '',
    d.state || '',
    d.x !== undefined ? d.x : '',
    d.y !== undefined ? d.y : '',
    d.th !== undefined ? d.th : '',
    d.velL !== undefined ? d.velL : '',
    d.velR !== undefined ? d.velR : '',
    d.targetVelL !== undefined ? d.targetVelL : '',
    d.targetVelR !== undefined ? d.targetVelR : '',
    d.wp !== undefined ? d.wp : '',
    d.total !== undefined ? d.total : '',
    d.apf !== undefined ? d.apf : '',
    d.krep !== undefined ? d.krep : '',
    d.speedMax !== undefined ? d.speedMax : '',
    ...sensors
  ];
  loggedData.push(row);
  logSampleCount = loggedData.length;
  document.getElementById('log-sample-counter').textContent = logSampleCount;
};

const downloadCSV = () => {
  if (loggedData.length === 0) {
    alert("Tidak ada data yang dicatat!");
    return;
  }
  const headers = [
    "time_laptop", "millis", "state", "x", "y", "th", "velL", "velR",
    "targetVelL", "targetVelR", "wp", "total", "apf", "krep", "speedMax",
    "s0_depan", "s1_depan_kanan", "s2_kanan", "s3_belakang_kanan",
    "s4_belakang", "s5_belakang_kiri", "s6_kiri", "s7_depan_kiri"
  ];
  
  let csvRows = [headers.join(",")];
  loggedData.forEach(row => {
    const escapedRow = row.map(val => {
      const str = String(val);
      if (str.includes(",") || str.includes("\"") || str.includes("\n")) {
        return `"${str.replace(/"/g, '""')}"`;
      }
      return str;
    });
    csvRows.push(escapedRow.join(","));
  });
  
  const csvString = csvRows.join("\n");
  const blob = new Blob([csvString], { type: "text/csv;charset=utf-8;" });
  const url = URL.createObjectURL(blob);
  
  const link = document.createElement("a");
  const stamp = new Date().toISOString().replace(/T/, "_").replace(/\..+/, "").replace(/[:]/g, "-");
  link.setAttribute("href", url);
  link.setAttribute("download", `data_robot_${stamp}.csv`);
  link.style.visibility = 'hidden';
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);
  
  loggedData = [];
  logSampleCount = 0;
  document.getElementById('log-sample-counter').textContent = "0";
  updateLoggerStatusDisplay("SIAP");
};

const SENSOR_LABELS = ['DEP','D-KAN','KAN','B-KAN','BEL','B-KIR','KIR','D-KIR'];
const SENSOR_ANGLES_DEG = [0, -45, -90, -135, 180, 135, 90, 45];

const sendCmd = async (cmd) => {
  addLog(cmd, 'cmd');
  if (isSimulator) {
    processMockCommand(cmd);
    return;
  }
  try {
    const res = await fetch(getRobotUrl('/cmd?v=' + encodeURIComponent(cmd)));
    const txt = await res.text();
    if (txt !== "OK") addLog(txt, 'ok');
  } catch (e) {
    addLog('ERR: ' + e.message, 'err');
  }
};

const fetchStatus = async () => {
  if (isSimulator) {
    updateDemoSimulation();
    return;
  }
  try {
    const res = await fetch(getRobotUrl('/status'));
    const d = await res.json();
    updateUI(d);
    if (d.sensors) updateSensors(d.sensors);
    document.getElementById('conn-dot').style.background = 'var(--accent2)';
    document.getElementById('conn-text').textContent = 'CONNECTED';
  } catch (e) {
    document.getElementById('conn-dot').style.background = 'var(--danger)';
    document.getElementById('conn-text').textContent = 'OFFLINE';
  }
};

const updateUI = (d) => {
  document.getElementById('tel-x').textContent = parseFloat(d.x).toFixed(1);
  document.getElementById('tel-y').textContent = parseFloat(d.y).toFixed(1);
  document.getElementById('tel-th').textContent = parseFloat(d.th).toFixed(1);
  document.getElementById('tel-wp').textContent = d.wp + '/' + d.total;

  robotPos.x = parseFloat(d.x);
  robotPos.y = parseFloat(d.y);
  robotPos.theta = parseFloat(d.th) * Math.PI / 180;
  currentWP = parseInt(d.wp);
  robotState = d.state;

  if (d.use_imu_th !== undefined) {
    const chk = document.getElementById('chk-use-imu-th');
    if (chk) chk.checked = (d.use_imu_th === true || d.use_imu_th === "true" || d.use_imu_th === 1);
  }

  updateStateLockUI();

  if (isLogging) {
    recordLogSample(d);
  } else if (isAutoLogArmed) {
    const activeStates = ["ROTATE", "WAIT", "MOVE", "MANUAL"];
    if (activeStates.includes(d.state)) {
      isLogging = true;
      loggedData = [];
      logSampleCount = 0;
      updateLoggerStatusDisplay("Logging Otomatis Berjalan...", "var(--accent)");
      recordLogSample(d);
    }
  }

  if (isLogging && isAutoLogArmed) {
    const activeStates = ["ROTATE", "WAIT", "MOVE", "MANUAL"];
    const stoppedByFinish = d.state === "FINISHED";
    const stoppedByIdle = d.state === "IDLE" && activeStates.includes(lastRobotStateForLog);
    if (stoppedByFinish || stoppedByIdle) {
      isLogging = false;
      isAutoLogArmed = false;
      updateLoggerStatusDisplay("Auto Selesai. Mengunduh CSV...");
      downloadCSV();
    }
  }
  lastRobotStateForLog = d.state;

  const current_pos = { x: robotPos.x, y: robotPos.y };
  if (robotState === 'MANUAL') {
    if (manualTrail.length === 0 || distPoints(manualTrail[manualTrail.length - 1], current_pos) > 1.0) {
      manualTrail.push(current_pos);
      if (manualTrail.length > 2000) manualTrail.shift();
    }
  } else if (robotState === 'MOVE' || robotState === 'ROTATE' || robotState === 'WAIT') {
    if (autoTrail.length === 0 || distPoints(autoTrail[autoTrail.length - 1], current_pos) > 1.0) {
      autoTrail.push(current_pos);
      if (autoTrail.length > 2000) autoTrail.shift();
    }
  }

  updateStateDisplay(d.state);
  updateCompass(parseFloat(d.th));
  renderMap();
  renderWaypointList();
};

const updateStateDisplay = (state) => {
  const dot = document.getElementById('state-dot');
  const txt = document.getElementById('state-text');
  txt.textContent = state;
  dot.className = 'status-dot';
  if (state === 'MOVE') { dot.classList.add('active'); }
  else if (state === 'IDLE' || state === 'FINISHED') { dot.classList.add('idle'); }
  else if (state === 'ROTATE' || state === 'WAIT' || state === 'PAUSED') { dot.classList.add('warn'); }
  else if (state === 'MANUAL') { dot.classList.add('warn'); }

  const resumeBtn = document.getElementById('btn-resume-nav');
  if (resumeBtn) {
    if (state === 'PAUSED') {
      resumeBtn.disabled = false;
      resumeBtn.style.animation = 'pulse 1s infinite alternate';
      resumeBtn.style.background = 'rgba(0, 255, 157, 0.18)';
    } else {
      resumeBtn.disabled = true;
      resumeBtn.style.animation = 'none';
      resumeBtn.style.background = 'rgba(0, 255, 157, 0.05)';
    }
  }
};

const updateCompass = (deg) => {
  const needle = document.getElementById('compass-needle');
  needle.setAttribute('transform', 'rotate(' + (-(deg - 90)) + ' 70 70)');
  document.getElementById('compass-deg').textContent = deg.toFixed(1).padStart(5, '0') + '°';
};

const updateSensors = (data) => {
  sensorData = data;
  drawSensorRadar();
  renderSensorList();
  const minVal = Math.min(...data.filter(v => v < 900));
  const minEl = document.getElementById('sensor-min-val');
  if (minEl) {
    minEl.textContent = isFinite(minVal) ? minVal.toFixed(0) : '--';
  }
};

const drawSensorRadar = () => {
  const canvas = document.getElementById('sensor-canvas');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  const cx = 90, cy = 90, r = 78;
  ctx.clearRect(0, 0, 180, 180);

  [0.25, 0.5, 0.75, 1].forEach(f => {
    ctx.beginPath();
    ctx.arc(cx, cy, r * f, 0, Math.PI * 2);
    ctx.strokeStyle = 'rgba(0,229,255,0.08)';
    ctx.lineWidth = 1;
    ctx.stroke();
  });

  for (let i = 0; i < 8; i++) {
    const a = (i * 45 - 90) * Math.PI / 180;
    ctx.beginPath();
    ctx.moveTo(cx, cy);
    ctx.lineTo(cx + Math.cos(a) * r, cy + Math.sin(a) * r);
    ctx.strokeStyle = 'rgba(0,229,255,0.06)';
    ctx.lineWidth = 1;
    ctx.stroke();
  }

  ctx.beginPath();
  for (let i = 0; i < 8; i++) {
    const a = (-SENSOR_ANGLES_DEG[i] - 90) * Math.PI / 180;
    const d = sensorData[i];
    const dist = d > 100 ? r : (d / 100) * r;
    const x = cx + Math.cos(a) * dist;
    const y = cy + Math.sin(a) * dist;
    i === 0 ? ctx.moveTo(x, y) : ctx.lineTo(x, y);
  }
  ctx.closePath();
  ctx.fillStyle = 'rgba(0,229,255,0.07)';
  ctx.strokeStyle = 'rgba(0,229,255,0.5)';
  ctx.lineWidth = 1.5;
  ctx.fill();
  ctx.stroke();

  for (let i = 0; i < 8; i++) {
    const a = (-SENSOR_ANGLES_DEG[i] - 90) * Math.PI / 180;
    const d = sensorData[i];
    const dist = d > 100 ? r : (d / 100) * r;
    const x = cx + Math.cos(a) * dist;
    const y = cy + Math.sin(a) * dist;
    ctx.beginPath();
    ctx.arc(x, y, 3, 0, Math.PI * 2);
    const danger = d < 40;
    ctx.fillStyle = danger ? '#ff3d3d' : '#00e5ff';
    ctx.shadowColor = danger ? '#ff3d3d' : '#00e5ff';
    ctx.shadowBlur = 6;
    ctx.fill();
    ctx.shadowBlur = 0;
  }

  ctx.beginPath();
  ctx.arc(cx, cy, 5, 0, Math.PI * 2);
  ctx.fillStyle = '#00ff9d';
  ctx.shadowColor = '#00ff9d';
  ctx.shadowBlur = 8;
  ctx.fill();
  ctx.shadowBlur = 0;
};

const renderSensorList = () => {
  const el = document.getElementById('sensor-list');
  el.innerHTML = SENSOR_LABELS.map((name, i) => {
    const v = sensorData[i];
    const safe = v > 100;
    const warn = v <= 40;
    const col = safe ? 'var(--text2)' : warn ? 'var(--danger)' : 'var(--warn)';
    const txt = safe ? '>100' : v.toFixed(0);
    return `<div style="background:var(--bg2);border:1px solid var(--border2);border-radius:3px;padding:4px 8px;display:flex;justify-content:space-between;align-items:center">
      <span style="font-family:'Share Tech Mono',monospace;font-size:10px;color:var(--text2)">${name}</span>
      <span style="font-family:'Share Tech Mono',monospace;font-size:12px;color:${col}">${txt}</span>
    </div>`;
  }).join('');
};

let mapScale = 2;
let mapOffX = 0, mapOffY = 0;

const resizeMap = () => {
  const canvas = document.getElementById('map-canvas');
  if (!canvas) return;
  canvas.width = canvas.clientWidth;
  canvas.height = canvas.clientWidth * 10 / 16;
  mapOffX = canvas.width / 2;
  mapOffY = canvas.height / 2;
  renderMap();
};

const renderMap = () => {
  const canvas = document.getElementById('map-canvas');
  if (!canvas) return;
  const ctx = canvas.getContext('2d');
  const W = canvas.width, H = canvas.height;
  if (W === 0 || H === 0) return;

  ctx.clearRect(0, 0, W, H);

  ctx.strokeStyle = 'rgba(0,229,255,0.04)';
  ctx.lineWidth = 1;
  const gridCm = 50;
  const gridPx = gridCm * mapScale;
  const ox = mapOffX % gridPx, oy = mapOffY % gridPx;
  for (let x = ox; x < W; x += gridPx) { ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, H); ctx.stroke(); }
  for (let y = oy; y < H; y += gridPx) { ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(W, y); ctx.stroke(); }

  ctx.strokeStyle = 'rgba(0,229,255,0.15)';
  ctx.lineWidth = 1;
  ctx.setLineDash([4, 4]);
  ctx.beginPath(); ctx.moveTo(mapOffX, 0); ctx.lineTo(mapOffX, H); ctx.stroke();
  ctx.beginPath(); ctx.moveTo(0, mapOffY); ctx.lineTo(W, mapOffY); ctx.stroke();
  ctx.setLineDash([]);

  ctx.font = '9px Share Tech Mono, monospace';
  ctx.fillStyle = 'rgba(0,229,255,0.2)';
  for (let c = -200; c <= 200; c += 50) {
    if (c === 0) continue;
    const px = mapOffX + c * mapScale;
    const py = mapOffY - c * mapScale;
    if (px > 10 && px < W - 10) { ctx.fillText(c + 'cm', px + 2, mapOffY + 12); }
    if (py > 10 && py < H - 10) { ctx.fillText(c + 'cm', mapOffX + 3, py - 3); }
  }

  if (DEMO) {
    mockObstacles.forEach(obs => {
      const px = obs.x * mapScale + mapOffX;
      const py = -obs.y * mapScale + mapOffY;
      ctx.beginPath();
      ctx.arc(px, py, obs.r * mapScale, 0, Math.PI * 2);
      ctx.fillStyle = 'rgba(255, 61, 61, 0.15)';
      ctx.strokeStyle = 'rgba(255, 61, 61, 0.4)';
      ctx.lineWidth = 1;
      ctx.fill();
      ctx.stroke();
      ctx.fillStyle = 'rgba(255, 61, 61, 0.4)';
      ctx.fillText('OBSTACLE', px - 20, py + 3);
    });
  }

  if (waypoints.length > 0) {
    ctx.strokeStyle = '#eab308';
    ctx.lineWidth = 1.5;
    ctx.setLineDash([6, 4]);
    ctx.beginPath();
    ctx.moveTo(0 * mapScale + mapOffX, -0 * mapScale + mapOffY);
    waypoints.forEach(wp => {
      ctx.lineTo(wp.x * mapScale + mapOffX, -wp.y * mapScale + mapOffY);
    });
    ctx.stroke();
    ctx.setLineDash([]);
  }

  if (autoTrail.length > 1) {
    ctx.strokeStyle = '#00e5ff';
    ctx.lineWidth = 2.5;
    ctx.beginPath();
    ctx.moveTo(autoTrail[0].x * mapScale + mapOffX, -autoTrail[0].y * mapScale + mapOffY);
    for (let i = 1; i < autoTrail.length; i++) {
      ctx.lineTo(autoTrail[i].x * mapScale + mapOffX, -autoTrail[i].y * mapScale + mapOffY);
    }
    ctx.stroke();
  }

  if (manualTrail.length > 1) {
    ctx.strokeStyle = '#d946ef';
    ctx.lineWidth = 2.5;
    ctx.beginPath();
    ctx.moveTo(manualTrail[0].x * mapScale + mapOffX, -manualTrail[0].y * mapScale + mapOffY);
    for (let i = 1; i < manualTrail.length; i++) {
      ctx.lineTo(manualTrail[i].x * mapScale + mapOffX, -manualTrail[i].y * mapScale + mapOffY);
    }
    ctx.stroke();
  }

  waypoints.forEach((wp, i) => {
    const px = wp.x * mapScale + mapOffX;
    const py = -wp.y * mapScale + mapOffY;
    const done = i < currentWP;
    const active = i === currentWP;

    if (done) {
      ctx.beginPath(); ctx.arc(px, py, 5, 0, Math.PI * 2);
      ctx.fillStyle = 'rgba(0,229,255,0.2)'; ctx.fill();
    } else {
      ctx.beginPath(); ctx.arc(px, py, active ? 7 : 5, 0, Math.PI * 2);
      ctx.strokeStyle = active ? '#00e5ff' : 'rgba(0,229,255,0.5)';
      ctx.lineWidth = active ? 2 : 1;
      if (active) { ctx.shadowColor = '#00e5ff'; ctx.shadowBlur = 10; }
      ctx.stroke();
      ctx.shadowBlur = 0;

      if (active) {
        ctx.strokeStyle = 'rgba(0,229,255,0.4)'; ctx.lineWidth = 0.5;
        ctx.beginPath(); ctx.moveTo(px - 14, py); ctx.lineTo(px + 14, py); ctx.stroke();
        ctx.beginPath(); ctx.moveTo(px, py - 14); ctx.lineTo(px, py + 14); ctx.stroke();
      }
    }

    ctx.font = '10px Share Tech Mono, monospace';
    ctx.fillStyle = active ? '#00e5ff' : (done ? 'rgba(0,229,255,0.25)' : 'rgba(0,229,255,0.5)');
    ctx.fillText('#' + i, px + 9, py - 5);
  });

  const rx = robotPos.x * mapScale + mapOffX;
  const ry = -robotPos.y * mapScale + mapOffY;
  const th = robotPos.theta;

  // Draw 360 Sensor Radar directly around the robot model
  if (sensorData && sensorData.length === 8) {
    ctx.beginPath();
    for (let i = 0; i < 8; i++) {
      const a = th + (SENSOR_ANGLES_DEG[i] * Math.PI / 180);
      const d = sensorData[i];
      const dist = d > 100 ? 100 : d;
      const px = rx + Math.cos(a) * dist * mapScale;
      const py = ry - Math.sin(a) * dist * mapScale;
      i === 0 ? ctx.moveTo(px, py) : ctx.lineTo(px, py);
    }
    ctx.closePath();
    ctx.fillStyle = 'rgba(0, 229, 255, 0.04)';
    ctx.strokeStyle = 'rgba(0, 229, 255, 0.15)';
    ctx.lineWidth = 1;
    ctx.fill();
    ctx.stroke();

    // Draw dots at sensor collision points
    for (let i = 0; i < 8; i++) {
      const a = th + (SENSOR_ANGLES_DEG[i] * Math.PI / 180);
      const d = sensorData[i];
      if (d > 100) continue;
      const px = rx + Math.cos(a) * d * mapScale;
      const py = ry - Math.sin(a) * d * mapScale;
      ctx.beginPath();
      ctx.arc(px, py, 3.5, 0, Math.PI * 2);
      const danger = d < 40;
      ctx.fillStyle = danger ? '#ff3d3d' : '#00e5ff';
      ctx.shadowColor = danger ? '#ff3d3d' : '#00e5ff';
      ctx.shadowBlur = danger ? 8 : 3;
      ctx.fill();
      ctx.shadowBlur = 0;
    }
  }

  ctx.strokeStyle = 'rgba(0,255,157,0.5)'; ctx.lineWidth = 1.5;
  ctx.beginPath();
  ctx.moveTo(rx, ry);
  ctx.lineTo(rx + Math.cos(th) * 20, ry - Math.sin(th) * 20);
  ctx.stroke();

  ctx.beginPath();
  for (let i = 0; i < 8; i++) {
    const a = th + (i * Math.PI / 4) + (Math.PI / 8);
    const bx = rx + Math.cos(a) * 10;
    const by = ry - Math.sin(a) * 10;
    i === 0 ? ctx.moveTo(bx, by) : ctx.lineTo(bx, by);
  }
  ctx.closePath();
  ctx.fillStyle = 'rgba(0,255,157,0.12)';
  ctx.strokeStyle = '#00ff9d';
  ctx.lineWidth = 1.5;
  ctx.shadowColor = '#00ff9d'; ctx.shadowBlur = 10;
  ctx.fill(); ctx.stroke();
  ctx.shadowBlur = 0;

  const lx = W - 140;
  const ly = 15;
  
  ctx.fillStyle = 'rgba(11, 19, 26, 0.85)';
  ctx.strokeStyle = 'rgba(31, 41, 55, 0.8)';
  ctx.lineWidth = 1;
  ctx.fillRect(lx - 10, ly - 5, 140, 65);
  ctx.strokeRect(lx - 10, ly - 5, 140, 65);
  
  ctx.strokeStyle = '#eab308';
  ctx.lineWidth = 1.5;
  ctx.setLineDash([4, 2]);
  ctx.beginPath();
  ctx.moveTo(lx, ly + 10); ctx.lineTo(lx + 25, ly + 10);
  ctx.stroke();
  ctx.setLineDash([]);
  ctx.fillStyle = '#c8dae8';
  ctx.font = '10px Share Tech Mono, monospace';
  ctx.fillText('Jalur Target', lx + 35, ly + 13);
  
  ctx.strokeStyle = '#00e5ff';
  ctx.lineWidth = 2.5;
  ctx.beginPath();
  ctx.moveTo(lx, ly + 30); ctx.lineTo(lx + 25, ly + 30);
  ctx.stroke();
  ctx.fillStyle = '#c8dae8';
  ctx.fillText('Jalur Auto (Odom)', lx + 35, ly + 33);
  
  ctx.strokeStyle = '#d946ef';
  ctx.lineWidth = 2.5;
  ctx.beginPath();
  ctx.moveTo(lx, ly + 50); ctx.lineTo(lx + 25, ly + 50);
  ctx.stroke();
  ctx.fillStyle = '#c8dae8';
  ctx.fillText('Jalur Manual', lx + 35, ly + 53);

  ctx.beginPath(); ctx.arc(mapOffX, mapOffY, 3, 0, Math.PI * 2);
  ctx.fillStyle = 'rgba(0,229,255,0.4)'; ctx.fill();
};

let mapMouseDownPos = { x: 0, y: 0 };
let mapMouseDownTime = 0;

document.getElementById('map-canvas').addEventListener('mousedown', function(e) {
  mapMouseDownPos.x = e.clientX;
  mapMouseDownPos.y = e.clientY;
  mapMouseDownTime = Date.now();
});

document.getElementById('map-canvas').addEventListener('mouseup', function(e) {
  const dx = e.clientX - mapMouseDownPos.x;
  const dy = e.clientY - mapMouseDownPos.y;
  const dist = Math.sqrt(dx * dx + dy * dy);
  const elapsed = Date.now() - mapMouseDownTime;
  
  if (dist < 6 && elapsed < 300) {
    const rect = this.getBoundingClientRect();
    const px = e.clientX - rect.left;
    const py = e.clientY - rect.top;
    const cx = (px - mapOffX) / mapScale;
    const cy = -(py - mapOffY) / mapScale;
    
    if (document.getElementById('wp-x').disabled) return;
    
    document.getElementById('wp-x').value = Math.round(cx);
    document.getElementById('wp-y').value = Math.round(cy);
  }
});

let mapTouchStartPos = { x: 0, y: 0 };
let mapTouchStartTime = 0;

document.getElementById('map-canvas').addEventListener('touchstart', function(e) {
  if (e.touches.length === 1) {
    mapTouchStartPos.x = e.touches[0].clientX;
    mapTouchStartPos.y = e.touches[0].clientY;
    mapTouchStartTime = Date.now();
  }
}, { passive: true });

document.getElementById('map-canvas').addEventListener('touchend', function(e) {
  if (e.changedTouches.length === 1) {
    const dx = e.changedTouches[0].clientX - mapTouchStartPos.x;
    const dy = e.changedTouches[0].clientY - mapTouchStartPos.y;
    const dist = Math.sqrt(dx * dx + dy * dy);
    const elapsed = Date.now() - mapTouchStartTime;
    
    if (dist < 6 && elapsed < 300) {
      const rect = this.getBoundingClientRect();
      const px = e.changedTouches[0].clientX - rect.left;
      const py = e.changedTouches[0].clientY - rect.top;
      const cx = (px - mapOffX) / mapScale;
      const cy = -(py - mapOffY) / mapScale;
      
      if (document.getElementById('wp-x').disabled) return;
      
      document.getElementById('wp-x').value = Math.round(cx);
      document.getElementById('wp-y').value = Math.round(cy);
    }
  }
}, { passive: true });

document.getElementById('map-canvas').addEventListener('mousemove', function(e) {
  const rect = this.getBoundingClientRect();
  const px = e.clientX - rect.left;
  const py = e.clientY - rect.top;
  const cx = ((px - mapOffX) / mapScale).toFixed(0);
  const cy = (-(py - mapOffY) / mapScale).toFixed(0);
  document.getElementById('cursor-pos').textContent = 'X: ' + cx + '  Y: ' + cy;
});

document.getElementById('map-canvas').addEventListener('wheel', function(e) {
  e.preventDefault();
  mapScale = Math.max(0.25, Math.min(20, mapScale - e.deltaY * 0.005));
  document.getElementById('map-scale-label').textContent = '1px = ' + (1 / mapScale).toFixed(2) + 'cm';
  renderMap();
}, { passive: false });

const addWaypoint = () => {
  const x = parseFloat(document.getElementById('wp-x').value);
  const y = parseFloat(document.getElementById('wp-y').value);
  if (isNaN(x) || isNaN(y)) { addLog('Masukkan nilai X dan Y!', 'err'); return; }
  waypoints.push({ x, y });
  sendCmd('ADD ' + x + ' ' + y);
  document.getElementById('wp-x').value = '';
  document.getElementById('wp-y').value = '';
  renderWaypointList();
  renderMap();
};

const deleteWaypoint = (i) => {
  waypoints.splice(i, 1);
  renderWaypointList();
  renderMap();
};

const renderWaypointList = () => {
  const tbody = document.getElementById('wp-tbody');
  if (waypoints.length === 0) {
    tbody.innerHTML = '<tr><td colspan="5" style="color:var(--muted);font-size:11px;padding:10px 8px">Belum ada waypoint.</td></tr>';
    return;
  }
  tbody.innerHTML = waypoints.map((wp, i) => {
    const dx = wp.x - robotPos.x, dy = wp.y - robotPos.y;
    const dist = Math.sqrt(dx * dx + dy * dy).toFixed(0);
    const cls = i === currentWP && robotState !== 'IDLE' && robotState !== 'FINISHED' ? 'active' : (i < currentWP ? 'done' : '');
    return `<tr class="${cls}">
      <td>${i}</td>
      <td>${wp.x}</td>
      <td>${wp.y}</td>
      <td>${dist}cm</td>
      <td><button class="wp-del" onclick="deleteWaypoint(${i})">✕</button></td>
    </tr>`;
  }).join('');
};

const clearAll = () => {
  waypoints = [];
  currentWP = 0;
  autoTrail = [];
  manualTrail = [];
  sendCmd('CLEAR');
  renderWaypointList();
  renderMap();
};

// ================================================================
// MAP TOOLBAR FUNCTIONS
// ================================================================
const zoomMap = (delta) => {
  mapScale = Math.max(0.25, Math.min(20, mapScale + delta));
  document.getElementById('map-scale-label').textContent = '1px = ' + (1 / mapScale).toFixed(2) + 'cm';
  renderMap();
  addLog(`Zoom peta: 1px = ${(1/mapScale).toFixed(2)}cm`, 'ok');
};

const resetMapView = () => {
  mapScale = 2;
  const canvas = document.getElementById('map-canvas');
  mapOffX = canvas.width / 2;
  mapOffY = canvas.height / 2;
  document.getElementById('map-scale-label').textContent = '1px = ' + (1 / mapScale).toFixed(2) + 'cm';
  renderMap();
  addLog('Tampilan peta direset ke posisi awal.', 'ok');
};

const clearTrails = () => {
  autoTrail = [];
  manualTrail = [];
  renderMap();
  addLog('Trail jalur auto dan manual dihapus.', 'ok');
};

const downloadMap = () => {
  const src = document.getElementById('map-canvas');
  if (!src || src.width === 0 || src.height === 0) {
    alert('Peta belum dimuat!');
    return;
  }

  const offCanvas = document.createElement('canvas');
  offCanvas.width = src.width;
  offCanvas.height = src.height;
  const offCtx = offCanvas.getContext('2d');

  offCtx.fillStyle = '#080c10';
  offCtx.fillRect(0, 0, offCanvas.width, offCanvas.height);
  offCtx.drawImage(src, 0, 0);

  const now = new Date();
  const tsStr = now.toLocaleDateString('id-ID') + ' ' + now.toLocaleTimeString('id-ID');
  offCtx.font = 'bold 12px Share Tech Mono, monospace';
  offCtx.fillStyle = 'rgba(0,229,255,0.55)';
  offCtx.fillText('OKTO-BOT Navigation Map  |  ' + tsStr, 10, offCanvas.height - 12);

  const stamp = now.toISOString().replace(/T/, '_').replace(/\..+/, '').replace(/[:]/g, '-');
  const link = document.createElement('a');
  link.setAttribute('href', offCanvas.toDataURL('image/png'));
  link.setAttribute('download', `peta_navigasi_${stamp}.png`);
  link.style.display = 'none';
  document.body.appendChild(link);
  link.click();
  document.body.removeChild(link);

  addLog('Peta navigasi berhasil diunduh sebagai PNG.', 'ok');
};

const toggleStepMode = () => {
  const btn = document.getElementById('step-mode-toggle');
  const active = btn.classList.toggle('on');
  sendCmd(active ? 'STEPMODE ON' : 'STEPMODE OFF');
};

const toggleAPF = () => {
  apfOn = !apfOn;
  const btn = document.getElementById('apf-toggle');
  const txt = document.getElementById('apf-status-txt');
  btn.classList.toggle('on', apfOn);
  txt.textContent = apfOn ? 'AKTIF' : 'BYPASS';
  txt.style.color = apfOn ? 'var(--accent2)' : 'var(--warn)';
  sendCmd(apfOn ? 'APF ON' : 'APF OFF');
};

const updateSpeed = (v) => {
  document.getElementById('speed-val').textContent = v + ' cm/s';
  sendCmd('SPEED ' + v);
};


// ================================================================
// TABS HANDLING
// ================================================================
const switchTab = (tabId) => {
  const contents = document.querySelectorAll('.tab-content');
  contents.forEach(c => c.style.display = 'none');
  document.getElementById(tabId).style.display = 'block';
  
  const btns = document.querySelectorAll('.tab-btn');
  btns.forEach(b => {
    b.classList.remove('active');
  });
  
  event.currentTarget.classList.add('active');
};

// ================================================================
// SYSTEM CONFIGURATION
// ================================================================
const setPose = () => {
  let x = parseFloat(document.getElementById('sys-posx').value) || 0;
  let y = parseFloat(document.getElementById('sys-posy').value) || 0;
  let th = parseFloat(document.getElementById('sys-posth').value) || 90;
  sendCmd(`SETPOSE ${x} ${y} ${th}`);
};

const resetPose = () => {
  autoTrail = [];
  manualTrail = [];
  sendCmd('RESETPOSE');
  document.getElementById('sys-posx').value = '';
  document.getElementById('sys-posy').value = '';
  document.getElementById('sys-posth').value = '';
};

// ================================================================
// JOYSTICK MANUAL KONTROL
// ================================================================
let joystickZone = document.getElementById('joystick-zone');
let joystickHandle = document.getElementById('joystick-handle');
let isDragging = false;
let joystickCenterX = 65; // 130 / 2
let joystickCenterY = 65;
let maxJoystickRadius = 45;

let lastManualTime = 0;
let manualSpeedL = 0;
let manualSpeedR = 0;

const handleJoystickMove = (clientX, clientY) => {
  if (robotState === 'MOVE' || robotState === 'ROTATE' || robotState === 'WAIT') {
    return;
  }

  const rect = joystickZone.getBoundingClientRect();
  let x = clientX - rect.left;
  let y = clientY - rect.top;
  
  let dx = x - joystickCenterX;
  let dy = y - joystickCenterY;
  let dist = Math.sqrt(dx * dx + dy * dy);
  
  if (dist > maxJoystickRadius) {
    dx = (dx / dist) * maxJoystickRadius;
    dy = (dy / dist) * maxJoystickRadius;
    dist = maxJoystickRadius;
  }
  
  joystickHandle.style.transform = `translate(${dx}px, ${dy}px)`;
  
  let speedFactor = parseFloat(document.getElementById('speed-slider').value);
  let fwd = (-dy / maxJoystickRadius) * speedFactor;
  let turn = (dx / maxJoystickRadius) * (speedFactor * 0.7);
  
  manualSpeedL = Math.round(fwd + turn);
  manualSpeedR = Math.round(fwd - turn);
  
  document.getElementById('manual-speed-l').textContent = manualSpeedL;
  document.getElementById('manual-speed-r').textContent = manualSpeedR;
  
  let now = Date.now();
  if (now - lastManualTime > 100) {
    lastManualTime = now;
    sendCmd(`MANUAL ${manualSpeedL} ${manualSpeedR}`);
  }
};

const resetJoystick = () => {
  joystickHandle.style.transform = 'translate(0px, 0px)';
  manualSpeedL = 0;
  manualSpeedR = 0;
  document.getElementById('manual-speed-l').textContent = '0';
  document.getElementById('manual-speed-r').textContent = '0';
  sendCmd('MANUAL 0 0');
  sendCmd('STOP');
};

joystickZone.addEventListener('mousedown', e => { isDragging = true; joystickHandle.style.transition = 'none'; });
document.addEventListener('mousemove', e => {
  if (!isDragging) return;
  handleJoystickMove(e.clientX, e.clientY);
});
document.addEventListener('mouseup', e => {
  if (isDragging) {
    isDragging = false;
    joystickHandle.style.transition = 'transform 0.2s';
    resetJoystick();
  }
});

// Touch Devices
joystickZone.addEventListener('touchstart', e => { isDragging = true; joystickHandle.style.transition = 'none'; e.preventDefault(); });
document.addEventListener('touchmove', e => {
  if (!isDragging) return;
  let t = e.touches[0];
  handleJoystickMove(t.clientX, t.clientY);
}, { passive: false });
document.addEventListener('touchend', e => {
  if (isDragging) {
    isDragging = false;
    joystickHandle.style.transition = 'transform 0.2s';
    resetJoystick();
  }
});

// ================================================================
// WASD KEYBOARD KONTROL
// ================================================================
let keysPressed = {};
let keyInterval = null;

document.addEventListener('keydown', e => {
  if (e.target.tagName === 'INPUT') return;
  let key = e.key.toLowerCase();
  if (['w', 'a', 's', 'd', ' '].includes(key)) {
    e.preventDefault();
    keysPressed[key] = true;
    if (!keyInterval) {
      keyInterval = setInterval(sendManualCmdFromKeys, 100);
    }
  }
});

document.addEventListener('keyup', e => {
  let key = e.key.toLowerCase();
  if (['w', 'a', 's', 'd', ' '].includes(key)) {
    keysPressed[key] = false;
    let anyPressed = Object.values(keysPressed).some(v => v);
    if (!anyPressed) {
      clearInterval(keyInterval);
      keyInterval = null;
      resetJoystick();
    }
  }
});

const sendManualCmdFromKeys = () => {
  if (robotState === 'MOVE' || robotState === 'ROTATE' || robotState === 'WAIT') return;

  let fwd = 0;
  let turn = 0;
  let speedFactor = parseFloat(document.getElementById('speed-slider').value) * 0.8;
  
  if (keysPressed['w']) fwd = speedFactor;
  if (keysPressed['s']) fwd = -speedFactor;
  if (keysPressed['a']) turn = -speedFactor * 0.7;
  if (keysPressed['d']) turn = speedFactor * 0.7;
  if (keysPressed[' ']) { fwd = 0; turn = 0; }
  
  manualSpeedL = Math.round(fwd + turn);
  manualSpeedR = Math.round(fwd - turn);
  
  document.getElementById('manual-speed-l').textContent = manualSpeedL;
  document.getElementById('manual-speed-r').textContent = manualSpeedR;
  
  sendCmd(`MANUAL ${manualSpeedL} ${manualSpeedR}`);
};

// ================================================================
// LOG & BASE FUNCTIONALITY
// ================================================================
const addLog = (msg, type) => {
  const log = document.getElementById('log');
  if (!log) return;
  const now = new Date();
  const ts = now.getHours().toString().padStart(2,'0') + ':' + now.getMinutes().toString().padStart(2,'0') + ':' + now.getSeconds().toString().padStart(2,'0');
  const cls = type || 'ok';
  const prefix = type === 'cmd' ? '> ' : type === 'err' ? 'X ' : type === 'warn' ? '! ' : '* ';
  const line = document.createElement('div');
  line.className = 'log-' + cls;
  line.textContent = '[' + ts + '] ' + prefix + msg;
  log.appendChild(line);
  log.scrollTop = log.scrollHeight;
  if (log.children.length > 80) log.removeChild(log.firstChild);
};

const clearLog = () => {
  document.getElementById('log').innerHTML = '';
};

// ================================================================
// SIMULASI MODE DEMO LOKAL (MOCK SERVER LOGIC)
// ================================================================
const processMockCommand = (cmd) => {
  const parts = cmd.split(' ');
  const action = parts[0].toUpperCase();

  if (action === 'STEPMODE') {
    const mode = parts[1].toUpperCase();
    if (mode === 'ON') {
      stepModeActive = true;
      document.getElementById('step-mode-toggle').classList.add('on');
      addLog('>>> STEP MODE: AKTIF (MOCK)', 'ok');
    } else if (mode === 'OFF') {
      stepModeActive = false;
      document.getElementById('step-mode-toggle').classList.remove('on');
      addLog('>>> STEP MODE: NON-AKTIF (MOCK)', 'ok');
    }
  }
  else if (action === 'RESUME') {
    if (robotState === 'PAUSED') {
      robotState = nextStateAfterPause;
      addLog('>>> STEP MODE: Resuming navigation... (MOCK)', 'ok');
      if (robotState === 'WAIT') {
        setTimeout(() => {
          if (robotState === 'WAIT') robotState = 'MOVE';
        }, WAIT_MS);
      }
    } else {
      addLog('>>> STEP MODE: Robot tidak dalam kondisi pause! (MOCK)', 'err');
    }
  }

  if (action === 'START') {
    if (waypoints.length === 0) {
      addLog('>>> ERROR: Tidak ada waypoint (MOCK)', 'err');
      return;
    }
    currentWP = 0;
    robotState = 'ROTATE';
    addLog('>>> MEMULAI NAVIGASI MENUJU ' + waypoints.length + ' WAYPOINTS (MOCK)', 'ok');
  } 
  else if (action === 'STOP') {
    robotState = 'IDLE';
    addLog('>>> Robot dihentikan (MOCK)', 'warn');
  }
  else if (action === 'CLEAR') {
    waypoints = [];
    currentWP = 0;
    robotState = 'IDLE';
    addLog('>>> WAYPOINTS CLEARED (MOCK)', 'ok');
  }
  else if (action === 'ADD') {
    const x = parseFloat(parts[1]);
    const y = parseFloat(parts[2]);
    addLog(`>>> WAYPOINT [${waypoints.length}] DITAMBAHKAN: (${x}, ${y}) (MOCK)`, 'ok');
  }
  else if (action === 'SETPOSE') {
    robotPos.x = parseFloat(parts[1]);
    robotPos.y = parseFloat(parts[2]);
    robotPos.theta = parseFloat(parts[3]) * Math.PI / 180;
    addLog(`>>> Pose diatur ke: X=${robotPos.x}, Y=${robotPos.y}, Th=${parts[3]} deg (MOCK)`, 'ok');
  }
  else if (action === 'RESETPOSE') {
    robotPos.x = 0;
    robotPos.y = 0;
    robotPos.theta = Math.PI / 2;
    addLog('>>> Pose direset ke Home (MOCK)', 'ok');
  }
  else if (action === 'MANUAL') {
    robotState = 'MANUAL';
  }
};

// Update Pergerakan Simulasi Mode Demo
const updateDemoSimulation = () => {
  const dt = 0.05;

  if (robotState === 'ROTATE') {
    const target = waypoints[currentWP];
    const dx = target.x - robotPos.x;
    const dy = target.y - robotPos.y;
    const targetTheta = Math.atan2(dy, dx);
    
    let diff = targetTheta - robotPos.theta;
    while (diff > Math.PI) diff -= Math.PI * 2;
    while (diff < -Math.PI) diff += Math.PI * 2;

    if (Math.abs(diff) < (ROTATE_DONE_DEG * Math.PI / 180)) {
      if (stepModeActive) {
        robotState = 'PAUSED';
        nextStateAfterPause = 'WAIT';
        addLog('>>> STEP MODE: Selesai rotasi. Silakan ukur fisik robot lalu tekan LANJUTKAN!', 'warn');
      } else {
        robotState = 'WAIT';
        setTimeout(() => {
          if (robotState === 'WAIT') robotState = 'MOVE';
        }, WAIT_MS);
      }
    } else {
      const dir = diff > 0 ? 1 : -1;
      robotPos.theta += dir * 0.1;
    }
  } 
  else if (robotState === 'MOVE') {
    const target = waypoints[currentWP];
    const dx = target.x - robotPos.x;
    const dy = target.y - robotPos.y;
    const dist = Math.sqrt(dx * dx + dy * dy);

    if (dist < MOVE_DONE_CM) {
      currentWP++;
      if (currentWP >= waypoints.length) {
        robotState = 'FINISHED';
        addLog('ALL WAYPOINTS REACHED (MOCK)', 'ok');
      } else {
        if (stepModeActive) {
          robotState = 'PAUSED';
          nextStateAfterPause = 'ROTATE';
          addLog(`>>> STEP MODE: Sampai di Waypoint ${currentWP - 1}. Silakan ukur fisik robot lalu tekan LANJUTKAN!`, 'warn');
        } else {
          robotState = 'ROTATE';
        }
      }
    } else {
      let F_att_x = dx / dist;
      let F_att_y = dy / dist;
      
      let F_rep_x = 0;
      let F_rep_y = 0;

      mockObstacles.forEach(obs => {
        const dx_obs = obs.x - robotPos.x;
        const dy_obs = obs.y - robotPos.y;
        const d_obs = Math.sqrt(dx_obs * dx_obs + dy_obs * dy_obs) - obs.r;
        
        if (d_obs < R_OBS + S_OBS) {
          const mag = d_obs < R_OBS ? K_REP : K_REP * (S_OBS + R_OBS - d_obs) / S_OBS;
          const angle = Math.atan2(dy_obs, dx_obs) + Math.PI;
          F_rep_x += mag * Math.cos(angle);
          F_rep_y += mag * Math.sin(angle);
        }
      });

      const F_tot_x = F_att_x * K_ATT + F_rep_x;
      const F_tot_y = F_att_y * K_ATT + F_rep_y;
      
      const moveAngle = Math.atan2(F_tot_y, F_tot_x);
      
      robotPos.theta = moveAngle;
      robotPos.x += Math.cos(moveAngle) * MAX_MOVE_VEL * dt;
      robotPos.y += Math.sin(moveAngle) * MAX_MOVE_VEL * dt;
    }
  }
  else if (robotState === 'MANUAL') {
    const fwd = (manualSpeedL + manualSpeedR) / 2.0;
    const rot = (manualSpeedR - manualSpeedL) / WHEEL_BASE;
    
    robotPos.theta += rot * dt;
    robotPos.x += Math.cos(robotPos.theta) * fwd * dt;
    robotPos.y += Math.sin(robotPos.theta) * fwd * dt;
  }

  for (let i = 0; i < 8; i++) {
    const a = robotPos.theta + (SENSOR_ANGLES_DEG[i] * Math.PI / 180);
    let min_dist = 999.0;
    
    mockObstacles.forEach(obs => {
      const ox = obs.x - robotPos.x;
      const oy = obs.y - robotPos.y;
      const proj = ox * Math.cos(a) + oy * Math.sin(a);
      if (proj > 0) {
        const perp_dist2 = (ox * ox + oy * oy) - (proj * proj);
        const r2 = obs.r * obs.r;
        if (perp_dist2 < r2) {
          const intersect_dist = proj - Math.sqrt(r2 - perp_dist2);
          if (intersect_dist > 0 && intersect_dist < min_dist) {
            min_dist = intersect_dist;
          }
        }
      }
    });
    sensorData[i] = min_dist;
  }

  const mockStatus = {
    state: robotState,
    x: robotPos.x,
    y: robotPos.y,
    th: robotPos.theta * 180 / Math.PI,
    velL: robotState === 'MANUAL' ? manualSpeedL : robotState === 'MOVE' ? MAX_MOVE_VEL : 0,
    velR: robotState === 'MANUAL' ? manualSpeedR : robotState === 'MOVE' ? MAX_MOVE_VEL : 0,
    targetVelL: 0,
    targetVelR: 0,
    wp: currentWP,
    total: waypoints.length,
    apf: apfOn,
    sensors: sensorData
  };

  updateUI(mockStatus);
  updateSensors(sensorData);
  
  document.getElementById('conn-dot').style.background = 'var(--accent2)';
  document.getElementById('conn-text').textContent = 'CONNECTED (SIMULATOR)';
};

const toggleSimMode = () => {
  isSimulator = !isSimulator;
  const simToggle = document.getElementById('sim-toggle');
  const simStatusTxt = document.getElementById('sim-status-txt');
  const ipField = document.getElementById('robot-ip-field');
  
  if (simToggle) simToggle.classList.toggle('on', isSimulator);
  if (simStatusTxt) {
    simStatusTxt.textContent = isSimulator ? 'AKTIF' : 'NON-AKTIF';
    simStatusTxt.style.color = isSimulator ? 'var(--accent2)' : 'var(--warn)';
  }
  if (ipField) ipField.style.display = isSimulator ? 'none' : 'block';
  
  if (isSimulator) {
    addLog('Mode Simulasi Di-aktifkan.', 'warn');
    startFetchStatusLoop(100);
  } else {
    addLog(`Mencoba menghubungkan ke Robot di ${robotIp}...`, 'warn');
    startFetchStatusLoop(1000);
    loadParameters();
  }
};

const updateRobotIp = (val) => {
  robotIp = val.trim();
  addLog(`IP Robot diubah ke: ${robotIp}`, 'ok');
  if (!isSimulator) {
    loadParameters();
  }
};

window.addEventListener('load', () => {
  console.log('Dashboard initialized');
  resizeMap();
  renderSensorList();
  
  const simToggle = document.getElementById('sim-toggle');
  const simStatusTxt = document.getElementById('sim-status-txt');
  const ipField = document.getElementById('robot-ip-field');
  const ipVal = document.getElementById('robot-ip-val');
  
  if (simToggle) {
    simToggle.classList.toggle('on', isSimulator);
  }
  if (simStatusTxt) {
    simStatusTxt.textContent = isSimulator ? 'AKTIF' : 'NON-AKTIF';
    simStatusTxt.style.color = isSimulator ? 'var(--accent2)' : 'var(--warn)';
  }
  if (ipField) {
    ipField.style.display = isSimulator ? 'none' : 'block';
  }
  if (ipVal) {
    ipVal.value = robotIp;
  }
  
  if (isSimulator) {
    addLog('Aplikasi dibuka secara lokal. MODE SIMULASI DI-AKTIFKAN!', 'warn');
    addLog('Obstacle simulasi ditandai dengan lingkaran merah pada peta.', 'ok');
    loadParameters();
    startFetchStatusLoop(100);
  } else {
    addLog('Aplikasi terhubung ke server ESP32.', 'ok');
    loadParameters();
    startFetchStatusLoop(1000);
  }
  
  setInterval(drawSensorRadar, 200);
});

window.addEventListener('resize', resizeMap);

setTimeout(() => {
  resizeMap();
  drawSensorRadar();
  renderSensorList();
}, 100);

document.body.style.display = 'block';

// ================================================================
// A* PATH PLANNER
// ================================================================
let astarGrid = [];
let astarCols = 30, astarRows = 30, astarRes = 10;
let astarMapW = 300, astarMapH = 300;
let astarGoal = null; // {col, row}
let astarPath = [];   // raw A* path [{col,row}, ...]
let astarWaypoints = []; // simplified [{x,y}, ...] in cm
let astarMode = 'wall'; // 'wall', 'erase', 'goal'
let astarDrawing = false;

const initAstarGrid = () => {
  astarMapW = parseInt(document.getElementById('astar-w').value) || 300;
  astarMapH = parseInt(document.getElementById('astar-h').value) || 300;
  astarRes = parseInt(document.getElementById('astar-res').value) || 10;
  astarCols = Math.floor(astarMapW / astarRes);
  astarRows = Math.floor(astarMapH / astarRes);
  astarGrid = [];
  for (let r = 0; r < astarRows; r++) {
    astarGrid[r] = new Uint8Array(astarCols);
  }
  astarGoal = null;
  astarPath = [];
  astarWaypoints = [];
  document.getElementById('astar-grid-label').textContent = astarCols + '\u00d7' + astarRows + ' grid';
  document.getElementById('astar-path-len').textContent = '\u2014';
  document.getElementById('astar-wp-count').textContent = '\u2014';
  document.getElementById('astar-send-btn').disabled = true;
  resizeAstarCanvas();
};

const resizeAstarCanvas = () => {
  const c = document.getElementById('astar-canvas');
  if (!c) return;
  c.width = c.clientWidth;
  c.height = c.clientWidth;
  renderAstarCanvas();
};

const setAstarMode = (mode) => {
  astarMode = mode;
  const labels = { wall: 'GAMBAR DINDING', erase: 'HAPUS DINDING', goal: 'SET TUJUAN' };
  document.getElementById('astar-mode-lbl').textContent = labels[mode] || mode;
  document.querySelectorAll('.astar-btn[id^="astar-mode-"]').forEach(b => b.classList.remove('active'));
  const btn = document.getElementById('astar-mode-' + mode);
  if (btn) btn.classList.add('active');
};

const resetAstarMap = () => {
  initAstarGrid();
  addLog('A* Planner: Peta direset', 'ok');
};

// --- Canvas Rendering ---
const renderAstarCanvas = () => {
  const c = document.getElementById('astar-canvas');
  if (!c || c.width === 0) return;
  const ctx = c.getContext('2d');
  const W = c.width, H = c.height;
  const cellW = W / astarCols, cellH = H / astarRows;

  ctx.clearRect(0, 0, W, H);
  ctx.fillStyle = '#0a0e14';
  ctx.fillRect(0, 0, W, H);

  // Grid lines
  ctx.strokeStyle = 'rgba(0,229,255,0.06)';
  ctx.lineWidth = 0.5;
  for (let c2 = 0; c2 <= astarCols; c2++) {
    const x = c2 * cellW;
    ctx.beginPath(); ctx.moveTo(x, 0); ctx.lineTo(x, H); ctx.stroke();
  }
  for (let r = 0; r <= astarRows; r++) {
    const y = r * cellH;
    ctx.beginPath(); ctx.moveTo(0, y); ctx.lineTo(W, y); ctx.stroke();
  }

  // Walls
  for (let r = 0; r < astarRows; r++) {
    for (let c2 = 0; c2 < astarCols; c2++) {
      if (astarGrid[r][c2] === 1) {
        ctx.fillStyle = 'rgba(255,61,61,0.35)';
        ctx.fillRect(c2 * cellW + 0.5, r * cellH + 0.5, cellW - 1, cellH - 1);
        ctx.strokeStyle = 'rgba(255,61,61,0.6)';
        ctx.lineWidth = 0.5;
        ctx.strokeRect(c2 * cellW + 0.5, r * cellH + 0.5, cellW - 1, cellH - 1);
      }
    }
  }

  // A* raw path (yellow thin line)
  if (astarPath.length > 1) {
    ctx.strokeStyle = 'rgba(234,179,8,0.4)';
    ctx.lineWidth = 1.5;
    ctx.beginPath();
    ctx.moveTo((astarPath[0].col + 0.5) * cellW, (astarPath[0].row + 0.5) * cellH);
    for (let i = 1; i < astarPath.length; i++) {
      ctx.lineTo((astarPath[i].col + 0.5) * cellW, (astarPath[i].row + 0.5) * cellH);
    }
    ctx.stroke();
  }

  // Simplified waypoints path (cyan thick line)
  if (astarWaypoints.length > 1) {
    ctx.strokeStyle = '#00e5ff';
    ctx.lineWidth = 2.5;
    ctx.setLineDash([6, 3]);
    ctx.beginPath();
    // Convert cm back to grid for drawing
    const toGx = (x) => (x / astarRes + astarCols / 2) * cellW;
    const toGy = (y) => (astarRows / 2 - y / astarRes) * cellH;
    ctx.moveTo(toGx(astarWaypoints[0].x), toGy(astarWaypoints[0].y));
    for (let i = 1; i < astarWaypoints.length; i++) {
      ctx.lineTo(toGx(astarWaypoints[i].x), toGy(astarWaypoints[i].y));
    }
    ctx.stroke();
    ctx.setLineDash([]);

    // Waypoint dots
    astarWaypoints.forEach((wp, i) => {
      const px = toGx(wp.x), py = toGy(wp.y);
      ctx.beginPath(); ctx.arc(px, py, 5, 0, Math.PI * 2);
      ctx.fillStyle = i === 0 ? '#00ff9d' : (i === astarWaypoints.length - 1 ? '#ff3d3d' : '#00e5ff');
      ctx.fill();
      ctx.font = '9px Share Tech Mono, monospace';
      ctx.fillStyle = '#fff';
      ctx.fillText('#' + i, px + 7, py - 4);
    });
  }

  // Start (robot position) marker
  const startCol = Math.floor(astarCols / 2 + robotPos.x / astarRes);
  const startRow = Math.floor(astarRows / 2 - robotPos.y / astarRes);
  if (startCol >= 0 && startCol < astarCols && startRow >= 0 && startRow < astarRows) {
    const sx = (startCol + 0.5) * cellW, sy = (startRow + 0.5) * cellH;
    ctx.beginPath(); ctx.arc(sx, sy, 6, 0, Math.PI * 2);
    ctx.fillStyle = 'rgba(0,255,157,0.3)'; ctx.fill();
    ctx.strokeStyle = '#00ff9d'; ctx.lineWidth = 2; ctx.stroke();
    ctx.font = 'bold 9px Rajdhani, sans-serif';
    ctx.fillStyle = '#00ff9d';
    ctx.fillText('START', sx + 9, sy + 3);
  }

  // Goal marker
  if (astarGoal) {
    const gx = (astarGoal.col + 0.5) * cellW, gy = (astarGoal.row + 0.5) * cellH;
    ctx.beginPath(); ctx.arc(gx, gy, 6, 0, Math.PI * 2);
    ctx.fillStyle = 'rgba(255,61,61,0.3)'; ctx.fill();
    ctx.strokeStyle = '#ff3d3d'; ctx.lineWidth = 2; ctx.stroke();
    ctx.font = 'bold 9px Rajdhani, sans-serif';
    ctx.fillStyle = '#ff3d3d';
    ctx.fillText('GOAL', gx + 9, gy + 3);
  }

  // Axis labels
  ctx.font = '8px Share Tech Mono, monospace';
  ctx.fillStyle = 'rgba(0,229,255,0.25)';
  const halfC = Math.floor(astarCols / 2);
  const halfR = Math.floor(astarRows / 2);
  for (let c2 = 0; c2 < astarCols; c2 += 5) {
    const cm = (c2 - halfC) * astarRes;
    ctx.fillText(cm + '', (c2 + 0.2) * cellW, H - 2);
  }
  for (let r = 0; r < astarRows; r += 5) {
    const cm = (halfR - r) * astarRes;
    ctx.fillText(cm + '', 2, (r + 0.9) * cellH);
  }
};

// --- Binary Heap (Min-Priority Queue) ---
class MinHeap {
  constructor() { this.data = []; }
  push(item) {
    this.data.push(item);
    let i = this.data.length - 1;
    while (i > 0) {
      const p = (i - 1) >> 1;
      if (this.data[p].f <= this.data[i].f) break;
      [this.data[p], this.data[i]] = [this.data[i], this.data[p]];
      i = p;
    }
  }
  pop() {
    const top = this.data[0];
    const last = this.data.pop();
    if (this.data.length > 0) {
      this.data[0] = last;
      let i = 0;
      while (true) {
        let s = i, l = 2 * i + 1, r2 = 2 * i + 2;
        if (l < this.data.length && this.data[l].f < this.data[s].f) s = l;
        if (r2 < this.data.length && this.data[r2].f < this.data[s].f) s = r2;
        if (s === i) break;
        [this.data[s], this.data[i]] = [this.data[i], this.data[s]];
        i = s;
      }
    }
    return top;
  }
  get size() { return this.data.length; }
}

// --- A* Search Algorithm ---
const runAstar = () => {
  if (!astarGoal) { addLog('A*: Set tujuan terlebih dahulu!', 'err'); return; }

  const startCol = Math.floor(astarCols / 2 + robotPos.x / astarRes);
  const startRow = Math.floor(astarRows / 2 - robotPos.y / astarRes);

  if (startCol < 0 || startCol >= astarCols || startRow < 0 || startRow >= astarRows) {
    addLog('A*: Posisi robot di luar peta!', 'err'); return;
  }
  if (astarGrid[startRow][startCol] === 1) {
    addLog('A*: Posisi robot berada di dinding!', 'err'); return;
  }
  if (astarGrid[astarGoal.row][astarGoal.col] === 1) {
    addLog('A*: Tujuan berada di dinding!', 'err'); return;
  }

  const t0 = performance.now();

  const dirs = [
    [-1,0,1],[1,0,1],[0,-1,1],[0,1,1],
    [-1,-1,1.414],[-1,1,1.414],[1,-1,1.414],[1,1,1.414]
  ];
  const key = (r, c) => r * astarCols + c;
  const gr = astarGoal.row, gc = astarGoal.col;
  const heuristic = (r, c) => Math.sqrt((r - gr) * (r - gr) + (c - gc) * (c - gc));

  const gScore = new Float32Array(astarRows * astarCols).fill(Infinity);
  const cameFrom = new Int32Array(astarRows * astarCols).fill(-1);
  const closed = new Uint8Array(astarRows * astarCols);

  const open = new MinHeap();
  const sk = key(startRow, startCol);
  gScore[sk] = 0;
  open.push({ f: heuristic(startRow, startCol), g: 0, row: startRow, col: startCol });

  let found = false;

  while (open.size > 0) {
    const cur = open.pop();
    const ck = key(cur.row, cur.col);

    if (cur.row === gr && cur.col === gc) { found = true; break; }
    if (closed[ck]) continue;
    closed[ck] = 1;

    for (const [dr, dc, cost] of dirs) {
      const nr = cur.row + dr, nc = cur.col + dc;
      if (nr < 0 || nr >= astarRows || nc < 0 || nc >= astarCols) continue;
      if (astarGrid[nr][nc] === 1) continue;
      const nk = key(nr, nc);
      if (closed[nk]) continue;

      // Diagonal movement: check both adjacent cells to prevent corner cutting
      if (dr !== 0 && dc !== 0) {
        if (astarGrid[cur.row + dr][cur.col] === 1 || astarGrid[cur.row][cur.col + dc] === 1) continue;
      }

      const ng = cur.g + cost;
      if (ng < gScore[nk]) {
        gScore[nk] = ng;
        cameFrom[nk] = ck;
        open.push({ f: ng + heuristic(nr, nc), g: ng, row: nr, col: nc });
      }
    }
  }

  if (!found) {
    addLog('A*: Tidak ada jalur ke tujuan!', 'err');
    astarPath = [];
    astarWaypoints = [];
    document.getElementById('astar-send-btn').disabled = true;
    renderAstarCanvas();
    return;
  }

  // Reconstruct path
  astarPath = [];
  let cur = key(gr, gc);
  while (cur !== -1) {
    const r = Math.floor(cur / astarCols), c = cur % astarCols;
    astarPath.unshift({ row: r, col: c });
    cur = cameFrom[cur];
  }

  const elapsed = (performance.now() - t0).toFixed(1);
  const pathLenCm = (gScore[key(gr, gc)] * astarRes).toFixed(0);

  // Simplify path
  astarWaypoints = simplifyAstarPath(astarPath);

  document.getElementById('astar-path-len').textContent = pathLenCm + ' cm';
  document.getElementById('astar-wp-count').textContent = astarWaypoints.length;
  document.getElementById('astar-send-btn').disabled = false;

  addLog('A*: Jalur ditemukan! ' + pathLenCm + 'cm, ' + astarWaypoints.length + ' waypoints (' + elapsed + 'ms)', 'ok');
  renderAstarCanvas();
};

// --- Bresenham Line-of-Sight Check ---
const losCheck = (r0, c0, r1, c1) => {
  let dr = Math.abs(r1 - r0), dc = Math.abs(c1 - c0);
  let sr = r0 < r1 ? 1 : -1, sc = c0 < c1 ? 1 : -1;
  let err = dc - dr;
  let cr = r0, cc = c0;
  while (true) {
    if (cr === r1 && cc === c1) return true;
    if (astarGrid[cr][cc] === 1) return false;
    const e2 = 2 * err;
    if (e2 > -dr) { err -= dr; cc += sc; }
    if (e2 < dc) { err += dc; cr += sr; }
    if (cr < 0 || cr >= astarRows || cc < 0 || cc >= astarCols) return false;
  }
};

// --- Path Simplification ---
const simplifyAstarPath = (path) => {
  if (path.length <= 2) {
    return path.map(p => gridToCm(p.col, p.row));
  }
  const result = [path[0]];
  let anchor = 0;
  while (anchor < path.length - 1) {
    let farthest = anchor + 1;
    for (let i = anchor + 2; i < path.length; i++) {
      if (losCheck(path[anchor].row, path[anchor].col, path[i].row, path[i].col)) {
        farthest = i;
      }
    }
    result.push(path[farthest]);
    anchor = farthest;
  }
  // Ensure under MAX_WAYPOINTS (30)
  let simplified = result.map(p => gridToCm(p.col, p.row));
  if (simplified.length > 30) {
    // Keep evenly spaced subset
    const step = (simplified.length - 1) / 29;
    const reduced = [simplified[0]];
    for (let i = 1; i < 29; i++) reduced.push(simplified[Math.round(i * step)]);
    reduced.push(simplified[simplified.length - 1]);
    simplified = reduced;
  }
  return simplified;
};

const gridToCm = (col, row) => ({
  x: Math.round((col - astarCols / 2) * astarRes),
  y: Math.round((astarRows / 2 - row) * astarRes)
});

// --- Send to Robot ---
const sendAstarToRobot = async () => {
  if (astarWaypoints.length === 0) { addLog('A*: Tidak ada waypoint!', 'err'); return; }

  sendCmd('CLEAR');
  waypoints = [];
  currentWP = 0;

  addLog('A*: Mengirim ' + astarWaypoints.length + ' waypoints ke robot...', 'cmd');

  for (let i = 0; i < astarWaypoints.length; i++) {
    await new Promise(r => setTimeout(r, 60));
    const wp = astarWaypoints[i];
    sendCmd('ADD ' + wp.x + ' ' + wp.y);
    waypoints.push({ x: wp.x, y: wp.y });
  }

  renderWaypointList();
  renderMap();
  addLog('A*: ' + astarWaypoints.length + ' waypoints terkirim! Tekan START untuk mulai navigasi.', 'ok');
};

// --- Mouse / Touch Event Handlers ---
const getAstarCell = (e) => {
  const c = document.getElementById('astar-canvas');
  const rect = c.getBoundingClientRect();
  const sx = (e.clientX || e.touches[0].clientX) - rect.left;
  const sy = (e.clientY || e.touches[0].clientY) - rect.top;
  const col = Math.floor(sx / (c.width / astarCols));
  const row = Math.floor(sy / (c.height / astarRows));
  if (col < 0 || col >= astarCols || row < 0 || row >= astarRows) return null;
  return { col, row };
};

const handleAstarCell = (cell) => {
  if (!cell) return;
  if (astarMode === 'wall') {
    astarGrid[cell.row][cell.col] = 1;
  } else if (astarMode === 'erase') {
    astarGrid[cell.row][cell.col] = 0;
  } else if (astarMode === 'goal') {
    if (astarGrid[cell.row][cell.col] === 1) return;
    astarGoal = { col: cell.col, row: cell.row };
    addLog('A*: Tujuan diset ke (' + gridToCm(cell.col, cell.row).x + ', ' + gridToCm(cell.col, cell.row).y + ') cm', 'cmd');
  }
  renderAstarCanvas();
};

const astarCanvas = document.getElementById('astar-canvas');
if (astarCanvas) {
  astarCanvas.addEventListener('mousedown', (e) => {
    e.preventDefault();
    astarDrawing = true;
    handleAstarCell(getAstarCell(e));
  });
  astarCanvas.addEventListener('mousemove', (e) => {
    if (!astarDrawing) return;
    if (astarMode === 'goal') return;
    handleAstarCell(getAstarCell(e));
  });
  astarCanvas.addEventListener('mouseup', () => { astarDrawing = false; });
  astarCanvas.addEventListener('mouseleave', () => { astarDrawing = false; });

  astarCanvas.addEventListener('touchstart', (e) => {
    e.preventDefault();
    astarDrawing = true;
    handleAstarCell(getAstarCell(e));
  }, { passive: false });
  astarCanvas.addEventListener('touchmove', (e) => {
    e.preventDefault();
    if (!astarDrawing || astarMode === 'goal') return;
    handleAstarCell(getAstarCell(e));
  }, { passive: false });
  astarCanvas.addEventListener('touchend', () => { astarDrawing = false; });
}

// Initialize A* grid on load
initAstarGrid();
window.addEventListener('resize', resizeAstarCanvas);
</script>
</body>
</html>

)rawliteral";

#endif // WEB_DASHBOARD_H


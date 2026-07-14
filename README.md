[README.md](https://github.com/user-attachments/files/30005832/README.md)
# Oktobot-Skripsi# OKTO-BOT: Waypoint Navigation Robot using Artificial Potential Field (APF)

Repositori ini merupakan lampiran *source code* dan data pengujian untuk dokumentasi penelitian / Skripsi.

Repositori ini menyimpan kode program utama mikrokontroler (ESP32) yang mengatur pergerakan robot (OKTO-BOT), kalkulasi odometri, algoritma penghindar halangan (*Artificial Potential Field*), serta penyajian *dashboard* antarmuka berbasis Web Server. Selain itu, repositori ini juga memuat lampiran data hasil pengujian yang dilakukan selama penelitian.

## Struktur Direktori

- `waypoint_navigation_pio/` : Direktori proyek PlatformIO yang berisi seluruh *source code* robot.
  - `src/` : Kumpulan file C++ untuk logika navigasi, PID, APF, pembacaan sensor, motor, dan web server.
  - `include/` : File *header* (`.h`) yang menyimpan parameter konfigurasi dan variabel global.
- `Data_Pengujian/` : Direktori yang berisi file spreadsheet mentah `All_Tests_Combined.xlsx` hasil rekapitulasi pengujian jarak, sudut, APF, dan performa navigasi robot.

## Fitur Utama Program
1. **Navigasi Waypoint**: Robot bergerak menuju titik target (*waypoint*) berdasarkan koordinat (X, Y).
2. **Kendali PID Odometri**: Menggunakan *rotary encoder* untuk melacak posisi dan mengontrol pergerakan roda.
3. **Penghindar Halangan (APF)**: Menggunakan algoritma *Artificial Potential Field* dengan input dari 8 buah sensor ultrasonik HY-SRF05.
4. **Koreksi Sudut (IMU)**: Integrasi dengan MPU9250 (Accelerometer, Gyroscope, Magnetometer) untuk memperkuat kalkulasi orientasi heading.
5. **Web Dashboard Real-Time**: Pemantauan status robot (*monitoring*) dan penyetelan parameter (PID, APF) secara *real-time* nirkabel melalui jaringan *Access Point* Wi-Fi ESP32.

## Perangkat Keras Utama
- Mikrokontroler: **ESP32 DevKit V1**
- Sensor Jarak: **8x Ultrasonik HY-SRF05**
- Sensor Orientasi: **IMU MPU9250**
- Motor Driver: **2x BTS7960**
- Aktuator: **Motor DC Geared dengan Encoder (JGA25)**
- Display: **Layar OLED 0.96" (I2C)**

## Cara Penggunaan (Compile & Upload)
Proyek kode perangkat lunak ini dibangun menggunakan ekstensi **PlatformIO** pada IDE seperti Visual Studio Code.
1. Instal ekstensi PlatformIO IDE di VS Code.
2. Buka folder `waypoint_navigation_pio` melalui menu PlatformIO.
3. Hubungkan ESP32 ke komputer melalui kabel USB.
4. Klik tombol **Build** pada *Status Bar* untuk memastikan kode terkompilasi tanpa *error*.
5. Klik tombol **Upload** untuk memasukkan program (*flash*) ke dalam ESP32.

## Tentang
Repositori ini dilampirkan sebagai bentuk transparansi ilmiah dan untuk mempermudah replikasi sistem serta pengembangan penelitian lebih lanjut oleh mahasiswa maupun peneliti lain di masa depan.

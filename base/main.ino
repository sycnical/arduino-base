#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Standard-I2C-Pins deines Boards
#define I2C_SDA 21
#define I2C_SCL 22

// Instanzen für BEIDE Sensoren anlegen
Adafruit_MPL3115A2 mpl = Adafruit_MPL3115A2();
Adafruit_MPU6050 mpu = Adafruit_MPU6050(); // GY-521 basiert auf dem MPU6050

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  Serial.println("\n--- Dual-Sensor Test (MPL3115A2 + GY-521) ---");

  // I2C-Bus einmalig für BEIDE Sensoren starten
  Wire.begin(I2C_SDA, I2C_SCL);

  // 1. Höhenmesser initialisieren
  if (!mpl.begin()) {
    Serial.println("Fehler: MPL3115A2 (Höhenmesser) nicht gefunden!");
    while (1) { delay(100); } 
  }
  Serial.println("-> MPL3115A2 erfolgreich gestartet.");

  // 2. Gyroskop / Beschleunigungssensor initialisieren
  if (!mpu.begin()) {
    Serial.println("Fehler: GY-521 (MPU6050) nicht gefunden!");
    while (1) { delay(100); } 
  }
  Serial.println("-> GY-521 (MPU6050) erfolgreich gestartet.");

  // Optionale Einstellungen für das Gyroskop (für stabilen Flug)
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // Beschleunigung bis 8G
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);     // Rotation bis 500°/s
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);   // Filtert leichtes Rauschen heraus
}

void loop() {
  // --- Daten vom Höhenmesser abfragen ---
  float temperatur = mpl.getTemperature(); 
  float druck = mpl.getPressure();
  float hoehe = mpl.getAltitude();       

  // --- Daten vom GY-521 (MPU6050) abfragen ---
  sensors_event_t a, g, t;
  mpu.getEvent(&a, &g, &t); // Liest Beschleunigung (a), Rotation (g) und Temperatur (t)

  // // --- AUSGABE IM SERIELLEN MONITOR ---
  // Serial.println("====== SENSOR DATEN ======");
  
  // // MPL3115A2 Werte
  // Serial.print("Höhe:        "); Serial.print(hoehe); Serial.println(" m");
  // Serial.print("Luftdruck:   "); Serial.print(druck); Serial.println(" hPa");
  // Serial.print("Temperatur:   "); Serial.print(temperatur); Serial.println(" °C");
  
  // // GY-521 Beschleunigung (X, Y, Z Achse)
  // Serial.print("Beschleunig: X: "); Serial.print(a.acceleration.x);
  // Serial.print(" | Y: "); Serial.print(a.acceleration.y);
  // Serial.print(" | Z: "); Serial.println(a.acceleration.z);

  // // GY-521 Rotationsgeschwindigkeit (Kippen / Drehen)
  // Serial.print("Rotation:    X: "); Serial.print(g.gyro.x);
  // Serial.print(" | Y: "); Serial.print(g.gyro.y);
  // Serial.print(" | Z: "); Serial.println(g.gyro.z);

  // Serial.println("------------------------------------\n");

  // Alle Werte in einer einzigen Zeile, getrennt durch Kommas senden
  Serial.print(hoehe);            Serial.print(",");
  Serial.print(druck);            Serial.print(",");
  Serial.print(temperatur);       Serial.print(",");
  Serial.print(a.acceleration.x); Serial.print(",");
  Serial.print(a.acceleration.y); Serial.print(",");
  Serial.print(a.acceleration.z); Serial.print(",");
  Serial.print(g.gyro.x);         Serial.print(",");
  Serial.print(g.gyro.y);         Serial.print(",");
  Serial.println(g.gyro.z);       // Wichtig: Das letzte ist ein println für den Zeilenumbruch!

  delay(500); // Messung alle halbe Sekunde
}

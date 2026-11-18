#include <Wire.h>
#include <Adafruit_MPL3115A2.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Standard-I2C-Pins
#define I2C_SDA 21
#define I2C_SCL 22

Adafruit_MPL3115A2 mpl = Adafruit_MPL3115A2();
Adafruit_MPU6050 mpu = Adafruit_MPU6050();

void setup() {
  Serial.begin(115200);
  while (!Serial) { delay(10); }

  Serial.println("\n--- Dual-Sensor Test (MPL3115A2 + GY-521) ---");

  // start I2C-Bus once
  Wire.begin(I2C_SDA, I2C_SCL);

  if (!mpl.begin()) {
    Serial.println("Fehler: MPL3115A2 (Höhenmesser) not Found!");
    while (1) { delay(100); } 
  }
  Serial.println("-> MPL3115A2 sucsesfully started.");

  if (!mpu.begin()) {
    Serial.println("Errror: GY-521 (MPU6050) not found!");
    while (1) { delay(100); } 
  }
  Serial.println("-> GY-521 (MPU6050) erfolgreich gestartet.");

  // Optional settings (best for a flight?!)
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G); // Beschleunigung bis 8G
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);     // Rotation bis 500°/s
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);   // Filtert leichtes Rauschen heraus
}

void loop() {
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

  Serial.print(hoehe);            Serial.print(",");
  Serial.print(druck);            Serial.print(",");
  Serial.print(temperatur);       Serial.print(",");
  Serial.print(a.acceleration.x); Serial.print(",");
  Serial.print(a.acceleration.y); Serial.print(",");
  Serial.print(a.acceleration.z); Serial.print(",");
  Serial.print(g.gyro.x);         Serial.print(",");
  Serial.print(g.gyro.y);         Serial.print(",");
  Serial.println(g.gyro.z);

  delay(500); // repeat ever half second
}

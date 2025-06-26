#include <SoftwareSerial.h>
#include <Wire.h>
#include <Adafruit_BME280.h>

// Pines del SDS011
#define SDS_RX 14  // TX del SDS011
#define SDS_TX 12  // RX del SDS011

SoftwareSerial sds(SDS_RX, SDS_TX);
Adafruit_BME280 bme;  // Sensor BME280

float pm25 = 0, pm10 = 0;
int pm25int, pm10int;
float temperatura = 0.0;
float humedad = 0.0;   
float presion = 0.0;   

void setup() {
  Serial.begin(9600);
  sds.begin(9600);

  Wire.begin(22, 21);  // Pines SDA y SCL

  Serial.println("Iniciando sensores...");

  if (!bme.begin(0x76)) {
    Serial.println("Error: No se detectó el BME280");
    while (1);
  }
  delay(30000);
}

void loop() {
  delay(1000);

  sds_medir();
  temperatura = bme.readTemperature();
  humedad = bme.readHumidity();  
  presion = bme.readPressure() / 100.0F;

  Serial.print("PM2.5:");
  Serial.print(pm25);
  Serial.print("\tPM10:");
  Serial.print(pm10);
  Serial.print("\tTemp:");
  Serial.print(temperatura);
  Serial.print(" C\tHumedad:");
  Serial.print(humedad);
  Serial.print(" %\tPresion:");
  Serial.print(presion);
  Serial.println(" hPa");
}


void sds_medir() {
  while (sds.available() && sds.read() != 0xAA) {}

  byte buffer[10];
  buffer[0] = 0xAA;
  if (sds.available() >= 9) {
    sds.readBytes(&buffer[1], 9);
    if (buffer[9] == 0xAB) {
      pm25int = (buffer[3] << 8) | buffer[2];
      pm10int = (buffer[5] << 8) | buffer[4];
      pm25 = pm25int / 10.0;
      pm10 = pm10int / 10.0;
    }
  }
}

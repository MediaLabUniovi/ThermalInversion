/* ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
ARCHIVO PARA INCLUIR FUNCIONES DE SENSORES Y AÑADIR MEDICIONES A "txBuffer"

Este archivo ha sido modificado de manera considerable para implementar el sensor climático BME280 junto con el sensor de particulas de materia SDS011. La
librería de este último es una versión modificada de la original: https://github.com/misan/SDS011/tree/master 
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~ */

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Librerias de sensores
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#include "lvlbat.h"                                                                               // Se llama a la funcion de medicion de bateria para añadirlo al txBuffer
#include <Wire.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>                                                                      // Adafruit_BME280
#include <SoftwareSerial.h>                                                                       // EspSoftwareSerial
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define PCB_SDA_PIN 21
#define PCB_SCL_PIN 22
#define CODIGO_SDA_PIN 22
#define CODIGO_SCL_PIN 21
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Macros, constructores, variables... de sensores
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Adafruit_BME280 bme;                                                                              // Constructor del BME280
TwoWire customWire = TwoWire(0);
SoftwareSerial sds(SDS_RX, SDS_TX);                                                               // Se crea el segundo puerto serie para el sensor de particulas

float temp, hum, pm25, pm10, presfloat;
int pm25int, pm10int, tempint, humint;
long pres;
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Funciones setup de los sensores - Se crean aqui para tenerlas en el scope correcto, luego se llaman en el 'setup()' principal
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Setup del BME280 ------------------------------------------------------------------------------------------------------------------------------------------
void bme280_setup(){                                                                              // Inicializa I2C
customWire.begin(CODIGO_SDA_PIN,CODIGO_SCL_PIN);
  if(!bme.begin(0x76,&customWire) && !bme.begin(0x77,&customWire)){
    Serial.println("No encuentro un sensor BME280 valido!"); 
    while (1);
  }
}

// Setup del SDS011 ------------------------------------------------------------------------------------------------------------------------------------------
void sds011_setup(){
  sds.begin(9600);                                                                                // Inicializa el puerto serie del SDS
}

// Funcion para medir PM2.5 y PM10  del SDS011 ---------------------------------------------------------------------------------------------------------------
void sds_medir(){
  while(sds.available() && sds.read() != 0xAA){ }

  if(sds.available()) Serial.println(F("Data available from SDS011..."));

  byte buffer[10];
  buffer[0] = 0xAA;
  if (sds.available() >= 9) {
    sds.readBytes(&buffer[1], 9);

    if (buffer[9] == 0xAB) {
      pm25int = (buffer[3] << 8) | buffer[2];
      pm10int = (buffer[5] << 8) | buffer[4];
      pm25 = pm25int / 10.0;
      pm10 = pm10int / 10.0;
      Serial.print("PM2.5: "); Serial.print(pm25); Serial.print(" µg/m³");
      Serial.print("PM10: "); Serial.print(pm10); Serial.println(" µg/m³");
    } else {
      Serial.println("Invalid ending byte from SDS011.");
    }
  } else {
    Serial.println("Not enough data from SDS011.");
  }
}

// Funcion para despertar el SDS011, tomar tantas medidas como sea necesario hasta obtener medidas validas y quedarse con la ultima como buena ---------------
void sds_wake_up(uint8_t medidas){
  byte wakeUpCommand[] = {0xAA, 0xB4, 0x06, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x06, 0xAB};
  sds.write(wakeUpCommand, sizeof(wakeUpCommand));
  delay(30000);                                                                                   // Delay para preparar la recogida de datos del sensor

  for(int i = 0; i < medidas; i++){
    sds_medir();
    delay(500);
  }
}

// Funcion para dormir el SDS011, apagando el ventilador y el laser ------------------------------------------------------------------------------------------
void sds_sleep(){
  byte sleepCommand[] = {0xAA, 0xB4, 0x06, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0x05, 0xAB};
  sds.write(sleepCommand, sizeof(sleepCommand));
  delay(100);
}
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// ===========================================================================================================================================================
// FUNCION PRINCIPAL DE "sensor.ino" - Construir el paquete de datos que se enviara por LoRa, SE DEBE EDITAR PARA INCLUIR LAS VARIABLES DE OTROS SENSORES, PERO MANTENER LA ESTRUCTURA DE DESCOMPOSICION EN BYTES PARA SER AÑADIDOS AL "txBuffer"
// ===========================================================================================================================================================
void build_packet(uint8_t txBuffer[TX_BUFFER_SIZE]){                                              // Uso 'uint8_t' para que todos los datos sean del tamaño de 1byte (8bits)  
  // read the temperature from the BME280 --------------------------------------------------------------------------------------------------------------------
  temp = (bme.readTemperature());
  Serial.print("Temperature: "); Serial.print(temp); Serial.println(" *C");
  tempint = temp * 100;
  
  txBuffer[0] = lowByte(tempint);
  txBuffer[1] = highByte(tempint);

  // read the humidity from the BME280 -----------------------------------------------------------------------------------------------------------------------
  hum = (bme.readHumidity());
  Serial.print("RH: "); Serial.print(hum); Serial.println(" %");
  humint = hum * 100;
  
  txBuffer[2] = lowByte(humint);
  txBuffer[3] = highByte(humint);

  // read the pressure from the BME280 -----------------------------------------------------------------------------------------------------------------------
  pres = bme.readPressure();
  presfloat = pres / 100.0;
  Serial.print("Pressure: "); Serial.print(presfloat); Serial.println(" hPa");
  
  txBuffer[4] = (byte) ((pres & 0X000000FF)       );
  txBuffer[5] = (byte) ((pres & 0x0000FF00) >> 8  );
  txBuffer[6] = (byte) ((pres & 0x00FF0000) >> 16 );
  txBuffer[7] = (byte) ((pres & 0xFF000000) >> 24 );

  // read both, PM2.5 and PM10 -------------------------------------------------------------------------------------------------------------------------------
 sds_wake_up(3);                                                                                 // Llamo a la funcion para despertar el SDS011, la cual enciende el laser y el ventilador, espera 30 segundos y recoge tantas pedidas como se le pase a la funcion para garantizar que la que se envie sea correcta

  txBuffer[8] = lowByte(pm25int);
  txBuffer[9] = highByte(pm25int);

  txBuffer[10] = lowByte(pm10int);
  txBuffer[11] = highByte(pm10int);

  sds_sleep();

  // Bateria -------------------------------------------------------------------------------------------------------------------------------------------------
  uint16_t txBatVal = battery_value();

  Serial.print(F("Valor analógico de la batería: ")); Serial.print(txBatVal); Serial.println(F(" bits"));
  Serial.println(F("==================="));                                                      // Separador visual entre bloques de datos debug

  float VBAT = (float)(txBatVal) / 4095 * 2 * 3.3; // Conversión del ADC a voltaje
  float sin_offset = VBAT + 0.25; //Ajuste por el offset de medida
  uint8_t nivel_bateria = constrain((sin_offset - 3.1) * (100.0 / (4.1 - 3.1)), 0, 100); // Cálculo del porcentaje

  Serial.print("Lectura ADC: ");
  Serial.print(txBatVal);  
  Serial.print(" - Voltaje: ");
  Serial.print(sin_offset, 2);
  Serial.print("V - Porcentaje: ");
  Serial.print(nivel_bateria);
  Serial.println("%");

  txBuffer[12] = lowByte(nivel_bateria);
  txBuffer[13] = highByte(nivel_bateria);
}
// ===========================================================================================================================================================
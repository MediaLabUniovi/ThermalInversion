/* ***********************************************************************************************************************************************************
THERMAL INVERSION (Release Candidate) - MediaLab_ IoT, UNIVERSIDAD DE OVIEDO

Este archivo ha sido modificado de manera considerable para implementar el sensor de PM SDS011, al mismo tiempo que el original BME280.
El enlace para consultar el proyecto original del usuario de GitHub "rwanrooy" es el siguiente:
https://github.com/rwanrooy/TTGO-PAXCOUNTER-LoRa32-V2.1-TTN.git

A LO LARGO DEL PROYECTO, SE INDICA CON LINEAS HECHAS CON "~" AQUELLAS SECCIONES DE CODIGO QUE DEBEN SER EDITADAS A GUSTO, EL RESTO NO SE DEBE DE MODIFICAR.
*********************************************************************************************************************************************************** */
#include <Wire.h>
#include "configuration.h"                                               // Libreria de macros 
#include "sensor.h"                                                      // Libreria de funciones de sensores y construccion del "txBuffer" con las medidas a enviar
#include "sleep.h"                                                       // Libreria de funciones para activar el deep sleep
#include "rom/rtc.h"                                                     // Libreria para usar la memoria RTC del ESP32, donde se pueden guardar variables cuyos valores sobreviven al deep sleep

RTC_DATA_ATTR uint32_t bootCount = 0;                                    // Contador de despertares tras deep sleep, almacenado en la memoria RTC para sobrevivir deep sleep. Se usa para el control de los mensajes confirmados

static uint8_t txBuffer[TX_BUFFER_SIZE];                                 // Como el array de bytes que se envía a TTN se calcula en otro archivo, lo declaro como static para que sea visible. AJUSTAR EL NUMERO DE BYTES AL DEL PAYLOAD

uint32_t tiempoPrevio = 0;                                               // Variable para almacenar el tiempo previo en el que se ejecuto el envio de datos
bool first = true;                                                       // Booleano para entrar una vez en el bloque de envio de datos si se recibe confirmacion de TX

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Funcion para enviar el paquete de datos LoRa
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
void do_send(){
  build_packet(txBuffer);
  
  ttn_cnt(bootCount);
  ttn_send(txBuffer, sizeof(txBuffer), LORAWAN_PORT, false);

  bootCount++;                                                           // Se le suma uno al contador de arranques tras cada ciclo de envío de datos por LoRa
}

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Funcion para mostrar mensajes por monitor serial segun se interactue con TTN por medio de "ttn.ino"
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
void callback(uint8_t message){
  if(EV_JOINING       == message){ Serial.println(F("Conectando con TTN..."));                       Serial.println(F("-------------------")); }
  if(EV_JOINED        == message){ Serial.println(F("¡Conectado con TTN!"));                         Serial.println(F("-------------------")); }
  if(EV_JOIN_FAILED   == message){ Serial.println(F("¡Conexion con TTN fallida!"));                  Serial.println(F("-------------------")); }
  if(EV_REJOIN_FAILED == message){ Serial.println(F("¡Nuevo intento de conexion con TTN fallido!")); Serial.println(F("-------------------")); }
  if(EV_RESET         == message){ Serial.println(F("Reseteo de conexion con TTN"));                 Serial.println(F("-------------------")); }
  if(EV_LINK_DEAD     == message){ Serial.println(F("¡Link con TTN muerto!"));                       Serial.println(F("-------------------")); }
  if(EV_ACK           == message){ Serial.println(F("¡ACK recibido!"));                              Serial.println(F("-------------------")); }
  if(EV_PENDING       == message){ Serial.println(F("Mensaje descartado"));                          Serial.println(F("-------------------")); }
  if(EV_QUEUED        == message){ Serial.println(F("Mensaje en cola..."));                          Serial.println(F("-------------------")); }
  if(EV_TXCOMPLETE    == message){ Serial.println(F("¡TRANSMISION COMPLETADA!"));                    Serial.println(F("-------------------")); sleep(); }  // Me voy a dormir aquí, cuando se confirma el envío del payload
}
// -----------------------------------------------------------------------------------------------------------------------------------------------------------

// ===========================================================================================================================================================
// Setup main - MODIFICAR PIN MODES Y SUS CONDICIONES INICIALES
// ===========================================================================================================================================================
void setup(){
  #if ENABLE_DEBUG == 1                                                  // Activar o desactivar desde "configuration.h" el monitor serial para debugging
    DEBUG_PORT.begin(SERIAL_BAUD);
  #endif

  // Setup de los perifericos que envian por LoRa ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  bme280_setup();
  sds011_setup();  
  Serial.println(F("Sistema iniciado"));
  // TTN setup -----------------------------------------------------------------------------------------------------------------------------------------------
  if(!ttn_setup()){
    Serial.println(F("[ERR] Chip de radio LoRa no encontrado, ¡ACTIVANDO DEEP SLEEP HASTA REINICIO MANUAL!"));
    delay(MESSAGE_TO_SLEEP_DELAY);
    esp_deep_sleep_start();                                              // Aquí me voy a dormir para siempre, como si fuese un while(1) pero de bajo consumo
  }

  // TTN register --------------------------------------------------------------------------------------------------------------------------------------------
  ttn_register(callback);                                                // Funcion de eventos de TTN
  ttn_join();
  ttn_sf(LORAWAN_SF);                                                    // Spreading Factor
  ttn_adr(LORAWAN_ADR);
}
// ===========================================================================================================================================================

// ===========================================================================================================================================================
// Loop main
// ===========================================================================================================================================================
void loop(){
  ttn_loop();                                                            // Función definida en "ttn.ino" en la que se ejecuta la función principal "os_runloop_once()" de la librería LMIC. GESTION DE CALLBACKS
  
  if(tiempoPrevio == 0 || millis() - tiempoPrevio > SEND_INTERVAL){      // 'if' en el que solo se entra la primera vez que se ejecuta el loop al arrancar o despertar de deep sleep para tomar medidas. Despues, se encarga la funcion 'ttn_loop()'
    tiempoPrevio = millis();
    first = false;
    Serial.println(F("===================")); Serial.print(F("TRANSMISION NUMERO ")); Serial.println(bootCount + 1); Serial.println(F("==================="));
    do_send();                                                           // Llamamos a la función "send()", encargada de enviar los datos por LoRa
  }
}
// ===========================================================================================================================================================

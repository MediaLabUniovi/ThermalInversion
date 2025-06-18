/* ***********************************************************************************************************************************************************
ARCHIVO DE CLAVES OTAA PARA TTN

Se debe tener especial cuidado copiando las claves desde la consola de TTN ya que algunas deben venir en formato LSB y, otras, en MSB
*********************************************************************************************************************************************************** */
#pragma once

// Only one of these settings must be defined
//#define USE_ABP
#define USE_OTAA

#ifdef USE_ABP  // UPDATE WITH YOUR TTN KEYS AND ADDR. ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //static const PROGMEM u1_t NWKSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  //static const u1_t PROGMEM APPSKEY[16] = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };
  //static const u4_t DEVADDR = 0x26010000 ;                                                   // <-- Change this address for every node!
#endif

#ifdef USE_OTAA  // UPDATE WITH YOUR TTN KEYS ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
  //Sensor01
  //static const u1_t PROGMEM APPEUI[8]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // Debe ir en formato LSB (least-significant-byte first)
  //static const u1_t PROGMEM DEVEUI[8]  = { 0x36, 0x95, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };  // Debe ir en formato LSB (least-significant-byte first)
  //static const u1_t PROGMEM APPKEY[16] = { 0x50, 0x20, 0x06, 0x60, 0xF1, 0x40, 0x30, 0x5F, 0xC5, 0xAB, 0x9D, 0x51, 0xAC, 0x73, 0xC2, 0x34 };  // Debe ir en formato MSB (most-significant-byte first)

  //Sensor02
  //static const u1_t PROGMEM APPEUI[8]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // Debe ir en formato LSB (least-significant-byte first)
  //static const u1_t PROGMEM DEVEUI[8]  = { 0x40, 0x96, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };  // Debe ir en formato LSB (least-significant-byte first)
  //static const u1_t PROGMEM APPKEY[16] = { 0x17, 0x1F, 0xEC, 0xA7, 0x4B, 0x91, 0x22, 0x24, 0x99, 0x4F, 0x74, 0x40, 0xF7, 0x20, 0x38, 0x4A };  // Debe ir en formato MSB (most-significant-byte first)

  //Sensor03
  static const u1_t PROGMEM APPEUI[8]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // Debe ir en formato LSB (least-significant-byte first)
  static const u1_t PROGMEM DEVEUI[8]  = { 0x73, 0xF6, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };  // Debe ir en formato LSB (least-significant-byte first)
  static const u1_t PROGMEM APPKEY[16] = { 0x46, 0xCF, 0x67, 0xE2, 0xCB, 0x2F, 0xD6, 0xEC, 0x0A, 0x0E, 0xC1, 0xED, 0x63, 0x9C, 0xED, 0xF9 };  // Debe ir en formato MSB (most-significant-byte first)
  
  //Sensor04
  //static const u1_t PROGMEM APPEUI[8]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };  // Debe ir en formato LSB (least-significant-byte first)
  //static const u1_t PROGMEM DEVEUI[8]  = { 0x74, 0xF6, 0x06, 0xD0, 0x7E, 0xD5, 0xB3, 0x70 };  // Debe ir en formato LSB (least-significant-byte first)
  //static const u1_t PROGMEM APPKEY[16] = { 0x6C, 0x79, 0x42, 0x69, 0x5E, 0xAF, 0x77, 0xA4, 0x08, 0x14, 0xE6, 0xC6, 0xC0, 0x11, 0xEB, 0x16 };  // Debe ir en formato MSB (most-significant-byte first)
#endif
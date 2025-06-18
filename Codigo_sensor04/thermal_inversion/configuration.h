/* ***********************************************************************************************************************************************************
TTGO LoRa32 V2.1 for The Things Network

Copyright (C) 2019 by Roel van Wanrooy (www.connectix.nl)

This code requires LMIC library by Matthijs Kooijman: https://github.com/matthijskooijman/arduino-lmic

This program is free software: you can redistribute it and/or modify it under the terms of the GNU General Public License as published by the Free Software
Foundation, either version 3 of the License, or (at your option) any later version.

This program is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE.  See the GNU General Public License for more details.

You should have received a copy of the GNU General Public License along with this program.  If not, see <http://www.gnu.org/licenses/>.
*********************************************************************************************************************************************************** */
#pragma once

#include <Arduino.h>
#include <lmic.h>
void ttn_register(void (*callback)(uint8_t message));

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Configuration
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
//#define SINGLE_CHANNEL_GATEWAY  0           // If using a single-channel gateway, uncomment this next option and set to your gateway's channel
#define CLOCK_ERROR             1           // If you are having difficulty sending messages to TTN after the first successful send, uncomment the next option and experiment with values (~ 1 - 5)
#define DEBUG_PORT              Serial      // Serial debug port
#define SERIAL_BAUD             115200      // Serial debug baud rate
#define ENABLE_DEBUG            1           // Serial debug on/off
#define SEND_INTERVAL           900000      // Tiempo de espera entre mensajes
#define MESSAGE_TO_SLEEP_DELAY  1000        // Time after message before going to sleep
#define LORAWAN_PORT            1           // Port the messages will be sent to
#define LORAWAN_SF              DR_SF12     // Spreading factor
#define LORAWAN_ADR             0           // Enable ADR
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// DEBUG
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#ifdef DEBUG_PORT
#define DEBUG_MSG(...) DEBUG_PORT.printf( __VA_ARGS__ )
#else
#define DEBUG_MSG(...)
#endif
// -----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// Custom messages
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#define EV_QUEUED       100
#define EV_PENDING      101
#define EV_ACK          102
#define EV_RESPONSE     103
// -----------------------------------------------------------------------------------------------------------------------------------------------------------

// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// LoRa SPI
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#define SCK_GPIO        5
#define MISO_GPIO       19
#define MOSI_GPIO       27
#define NSS_GPIO        18
#define RESET_GPIO      23
#define DIO0_GPIO       26
#define DIO1_GPIO       33
#define DIO2_GPIO       32
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
// AXP192 (Rev1-specific options)
// -----------------------------------------------------------------------------------------------------------------------------------------------------------
#define LORA_POWER_CTRL_CH    2
#define PMU_IRQ               35
// -----------------------------------------------------------------------------------------------------------------------------------------------------------

// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
// Aportes (sensores, variables, ...)
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
#define SDS_RX          14                  // Pin RX del ESP32 al pin TX del sensor
#define SDS_TX          12                  // Pin TX del ESP32 al pin RX del sensor
#define VBAT_PIN        34                  // Pin al que conecto la batería para medir su voltaje
#define TX_BUFFER_SIZE  14                  // Tamaño del buffer de bytes a enviar a TTN
// ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
# ThermalInversion – Sistema IoT para Monitorización Ambiental

Este proyecto consiste en el desarrollo de un sistema de sensores autónomo basado en **LoRaWAN**, diseñado para monitorizar variables ambientales que afectan a la calidad del aire. Está orientado a la detección de patrones asociados a fenómenos atmosféricos persistentes mediante la recopilación de datos en tiempo real y su transmisión a plataformas de análisis.

## Características principales

- Sensorización ambiental en tiempo real: PM2.5, PM10, temperatura, humedad, presión y nivel de batería.
- Comunicación mediante **LoRaWAN** a través de **The Things Network (TTN)**.
- Consumo energético optimizado gracias a ciclos de deep sleep.
- Código modular en C++ compatible con **Arduino IDE** o **PlatformIO**.

## Hardware utilizado

- **Microcontrolador**: LILYGO T3 V1.6.1 (ESP32 + LoRa)
- **Sensor de partículas**: SDS011
- **Sensor climático**: BME280

## Estructura del firmware

- thermal_inversion.ino       # Lógica principal
- configuration.h             # Pines, intervalos, ajustes LoRa
- credentials.h               # Claves LoRaWAN (DevEUI, AppEUI, AppKey)
- sensor.{h,ino}              # Lectura de sensores BME280 y SDS011
- lvlbat.{h,ino}              # Medición de batería
- sleep.{h,ino}               # Gestión del deep sleep
- ttn.{h,ino}                 # Comunicación LoRa con TTN (LMIC)

## Cómo empezar

1. Clona el repositorio:
   git clone https://github.com/MediaLabUniovi/ThermalInversion.git

2. Abre el proyecto en Arduino IDE o PlatformIO.

3. Rellena el archivo `credentials.h` con las claves LoRaWAN (DevEUI, AppEUI, AppKey) desde tu cuenta de TTN.

4. Compila y sube el código al dispositivo LILYGO T3 (ESP32).

5. Visualiza los datos en Node-RED/Grafana (requiere configurar MQTT e InfluxDB).


## Objetivos del proyecto

- Crear una solución IoT funcional y de bajo coste.
- Obtener datos ambientales desde ubicaciones sin infraestructura.
- Evaluar la calidad del aire en diferentes condiciones meteorológicas.
- Facilitar la escalabilidad de nodos y su integración en entornos urbanos o rurales.


# 🌫️ ThermalInversion – Sistema IoT para Monitorización Ambiental

Este proyecto consiste en el desarrollo de un sistema de sensores autónomo basado en **LoRaWAN**, diseñado para monitorizar variables ambientales que afectan a la calidad del aire. Está orientado a la detección de patrones asociados a fenómenos atmosféricos persistentes mediante la recopilación de datos en tiempo real y su transmisión a plataformas de análisis.

Se trata de un proyecto desarrollado como Trabajo Fin de Grado en la Universidad de Oviedo y forma parte de las iniciativas de sensorización del entorno impulsadas por el MediaLab de la Escuela Politécnica de Ingeniería de Gijón.

## 📌 Características principales

- Sensorización ambiental en tiempo real: PM2.5, PM10, temperatura, humedad, presión y nivel de batería.
- Comunicación mediante **LoRaWAN** a través de **The Things Network (TTN)**.
- Consumo energético optimizado gracias a ciclos de deep sleep.
- Visualización en tiempo real con **Node-RED** y **Grafana**.
- Almacenamiento de datos en **InfluxDB** para análisis temporal.
- Código modular en C++ compatible con **Arduino IDE** o **PlatformIO**.

## 🛠️ Hardware utilizado

- **Microcontrolador**: LILYGO T3 (ESP32 + LoRa)
- **Sensor de partículas**: SDS011
- **Sensor climático**: BME280
- **Alimentación**: batería 18650 con regulador y opción de panel solar

## 📂 Estructura del firmware

/
├── thermal_inversion.ino       # Lógica principal
├── configuration.h             # Pines, intervalos, ajustes LoRa
├── credentials.h               # Claves LoRaWAN (DevEUI, AppEUI, AppKey)
├── sensor.{h,ino}              # Lectura de sensores BME280 y SDS011
├── lvlbat.{h,ino}              # Medición de batería
├── sleep.{h,ino}               # Gestión del deep sleep
└── ttn.{h,ino}                 # Comunicación LoRa con TTN (LMIC)

## 🚀 Cómo empezar

1. Clona el repositorio:
   git clone https://github.com/MediaLabUniovi/ThermalInversion.git

2. Abre el proyecto en Arduino IDE o PlatformIO.

3. Rellena el archivo `credentials.h` con las claves LoRaWAN (DevEUI, AppEUI, AppKey) desde tu cuenta de TTN.

4. Compila y sube el código al dispositivo LILYGO T3 (ESP32).

5. Visualiza los datos en Node-RED/Grafana (requiere configurar MQTT e InfluxDB).

## 📊 Visualización y análisis

Los datos transmitidos por los nodos se procesan a través de la siguiente arquitectura:

Nodo LoRa ⟶ TTN ⟶ MQTT ⟶ Node-RED ⟶ InfluxDB ⟶ Grafana

- **Node-RED**: decodifica el payload LoRa y enruta los datos.
- **InfluxDB**: almacena los datos como series temporales.
- **Grafana**: dashboards en tiempo real con los valores registrados.

## 🔍 Objetivos del proyecto

- Crear una solución IoT funcional y de bajo coste.
- Obtener datos ambientales desde ubicaciones sin infraestructura.
- Evaluar la calidad del aire en diferentes condiciones meteorológicas.
- Facilitar la escalabilidad de nodos y su integración en entornos urbanos o rurales.

## 🙋 Autor

**Alfonso Fernández Aybar**  
Grado en Ingeniería en Tecnologías y Servicios de Telecomunicación  
Universidad de Oviedo · EPIG  

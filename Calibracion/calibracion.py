import serial
import time
from openpyxl import Workbook

PORT = 'COM17'               
BAUD_RATE = 9600
OUTPUT_FILE = 'datos_sensor_1_pruebacasa.xlsx'

print("Recogiendo datos...")

# Crear archivo Excel
wb = Workbook()
ws = wb.active
ws.title = "Mediciones SDS011 + BME280"
ws.append(['Timestamp', 'PM2.5 (µg/m³)', 'PM10 (µg/m³)', 'Temperatura (°C)', 'Humedad (%)'])  # Encabezados actualizados

try:
    with serial.Serial(PORT, BAUD_RATE, timeout=2) as ser:
        while True:
            try:
                raw_line = ser.readline()
                try:
                    line = raw_line.decode('utf-8').strip()
                except UnicodeDecodeError:
                    continue  # Ignora líneas no válidas

                # Formato esperado: PM2.5:xx.x	PM10:yy.y	Temp:zz.z C	Humedad:hh.h %
                if line.startswith("PM2.5:") and "Humedad:" in line:
                    parts = line.replace("PM2.5:", "").replace("PM10:", "").replace("Temp:", "").replace(" C", "").replace("Humedad:", "").replace(" %", "").split("\t")
                    if len(parts) == 4:
                        pm25 = float(parts[0])
                        pm10 = float(parts[1])
                        temp = float(parts[2])
                        humedad = float(parts[3])
                        timestamp = time.strftime('%Y-%m-%d %H:%M:%S')
                        ws.append([timestamp, pm25, pm10, temp, humedad])
                        print(f"{timestamp} → PM2.5: {pm25} µg/m³, PM10: {pm10} µg/m³, Temp: {temp} °C, Humedad: {humedad} %")

            except KeyboardInterrupt:
                print("Interrumpido por el usuario.")
                break

finally:
    wb.save(OUTPUT_FILE)
    print(f"Archivo guardado como '{OUTPUT_FILE}'")

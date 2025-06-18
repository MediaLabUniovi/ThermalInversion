import requests
import pandas as pd
import json
import time
from datetime import datetime
from zoneinfo import ZoneInfo  # Python 3.9+

# CONFIGURACIÓN
INTERVALO_HORAS = 1  
SEGUNDOS_ENTRE_EJECUCIONES = INTERVALO_HORAS * 3600

API_KEY = "NNSXS.5FX4DV4XBOE4QGWFZUBONX77CKWY6SSUK6QPO5I.X2XCCHND5GNAOXOIQCZHHIK3VN5BCCOWHRPYP6FVGZ4Z5MOTDGCQ"
url = "https://eu1.cloud.thethings.network/api/v3/as/applications/thermal-inversion/packages/storage/uplink_message"

headers = {
    "Authorization": f"Bearer {API_KEY}",
    "Accept": "text/event-stream"
}
params = {
    "last": "1h"
}

def ejecutar_descarga_y_guardado():
    response = requests.get(url, headers=headers, params=params, stream=True)

    data_list = []

    if response.status_code == 200:
        print("Conectado. Descargando datos...")

        for line in response.iter_lines():
            if line:
                decoded_line = line.decode('utf-8')
                try:
                    raw_data = json.loads(decoded_line)
                    result = raw_data.get("result", {})
                    uplink = result.get("uplink_message", {})
                    payload = uplink.get("decoded_payload", {})
                    metadata = result.get("end_device_ids", {})

                    timestamp = result.get("received_at", "")
                    fecha, hora = "", ""

                    try:
                        dt_utc = datetime.fromisoformat(timestamp.replace("Z", "+00:00"))
                        dt_local = dt_utc.astimezone(ZoneInfo("Europe/Madrid"))
                        fecha = dt_local.strftime("%Y-%m-%d")
                        hora = dt_local.strftime("%H:%M:%S")
                    except Exception:
                        pass

                    record = {
                        "device_id": metadata.get("device_id", ""),
                        "Fecha": fecha,
                        "Hora": hora,
                        "Batería [%]": payload.get("battery"),
                        "Humedad [%]": payload.get("humidity"),
                        "PM10 [µg/m³]": payload.get("pm10"),
                        "PM2.5 [µg/m³]": payload.get("pm25"),
                        "Presión [hPa]": payload.get("pressure"),
                        "Temperatura [°C]": payload.get("temperature")
                    }

                    data_list.append(record)

                except json.JSONDecodeError:
                    print("❌ Línea no es JSON válido:", decoded_line)

    else:
        print(f"Error HTTP: {response.status_code}")
        return

    # Guardar en Excel
    if data_list:
        df = pd.DataFrame(data_list)
        grouped = df.groupby("device_id")

        fecha_actual = datetime.now(ZoneInfo("Europe/Madrid")).strftime("%Y-%m-%d")
        nombre_archivo = f"datos_ttn_inversion_termica_{fecha_actual}.xlsx"

        with pd.ExcelWriter(nombre_archivo, engine="openpyxl") as writer:
            for device_id, group in grouped:
                group_sorted = group.sort_values(by=["Fecha", "Hora"])
                group_sorted.drop(columns=["device_id"], inplace=True)
                group_sorted.to_excel(writer, sheet_name=device_id[:31], index=False)

        print(f"✅ Datos guardados en '{nombre_archivo}'")
    else:
        print("⚠️ No se recibieron datos para guardar.")


# Bucle infinito
while True:
    print(f"🕐 Ejecutando descarga a las {datetime.now().strftime('%Y-%m-%d %H:%M:%S')}")
    ejecutar_descarga_y_guardado()
    print(f"⏳ Esperando {INTERVALO_HORAS} horas para la próxima ejecución...\n")
    time.sleep(SEGUNDOS_ENTRE_EJECUCIONES)

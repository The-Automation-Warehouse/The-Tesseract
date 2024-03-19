import serial
import json
import time
import asyncio

EOT = '\x04'  # ASCII 'End of Transmission' character


@service
def send_data(action=None):


    if action == "send_data_to_tesseract":

        #log.info(f"{binary_sensor.omnitouch_button}")
        if binary_sensor.omnitouch_button == "off" and binary_sensor.omnitouch_asistence_probiha == "off":
            data = {
                'b': 0,
                'm': sensor.omnitouch_mode,
                'c': sensor.omnitouch_heading,
                'h': sensor.omnitouch_hall_effect,
                't': sensor.omnitouch_temperature,
                'a': 0,
            }
        elif binary_sensor.omnitouch_button == "on" and binary_sensor.omnitouch_asistence_probiha == "off":
            data = {
                'b': 1,
                'm': sensor.omnitouch_mode,
                'c': sensor.omnitouch_heading,
                'h': sensor.omnitouch_hall_effect,
                't': sensor.omnitouch_temperature,
                'a': 0,
            }
        elif binary_sensor.omnitouch_button == "off" and binary_sensor.omnitouch_asistence_probiha == "on":
            # set the first sensor to 3
            data = {
                'b': 0,
                'm': sensor.omnitouch_mode,
                'c': sensor.omnitouch_heading,
                'h': sensor.omnitouch_hall_effect,
                't': sensor.omnitouch_temperature,
                'a': 1,
            }
        elif binary_sensor.omnitouch_button == "on" and binary_sensor.omnitouch_asistence_probiha == "on":
            # set the first sensor to 4
            data = {
                'b': 1,
                'm': sensor.omnitouch_mode,
                'c': sensor.omnitouch_heading,
                'h': sensor.omnitouch_hall_effect,
                't': sensor.omnitouch_temperature,
                'a': 1,
            }
        elif binary_sensor.omnitouch_button == "unavailable":
            # set the first sensor to 2
            data = {
                'b': 2,
                'm': sensor.omnitouch_mode,
                'c': sensor.omnitouch_heading,
                'h': sensor.omnitouch_hall_effect,
                't': sensor.omnitouch_temperature,
                'a': 0,
            }
            
        json_data = json.dumps(data)
        ser = serial.Serial('/dev/serial0', 115200)
        ser.write(json_data.encode())
        ser.write(EOT.encode())
        asyncio.sleep(0.1)
        ser.close()

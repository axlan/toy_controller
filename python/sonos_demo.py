#!/usr/bin/env python
from soco import SoCo

import paho.mqtt.client as mqtt
import json
import requests

# Connect to the Sonos
sonos = SoCo('192.168.1.109')

# Define MQTT callbacks

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("controller/buttons_pressed")

def on_message(client, userdata, msg):
    json_str = msg.payload.decode('ascii')
    print(msg.topic+" "+json_str)
    new_buttons = json.loads(json_str)

    try:
        if 'LEFT' in new_buttons:
            sonos.previous()
        elif 'RIGHT' in new_buttons:
            sonos.next()
        elif 'UP' in new_buttons:
            pass
        elif 'DOWN' in new_buttons:
            pass

        if 'A' in new_buttons:
            sonos.play()
        elif 'B' in new_buttons:
            sonos.play()
        elif 'C' in new_buttons:
            sonos.pause()
        elif 'D' in new_buttons:
            sonos.pause()
    except Exception as e:
        print(e)

# Create an MQTT client
client = mqtt.Client()

# Set MQTT callbacks
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
client.connect("192.168.1.110", 1883, 60)

wled_url = "http://192.168.1.123/json/state"

# resp = requests.get(wled_url)
# cur_state = json.dumps(resp.json())

while True:
    client.loop(timeout=1.0, max_packets=1)

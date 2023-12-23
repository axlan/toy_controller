#!/usr/bin/env python
import paho.mqtt.client as mqtt
import json
import requests
import dataclasses

@dataclasses.dataclass
class State:
    selected = 0
    color = [0, 0, 0]
    sent = True
    last_buttons = []

state = State()

# Define MQTT callbacks

def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("controller/buttons_pressed")

def on_message(client, userdata, msg):
    json_str = msg.payload.decode('ascii')
    print(msg.topic+" "+json_str)
    new_buttons = json.loads(json_str)
    pressed = [b for b in new_buttons if b not in state.last_buttons]
    state.last_buttons = new_buttons

    if len(pressed) == 0:
        return

    state.sent = False
    if 'LEFT' in pressed:
        state.selected = 2
    elif 'RIGHT' in pressed:
        state.selected = 0
    elif 'UP' in pressed:
        state.selected = 1
    elif 'DOWN' in pressed:
        state.selected = 3

    if 'A' in pressed:
        state.color = [0, 255, 0]
    elif 'B' in pressed:
        state.color = [0, 0, 255]
    elif 'C' in pressed:
        state.color = [255, 0, 0]
    elif 'D' in pressed:
        state.color = [255, 255, 0]

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

while client.loop(timeout=1.0, max_packets=1) == mqtt.MQTT_ERR_SUCCESS:
    if not state.sent:
        state.sent = True

        resp = requests.post(wled_url, json={
            'seg': [{
                'id': state.selected,
                'col': [state.color, [0, 0, 0], [0, 0, 0]]
            }]
        })
        print(resp.json())

print('Connection Failure')

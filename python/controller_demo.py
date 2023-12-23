#!/usr/bin/env python
import pygame
import paho.mqtt.client as mqtt
import json

pressed_buttons = []

# Define MQTT callbacks
def on_connect(client, userdata, flags, rc):
    print("Connected with result code "+str(rc))
    client.subscribe("controller/buttons_pressed")

def on_message(client, userdata, msg):
    json_str = msg.payload.decode('ascii')
    print(msg.topic+" "+json_str)
    pressed_buttons.clear()
    for val in json.loads(json_str):
      pressed_buttons.append(val)


# Create an MQTT client
client = mqtt.Client()

# Set MQTT callbacks
client.on_connect = on_connect
client.on_message = on_message

# Connect to the MQTT broker
client.connect("192.168.1.110", 1883, 60)

# Initialize pygame
pygame.init()

# Set the width and height of the screen
screen_width = 800
screen_height = 600
screen = pygame.display.set_mode((screen_width, screen_height))

# Load the sprite image
sprite_image = pygame.image.load("controller.png")

# Create a surface for the mask
mask_surface = pygame.Surface((60, 60), pygame.SRCALPHA)
# Set the mask color
mask_color = (0, 255, 0, 128)
pygame.draw.circle(mask_surface, mask_color, (30, 30), 30)


# Set the sprite position
sprite_x = 0
sprite_y = 0

BUTTON_LOCATIONS = {
    'LEFT' : (135, 385),
    'RIGHT' : (250, 362),
    'UP' : (185, 315),
    'DOWN' : (208, 427),
    'A' : (470, 197),
    'B' : (570, 145),
    'C' : (628, 235),
    'D' : (535, 287),
}

# Set the color changing frequency (in milliseconds)
color_frequency = 1000

# Run the game loop
running = True
while running:
    # Handle events
    for event in pygame.event.get():
        if event.type == pygame.QUIT:
            running = False

    # Clear the screen
    screen.fill((255, 255, 255))

    screen.blit(sprite_image, (0, 0))

    for button in pressed_buttons:
      screen.blit(mask_surface, BUTTON_LOCATIONS[button])

    # Update the screen
    pygame.display.flip()

    client.loop(timeout=1.0, max_packets=1)

# Quit pygame
pygame.quit()

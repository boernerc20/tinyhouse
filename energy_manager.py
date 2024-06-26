##########################################################################
# File name: energy_manager.py                                           #
# Author: Christopher Boerner                                            #
# Description: Script to manage PV and house consumption on raspberry pi #
##########################################################################

import paho.mqtt.client as mqtt
import time
import re

# MQTT settings
BROKER = "localhost"
PV_TOPIC = "ENMGR/PV"
HOUSE_TOPIC_BASE = "M"
NUM_HOUSES = 6

# Constants for PV power calculation
VOLTAGE_TO_POWER_RATIO = 7.2 / 2  # 2V equals 7.2kW

# Power ratings for appliances for each house (example values, replace with actual data)
appliance_power = {
    "M1": {
        # Define appliances for house M1
    },
    "M2": {
        # Define appliances for house M2
    },
    "M3": {
        # Define appliances for house M3
    },
    "M4": {
        # Define appliances for house M4
        "oven": 2000,
        "dish": 500,
        "hotplates": 2500,
        "bedroom2": 30,
        "kitchen": 30,
        "fridge": 100,
        "washing": 1000,
        "television": 100,
        "lounge": 30,
        "car": 30,
        "bedside": 30,
        "bedroom1": 30
    },
    "M5": {
        # Define appliances for house M5
    },
    "M6": {
        # Define appliances for house M6
        "oven": 2000,
        "boiler": 500,
        "hotplates": 2500,
        "bedroom2": 30,
        "kitchen": 30,
        "fridge": 100,
        "washing": 1000,
        "television": 100,
        "lounge": 30,
        "car": 30,
        "bedside": 30,
        "bedroom1": 30
    }
}

# Track power consumption and state for each appliance in each house
house_consumption = {f"M{i}": 0.0 for i in range(1, NUM_HOUSES + 1)}
appliance_state = {f"M{i}": {appl: False for appl in appliance_power[f"M{i}"]} for i in range(1, NUM_HOUSES + 1)}

# Callback when the client receives a message from the broker
def on_message(client, userdata, msg):
    global house_consumption, appliance_power, appliance_state
    topic = msg.topic
    payload = msg.payload.decode()
    print(f"Message received on topic {topic}: {payload}")

    # Process PV voltage reading
    if topic == PV_TOPIC:
        match = re.search(r"Panel Voltage:\s*(\d+\.\d+)\s*V", payload)
        if match:
            voltage = float(match.group(1))
            power_generated = voltage * VOLTAGE_TO_POWER_RATIO * 1000  # kW to W
            print(f"Power generated: {power_generated:.2f} W")
    # Check if the topic starts with the base topic for houses (e.g., "M1", "M2", etc.)
    elif topic.startswith(HOUSE_TOPIC_BASE):
        house_number = topic.split('/')[0]  # Extract the house number from the topic (e.g., "M1" from "M1/oven")
        appliance = topic.split('/')[1]  # Extract the appliance from the topic (e.g., "oven" from "M1/oven")
        
        # Split the payload to get the command and the value (state or wattage)
        data = payload.split()
        if len(data) >= 2:
            command = data[0].strip()  # Extract and strip any whitespace from the command
            value = data[1].strip()  # Extract and strip any whitespace from the value

            # For all devices
            if appliance == "all":
                # Turn ON/OFF command
                if command == "turn":
                    state = value == "ON"  # Determine if the command is to turn on (True) or off (False)
                    for appl, power in appliance_power[house_number].items():   # Iterate over each appliance and its power rating
                        if state and not appliance_state[house_number][appl]:   # If turning on and appliance is currently off
                            house_consumption[house_number] += power            # Add the appliance's power to house consumption
                            appliance_state[house_number][appl] = True          # Mark appliance as on
                        elif not state and appliance_state[house_number][appl]: # If turning off and appliance is currently on
                            house_consumption[house_number] = max(house_consumption[house_number] - power, 0)  # Subtract the appliance's power, ensuring no negative consumption
                            appliance_state[house_number][appl] = False         # Mark appliance as off
                    print(f"Turn all appliances {value}")
            else:
                # Turn ON/OFF command
                if command == "turn":
                    state = value == "ON"
                    # Logic for turning ON/OFF appliances set up as done before
                    if state and not appliance_state[house_number][appliance]:
                        house_consumption[house_number] += appliance_power[house_number].get(appliance, 0)
                        appliance_state[house_number][appliance] = True
                    elif not state and appliance_state[house_number][appliance]:
                        house_consumption[house_number] = max(house_consumption[house_number] - appliance_power[house_number].get(appliance, 0), 0)
                        appliance_state[house_number][appliance] = False
                    print(f"Turn {appliance} {value}")
                # Set command
                elif command == "set":
                    wattage = int(value)
                    # Adjust house consumption only if the appliance is currently on
                    if appliance_state[house_number][appliance]:
                        old_wattage = appliance_power[house_number].get(appliance, 0)
                        house_consumption[house_number] += wattage - old_wattage
                    appliance_power[house_number][appliance] = wattage
                    print(f"Set {appliance} to {wattage}W")
                
    print_status()

# Print the current status
def print_status():
    global house_consumption
    total_consumption = sum(house_consumption.values())
    print(f"Total Consumption: {total_consumption:.2f} W")
    for house, consumption in house_consumption.items():
        print(f"{house} Consumption: {consumption:.2f} W")
    print("-" * 40)

# MQTT setup
client = mqtt.Client()
client.on_message = on_message

client.connect(BROKER)
client.loop_start()

# Subscribe to all house topics and PV production topic
client.subscribe(PV_TOPIC)
for i in range(1, NUM_HOUSES + 1):
    client.subscribe(f"{HOUSE_TOPIC_BASE}{i}/#")

# Keep script running while reducing CPU usage
try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Exiting...")
    client.loop_stop()
    client.disconnect()

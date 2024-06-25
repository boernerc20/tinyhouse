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

# Callback when the client receives a message from the broker
def on_message(client, userdata, msg):
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
        else:
            print("Invalid voltage reading format")

# MQTT setup
client = mqtt.Client()
client.on_message = on_message

client.connect(BROKER)
client.loop_start()

client.subscribe(PV_TOPIC)
for i in range(1, NUM_HOUSES + 1):
    client.subscribe(f"{HOUSE_TOPIC_BASE}{i}/#")

try:
    while True:
        time.sleep(1)
except KeyboardInterrupt:
    print("Exiting...")
    client.loop_stop()
    client.disconnect()

# tinyhouse
Folder/File Organization:
- smartHouseM#: Include the Arduino code for each house to communicate with the ESP8266 over UART and physically control the miniature houses.
- ESP8266_smartHouse: The Arduino code for the ESP8266 wifi-enabled microcontroller and allow communication with the MQTT broker.
- I2C_Scanner & WiFiScan: Helper Arduino code for checking I2C buses and scanning for WiFi networks by the ESP8266.
- PV_Sim: Arduino code for another ESP8266 connected to a photovoltaic panel simulation and publish the voltage readings to the broker.
- energy_manager.py: Python script run on a raspberry pi that monitors the PV readings and the commands sent to the houses. This script than updates the energy consumption (wattage) for the PV panels and houses.

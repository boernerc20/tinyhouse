#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>
#include <cstring>

// Configuration files
#include "config_M1.h"
#include "config_M2.h"
#include "config_M3.h"
#include "config_M4.h"
#include "config_M5.h"

struct Device {
  const char* name;
  float consumption;
  int pinLED;
  bool manualControl; 
  bool manualOrder;   
  int beginHours[Max_slot];
  int endHours[Max_slot];
  int numbSlot; 
};

// WiFi and MQTT Server details
const char* ssid = "OpenWrt";
const char* password = "G2ElabMonitoringHabitat";
const char* mqtt_server = "192.168.1.192"; 
const int mqtt_port = 1883; 

WiFiClient espClient;
PubSubClient client(espClient);

Device devices[] = { 
  {"oven", 2000, led_oven, 0, 0,{0},{24},1},
  {"dish", 500, led_dish, 0, 0,{0},{24},1},
  {"hotplates", 2500, led_hotplates, 0, 0, {12,19},{13,20},2},
  {"bedroom2",30,led_bedroom2,0,0,{6,20},{8,22},2},
  {"kitchen",30,led_kitchen,0,0,{6,20},{8,22},2},
  {"fridge", 100, led_fridge, 0, 0,{0},{24},1},
  {"washing", 1000, led_washing, 0, 0, {17},{20},1},
  {"television", 100, led_television, 0, 0,{0},{24},1},
  {"lounge",30,led_lounge,0,0,{6,20},{8,22},2},
  {"car",30,led_car,0,0,{6,20},{8,22},2},
  {"bedside",30,led_bedside,0,0,{6,20},{8,22},2},
  {"bedroom1",30,led_bedroom1,0,0,{6,20},{8,22},2}
};

const int number_devices = sizeof(devices) / sizeof(Device);
const int houseNumber = 4; // Example house number

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // UART1 is GPIO2 - D4

  while (!Serial) {
    ; 
  }

  Serial.println("Select configuration file:");
  Serial.println("1: config_M1");
  Serial.println("2: config_M2");
  Serial.println("3: config_M3");
  Serial.println("4: config_M4");
  Serial.println("5: config_M5");

  int configSelected = 0;
  while (configSelected < 1 || configSelected > 5) {
    if (Serial.available() > 0) {
      configSelected = Serial.parseInt();
      Serial.print("Selected configuration: ");
      Serial.println(configSelected);
    }
  }

  setup_wifi(); 
  client.setServer(mqtt_server, mqtt_port); 
  client.setCallback(callback); 
}

void setup_wifi() {
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println();
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (unsigned int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();

  payload[length] = '\0';
  String message = String((char*)payload);
  String houseNumberStr = String(houseNumber);

  // Extract appliance from topic
  String topicStr = String(topic);
  int lastSlash = topicStr.lastIndexOf('/');
  String appliance = topicStr.substring(lastSlash + 1);

  // Construct the command and value from the message
  String command, value;
  int spaceIndex = message.indexOf(' ');
  if (spaceIndex != -1) {
    command = message.substring(0, spaceIndex);
    value = message.substring(spaceIndex + 1);
  } else {
    command = message;
    value = "";
  }

  sendUARTMessage(houseNumberStr.c_str(), appliance.c_str(), command.c_str(), value.c_str());
}

void sendUARTMessage(const char* houseNumber, const char* appliance, const char* command, const char* value) {
  String msg = "<" + String(houseNumber) + "," + String(appliance) + "," + String(command) + "," + String(value) + ">";
  Serial1.print(msg.c_str()); 
  Serial.println("Sent UART Message: " + msg); 
}

void reconnect() {
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      client.subscribe("M4/#"); // Subscribe to all topics under house M4
    } 
    else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();
}

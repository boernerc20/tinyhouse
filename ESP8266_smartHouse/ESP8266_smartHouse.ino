#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <Arduino.h>
#include <cstring>

// WiFi and MQTT Server details
const char* ssid = "OpenWrt";
const char* password = "G2ElabMonitoringHabitat";
const char* mqtt_server = "192.168.1.192"; 
const int mqtt_port = 1883; 

int number_houses = 6; // Replace with number of houses connected to broker

WiFiClient espClient;
PubSubClient client(espClient);

void setup() {
  Serial.begin(9600);
  Serial1.begin(9600); // UART1 is GPIO2 - D4

  while (!Serial) {
    ; 
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

  // Extract house number and appliance from topic
  String topicStr = String(topic);
  int firstSlash = topicStr.indexOf('/');
  int lastSlash = topicStr.lastIndexOf('/');
  String houseNumber = topicStr.substring(0, firstSlash);
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

  sendUARTMessage(houseNumber.c_str(), appliance.c_str(), command.c_str(), value.c_str());
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
      for(int i = 0; i <= number_houses; i++){
        String sub = "M" + String(i) + "/#";
        client.subscribe(sub.c_str()); 
      }
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

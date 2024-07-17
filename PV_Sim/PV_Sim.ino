#include <ESP8266WiFi.h>
#include <PubSubClient.h>

// WiFi and MQTT Server details
const char* ssid = "OpenWrt";
const char* password = "G2ElabMonitoringHabitat";
// const char* ssid = "OpenWrt_5G";
// const char* password = "openwrt2022";
const char* mqtt_server = "192.168.1.192";
const int mqtt_port = 1883;

WiFiClient espClient;
PubSubClient client(espClient);

const int analogPin = A0; // Analog pin for voltage reading
const int numSamples = 100; // Number of samples for averaging
const float calibrationFactor = 0.93; // Adjust this value based on calibration of ADC

void setup() {
  Serial.begin(9600);

  // Connect to WiFi
  setup_wifi();

  // Setup MQTT
  client.setServer(mqtt_server, mqtt_port);
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

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection...");
    // Attempt to connect
    if (client.connect("ESP8266Client")) {
      Serial.println("connected");
      // Once connected, publish an announcement
      client.publish("ENMGR/PV", "ESP8266 connected");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void loop() {
  if (!client.connected()) {
    reconnect();
  }
  client.loop();

  long sum = 0;

  // Take multiple samples
  for (int i = 0; i < numSamples; i++) {
    sum += analogRead(analogPin);
    delay(10); // Short delay between samples
  }

  // Calculate the average
  int avgValue = sum / numSamples;
  float voltage = avgValue * (3.3 / 1024.0) * calibrationFactor; // Convert ADC value to voltage with calibration

  Serial.print("Panel Voltage: ");
  Serial.print(voltage, 3); // Print voltage with 3 decimal places
  Serial.println(" V");

  // Convert voltage to string and concatenate with "Panel Voltage: "
  char voltageString[50];
  snprintf(voltageString, sizeof(voltageString), "Panel Voltage: %.3f V", voltage);

  // Publish the concatenated string to MQTT topic
  client.publish("ENMGR/PV", voltageString);

  delay(500); // Read every 500 ms
}

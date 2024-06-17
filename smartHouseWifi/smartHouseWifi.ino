#include <ESP8266WiFi.h>
#include <Wire.h>

int port = 8888;  // Port number
WiFiServer server(port);

// Test variable for I2C
byte x = 0;

// Server connect to WiFi Network
const char *ssid = "ChrisPhone";  // Enter your wifi SSID
const char *password = "test1234";  // Enter your wifi Password

void setup() 
{
  // Set ESP8266 as the master
  Wire.begin(D2, D1); // SDA = D2 (GPIO-4), SCL = D1 (GPIO-5)

  Serial.begin(9600);
  Serial.println();

  WiFi.mode(WIFI_STA); // Wifi in station mode
  WiFi.begin(ssid, password); // Connect to wifi
 
  // Wait for connection  
  Serial.println("Connecting to Wifi");
  while (WiFi.status() != WL_CONNECTED) {   
    delay(500);
    Serial.print(".");
    delay(500);
  }

  Serial.println("");
  Serial.print("Connected to ");
  Serial.println(ssid);

  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());  
  server.begin();
  Serial.print("Open Telnet and connect to IP:");
  Serial.print(WiFi.localIP());
  Serial.print(" on port ");
  Serial.println(port);
}

void loop() 
{
  WiFiClient client = server.available();
  
  if (client) {
    if(client.connected()) {
      Serial.println("Client Connected");
    }
    
    while(client.connected()){      
      while(client.available() > 0){
        x = client.read(); // Read data from the connected client
        Serial.write(x); // Print the received data to the Serial Monitor
        Wire.beginTransmission(1); // Transmit to device #1
        Wire.write(x); // Send the byte
        Wire.endTransmission(); // Stop transmitting
      }
    }
    client.stop();
    Serial.println("Client disconnected");    
  }
}

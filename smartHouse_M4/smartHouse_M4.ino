/*
Main program for each smart house
18/06/2024, Christopher Boerner
*/ 

#include "config_main.h"
#include "config_M4.h" // Change to house number
#include <LiquidCrystal.h>
#include <Wire.h>

volatile bool messageReceived = false;
String message = "";
bool receivingMessage = false;

float globalPower;  
unsigned long old_time, current_time;     
int globalTime = 0;
byte packet = 0;

String houseNumber = "";
String appliance = "";
String command = "";
String value = "";

int control = 1; // Change between 1 - manualWired, 2 - manualWireless, 3 - automatic

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);  

Device* devices = nullptr;
int number_devices;

void setup() {
  Serial.begin(9600);  
  Serial1.begin(9600);   

  Serial.println("Select mode for house:");
  Serial.println("1: Manual Wired Mode");
  Serial.println("2: Manual Wireless Mode");
  Serial.println("3: Automatic Mode");

  // while (control < 1 || control > 3) {
  //   if (Serial.available() > 0) {
  //     control = Serial.parseInt();
  //     Serial.print("Selected mode: ");
  //     Serial.println(control);
  //   }
  // }

  // Change based on house number
  devices = devices4;
  number_devices = sizeof(devices4) / sizeof(Device); 
  Serial.print("Number of devices simulated : ");
  Serial.println(number_devices);

  setupLCD();

  setup_LED(); 
  pinMode(CH1,INPUT);
  pinMode(CH2,INPUT);
  pinMode(CH3,INPUT);
  pinMode(CH4,INPUT);
  pinMode(CH5,INPUT);
  pinMode(CH6,INPUT);
  pinMode(CH7,INPUT); 

  if(control == 1 || control == 2){
    for (int i = 0; i < number_devices; i++) {
      setControl(devices[i].name, 1);
    }
  }
  old_time = millis();
}

void loop() {
  current_time = millis(); 

  if (current_time - old_time >= deltaT) {
    globalTime = (globalTime + 1) % 24;

    if(control == 1){
      setOrder("washing",digitalRead(CH1));  
      setOrder("hotplates",digitalRead(CH2)); 
      setOrder("bedroom1",digitalRead(CH3));
      setOrder("television",digitalRead(CH4));
      setOrder("oven",digitalRead(CH5));
      setOrder("bedroom2",digitalRead(CH7)); 

      packet = 0;

      packet |= (digitalRead(CH1) << 0);
      packet |= (digitalRead(CH2) << 1);
      packet |= (digitalRead(CH3) << 2);
      packet |= (digitalRead(CH4) << 3);
      packet |= (digitalRead(CH5) << 4);
      packet |= (digitalRead(CH6) << 5);
      packet |= (digitalRead(CH7) << 6);

      // Print packet for debugging
      Serial.print("Packed data: ");
      for (int i = 7; i >= 0; i--) {
        Serial.print((packet >> i) & 1);
      }
      Serial.println();

      // Send packet over UART
      Serial1.write(packet);
    }
    else if(control == 2){
      receiveEvent(); 
      processMessage();
    }
    else if(control == 3){
      Serial.println("Auto");
    }

    globalPower = 0;
    powerConsumption();  

    Serial.print("Actual hour: ");
    Serial.print(globalTime);
    Serial.print("h, Power consumed: ");
    Serial.print(globalPower);
    Serial.println(" W");
    
    updatePowerLCD();  
    
    old_time = current_time;  
  }
}

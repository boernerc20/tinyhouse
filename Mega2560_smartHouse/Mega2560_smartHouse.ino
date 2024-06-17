#include "config_M4.h"  // Replace by the number of your house
#include <LiquidCrystal.h>
#include <Wire.h>

volatile bool messageReceived = false;
String message = "";
bool receivingMessage = false;

float globalPower;  
unsigned long old_time, current_time;     
int globalTime = 0;
String houseNumber = "";
String appliance = "";
String command = "";
String value = "";

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);  

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

void setup() {
  Serial.begin(9600);  
  Serial1.begin(9600);   
  Serial.print("Number of devices simulated: ");
  Serial.println(number_devices);
  
  setup_LED();  

  setupLCD();

  for (int i = 0; i < number_devices; i++) {
    setControl(devices[i].name, 1);
  }
  old_time = millis();
}

void receiveEvent() {
  while (Serial1.available() > 0) {
    char c = Serial1.read();
    if (c == '<') {
      message = "";
      receivingMessage = true;
    } else if (c == '>') {
      receivingMessage = false;
      messageReceived = true;
    } else if (receivingMessage) {
      message += c;
    }
  }
}

void processMessage() {
  if (messageReceived) {
    messageReceived = false;
    Serial.println("Received message: " + message);
    parseMessage(message);  
    message = "";  
  }
}

void parseMessage(String msg) {
  int firstComma = msg.indexOf(',');
  int secondComma = msg.indexOf(',', firstComma + 1);
  int thirdComma = msg.indexOf(',', secondComma + 1);
  
  if (firstComma == -1 || secondComma == -1 || thirdComma == -1) return; 

  houseNumber = msg.substring(0, firstComma);
  appliance = msg.substring(firstComma + 1, secondComma);
  command = msg.substring(secondComma + 1, thirdComma);
  value = msg.substring(thirdComma + 1);
  
  handleCommand();
}

void handleCommand() {
  if (houseNumber != "4") return; // Check if the house number matches
  for (int i = 0; i < number_devices; i++) {
    if (appliance.equals(devices[i].name)) {
      if (command.equals("turn")) {
        int state = value.equals("ON") ? 1 : 0;
        setOrder(devices[i].name, state);
        Serial.print("Turn ");
        Serial.print(devices[i].name);
        Serial.print(state ? " ON" : " OFF");
        Serial.println();
        lcdMessage();
      } 
      else if (command.equals("set")) {
        int consumption = value.toInt();
        // Here you can implement the functionality to set the wattage
        setConsumption(devices[i].name, consumption);
        Serial.print("Set ");
        Serial.print(devices[i].name);
        Serial.print(" to ");
        Serial.print(consumption);
        Serial.println("W");
        lcdMessage();
      }
      break;
    }
  }
  if(appliance.equals("all")){
      if (command.equals("turn")) {
      int state = value.equals("ON") ? 1 : 0;
      for (int i = 0; i < number_devices; i++) {
        setOrder(devices[i].name, state);
      }
      Serial.print("Turn ");
      Serial.print("all");
      Serial.print(state ? " ON" : " OFF");
      Serial.println();
    } 
  }
}

void loop() {
  current_time = millis(); 

  receiveEvent(); 
  processMessage();  
  if (current_time - old_time >= deltaT) {
    globalTime = (globalTime + 1) % 24;

    // int fullManual = 1;
    // if (fullManual == 1) {
    //   // Manual operations handled by handleCommand()
    // } else {
    //   setOrder("hotplates", digitalRead(CH1));
    //   setOrder("bedroom1", digitalRead(CH2)); 
    //   setOrder("washing", digitalRead(CH3));
    //   setOrder("fridge", digitalRead(CH4)); 
    //   setOrder("television", digitalRead(CH5));
    //   setOrder("oven", digitalRead(CH6));
    //   setOrder("bedroom2", digitalRead(CH7));
    // }

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

#include "config_M4.h"  // Replace by the number of your house
#include <LiquidCrystal.h>
#include <Wire.h>

volatile bool messageReceived = false;
String message = "";
bool receivingMessage = false;

float globalPower;  
unsigned long old_time, current_time;     
int globalTime = 0;
String deviceName = "";
int state = -1;

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

  pinMode(CH1, INPUT);
  pinMode(CH2, INPUT);
  pinMode(CH3, INPUT);
  pinMode(CH4, INPUT);
  pinMode(CH5, INPUT);
  pinMode(CH6, INPUT);
  pinMode(CH7, INPUT);

  lcd.begin(16, 2);
  lcd.print("hello, world!");
  delay(2000);  
  lcd.clear();

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
  int separator = msg.indexOf(',');
  if (separator == -1) return;  

  deviceName = msg.substring(0, separator);
  state = msg.substring(separator + 1).toInt();
}

void updateLCD() {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Puissance");
  lcd.setCursor(0, 1);
  lcd.print("consommee:");
  lcd.print(int(globalPower));
  lcd.print("W");
}

void loop() {
  current_time = millis(); 

  receiveEvent(); 
  processMessage();  

  if (current_time - old_time >= deltaT) {
    globalTime = (globalTime + 1) % 24;

    int fullManual = 1;
    if (fullManual == 1) {
      for (int i = 0; i < number_devices; i++) {
        if (deviceName.equals(devices[i].name)) {  
          setOrder(devices[i].name, state);
          Serial.print("Set ");
          Serial.print(devices[i].name);
          Serial.print(" to ");
          Serial.println(state);
          deviceName = "";
          state = -1;
        }
      }
      if (deviceName.equals("all")) {
        for (int i = 0; i < number_devices; i++) {
          setOrder(devices[i].name, state);
          Serial.print("Set ");
          Serial.print(devices[i].name);
          Serial.print(" to ");
          Serial.println(state);
        }
        deviceName = "";
        state = -1;
      }
    } else {
      setOrder("hotplates", digitalRead(CH1));
      setOrder("bedroom1", digitalRead(CH2)); 
      setOrder("washing", digitalRead(CH3));
      setOrder("fridge", digitalRead(CH4)); 
      setOrder("television", digitalRead(CH5));
      setOrder("oven", digitalRead(CH6));
      setOrder("bedroom2", digitalRead(CH7));
    }

    globalPower = 0;
    powerConsumption();  

    Serial.print("Actual hour: ");
    Serial.print(globalTime);
    Serial.print("h, Power consumed: ");
    Serial.print(globalPower);
    Serial.println(" W");
    
    updateLCD();  
    
    old_time = current_time;  
  }
}

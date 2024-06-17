#include "config_M5.h"  //replace by the number off your house
#include <LiquidCrystal.h>

float globalPower;  //global variable representing the consumption of the house 
float measuredPower;//measured power
unsigned long old_time, current_time;     //use of external clock ?
int globalTime = 0;

LiquidCrystal lcd(RS, EN, D4, D5, D6, D7);

Device devices[] = {  //structure defined in config_Mx.h
  //to be completed
  {"hotplates", 2500, led_hotplates, 0, 0, {12,19},{13,20},2},
  {"bedroom1",30,led_bedroom1,0,0,{6,20},{8,22},2},
  {"washing", 1000, led_washing, 0, 0, {17},{20},1},
  {"fridge", 100, led_fridge, 0, 0,{0},{24},1},
  {"television", 100, led_television, 0, 0,{0},{24},1},
  {"oven", 2000, led_oven, 0, 0,{0},{24},1},
  {"boiler", 1500, led_boiler, 0, 0,{0},{24},1},
  {"bedroom2",30,led_bedroom2,0,0,{6,20},{8,22},2},
};

const int number_devices = sizeof(devices)/sizeof(Device);  //nomber of devices registered

void setup() {
  Serial.begin(BaudRate);
  Serial.print("Number of devices simulated : ");
  Serial.println(number_devices);
  
  setup_LED();
  pinMode(CH1,INPUT);
  pinMode(CH2,INPUT);
  pinMode(CH3,INPUT);
  pinMode(CH4,INPUT);
  pinMode(CH5,INPUT);
  pinMode(CH6,INPUT);
  pinMode(CH7,INPUT);

  lcd.begin(16, 2);
  lcd.print("hello, world!");  

  setControl("washing",1);  //set control of washing machine to Manual
  setControl("hotplates",1);  //set control of washing machine to Manual
  setControl("bedroom1",1);  //set control of washing machine to Manual
  setControl("television",1);  //set control of washing machine to Manual
  setControl("oven",1);  //set control of washing machine to Manual
  setControl("boiler",1);  //set control of washing machine to Manual
  setControl("bedroom2",1);  //set control of washing machine to Manual
  old_time = millis();
}

void loop() {
  current_time = millis(); 
  if(current_time - old_time >= deltaT){
    
    globalTime = (globalTime+1) % 24;
    /*
    //launch the washing machine between 2h and 5h
    if(globalTime >= 2 && globalTime < 5){setOrder("washing",1);}
    else{setOrder("washing",0);}
    */
    setOrder("washing",digitalRead(CH1));  
    Serial.print(digitalRead(CH1));
    setOrder("hotplates",digitalRead(CH2)); 
    Serial.print(digitalRead(CH2));
    setOrder("bedroom1",digitalRead(CH3));
    Serial.print(digitalRead(CH3));
    setOrder("television",digitalRead(CH4));
    Serial.print(digitalRead(CH4));
    setOrder("oven",digitalRead(CH5));
    Serial.print(digitalRead(CH5));
    setOrder("boiler",digitalRead(CH6)); 
    Serial.print(digitalRead(CH6)); 
    setOrder("bedroom2",digitalRead(CH7));
    Serial.print(digitalRead(CH7));

    //Calculates the power of the hour
    globalPower = 0;
    powerConsumption(); //check the consumption of all devices and adjust global power and LEDs state
      //possibility to adjust power consumed from measurements
    //measured_power = ...
    //globalPower += measured_power;
    lcd.clear();
    //Possibility to put the following lines in a communication.ino file
    Serial.print("Actual hour : ");
    Serial.print(globalTime);
    Serial.print("h, Power consummed : ");
    Serial.print(globalPower);
    Serial.println(" W");

    //add the LCD control
    lcd.setCursor(0, 0);
    lcd.print("Puissance"); 
    lcd.setCursor(0, 1);
    lcd.print("consommee:"); 
    lcd.print(int(globalPower)); 
    lcd.print("W"); 
    old_time = current_time;
  }
}

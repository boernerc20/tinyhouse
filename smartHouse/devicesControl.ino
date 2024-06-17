/*
Functions for controlling all the devices in the home
17/05/2024, Simon-Emmanuel HAIM
*/ 
int choicePin;

void setup_LED(void){
  //initialization of the pin for the LEDs
  for(choicePin=22 ; choicePin<=35 ; choicePin++){
    pinMode(choicePin,OUTPUT);
  }
}

void powerConsumption(void){
  //check the total power consumption based on the global variable "globalTime" 
  //and adjust the global variable "power"

  for(int i=0; i<number_devices; i++){
    if(devices[i].manualControl){
      //Manual mode
      if(devices[i].manualOrder){
        globalPower += devices[i].consumption;
        digitalWrite(devices[i].pinLED, HIGH);
      }
      else{
        digitalWrite(devices[i].pinLED, LOW);
      }
    }
    else{ //Automatic mode
      for(int j=0; j<devices[i].numbSlot; j++){
        if(globalTime >= devices[i].beginHours[j] && globalTime < devices[i].endHours[j]){
          globalPower += devices[i].consumption;
          digitalWrite(devices[i].pinLED, HIGH);
          break;  //no need to check the following time slots
        }
        else{digitalWrite(devices[i].pinLED, LOW);}
      }
    }
  }
}

void  setControl(const char* deviceName, bool newControl){
  //adjust the control mode of a device
  for(int i=0; i < number_devices; i++){
    if(deviceName == devices[i].name){
      devices[i].manualControl = newControl;
      return;
    }
  }
  Serial.println("Device unknown, control not updated.");
}

void  setOrder(const char* deviceName, bool newOrder){
  //adjust the manual order of a device
  for(int i=0; i < number_devices; i++){
    if(deviceName == devices[i].name){
      devices[i].manualOrder = newOrder;
      return;
    }
  }
  Serial.println("Device unknown, order not updated.");
}
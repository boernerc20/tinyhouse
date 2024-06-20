/*
Functions for controlling the LCD screen connected to the Arduino Megaa
18/06/2024, Christopher Boerner
*/ 

void setupLCD(void){
  lcd.begin(16, 2);
  lcd.print("Number of");
  lcd.setCursor(0, 1);
  lcd.print("Devices: ");
  lcd.print((int)number_devices);
  delay(2000);  
  lcd.clear();
}

void updatePowerLCD(void) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Puissance");
  lcd.setCursor(0, 1);
  lcd.print("consommee:");
  lcd.print(int(globalPower));
  lcd.print("W");
}

void lcdMessage(void){
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Update:");
  lcd.setCursor(0, 1);
  if(command.equals("set")){
      lcd.print(appliance);
      lcd.print(" at ");
      lcd.print(value);
      lcd.print("W");
  }
  else if(command.equals("turn")){
      lcd.print(appliance);
      lcd.print(" ");
      lcd.print(value);
  }
  else{

  }
  delay(2000);  
}
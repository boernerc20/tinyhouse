void setupLCD(void){
  lcd.begin(16, 2);
  lcd.print("hello, world!");
  delay(1000);  
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
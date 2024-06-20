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
  if (houseNumber != "M4"){ // Switch house number
    Serial.println("Bad house number");
    return;
  } // Check if the house number matches
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
const int analogPin = A0; // Analog pin for voltage reading
const int numSamples = 100; // Number of samples for averaging
const float calibrationFactor = 0.93; // Adjust this value based on calibration of ADC

void setup() {
  Serial.begin(9600);
}

void loop() {
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
  
  delay(500); // Read every 500 ms
}

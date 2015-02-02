int analogInPin = 4;
int circuitReading;

void setup()
{
  Serial.begin(4800);
  pinMode(analogInPin, INPUT);
}

void loop()
{
  circuitReading = analogRead(analogInPin);
  
  Serial.print("The reading is: ");
  Serial.println(circuitReading);
}

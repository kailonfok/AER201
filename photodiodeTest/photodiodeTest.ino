int analogInPin = 5;
int circuitReading;
int previousReading = 0;
int counter = 0;


void setup()
{
  Serial.begin(19200);
  pinMode(analogInPin, INPUT);
}

void loop()
{
  circuitReading = analogRead(analogInPin);

  if (circuitReading == 0)
    counter++;
  previousReading = circuitReading;    
  
  Serial.print("The reading is: ");
  Serial.println(circuitReading);
//  Serial.print("The count is: ");
//  Serial.println(counter);
  
  //delay(50);
}

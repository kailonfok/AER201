int analogInPin = 4;
int circuitReading;
int previousReading = 0;
int counter = 0;


void setup()
{
  Serial.begin(2400);
  pinMode(analogInPin, INPUT);
}

void loop()
{
  circuitReading = analogRead(analogInPin);
//  if(previousReading > 10)
//  {
//    if(circuitReading < 10)
//    {
//      counter++;
//    }
//  }  
  if (circuitReading == 0)
    counter++
  previousReading = circuitReading;    
  
  Serial.print("The reading is: ");
  Serial.println(circuitReading);
  Serial.print("The count is: ");
  Serial.println(counter);
  
  //delay(50);
}

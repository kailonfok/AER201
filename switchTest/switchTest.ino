const int switchPin = 34;

int switchVal;

void setup()
{
  Serial.begin(9600);
  pinMode(switchPin, INPUT);
}

void loop()
{
  switchVal = digitalRead(switchPin);
  delay(1);
  
  Serial.print("Switch value: ");
  Serial.println(switchVal);
}

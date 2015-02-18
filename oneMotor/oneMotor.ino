const int motorPins[] = {32,33,34,35};
const int enablePin[] = {1,3};

void setup()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(motorPins[i], OUTPUT);
  }
  pinMode(enablePin[0], OUTPUT);
  pinMode(enablePin[1], OUTPUT);
}

void  loop()
{
  analogWrite(enablePin[0], 150);
  analogWrite(enablePin[1], 90);

//  digitalWrite(enablePin[0], HIGH);
//  digitalWrite(enablePin[1], HIGH);
  
  digitalWrite(motorPins[0], HIGH);
  digitalWrite(motorPins[1], LOW);
  digitalWrite(motorPins[2], LOW);
  digitalWrite(motorPins[3], HIGH);
}

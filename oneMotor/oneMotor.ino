const int motorPins[] = {34,35,36,37};
const int enablePin[] = {4,5};

void setup()
{
  for (int i = 0; i < 4; i++)
  {
    pinMode(motorPins[i], OUTPUT);
  }
//  pinMode(enablePin[0], OUTPUT);
//  pinMode(enablePin[1], OUTPUT);
}

void  loop()
{
  analogWrite(enablePin[0], 117);
  analogWrite(enablePin[1], 175);

//  digitalWrite(enablePin[0], HIGH);
//  digitalWrite(enablePin[1], HIGH);
  
  digitalWrite(motorPins[1], HIGH);
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[2], LOW);
  digitalWrite(motorPins[3], HIGH);
}

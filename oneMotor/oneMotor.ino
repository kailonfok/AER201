const int motorPins[] = {32,33,34,35};
const int enablePin[] = {2,3};

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
  analogWrite(enablePin[0], 155);
  analogWrite(enablePin[1], 155);

//  digitalWrite(enablePin[0], HIGH);
//  digitalWrite(enablePin[1], HIGH);
  
  digitalWrite(motorPins[1], HIGH);
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[2], HIGH);
  digitalWrite(motorPins[3], LOW);
}

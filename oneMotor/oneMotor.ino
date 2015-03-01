const int motorPins[] = {34,35,36,37};
const int enablePin[] = {4,5};

//const int motorPins[] = {30,31,32,33};
//const int enablePin[] = {2,3};

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
  analogWrite(enablePin[0], 255);
  analogWrite(enablePin[1], 255);

//  digitalWrite(enablePin[0], HIGH);
//  digitalWrite(enablePin[1], HIGH);
  
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[1], HIGH);
  digitalWrite(motorPins[2], LOW);
  digitalWrite(motorPins[3], HIGH);  
  
//  digitalWrite(motorPins[0], HIGH);
//  digitalWrite(motorPins[1], LOW);
//  digitalWrite(motorPins[2], LOW);
//  digitalWrite(motorPins[3], HIGH);
}

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
<<<<<<< HEAD
  analogWrite(enablePin[0], 120);
  analogWrite(enablePin[1], 120);
=======
  analogWrite(enablePin[0], 117);
  analogWrite(enablePin[1], 175);

//  digitalWrite(enablePin[0], HIGH);
//  digitalWrite(enablePin[1], HIGH);
>>>>>>> parent of 25d4688... Fine tuned programs
  
  digitalWrite(motorPins[1], HIGH);
  digitalWrite(motorPins[0], LOW);
  digitalWrite(motorPins[2], LOW);
<<<<<<< HEAD
  digitalWrite(motorPins[3], HIGH);  
  
//  digitalWrite(motorPins[1], HIGH);
//  digitalWrite(motorPins[0], LOW);
//  digitalWrite(motorPins[3], LOW);
//  digitalWrite(motorPins[2], HIGH);
=======
  digitalWrite(motorPins[3], HIGH);
>>>>>>> parent of 25d4688... Fine tuned programs
}

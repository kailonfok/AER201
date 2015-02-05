const int motorPin[] = {36,37,38, 39};
const int pwmPin[] = {2,3};

void setup()
{
  pinMode(motorPin[0], OUTPUT);
  pinMode(motorPin[1], OUTPUT);
  pinMode(motorPin[2], OUTPUT);
  pinMode(motorPin[3], OUTPUT);
//  for(int i = 0; i < 4; i++)
//  {
//    pinMode(motorPin[i], OUTPUT);
//    if (i < 2)
//      pin
//  }
  pinMode(pwmPin[0], OUTPUT);
  pinMode(pwmPin[1], OUTPUT);
}

void loop()
{
  analogWrite(pwmPin[0], 100);
  analogWrite(pwmPin[1], 255);
  
  digitalWrite(motorPin[0], HIGH);
  digitalWrite(motorPin[1], LOW);
  digitalWrite(motorPin[2], HIGH);
  digitalWrite(motorPin[3], LOW);
}

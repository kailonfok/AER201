const int motorPin[] = {2,3,4,5,6,7,8,9};
const int directionButPin = 11;
const int resetButPin = 12;
const int enablePin = 10;

int resetState = 0;
int prevResetState = 0;
int wheelState = 0;

int motorDirection = 0;
int dir = 0;
int prevDir = 0;

void setup()
{
  Serial.begin(9600);
  for (int i = 0; i < 9; i++)
  {
    pinMode(motorPin[i], OUTPUT);
  }
  pinMode(directionButPin, INPUT);
  pinMode(resetButPin, INPUT);
  pinMode(enablePin, OUTPUT);
  
  digitalWrite(enablePin, LOW);
}

void loop()
{
  dir = digitalRead(directionButPin);
  delay(1);
  
  resetState = digitalRead(resetButPin);
  delay(1);
  
  if(resetState != prevResetState)
  {
    if(resetState)
      wheelState = 0;
  }
   
  if(dir != prevDir)
  {
    if(dir)
    {
      if (wheelState < 3)
        wheelState++;
      else
        wheelState = 0;
    }
  }
  
  analogWrite(enablePin, HIGH);    
  movement(motorDirection);

  Serial.print("Motor Direction:  ");
  Serial.println(wheelState);
  
  prevResetState = resetState;
  prevDir = dir;
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left
{
   if(motorDirection == 0)
   {
     //Move forward
     Serial.println("Moving forward");     
     digitalWrite(motorPin[0], HIGH); // front left wheel
     digitalWrite(motorPin[1], LOW);
     
     digitalWrite(motorPin[2], HIGH); // front right wheel
     digitalWrite(motorPin[3], LOW);
     
     digitalWrite(motorPin[4], HIGH); // back right wheel
     digitalWrite(motorPin[5], LOW);
     
     digitalWrite(motorPin[6], HIGH); // back left wheel
     digitalWrite(motorPin[7], LOW);     
   }
   else if (motorDirection == 1)
   {
     Serial.println("Moving right");
     digitalWrite(motorPin[0], HIGH);
     digitalWrite(motorPin[1], LOW);
     
     digitalWrite(motorPin[2], LOW);
     digitalWrite(motorPin[3], HIGH);
     
     digitalWrite(motorPin[4], HIGH);
     digitalWrite(motorPin[5], LOW);
     
     digitalWrite(motorPin[6], LOW);
     digitalWrite(motorPin[7], HIGH);     
   }
   else if (motorDirection == 2)
   {
     Serial.println("Moving backwards");
     digitalWrite(motorPin[0], LOW);
     digitalWrite(motorPin[1], HIGH);
     
     digitalWrite(motorPin[2], LOW);
     digitalWrite(motorPin[3], HIGH);
     
     digitalWrite(motorPin[4], LOW);
     digitalWrite(motorPin[5], HIGH);
     
     digitalWrite(motorPin[6], LOW);
     digitalWrite(motorPin[7], HIGH);     
   }
   else
   {
     Serial.println("Moving left");     
     digitalWrite(motorPin[0], LOW);
     digitalWrite(motorPin[1], HIGH);
     
     digitalWrite(motorPin[2], HIGH);
     digitalWrite(motorPin[3], LOW);
     
     digitalWrite(motorPin[4], LOW);
     digitalWrite(motorPin[5], HIGH);
     
     digitalWrite(motorPin[6], HIGH);
     digitalWrite(motorPin[7], LOW);     
   }
}

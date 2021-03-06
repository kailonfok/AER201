const int motorPin[] = {32,33,34,35,36,37,38,39};
const int directionButPin = 41;
const int resetButPin = 42;
const int enablePin = 40;

boolean resetState = 0;
boolean prevResetState = 0;
int wheelState = 0;

boolean dir = 0;
boolean prevDir = 0;

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
      {
        wheelState++;
//        delay(10000);
      }
      else
        wheelState = 0;
    }
  }
  
  analogWrite(enablePin, 255);    
  movement(wheelState);

  Serial.print("Motor Direction:  ");
  Serial.println(wheelState);
  
  prevResetState = resetState;
  prevDir = dir;
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left
{
   boolean highLow = 0;
   
   if(motorDirection == 0)
   {
     highLow = 1;
     Serial.println("Moving forward");     
   }
   else if (motorDirection == 1)
   {
     Serial.println("Moving right");
     highLow = 1;
   }
   else if (motorDirection == 2)
   {
     Serial.println("Moving backwards");
     highLow = 0;
   }
   else
   {
     Serial.println("Moving left");     
     highLow = 0;
   }
 
   if(motorDirection == 0 || motorDirection == 2)
   {
     digitalWrite(motorPin[0], highLow); // back right wheel
     digitalWrite(motorPin[1], !highLow);
     
     digitalWrite(motorPin[2], highLow); // front right wheel
     digitalWrite(motorPin[3], !highLow);
     
     digitalWrite(motorPin[4], highLow); // back left wheel
     digitalWrite(motorPin[5], !highLow);
     
     digitalWrite(motorPin[6], !highLow); // front left wheel
     digitalWrite(motorPin[7], highLow);          
   }
   else
   {
     digitalWrite(motorPin[0], highLow);
     digitalWrite(motorPin[1], !highLow);
     
     digitalWrite(motorPin[2], !highLow);
     digitalWrite(motorPin[3], highLow);
     
     digitalWrite(motorPin[4], !highLow);
     digitalWrite(motorPin[5], highLow);
     
     digitalWrite(motorPin[6], !highLow);
     digitalWrite(motorPin[7], highLow);
   }
 }

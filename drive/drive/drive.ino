const int motorPin[] = {2,3,4,5,6,7,8,9};
const int directionButPin = 11;
const int resetButPin = 12;
const int enablePin = 10;

boolean resetState = 0;
boolean prevResetState = 0;
int wheelState = 0;

boolean dir = 0;
boolean prevDir = 0;

int n = 6;

byte board[6][6] = {{0}};
byte presentLocation[2] = {3,1};
int xLocationHopper = 1;
int yLocationHopper = 1;
int distance = 0;

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
  
  analogWrite(enablePin, HIGH);    
//  movement(wheelState);

  if(xLocationHopper != presentLocation[0])
  {
    drive(xLocationHopper, presentLocation[0], 0);
  }
  Serial.print("Present Location-X:  ");
  Serial.println(presentLocation[0]);
  
  presentLocation[0] = xLocationHopper;
  
  if(yLocationHopper != presentLocation[1])
  {
    drive(yLocationHopper, presentLocation[1], 1);
  }
  presentLocation[1] = yLocationHopper;
  Serial.print("Present Location-Y:  ");
  Serial.println(presentLocation[1]);  

  Serial.print("Motor Direction:  ");
  Serial.println(wheelState);
  
  prevResetState = resetState;
  prevDir = dir;
}

void drive(int locationHopper, int presentLocation, int VH) // VH = 1, F/B - VH = 0, L/R
{
  int moved = 0;
  distance = locationHopper - presentLocation;
  Serial.print("Distance:  ");
  Serial.println(distance);
  while(distance != 0)
  {
    distance = locationHopper - presentLocation;    
    if(VH)
    {
      if(distance > 0)
      {
        wheelState = 0;
        moved = 1;
      }
      else
      {
        wheelState = 2;
        moved = -1;
      }
    }
    else
    {
      if(distance > 0)
      {
        wheelState = 1;
        moved = 1;
      }
      else
      {
        wheelState = 3;        
        moved = -1;
      }
    }
    
    movement(wheelState);
    presentLocation += moved;
    Serial.print("Present Location:  ");
    Serial.println(presentLocation);
  }
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
     digitalWrite(motorPin[0], highLow); // front left wheel
     digitalWrite(motorPin[1], !highLow);
     
     digitalWrite(motorPin[2], highLow); // front right wheel
     digitalWrite(motorPin[3], !highLow);
     
     digitalWrite(motorPin[4], highLow); // back right wheel
     digitalWrite(motorPin[5], !highLow);
     
     digitalWrite(motorPin[6], highLow); // back left wheel
     digitalWrite(motorPin[7], !highLow);          
   }
   else
   {
     digitalWrite(motorPin[0], highLow);
     digitalWrite(motorPin[1], !highLow);
     
     digitalWrite(motorPin[2], !highLow);
     digitalWrite(motorPin[3], highLow);
     
     digitalWrite(motorPin[4], highLow);
     digitalWrite(motorPin[5], !highLow);
     
     digitalWrite(motorPin[6], !highLow);
     digitalWrite(motorPin[7], highLow);
   }
 }

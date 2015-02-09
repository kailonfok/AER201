// define constants for sonar sensors
const int echoPin[] = {
  22,24,26,28}; // Echo Pin
const int trigPin[] = {
  23,25,27,29}; // Trigger Pin
const int LEDPin1 = 52; // red LED for out of range
const int LEDPin2 = 53; // blue LED for in range
const float distanceConstant = 58.2;

// define variables and constants for encoders
//const int encoderPins[] = {2,3,4,5};
//int encoderReadings[] = {0,0}; // match to enable pins
//int encoderCount[] = {0,0};

int maxRange = 25;

//define constants for motors
const int motorPin[] = {
  32,33,34,35,36,37,38,39};
const int enablePin[] = {
  2,3,4,5}; // Starts from top, going clockwise

long duration, distance;
long prevDistance = 0;

int wheelState = 0;

int dir = 1; // 0 - front, 1 - right, 2 - back, 3 - left
int prevDir = 2;
int sensorNum = 1;
boolean highLow = 0;
boolean backingUp = 0;

void setup()
{
  Serial.begin(9600);

  pinMode(LEDPin1, OUTPUT);
  pinMode(LEDPin2, OUTPUT);  

  for (int i = 0; i < 9; i++)
  {
    pinMode(motorPin[i], OUTPUT);
    if (i < 4)
    {
      pinMode(trigPin[i], OUTPUT);
      pinMode(echoPin[i], INPUT);
//      pinMode(encoderPins[i], INPUT);
    }
  }
}

void loop()
{
  sensor(sensorNum);
  Serial.print("Sensor Number: ");
  Serial.println(sensorNum);

  Serial.print("The distance is: ");

  if(backingUp == 0)
  {    
    keepDriving();
  }
  else
  {
    driveOutHopper();
  }
    

  Serial.println(distance);
  Serial.print("Previous direction: ");
  Serial.println(prevDir);

  Serial.print("Motor Direction:  ");
  Serial.println(wheelState);
}

void turnMotorsOff()
{
  analogWrite(enablePin[0], 0);
  analogWrite(enablePin[1], 0);
  analogWrite(enablePin[2], 0);
  analogWrite(enablePin[3], 0);

  for(int i = 0; i < 8; i++)
  {
    digitalWrite(motorPin[i], LOW);
  }  
  
  Serial.println("Motors off?");
  delay(1000);
}

void driveOutHopper()
{
  Serial.println("Got to third checkpoint");
  delay(1000);      
  prevDir = dir;
  sensor(sensorNum);

  Serial.print("Distance: ");
  Serial.println(distance);
  Serial.print("Previous Distance: ");
  Serial.println(prevDistance);
  delay(5000);

  while(distance <= prevDistance)
  {
    sensor(2);
    movement(0);
  }
  
  Serial.println("Drove out of hopper");
  delay(1000);
  
  turnMotorsOff();      
  
  dir = rotateRobot(prevDir);
  sensorNum = 0;  
  backingUp = 0;
}

void keepDriving()
{
  if (distance <= maxRange)
  {
    digitalWrite(LEDPin1, LOW);
    digitalWrite(LEDPin2, HIGH);    
    turnMotorsOff();
    if(dir == 1)
    {
      Serial.println("Got to first checkpoint");
      delay(1000);
      prevDir = dir;
      prevDistance = distance;
      dir = rotateRobot(prevDir);
      sensorNum = 2;
    }
    else if (dir == 0)
    {
        Serial.println("Going to board");
        delay(1000);
        prevDir = dir;
        dir = 3;
        sensorNum = 3;
    }
    else if (dir == 2)
    {
      Serial.println("Got to third checkpoint");
      delay(1000);      
      backingUp = 1;
    }
  }
  else if (distance >= maxRange){
    digitalWrite(LEDPin1, HIGH); 
    digitalWrite(LEDPin2, LOW);
    movement(dir);    
  }  
}

int rotateRobot(int whichWay)
{
  int lastTime = millis();
  int currentTime = millis();
  int newDir;    

  turnMotorsOff();

  if(whichWay == 1)
  {
    Serial.println("Got to second checkpoint");
    delay(1000);
    analogWrite(enablePin[2], 175);
    analogWrite(enablePin[3], 175);
    
    digitalWrite(motorPin[4], LOW);
    digitalWrite(motorPin[5], HIGH);
    digitalWrite(motorPin[6], HIGH);
    digitalWrite(motorPin[7], LOW);        
    while(currentTime - lastTime < 3000)
    {
      currentTime = millis();
      Serial.print("Last time: ");
      Serial.println(lastTime);
      Serial.print("Current time: ");
      Serial.println(currentTime);
      delay(1000);  
    }
    maxRange = 5;

    newDir = 2;
  }
  else if (whichWay == 2)
  {
    Serial.println("Got to fourth checkpoint");
    delay(1000);    
    analogWrite(enablePin[2], 175);
    analogWrite(enablePin[3], 175);

    digitalWrite(motorPin[4], highLow);
    digitalWrite(motorPin[5], !highLow);
    digitalWrite(motorPin[6], !highLow);
    digitalWrite(motorPin[7], highLow);   
    while(millis() - lastTime < 3000)
    {
      currentTime = millis();
      Serial.print("Last time: ");
      Serial.println(lastTime);
      Serial.print("Current time: ");
      Serial.println(currentTime);
      delay(1000);        
    }    

    maxRange = 25;

    newDir = 0;    
  }
  digitalWrite(motorPin[4], LOW);
  digitalWrite(motorPin[5], LOW);
  digitalWrite(motorPin[6], LOW);
  digitalWrite(motorPin[7], LOW); 
  
  return newDir;  
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
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
  else if (motorDirection == 3)
  {
    Serial.println("Moving left");     
    highLow = 0;
  }
  else
  {
    Serial.println("Don't need to move");
  }

  if(motorDirection == 0 || motorDirection == 2)
  {
    analogWrite(enablePin[0], 175);
    analogWrite(enablePin[1], 175);
    digitalWrite(motorPin[0], !highLow);
    digitalWrite(motorPin[1], highLow);
    digitalWrite(motorPin[2], !highLow);
    digitalWrite(motorPin[3], highLow);
    digitalWrite(motorPin[4], LOW);
    digitalWrite(motorPin[5], LOW);
    digitalWrite(motorPin[6], LOW);
    digitalWrite(motorPin[7], LOW);            
  }
  else if(motorDirection == 1 || motorDirection == 3)
  {
    analogWrite(enablePin[2], 175);
    analogWrite(enablePin[3], 175);    
    digitalWrite(motorPin[0], LOW);
    digitalWrite(motorPin[1], LOW);
    digitalWrite(motorPin[2], LOW);
    digitalWrite(motorPin[3], LOW);    
    digitalWrite(motorPin[4], highLow);
    digitalWrite(motorPin[5], !highLow);
    digitalWrite(motorPin[6], highLow);
    digitalWrite(motorPin[7], !highLow);    
  }
  else
  {
    for(int i = 0; i < 8; i++)
    {
      digitalWrite(motorPin[i], 0);
    }
  }
}

void sensor(int sensorNum)
{
  digitalWrite(trigPin[sensorNum], LOW);
  delayMicroseconds(2); 

  digitalWrite(trigPin[sensorNum], HIGH);
  delayMicroseconds(10); 

  digitalWrite(trigPin[sensorNum], LOW);
  duration = pulseIn(echoPin[sensorNum], HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/distanceConstant;  

  delay(50);
}

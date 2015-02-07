// define constants for sonar sensors
const int echoPin[] = {
  22,24,26,28}; // Echo Pin
const int trigPin[] = {
  23,25,27,29}; // Trigger Pin
const int LEDPin1 = 52; // red LED for out of range
const int LEDPin2 = 53; // blue LED for in range
const float distanceConstant = 58.2;
int maxRange = 25;

//define constants for motors
const int motorPin[] = {
  32,33,34,35,36,37,38,39};
const int directionButPin = 41;
const int enablePin[] = {
  2,3,4,5};

long duration, distance;
long prevDistance = 0;

//boolean enableState = 0;
//boolean prevEnableState = 0;
int wheelState = 0;

int dir = 1; // 0 - front, 1 - right, 2 - back, 3 - left
int prevDir = 2;
int sensorNum = 1;
boolean highLow = 0;

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
    }
  }
  pinMode(directionButPin, INPUT);
//  pinMode(enableButPin, INPUT);
//  pinMode(enablePin, OUTPUT);
//
//  digitalWrite(enablePin, LOW);
}

void loop()
{
  sensor(sensorNum);
  Serial.print("Sensor Number: ");
  Serial.println(sensorNum);

  //  enableState = digitalRead(enableButPin);
  //  delay(1);
  //
  //  if(enableState != prevEnableState)
  //  {
  //    if(enableState)
  //      analogWrite(enablePin, 255);
  //    else
  //      analogWrite(enablePin, 0);
  //  }

  Serial.print("The distance is: ");

  keepDriving();

  Serial.println(distance);
  Serial.print("Previous direction: ");
  Serial.println(prevDir);

  Serial.print("Motor Direction:  ");
  Serial.println(wheelState);

  //  prevEnableState = enableState;
}

void keepDriving()
{
  if (distance <= maxRange)
  {
    digitalWrite(LEDPin1, LOW);
    digitalWrite(LEDPin2, HIGH);    
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
//      if(prevDir == 2)
//      {
//        prevDir = 0;
//        dir = 0;
//        Serial.println("Got to fifth checkpoint");
//        delay(1000);        
//      }
//      else
//      {
        Serial.println("Going to board");
        delay(1000);
        prevDir = dir;
        dir = 3;
        sensorNum = 3;
//      }
    }
    else if (dir == 2)
    {
      Serial.println("Got to third checkpoint");
      delay(1000);      
      prevDir = dir;
      sensorNum = 0;
      sensor(sensorNum);

      while(distance <= prevDistance)
      {
        sensor(0);
        movement(sensorNum);
      }

      dir = rotateRobot(prevDir);
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

  if(whichWay == 1)
  {
    Serial.println("Got to second checkpoint");
    delay(1000);
    analogWrite(enablePin[2], 175);
    analogWrite(enablePin[3], 125);

    while(millis() - lastTime < 1000)
    {
      digitalWrite(motorPin[4], !highLow);
      digitalWrite(motorPin[5], highLow);
      digitalWrite(motorPin[6], highLow);
      digitalWrite(motorPin[7], !highLow);      
    }

    digitalWrite(motorPin[4], LOW);
    digitalWrite(motorPin[5], LOW);
    digitalWrite(motorPin[6], LOW);
    digitalWrite(motorPin[7], LOW);      

    maxRange = 5;

    return 2;
  }
  else if (whichWay == 2)
  {
    Serial.println("Got to fourth checkpoint");
    delay(1000);    
    analogWrite(enablePin[2], 175);
    analogWrite(enablePin[3], 125);

    while(millis() - lastTime < 1000)
    {
      digitalWrite(motorPin[4], highLow);
      digitalWrite(motorPin[5], !highLow);
      digitalWrite(motorPin[6], !highLow);
      digitalWrite(motorPin[7], highLow);       
    } 

    digitalWrite(motorPin[4], LOW);
    digitalWrite(motorPin[5], LOW);
    digitalWrite(motorPin[6], LOW);
    digitalWrite(motorPin[7], LOW);      

    maxRange = 25;

    return 0;    
  }
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
    digitalWrite(motorPin[0], highLow);
    digitalWrite(motorPin[1], !highLow);
    digitalWrite(motorPin[2], highLow);
    digitalWrite(motorPin[3], !highLow);
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

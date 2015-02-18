// Servo declarations
#include <Servo.h> 
Servo myservo;  // create servo object to control a servo
int pos = 0;

// define constants for sonar sensors
const int echoPin[] = {
  22,24,26,28}; // Echo Pin
const int trigPin[] = {
  23,25,27,29}; // Trigger Pin
const int LEDPin1 = 52; // red LED for out of range
const int LEDPin2 = 53; // blue LED for in range
const float distanceConstant = 58.2;

int maxRange = 15;

//define constants for motors
const int motorPin[] = {
  32,33,34,35,36,37,38,39};
const int enablePin[] = {
  2,3,4,5}; // Starts from top, going clockwise

long duration, distance;

int wheelState = 0;

int dir = 1; // 0 - front, 1 - right, 2 - back, 3 - left
int prevDir = 2;
int sensorNum = 1;
boolean highLow = 0;

boolean atHopper = false; // variable to determine if at hopper
boolean atBoard = false; // variable to determine if at board

void setup()
{
  Serial.begin(9600);

  myservo.attach(40); // attach servo to pin 40 to servo object

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
}

void loop()
{
  sensor(sensorNum);
  Serial.print("Sensor Number: ");
  Serial.println(sensorNum);

  Serial.print("The distance is: ");


  if (!atHopper && !atBoard)
  {
    keepDriving();
  }
  else if (atHopper)
  {
    dir = closeClaw();
    atHopper = false;
  }
  else if (atBoard)
  {
    atBoard = false;
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

  Serial.println("Motors off");
  delay(1000);
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
      Serial.println("Got to first hopper");
      delay(1000);
      prevDir = dir;
      atHopper = true;
      sensorNum = 0;
    }
    else if (dir == 0)
    {
      Serial.println("Going to board");
      delay(1000);
      prevDir = dir;
      maxRange = 90;
      dir = 3;
      sensorNum = 3;
    }
    else if (dir == 3)
    {
      Serial.println("Got to game board");
      delay(1000);
      prevDir = dir;
      atBoard = true;
    }
  }
  else if (distance >= maxRange){
    digitalWrite(LEDPin1, HIGH); 
    digitalWrite(LEDPin2, LOW);
    movement(dir);    
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

  if(motorDirection == 0 || motorDirection == 2)
  {
    analogWrite(enablePin[0], 175);
    analogWrite(enablePin[1], 175);

    digitalWrite(motorPin[0], !highLow);
    digitalWrite(motorPin[1], highLow);
    digitalWrite(motorPin[2], !highLow);
    digitalWrite(motorPin[3], highLow);         
  }
  else if(motorDirection == 1 || motorDirection == 3)
  {
    analogWrite(enablePin[2], 200);
    analogWrite(enablePin[3], 200);          

    digitalWrite(motorPin[4], highLow);
    digitalWrite(motorPin[5], !highLow);
    digitalWrite(motorPin[6], highLow);
    digitalWrite(motorPin[7], !highLow);    
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

// Beginning of functions for retrieval of ping pong ball

int closeClaw()
{
  for(pos = 0; pos < 180; pos += 1)  // goes from 0 degrees to 180 degrees 
  {                                  // in steps of 1 degree 
    myservo.write(pos);              // tell servo to go to position in variable 'pos' 
    delay(15);                       // waits 15ms for the servo to reach the position 
  }

  return 0; 
}

// Code for depositing the ball (2 functions)
void liftArm()
{
}

void openClaw()
{
}

int lowerArm()
{
}

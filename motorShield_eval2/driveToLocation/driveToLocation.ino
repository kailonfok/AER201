#include <AFMotor.h>

// define constants for sonar sensors
const int echoPin[] = {
  46,48,50,52}; // Echo Pin
const int trigPin[] = {
  47,49,51,53}; // Trigger Pin

const float distanceConstant = 58.2;

int maxRange = 12;

//define objects for motors
AF_DCMotor frontMotor(1);
AF_DCMotor leftMotor(2);
AF_DCMotor backMotor(3);
AF_DCMotor rightMotor(4);

long duration, distance;

int dir = 1; // 0 - front, 1 - right, 2 - back, 3 - left
int prevDir = 2;
int sensorNum = 1;

//enable pin to start
boolean start;
const int enableButPin = 50;
int enableState = 0;
int prevEnableState = 0;

void setup()
{
  Serial.begin(9600);

  for (int i = 0; i < 4; i++)
  {
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }
  pinMode(enableButPin, INPUT);
  
  frontMotor.run(RELEASE);
  leftMotor.run(RELEASE);
  backMotor.run(RELEASE);
  rightMotor.run(RELEASE);  
}

void loop()
{
  enableState = digitalRead(enableButPin);
  delay(1);
  if(enableState != prevEnableState)
  {
    if(enableState)
      start = 1;
  }
  Serial.print("Start: ");
  Serial.println(start);
  
  if(start)
  {
    //Every iteration, check distance to wall
    sensor(sensorNum);
    Serial.print("Sensor Number: ");
    Serial.println(sensorNum);    
  
    // diagnostic outputs
    Serial.print("The distance is: ");    
    Serial.println(distance);
    Serial.print("Previous direction: ");
    Serial.println(prevDir);
  
    Serial.print("Motor Direction:  ");
    Serial.println(dir);
    
    // function call to determine if at wall or not
    keepDriving();    
  }
  else
  {    
    Serial.println("Don't do anything");
    delay(1000);
  }
}

// Function to turn off motors, before switching directions
void turnMotorsOff()
{
  // Write low to all enable pins on H-bridge (0 volts)
  frontMotor.run(RELEASE);
  leftMotor.run(RELEASE);
  backMotor.run(RELEASE);
  rightMotor.run(RELEASE);  
  
  Serial.println("Motors off?");
  delay(1000);
}

void keepDriving()
{
  // At designated location
  if (distance <= maxRange)
  {  
    turnMotorsOff(); // turn motors off before making decision, so it doesn't keep driving
    
    if(dir == 1) // If going right, switch to going forward (for first hopper and first instance)
    {
      Serial.println("Got to first checkpoint");
      delay(1000);
      prevDir = dir;
      dir = 0;
      sensorNum = 0;
    }
    else if (dir == 0) // If going forward, switch to going left
    {
        Serial.println("Going to board");
        delay(1000);
        prevDir = dir;
        dir = -1;
        sensorNum = 1;
        maxRange = 12;
    }
    else if (dir == 2)
    {
      Serial.println("At game board!");
      delay(1000);
      prevDir = dir;
      dir = -1;
      sensorNum = 1; // There needs to be some logic here to then place ball and return to last hopper
      maxRange = 12;
    }
    else if (dir == -1) // stop running
    {
      turnMotorsOff();
    }
  }
  else if (distance >= maxRange){ // Not close enough, keep driving in direction
    movement(dir);    
  }  
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
  // direct motors to turn in appropriate direction and speed
  if (motorDirection == 0 || motorDirection == 2)
  {
    leftMotor.setSpeed(180);
    rightMotor.setSpeed(255);
    if(motorDirection == 0)
    {
      Serial.println("Moving forward");
      delay(1000);      
      leftMotor.run(FORWARD);
      rightMotor.run(FORWARD);
    }
    else
    {
      Serial.println("Moving back");
      delay(1000);      
      leftMotor.run(BACKWARD);
      rightMotor.run(BACKWARD);
    }
  }
  else if(motorDirection == 1 || motorDirection == 3)
  {
    frontMotor.setSpeed(255);
    backMotor.setSpeed(255);
    if(motorDirection == 1)
    {
      Serial.println("Moving right");
      delay(1000);
      frontMotor.run(BACKWARD);
      backMotor.run(BACKWARD);
    }
    else
    {
      Serial.println("Moving left");
      delay(1000);      
      frontMotor.run(FORWARD);
      backMotor.run(FORWARD);
    }
  }
  else
  {
    Serial.println("Don't need to move");
    turnMotorsOff();
  }
}

// function for determining distance to wall/objects
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

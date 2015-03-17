#include <AFMotor.h>

//define objects for motors
AF_DCMotor frontMotor(1);
AF_DCMotor leftMotor(2);
AF_DCMotor backMotor(3);
AF_DCMotor rightMotor(4);

const byte echoPin = 46;
const byte trigPin = 47;
const float distanceConstant = 58.2;
byte maxRange = 17;

long duration, distance;

byte dir = 0;
byte sensorNum = 0;

byte switchKeepDriving = 1;
const byte leftSwitchPin = 33;

boolean enableState = 0;
boolean prevEnableState = 0;
boolean start;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  
  pinMode(echoPin, INPUT);
  pinMode(trigPin, OUTPUT);
  
  frontMotor.run(RELEASE);
  leftMotor.run(RELEASE);
  backMotor.run(RELEASE);
  rightMotor.run(RELEASE);  
  pinMode(leftSwitchPin, INPUT);  
}

void loop() {
  // put your main code here, to run repeatedly:
  
  enableState = digitalRead(leftSwitchPin);
  delay(1);
  
  if(enableState != prevEnableState) // if button is pressed and released
  {
    if(enableState) // if switched to on
    {
      start = 1;
    }
  }  
  
  if(start)
  {
    //Every iteration, check distance to wall
    sensor(sensorNum);
    Serial.print("Sensor Number: ");
    Serial.println(sensorNum);
  
    // diagnostic outputs
    Serial.print("The distance is: ");    
    Serial.println(distance);           
    // function call to determine if at wall or not
    keepDriving(switchKeepDriving);
   
    sensor(0);
   
    /*
      THIS NEEDS TO BE TESTED!!!!!!!!!!!
    */
    if(distance >= 17)
    {
      Serial.println("Stopped too early");
      delay(1000);
      maxRange = 17;
      switchKeepDriving = 1;
      dir = 0;
      sensorNum = 0;
      keepDriving(switchKeepDriving);
    }    
  }
  else
  {
    Serial.println("Not on.");
  }
}

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

void keepDriving(byte lessGreater)
{
  if (lessGreater == 1)
  {
    if (distance <= maxRange)
    {   
      turnMotorsOff();    
      if(dir == 0)
      {
        dir = 3;
        maxRange = 55;
        sensorNum = 1;
        switchKeepDriving = 0;
      }
      else if(dir == 3)
      {
        dir = -1;
        //inPosition = 1;
      }
    }
    else if (distance >= maxRange){
      movement(dir);    
    }
  }
  else
  {
    if (distance >= maxRange)
    {       
      turnMotorsOff();    
      if(dir == 0)
      {
        dir = 3;
        maxRange = 55;
        sensorNum = 1;
      }
      else if(dir == 3)
      {
        dir = -1;
        //inPosition = 1;
      }
    }
    else if (distance <= maxRange){
      movement(dir);    
    }  
  }
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
  byte oneTwo = 0;
  
  // direct motors to turn in appropriate direction and speed
  if(motorDirection == 0)
  {
    oneTwo = 1;
    Serial.println("Moving forwards");
    delay(1000);
  }
  else if(motorDirection == 1)
  {
    oneTwo = 2;
    Serial.println("Moving right");
    delay(1000);    
  }
  else if(motorDirection == 2)
  {
    oneTwo = 2;
    Serial.println("Moving backwards");  
    delay(1000);    
  }
  else
  {
    oneTwo = 1;
    Serial.println("Moving left");
    delay(1000);    
  }
  
  if (motorDirection == 0 || motorDirection == 2)
  {
    leftMotor.setSpeed(240);
    rightMotor.setSpeed(240);
    leftMotor.run(oneTwo);
    rightMotor.run(oneTwo);
  }
  else if(motorDirection == 1 || motorDirection == 3)
  {
    frontMotor.setSpeed(80);
    backMotor.setSpeed(255);
    frontMotor.run(oneTwo);
    backMotor.run(oneTwo);
  }
  else
  {
    Serial.println("Don't need to move");
    turnMotorsOff();
  }
}

void sensor(int sensorNum)
{
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2); 

  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10); 

  digitalWrite(trigPin, LOW);
  duration = pulseIn(echoPin, HIGH);

  //Calculate the distance (in cm) based on the speed of sound.
  distance = duration/distanceConstant;  
  
  delay(50);
}

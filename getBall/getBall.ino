#include <AFMotor.h>
#include <Servo.h>

//define objects for motors
AF_DCMotor frontMotor(1);
AF_DCMotor leftMotor(2);
AF_DCMotor backMotor(3);
AF_DCMotor rightMotor(4);

// Define pins for wing pins
const int leftSwitchPin = 43;
const int rightSwitchPin = 42;

int leftSwitchVal, rightSwitchVal;

const int echoPin[] = {26,22};
const int trigPin[] = {27,23};
const float distanceConstant = 58.2;
const int maxRange = 10;

long duration, distance;

int dir = 1;
int sensorNum = 0;

// Define servo objects
Servo leftServo;
Servo rightServo;

int leftPos = 0; // Starting positions for servo claws
int rightPos = 180;
boolean onOff = 1;
boolean start;
boolean rotated = 0;

const int enableButPin = 40;
boolean enableState = 0;
boolean prevEnableState = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(enableButPin, INPUT);
  pinMode(leftSwitchPin, INPUT);
  pinMode(rightSwitchPin, INPUT);
  
  for (int i = 0; i < 2; i++)
  {
    pinMode(echoPin[i], INPUT);
    pinMode(trigPin[i], OUTPUT);
  }
  
  frontMotor.run(RELEASE);
  leftMotor.run(RELEASE);
  backMotor.run(RELEASE);
  rightMotor.run(RELEASE);  

  leftServo.write(leftPos);
  rightServo.write(rightPos);  
}

void loop()
{
  enableState = digitalRead(enableButPin);
  delay(1);
  
  if(enableState != prevEnableState) // if button is pressed and released
  {
    if(enableState) // if switched to on
    {
      start = 1;
    }
  }  
  
  Serial.print("Start: ");
  Serial.println(start);
  
  leftSwitchVal = digitalRead(leftSwitchPin);
  delay(1);

  if(start)
  {
    if(!rotated)
    {
      if(!leftSwitchVal) // modify later for a state variable, to account for opposite hopper
      {
        movement(2);
      }
      else
      {
        rotate();
      }
    }
    else
    {
      if (dir == 1) // first retrieve the ball
      {
        dir = closeClaw(); // change direction to forward      
      }
      else
      {
        sensor(sensorNum);
        Serial.print("Sensor Number: ");
        Serial.println(sensorNum);
        
        keepDriving();
        
        Serial.print("The distance is: ");  
        Serial.println(distance);
        
        Serial.print("The direction is: ");
        Serial.println(dir);
      }  
    }
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
  turnMotorsOff();
  if (distance >= maxRange)
  {   
    dir = -1;
  }
  else if (distance <= maxRange){
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

void rotate()
{
  turnMotorsOff();
  rightSwitchVal = digitalRead(rightSwitchPin);
  leftMotor.setSpeed(255);
  leftMotor.run(BACKWARD);  
  
  do
  {
    Serial.println("rotating");
    delay(100);
    rightSwitchVal = digitalRead(rightSwitchPin);  
  }while(!rightSwitchVal);
  
  rotated = 1;
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

int closeClaw()
{
  Serial.println("Closing claw");
  do // loop to close the claws (left and right claws go 180 degrees)
  {
    leftServo.write(leftPos);
    delay(15);
    rightServo.write(rightPos);
    rightPos--;
    leftPos++;
         
  }while(leftPos != 40 && rightPos != 140);

  return 0; 
}

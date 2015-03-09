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
      Serial.println("Done");
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

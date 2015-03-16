#include <AFMotor.h>
#include <Servo.h>

//define objects for motors
AF_DCMotor frontMotor(1);
AF_DCMotor leftMotor(2);
AF_DCMotor backMotor(3);
AF_DCMotor rightMotor(4);

// Define pins for wing pins
const byte leftSwitchPin = 33;
const byte rightSwitchPin = 32;

int leftSwitchVal, rightSwitchVal;

const byte echoPin[] = {46,48,50,52};
const byte trigPin[] = {47,49,51,53};
const float distanceConstant = 58.2;
const byte maxRange = 12;

long duration, distance;
long wallDistance;

byte dir = 1;
byte sensorNum = 0;

// Define servo objects
Servo leftServo;
Servo rightServo;

byte leftPos = 0; // Starting positions for servo claws
byte rightPos = 180;
boolean onOff = 1;
boolean start;
boolean rotated = 0;

boolean enableState = 0;
boolean prevEnableState = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(leftSwitchPin, INPUT);
  pinMode(rightSwitchPin, INPUT);
  
  leftServo.attach(41); 
  rightServo.attach(40);  
  
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
  enableState = digitalRead(rightSwitchPin);
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
        sensor(1);
        wallDistance = distance; // check distance to wall - to be able to turn back
        rotateIn();
      }
    }
    else
    {
      if (dir == 1) // first retrieve the ball
      {
        dir = closeClaw(); // change direction to forward      
        rotateOut();        
      }
      else
      { 
        //Every iteration, check distance to wall
        sensor(sensorNum);
        Serial.print("Sensor Number: ");
        Serial.println(sensorNum);
      
        // diagnostic outputs
        Serial.print("The distance is: ");    
        Serial.println(distance);  
      
        // function call to determine if at wall or not
        keepDriving();
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
    leftMotor.setSpeed(150);
    rightMotor.setSpeed(150);
    leftMotor.run(oneTwo);
    rightMotor.run(oneTwo);
  }
  else if(motorDirection == 1 || motorDirection == 3)
  {
    frontMotor.setSpeed(70);
    backMotor.setSpeed(200);
    frontMotor.run(oneTwo);
    backMotor.run(oneTwo);
  }
  else
  {
    Serial.println("Don't need to move");
    turnMotorsOff();
  }
}



void rotateIn()
{
  turnMotorsOff();
  rightSwitchVal = digitalRead(rightSwitchPin);
  leftMotor.setSpeed(255);
  frontMotor.setSpeed(140);
  leftMotor.run(BACKWARD); 
  frontMotor.run(FORWARD); 
  
  do
  {
    Serial.println("rotating");
    rightSwitchVal = digitalRead(rightSwitchPin);  
  }while(!rightSwitchVal);
  
  turnMotorsOff();
  
  rightMotor.setSpeed(255);
  frontMotor.setSpeed(140);
  rightMotor.run(BACKWARD);
  frontMotor.run(BACKWARD);
  
  do
  {
    Serial.println("Aligning");
    leftSwitchVal = digitalRead(leftSwitchPin);
  }while(!leftSwitchVal);
  
  turnMotorsOff();
  
  rotated = 1;
}

void rotateOut()
{
  int currentTime = millis();
  int previousTime = currentTime;
  
  leftMotor.setSpeed(255);
  frontMotor.setSpeed(180);
  leftMotor.run(FORWARD);
  frontMotor.run(BACKWARD); 
  
  do
  {
    Serial.println("rotating out");
    currentTime = millis();
    
  }while(currentTime - previousTime <= 2000);
  previousTime = currentTime;  
  
  turnMotorsOff();
  
  leftMotor.setSpeed(255);
  backMotor.setSpeed(140);
  leftMotor.run(BACKWARD);
  backMotor.run(FORWARD);
  
  do
  {
    Serial.println("Back to wall");
    currentTime = millis();
  }while(currentTime - previousTime <= 2000);
  
  turnMotorsOff();  
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
         
  }while(leftPos != 110 && rightPos != 70);

  return 0; 
}

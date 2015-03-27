#include <AFMotor.h>
#include <Servo.h>

//define objects for motors
AF_DCMotor frontMotor(1);
AF_DCMotor leftMotor(2);
AF_DCMotor backMotor(3);
AF_DCMotor rightMotor(4);

// Define pins for wing pins
const byte frontSwitchPin = 34;
const byte leftSwitchPin = 33;
const byte rightSwitchPin = 32;

int leftSwitchVal, rightSwitchVal, frontSwitchVal;

const byte echoPin[] = {46,48,50,52};
const byte trigPin[] = {47,49,51,53};
const float distanceConstant = 58.2;
byte maxRange = 17;

long duration, distance;
long wallDistance;

byte dir = 2;
byte sensorNum = 0;
boolean doneOnce = 0;

// Define servo objects
Servo leftServo;
Servo rightServo;
Servo armServo;

byte leftPos = 0; // Starting positions for servo claws
byte rightPos = 180;
byte armPos = 180;
boolean start;
boolean rotated = 0;
boolean inPosition = 0;

boolean enableState = 0;
boolean prevEnableState = 0;

byte switchKeepDriving = 1;

char incomingByte = 0;
int counter = 0;
int xpos = 0;
//byte currentTime, previousTime;
int  tempValue = 0;
boolean clearedOne = 1;

void setup()
{
  Serial.begin(9600);
  pinMode(leftSwitchPin, INPUT);
  pinMode(rightSwitchPin, INPUT);
  pinMode(frontSwitchPin, INPUT);
  
  armServo.attach(42);
  leftServo.attach(41); 
  rightServo.attach(40);  
  
  for (int i = 0; i < 4; i++)
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
  armServo.write(armPos);
}

void loop()
{
  enableState = digitalRead(leftSwitchPin);
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
  
  rightSwitchVal = digitalRead(rightSwitchPin);
  delay(1);
  
  if(!start)
  {
    if(Serial.available() > 0)
    {
      while(true)
      {
        incomingByte = Serial.read();
        if(incomingByte == '\n')
          break;
        tempValue *= 10;
        tempValue += (int) (incomingByte - '0');
      }
      xpos = tempValue;
    }
    Serial.print("x-pos: ");
    Serial.println(xpos);
  }
  else if(start)
  {
    if(!clearedOne)
    {
      if(!rotated)
      {
        if(!rightSwitchVal) // modify later for a state variable, to account for opposite hopper
        {
          movement(dir);
        }
        else
        {
          rotateIn();
        }
      }
      else
      {
        if (dir == 2) // first retrieve the ball
        {
          dir = closeClaw(); // change direction to forward      
          rotateOut();        
        }
        else
        {      
          if (!inPosition)
          {
            movement(0);
            do
            {
              frontSwitchVal = digitalRead(frontSwitchPin);
              Serial.println("Waiting for bump");
              delay(1000);
            }while(frontSwitchVal == 0);
            
            turnMotorsOff();
            
            movement(2);
            delay(400);
            turnMotorsOff();
            maxRange = 55;

            movement(3);
            do
            {
              sensor(sensorNum);
            }while(distance <= maxRange);
            turnMotorsOff(); 
   
            maxRange = 5;
            inPosition = 1;              
          }
          else
          {
            movement(0);
            do
            {
              frontSwitchVal = digitalRead(frontSwitchPin);
              Serial.println("Waiting for bump");
              delay(1000);
            }while(frontSwitchVal == 0);          
            turnMotorsOff();            
            if(moveArm(-1))
            {
              Serial.println("First checkpoint");
              delay(1000);
              if(armPos == 0)
              {
                if(openClaw())
                {
                  moveArm(1);
                }
              }
            }          
          }
        }  
      }
    }
    else
    {
      rightSwitchVal = digitalRead(rightSwitchPin);
      if(!rotated)
      {
        if(!doneOnce)
        {
          sensor(1);
          movement(1);
          while(true)
          {
            sensor(1);
            Serial.print("Distance: ");
            Serial.println(distance);          
            if(distance == xpos-10)
            {
              turnMotorsOff();
              doneOnce = 1;
              break;
            }
          }
        }
        
        if(!rightSwitchVal) // modify later for a state variable, to account for opposite hopper
        {
          movement(2);
        }
        else
        {
          rotateIn();
        }        
      }
      else
      {
        dir = closeClaw();
//        movement(0);
        leftMotor.setSpeed(175);
        rightMotor.setSpeed(200);
        leftMotor.run(FORWARD);
        rightMotor.run(FORWARD);
        delay(3000);
        turnMotorsOff();
          
        rotateOutRight();
        
        if (!inPosition)
        {
          movement(0);
          do
          {
            frontSwitchVal = digitalRead(frontSwitchPin);
            Serial.println("Waiting for bump");
            delay(1000);
          }while(frontSwitchVal == 0);
          
          turnMotorsOff();
          
          movement(2);
          delay(400);
          turnMotorsOff();
          maxRange = 55;
          movement(3);
          do
          {
            sensor(1);
          }while(distance <= maxRange);
          turnMotorsOff(); 
 
          if(dir == 1)
          {
            dir = 3;
          }
          else if(dir == 3)
          {
            dir = 1;
          }   
          maxRange = 5;
          inPosition = 1;          
        }
        else
        {
          if(moveArm(-1))
          {
            Serial.println("First checkpoint");
            delay(1000);
            if(armPos == 0)
            {
              if(openClaw())
              {
                moveArm(1);
              }
            }
          }          
        }        
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
        inPosition = 1;
      }
      else if(dir == 1)
      {
        dir = 2;
        sensorNum = 3;
        maxRange = 20;
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
        inPosition = 1;
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

void rotateIn()
{
  turnMotorsOff();
  rightSwitchVal = digitalRead(rightSwitchPin);
  rightMotor.setSpeed(255);
  frontMotor.setSpeed(175);
  rightMotor.run(BACKWARD); 
  frontMotor.run(BACKWARD); 
  
  do
  {
    Serial.println("rotating");
    leftSwitchVal = digitalRead(leftSwitchPin);  
  }while(!leftSwitchVal);
  
  turnMotorsOff();
  
  leftMotor.setSpeed(255);
  frontMotor.setSpeed(175);
  leftMotor.run(BACKWARD);
  frontMotor.run(FORWARD);
  
  do
  {
    Serial.println("Aligning");
    rightSwitchVal = digitalRead(rightSwitchPin);
  }while(!rightSwitchVal);
  
  turnMotorsOff();
  
  rotated = 1;
}

void rotateOut()
{
  int currentTime = millis();
  int previousTime = currentTime;
  
  leftMotor.setSpeed(255);
  frontMotor.setSpeed(150);
  leftMotor.run(FORWARD);
  frontMotor.run(BACKWARD); 
  
  do
  {
    Serial.println("rotating out");
    currentTime = millis();
  }while(currentTime - previousTime <= 2000);
  previousTime = currentTime;  
  
  turnMotorsOff();
  
  leftMotor.setSpeed(150);
  backMotor.setSpeed(200);
  leftMotor.run(BACKWARD);
  backMotor.run(BACKWARD);
  
  do
  {
    Serial.println("Back to wall");
    currentTime = millis();
  }while(currentTime - previousTime <= 2000);
  
  turnMotorsOff();  
}

void rotateOutRight()
{
  int currentTime = millis();
  int previousTime = currentTime;
  
  leftMotor.setSpeed(255);
  frontMotor.setSpeed(150);
  leftMotor.run(FORWARD);
  frontMotor.run(BACKWARD); 
  
  do
  {
    Serial.println("rotating out");
    currentTime = millis();
  }while(currentTime - previousTime <= 2000);
  previousTime = currentTime;  
  
  turnMotorsOff();
  
  leftMotor.setSpeed(150);
  backMotor.setSpeed(200);
  leftMotor.run(BACKWARD);
  backMotor.run(BACKWARD);
  
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

//Functions for depositing ball start here
boolean moveArm(int upDown) // -1 for up, 1 for down
{
  boolean value;
  boolean exit = 1;
  // loop until all the way up/down
  if(upDown == -1)
  {
    Serial.println("Raising arm");
  }
  else if (upDown == 1)
  {
    Serial.println("Lowering arm");
  }
  
  do
  {
    armServo.write(armPos);
    delay(15);
    armPos += upDown;
    Serial.println("Stuck??");
    if(armPos == 0 || armPos == 172)
      exit = 0;
  }while(exit == 1);
  
  Serial.println("Reaching end of loop");
  
  if(upDown == -1)
  {
    value = 1;
  }
  else
  {
    start = 1;
    inPosition = 0;
    rotated = 0;
    switchKeepDriving = 1;
    maxRange = xpos;
    dir = 1;
    sensorNum = 1;
    clearedOne = 1;
  }
    
  return value;   
}

boolean openClaw()
{
  Serial.println("Opening claw");
  do // loop to open the claws (left and right claws go 180 degrees)
  {
    leftServo.write(leftPos);
    rightServo.write(rightPos);
    delay(15);
    //increment/decrement the right and left claw positions each iteration
    rightPos++;
    leftPos--;
      
  }while(leftPos != 0 && rightPos != 180);
  return 1;
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
         
  }while(leftPos != 120 && rightPos != 60);  

  return 0; 
}

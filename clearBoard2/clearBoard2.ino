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

const byte echoPin[] = {46, 48, 50, 52};
const byte trigPin[] = {47, 49, 51, 53};
const float distanceConstant = 58.2;
byte maxRange = 30;

long duration, distance;

byte dir = 2;
byte sensorNum = 1;

// Define servo objects
Servo leftServo;
Servo rightServo;
Servo armServo;
Servo frontServo;

byte leftPos = 0; // Starting positions for servo claws
byte rightPos = 180;
byte armPos = 180;
byte frontPos = 100;
boolean onOff = 1;
boolean start;
boolean rotated = 0;
boolean inPosition = 0;

boolean enableState = 0;
boolean prevEnableState = 0;

byte switchKeepDriving = 1;

char incomingByte = 0;
int counter = 0;
int xpos[2];
int tempValue = 0;

int rotatingSpeed1 = 245;
int rotatingSpeed2 = 235;

int motorSpeeds[] = {100, 255, 245, 210};

int numBallsLeft[] = {6, 6, 7};
int index = 0;

void setup()
{
  Serial.begin(9600);
  pinMode(leftSwitchPin, INPUT);
  pinMode(rightSwitchPin, INPUT);
  pinMode(frontSwitchPin, INPUT);

  armServo.attach(42);
  leftServo.attach(41);
  rightServo.attach(40);
  frontServo.attach(39);
  frontServo.write(frontPos);

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
  enableState = digitalRead(frontSwitchPin);
  delay(1);

  if (enableState != prevEnableState) // if button is pressed and released
  {
    if (enableState) // if switched to on
    {
      start = 1;
    }
  }

  if (numBallsLeft[2] == 0)
  {
    start = 0;
  }

  if (!start)
  {
    tempValue = 0;
    if (Serial.available() > 0)
    {
      while (true)
      {
        incomingByte = Serial.read();
        if (incomingByte == '\n')
          break;
        tempValue *= 10;
        tempValue += (int) (incomingByte - '0');
      }
      if(counter < 2)
      {
        xpos[counter] = tempValue;
      }
      else
      {
        motorSpeeds[counter-2] = tempValue;
      }
      counter++;
    }
    Serial.print("FMS: ");
    Serial.println(motorSpeeds[0]);
  }
  else
  {
    if (index != 2)
    {
      if (!rotated)
      {
        rightSwitchVal = digitalRead(rightSwitchPin);
        delay(1);

        if (numBallsLeft[0] == 6) // only for very initial retrieval
        {
          if (!rightSwitchVal)
          {
            leftMotor.setSpeed(245);
            rightMotor.setSpeed(245);
            rightMotor.run(BACKWARD);
            leftMotor.run(BACKWARD);
          }
          else
          {
            rotateIn();
          }
        }
        else
        {
          leftSwitchVal = digitalRead(leftSwitchPin);
          delay(1);
          rightSwitchVal = digitalRead(rightSwitchPin);
          delay(1);

          if (!leftSwitchVal && !rightSwitchVal) // modify later for a state variable, to account for opposite hopper
          {            
            //Every iteration, check distance to wall
            sensor(sensorNum);
            // function call to determine if at wall or not
            keepDriving(switchKeepDriving);
          }
          else
          {
            rotateIn();
          }
        }
      }
      else
      {
        if (dir == 2) // first retrieve the ball
        {
          dir = closeClaw(); // change direction to forward

          if (index == 0)
            rotateOutRight();
          else
            rotateOutLeft();

          liftFrontServo(-1);
        }
        else
        {
          if (!inPosition)
          {
            if(waitForBump())
            {
              if (moveArm(-1))
              {  
                if (index == 0)
                {
                  dir = 1;
                  sensorNum = 1;
                }
                else
                {
                  dir = 3;
                  sensorNum = 3;
                }
                
                movement(dir);
                do
                {
                  sensor(sensorNum);
                }while(distance >= 4);
                turnMotorsOff();
  
                movement(2);
                delay(400);
                turnMotorsOff();
                maxRange = 50;
                if (numBallsLeft[0] != 0)
                {
                  sensorNum = 1;
                  dir = 3;
                }
                else
                {
                  sensorNum = 3;
                  dir = 1;
                }
                movement(dir);
                do
                {
                  sensor(sensorNum);
                } while (distance <= maxRange);
                turnMotorsOff();
  
                maxRange = 4;
                inPosition = 1;
              }
            }
          }
          else
          {
            if(waitForBump())
            {
              if (armPos == 0)
              {
                if (openClaw())
                {
                  moveArm(1);
                  liftFrontServo(1);
                  leftMotor.setSpeed(245);
                  rightMotor.setSpeed(245);
                  leftMotor.run(BACKWARD);
                  rightMotor.run(BACKWARD);
                  delay(300);
                  turnMotorsOff();
//                  Serial.print("Direction: ");
//                  Serial.println(dir);
//                  Serial.print("Num balls left: ");
//                  Serial.println(numBallsLeft[index]);
//                  delay(2000);
                }
              }
            }
          }
        }
      }
    }
    else
    {
      if (xpos[0] == 1)
      {
        movement(1);
        do
        {
          sensor(1);
        } while (distance <= xpos[1]);

        dir = 3;
        sensorNum = 1;
      }
      else
      {
        movement(3);
        do
        {
          sensor(3);
        } while (distance <= xpos[1]);
        dir = 1;
        sensorNum = 3;
      }
      turnMotorsOff();

      movement(2);
      do
      {
        rightSwitchVal = digitalRead(rightSwitchPin);
        leftSwitchVal = digitalRead(leftSwitchPin);
      } while (!rightSwitchVal && !leftSwitchVal);
      turnMotorsOff();
      closeClaw();
      liftFrontServo(-1);

      waitForBump();
      if (moveArm(-1))
      {
        movement(2);
        delay(300);
        turnMotorsOff();
        maxRange = 55;

        movement(dir);
        do
        {
          sensor(sensorNum);
        } while (distance <= maxRange);
        turnMotorsOff();

        waitForBump();

//        delay(1000);
        if (armPos == 0)
        {
          if (openClaw())
          {
            moveArm(1);
            liftFrontServo(1);
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

  //  Serial.println("Motors off?");
  delay(1000);
}

boolean waitForBump()
{
  int currentTime, prevTime;
  movement(0);
  
  while(true)
  {
    frontSwitchVal = digitalRead(frontSwitchPin);
    
    if(frontSwitchVal)
    {
      prevTime = millis();    
      do
      {
        Serial.println("Stuck");
        currentTime = millis();
        frontSwitchVal = digitalRead(frontSwitchPin);
        if(!frontSwitchVal)
        {
          return 0;
        }
      }while(frontSwitchVal && (currentTime - prevTime <= 2000));    
      turnMotorsOff();
      return 1;       
    }
  }
}

void keepDriving(byte lessGreater)
{
  if (lessGreater == 1)
  {
    if (distance <= maxRange && distance != 0)
    {
      turnMotorsOff();
      if (dir == 0)
      {
        if (index == 0)
        {
          dir = 3;
          sensorNum = 1;
        }
        else
        {
          dir = 1;
          sensorNum = 3;
        }
        maxRange = 55;
        switchKeepDriving = 0;
      }
      else if (dir == 1)
      {
        sensorNum = 3;
        dir = 2;
      }
      else if (dir == 3)
      {
        sensorNum = 1;
        dir = 2;
      }
    }
    else if (distance >= maxRange) {
      movement(dir);
    }
  }
  else
  {
    if (distance >= maxRange)
    {
      turnMotorsOff();
      if (dir == 3)
      {
        dir = 1;
        inPosition = 1;
        maxRange = 4;
      }
      else if (dir == 1)
      {
        dir = 3;
        inPosition = 1;
        maxRange = 4;
      }
    }
    else if (distance <= maxRange) {
      movement(dir);
    }
  }
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
  byte oneTwo = 0;

  // direct motors to turn in appropriate direction and speed
  if (motorDirection == 0)
  {
    oneTwo = 1;
  }
  else if (motorDirection == 1)
  {
    oneTwo = 2;
  }
  else if (motorDirection == 2)
  {
    oneTwo = 2;
  }
  else
  {
    oneTwo = 1;
  }

  if (motorDirection == 0 || motorDirection == 2)
  {
    byte speed1 = 250;
    byte speed2 = 210;

    if (index == 0)
    {
      leftMotor.setSpeed(motorSpeeds[1]);
      rightMotor.setSpeed(motorSpeeds[3]);
    }
    else if (index == 1)
    {
      leftMotor.setSpeed(motorSpeeds[3]);
      rightMotor.setSpeed(motorSpeeds[1]);
    }
    else
    {
      leftMotor.setSpeed(235);
      rightMotor.setSpeed(235);
    }
    leftMotor.run(oneTwo);
    rightMotor.run(oneTwo);
  }
  else if (motorDirection == 1 || motorDirection == 3)
  {
    frontMotor.setSpeed(motorSpeeds[0]);
    backMotor.setSpeed(motorSpeeds[2]);
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
  int speed1 = 255;
  int speed2 = 140;
  turnMotorsOff();

  if (numBallsLeft[0] == 6 || index == 1)
  {
    rightSwitchVal = digitalRead(rightSwitchPin);
    rightMotor.setSpeed(speed2);
    frontMotor.setSpeed(speed1);
//    backMotor.setSpeed(speed2);
//    backMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
    frontMotor.run(BACKWARD);

    do
    {
      Serial.println("rotating");
      leftSwitchVal = digitalRead(leftSwitchPin);
    } while (!leftSwitchVal);

    turnMotorsOff();

    leftMotor.setSpeed(speed1);
    frontMotor.setSpeed(speed2);
    leftMotor.run(BACKWARD);
    frontMotor.run(FORWARD);

    do
    {
      Serial.println("Aligning");
      rightSwitchVal = digitalRead(rightSwitchPin);
    } while (!rightSwitchVal);
  }
  else
  {
    leftSwitchVal = digitalRead(leftSwitchPin);
    leftMotor.setSpeed(speed2);
    frontMotor.setSpeed(speed1);
//    backMotor.setSpeed(speed2);
//    backMotor.run(FORWARD);
    leftMotor.run(BACKWARD);
    frontMotor.run(FORWARD);

    do
    {
      Serial.println("rotating");
      rightSwitchVal = digitalRead(rightSwitchPin);
    } while (!rightSwitchVal);

    turnMotorsOff();

    rightMotor.setSpeed(speed1);
    frontMotor.setSpeed(speed2);
    rightMotor.run(BACKWARD);
    frontMotor.run(BACKWARD);

    do
    {
      Serial.println("Aligning");
      leftSwitchVal = digitalRead(leftSwitchPin);
    } while (!leftSwitchVal);
  }
  turnMotorsOff();

  leftMotor.setSpeed(240);
  rightMotor.setSpeed(240);
  rightMotor.run(FORWARD);
  leftMotor.run(FORWARD);
  delay(300);
  turnMotorsOff();

  leftMotor.setSpeed(250);
  rightMotor.setSpeed(250);
  leftMotor.run(BACKWARD);
  rightMotor.run(BACKWARD);

  do
  {
    rightSwitchVal = digitalRead(rightSwitchPin);
    delay(1);
    leftSwitchVal = digitalRead(leftSwitchPin);
    delay(1);
  } while (!rightSwitchVal || !leftSwitchVal);

  turnMotorsOff();

  rotated = 1;
}

void rotateOutRight()
{
  int currentTime = millis();
  int previousTime = currentTime;

  leftMotor.setSpeed(rotatingSpeed1);
  frontMotor.setSpeed(rotatingSpeed2);
  leftMotor.run(FORWARD);
  frontMotor.run(BACKWARD);

  do
  {
    Serial.println("rotating out");
    currentTime = millis();
    //    sensor(1);
  } while (currentTime - previousTime <= 2000);
  //  }while(distance <= 5);
  previousTime = currentTime;

  turnMotorsOff();

  leftMotor.setSpeed(rotatingSpeed2);
  backMotor.setSpeed(rotatingSpeed1);
  leftMotor.run(BACKWARD);
  backMotor.run(BACKWARD);

  do
  {
    Serial.println("Back to wall");
    currentTime = millis();
  } while (currentTime - previousTime <= 2000);

  turnMotorsOff();
}

void rotateOutLeft()
{
  int currentTime = millis();
  int previousTime = currentTime;

  rightMotor.setSpeed(rotatingSpeed1);
  frontMotor.setSpeed(rotatingSpeed2);
  rightMotor.run(FORWARD);
  frontMotor.run(FORWARD);

  do
  {
    Serial.println("rotating out");
    currentTime = millis();
    //    sensor(3);
  } while (currentTime - previousTime <= 2000);
  //  }while(distance <= 5);
  previousTime = currentTime;

  turnMotorsOff();

  rightMotor.setSpeed(rotatingSpeed2);
  backMotor.setSpeed(rotatingSpeed1);
  rightMotor.run(BACKWARD);
  backMotor.run(FORWARD);

  do
  {
    Serial.println("Back to wall");
    currentTime = millis();
  } while (currentTime - previousTime <= 2000);

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
  distance = duration / distanceConstant;

  delay(50);
}

//Functions for depositing ball start here
boolean moveArm(int upDown) // -1 for up, 1 for down
{
  boolean value;
  boolean exit = 1;
  // loop until all the way up/down
  if (upDown == -1)
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
    if (armPos == 0 || armPos == 180)
      exit = 0;
  } while (exit == 1);

  Serial.println("Reaching end of loop");

  if (upDown == -1)
  {
    value = 1;
  }
  else if (upDown == 1)
  {
    value = 0;
    inPosition = 0;
    rotated = 0;
    numBallsLeft[index] -= 1;
    if (numBallsLeft[index] == 0)
    {
      index++;
    }

    if (index == 0)
    {
      dir = 1;
      sensorNum = 1;
    }
    else if (index == 1)
    {
      dir = 3;
      sensorNum = 3;
    }

    switchKeepDriving = 1;
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

  } while (leftPos != 0 && rightPos != 180);
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

  } while (leftPos != 90 && rightPos != 90);

  return 0;
}

void liftFrontServo(int upDown)
{
  boolean exit = 1;
  do
  {
    frontServo.write(frontPos);
    delay(15);
    frontPos += upDown;
    if (frontPos == 5 || frontPos == 100)
      exit = 0;
  } while (exit == 1);
}

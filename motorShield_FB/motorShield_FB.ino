#include <AFMotor.h>

AF_DCMotor leftMotor(2);
AF_DCMotor rightMotor(4);

const int echoPin[] = {22,24};
const int trigPin[] = {23,25};

const float distanceConstant = 58.2;
const int maxRange = 10;

long duration, distance;

int dir = 0;
int sensorNum = 0;

void setup()
{
  Serial.begin(9600);
  
  for(int i = 0; i < 2; i++)
  {
    pinMode(trigPin[i], OUTPUT);
    pinMode(echoPin[i], INPUT);
  }
  
  leftMotor.setSpeed(175);
  rightMotor.setSpeed(175);

  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);  
}

void loop()
{
  sensor(sensorNum);
  Serial.print("Sensor Number: ");
  Serial.println(sensorNum);

  Serial.print("The distance is: ");
  Serial.println(distance);
  
  Serial.print("The direction is: ");
  Serial.println(dir);  
  
  if (distance <= maxRange)
  {
    leftMotor.run(RELEASE);
    rightMotor.run(RELEASE);
    
    if(dir == 0)
    {
      sensorNum = 1;
      dir = 2;
    }
    else if(dir == 2)
    {
      Serial.println("Work pls");
      delay(1000);
      sensorNum = 0;
      dir = 0;
    }
  }
  else if (distance >= maxRange){
    movement(dir);    
  }    
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
  if(motorDirection == 0)
  {
    leftMotor.run(FORWARD);
    rightMotor.run(FORWARD);
  }
  else if (motorDirection == 2)
  {
    leftMotor.run(BACKWARD);
    rightMotor.run(BACKWARD);
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

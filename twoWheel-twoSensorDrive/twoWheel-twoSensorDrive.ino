const int echoPin[] = {24,22};
const int trigPin[] = {25,23};
const float distanceConstant = 58.2;
const int maxRange = 10;

const int motorPin[] = {36,37,38,39};
const int enablePin[] = {2, 3};

long duration, distance;

int dir = 0;
int sensorNum = 0;

void setup()
{
  Serial.begin(9600);
  
  for(int i = 0; i < 4; i++)
  {
    pinMode(motorPin[i], OUTPUT);
    if (i < 2)
    {
      pinMode(trigPin[i], OUTPUT);
      pinMode(echoPin[i], INPUT);
//      pinMode(enablePin[i], OUTPUT);
    }
  }
//  pinMode(enablePin[0], OUTPUT);
//  pinMode(enablePin[1], OUTPUT);
}

void loop()
{
  sensor(sensorNum);
  Serial.print("Sensor Number: ");
  Serial.println(sensorNum);
  
  analogWrite(enablePin[0], 153);
  analogWrite(enablePin[1], 165);
  
  Serial.print("The distance is: ");
  
  if (distance <= maxRange)
  {
    if(dir == 0)
    {
      sensorNum = 1;
      dir = 2;
    }
    else if(dir == 2)
    {
      sensorNum = 0;
      dir = 0;
    }
  }
  else if (distance >= maxRange){
    movement(dir);    
  }
  
  Serial.println(distance);
  
  Serial.print("The direction is: ");
  Serial.println(dir);
}

void keepDriving(int dir)
{
  digitalWrite(enablePin[0], LOW);
  digitalWrite(enablePin[1], LOW);
  if (distance <= maxRange)
  {
    if(dir == 0)
    {
      sensorNum = 1;
      dir == 2;
    }
    else if(dir == 2)
    {
      sensorNum = 0;
      dir == 0;
    }
  }
  else if (distance >= maxRange){
    movement(dir);    
  }  
}

void movement(int motorDirection)//0 is forward, 1 is right, 2 is back, 3 is left, -1 is nothing
{
  boolean highLow = 0;

  if(motorDirection == 0)
  {
    highLow = 1;
    Serial.println("Moving forward");     
  }
  else if (motorDirection == 2)
  {
    Serial.println("Moving backwards");
    highLow = 0;
  }
    digitalWrite(motorPin[0], highLow);
    digitalWrite(motorPin[1], !highLow);
    digitalWrite(motorPin[2], highLow);
    digitalWrite(motorPin[3], !highLow);
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

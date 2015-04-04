#include <Servo.h> 

Servo leftServo;  // create servo object to control a servo 

int leftPos = 0;    // variable to store the servo position 

const int enableButPin = 34;
int enableState = 0;
int prevEnableState = 0;
boolean start;

void setup() 
{ 
  Serial.begin(4800);
  leftServo.attach(41);  // attaches the servo on pin 9 to the servo object 
  
  leftServo.write(leftPos);
  
  pinMode(enableButPin, INPUT);

  delay(50);
} 


void loop() 
{ 
  enableState = digitalRead(enableButPin);
  delay(1);
  
  Serial.println(enableState);
  
  if(enableState != prevEnableState) // if button is pressed and released
  {
    if(enableState) // if switched to on
    {
      start = 1;
    }
  }
  
  if(start)
  {
    do // loop to close the claws (left and right claws go 180 degrees)
    {
      Serial.println("lolol");
      leftServo.write(leftPos);
      delay(15);
      leftPos++;
           
    }while(leftPos != 100);
  }
}


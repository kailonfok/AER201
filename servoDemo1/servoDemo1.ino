#include <Servo.h> 

Servo leftServo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 
Servo rightServo;

int leftPos = 180;    // variable to store the servo position 
int rightPos = 167;

const int enableButPin = 33;
int enableState = 0;
int prevEnableState = 0;
boolean start;

void setup() 
{ 
  Serial.begin(4800);
  leftServo.attach(42);  // attaches the servo on pin 9 to the servo object 
//  rightServo.attach(53);
  
  leftServo.write(leftPos);
  rightServo.write(167);
  delay(50);
  pinMode(enableButPin, INPUT);
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
    else
    {
      start = 0;
    }
  }
  
  if (start)
  {
    do // loop to close the claws (left and right claws go 180 degrees)
    {
      Serial.println("lolol");
      leftServo.write(leftPos);
      delay(15);
      rightServo.write(rightPos);
      rightPos--;
      leftPos--;
           
    }while(leftPos != 0);// && rightPos != 97);
    
    delay(1000);
    
    do
    {
      leftServo.write(leftPos);
      delay(15);
      leftPos++;
    }while(leftPos != 175);
  } 
  else
  {
    Serial.println("Off, sort of");
  }
  
  prevEnableState = enableState;
}


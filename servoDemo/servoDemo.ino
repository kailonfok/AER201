#include <Servo.h> 

Servo myservo;  // create servo object to control a servo 
// a maximum of eight servo objects can be created 

int pos = 0;    // variable to store the servo position 
int temp = 0;

void setup() 
{ 
  myservo.attach(40);  // attaches the servo on pin 9 to the servo object 
} 


void loop() 
{ 
  if (!temp)
  {
    for(pos = 180; pos >= 0; pos -= 1)  // goes from 0 degrees to 180 degrees 
    {                                  // in steps of 1 degree 
      myservo.write(pos);              // tell servo to go to position in variable 'pos' 
      delay(20);                       // waits 15ms for the servo to reach the position 
    }
    temp = 1;
  } 
  else
  {
    myservo.attach(9);
  }
}


#include <AFMotor.h>

AF_DCMotor leftMotor(2);
AF_DCMotor rightMotor(4);

void setup()
{
  leftMotor.setSpeed(180);
  rightMotor.setSpeed(255);

  leftMotor.run(RELEASE);
  rightMotor.run(RELEASE);   
}

void  loop()
{
  leftMotor.run(BACKWARD);
  rightMotor.run(BACKWARD);
}

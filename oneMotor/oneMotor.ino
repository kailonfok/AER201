#include <AFMotor.h>

AF_DCMotor leftMotor(2); // change the numbers here to "connect" to different motors (labeled on the motor shield)
AF_DCMotor rightMotor(4);

void setup()
{
  leftMotor.setSpeed(240); // change these numbers to get varying speed, max is 255
  rightMotor.setSpeed(240);

  leftMotor.run(RELEASE); // RELEASE is to brake/stop, don't change these ones
  rightMotor.run(RELEASE);   
}

void  loop()
{
  leftMotor.run(FORWARD); // change these to change from forward/back, left/right
  rightMotor.run(FORWARD);
}

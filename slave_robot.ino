#include <UCMotor.h>
#include <Wire.h>
#include <stdlib.h>
#include <time.h>

#define FORWARD_COMMAND 1
#define BACKWARD_COMMAND 2
#define LEFT_COMMAND 3
#define RIGHT_COMMAND 4
#define STOP_COMMAND 5

#define OUT_PIN 8
#define IN_PIN 9

int MAX_SPEED_LEFT;
int MAX_SPEED_RIGHT;
int SZ_SPEEDTHR = 150;

bool objectDetected = false;

UC_DCMotor leftMotor1(3, MOTOR34_64KHZ);
UC_DCMotor rightMotor1(4, MOTOR34_64KHZ);
UC_DCMotor leftMotor2(1, MOTOR34_64KHZ);
UC_DCMotor rightMotor2(2, MOTOR34_64KHZ);

void setup()
{
  Serial.begin(115200);

  MAX_SPEED_LEFT = SZ_SPEEDTHR;
  MAX_SPEED_RIGHT = SZ_SPEEDTHR;
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, INPUT);
}

void loop()
{
  // get command from WiFi module
  int command = digitalRead(IN_PIN);
  switch(command) {
    case 1:
      moveForward();
      break;
    case 2:
      moveBackward();
      break;
    case 3:
      turnLeft();
      break;
    case 4:
      turnRight();
      break;
    case 5:
      moveStop();
      break;
    default:
      break;
  }
}

void moveForward(void)
{
  leftMotor1.run(FORWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(FORWARD);
  leftMotor1.setSpeed(MAX_SPEED_LEFT);
  rightMotor1.setSpeed(MAX_SPEED_RIGHT);
  leftMotor2.setSpeed(MAX_SPEED_LEFT);
  rightMotor2.setSpeed(MAX_SPEED_RIGHT);
}

void moveBackward(void)
{ 
  leftMotor1.run(BACKWARD);
  rightMotor1.run(BACKWARD);
  leftMotor2.run(BACKWARD);
  rightMotor2.run(BACKWARD);
  leftMotor1.setSpeed(MAX_SPEED_LEFT);
  rightMotor1.setSpeed(MAX_SPEED_RIGHT);
  leftMotor2.setSpeed(MAX_SPEED_LEFT);
  rightMotor2.setSpeed(MAX_SPEED_RIGHT);
}

void turnRight(void)
{
  static int MAX_SPEED_LEFT_AR, MAX_SPEED_RIGHT_AR;
  
  leftMotor1.run(FORWARD);
  rightMotor1.run(BACKWARD);
  leftMotor2.run(FORWARD);
  rightMotor2.run(BACKWARD);
  MAX_SPEED_LEFT_AR = MAX_SPEED_LEFT > 200 ? 200 : MAX_SPEED_LEFT;
  MAX_SPEED_RIGHT_AR = MAX_SPEED_RIGHT > 200 ? 200 : MAX_SPEED_RIGHT;
  leftMotor1.setSpeed(MAX_SPEED_LEFT_AR);
  rightMotor1.setSpeed(MAX_SPEED_RIGHT_AR);
  leftMotor2.setSpeed(MAX_SPEED_LEFT_AR);
  rightMotor2.setSpeed(MAX_SPEED_RIGHT_AR);
}

void turnLeft(void)
{
  static int MAX_SPEED_LEFT_AL, MAX_SPEED_RIGHT_AL;
  
  leftMotor1.run(BACKWARD);
  rightMotor1.run(FORWARD);
  leftMotor2.run(BACKWARD);
  rightMotor2.run(FORWARD);
  MAX_SPEED_LEFT_AL = MAX_SPEED_LEFT > 200 ? 200 : MAX_SPEED_LEFT;
  MAX_SPEED_RIGHT_AL = MAX_SPEED_RIGHT > 200 ? 200 : MAX_SPEED_RIGHT;
  leftMotor1.setSpeed(MAX_SPEED_LEFT_AL);
  rightMotor1.setSpeed(MAX_SPEED_RIGHT_AL);
  leftMotor2.setSpeed(MAX_SPEED_LEFT_AL);
  rightMotor2.setSpeed(MAX_SPEED_RIGHT_AL);
}

void moveStop(void)
{ 
  leftMotor1.run(5); rightMotor1.run(5);
  leftMotor2.run(5); rightMotor2.run(5);
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

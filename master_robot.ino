#include <UCMotor.h>
#include <Wire.h>
#include <stdlib.h>
#include <time.h>

#define TRIG_PIN A2
#define ECHO_PIN A3

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
  pinMode(ECHO_PIN, INPUT); 
  pinMode(TRIG_PIN, OUTPUT);
  Serial.begin(115200);

  MAX_SPEED_LEFT = SZ_SPEEDTHR;
  MAX_SPEED_RIGHT = SZ_SPEEDTHR;
  pinMode(OUT_PIN, OUTPUT);
  pinMode(IN_PIN, INPUT);
}

void loop()
{
  int count = 0;
  
  while (true) {
    detectObject();
    moveForward();
    count++;
    
    if (objectDetected) {
      moveStop();
      delay(500);
      moveBackward();
      delay(500);
      turnRandom();
      delay(1000);
      continue;
    } else {
      Serial.println(count);
      // most likely stuck on something, turn around after 1000 cycles
      if (count > 1000) { 
        moveStop();
        delay(500);
        moveBackward();
        delay(750);
        turnRight(); // rotate 180
        delay(2000);
        count = 0;
        continue;
      }
    }
  }  
}

bool detectObject() {
   long duration, inches;
   
   digitalWrite(TRIG_PIN, LOW);
   delayMicroseconds(2);
   digitalWrite(TRIG_PIN, HIGH);
   delayMicroseconds(5);
   digitalWrite(TRIG_PIN, LOW);
   
   duration = pulseIn(ECHO_PIN, HIGH);
   inches = microsecondsToInches(duration);

   if (inches < 6) {
    objectDetected = true;
   } else {
    objectDetected = false;
   }
}

void turnRandom() {
  int r = rand();

  if (r % 2 == 0) {
    turnLeft();
  } else {
    turnRight();
  }
}

void moveForward(void)
{
  // send signal to NodeMCU
  digitalWrite(OUT_PIN, FORWARD_COMMAND);
  
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
  // send signal to NodeMCU
  digitalWrite(OUT_PIN, BACKWARD_COMMAND);
  
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

  // send signal to NodeMCU
  digitalWrite(OUT_PIN, RIGHT_COMMAND);
  
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

  // send signal to NodeMCU
  digitalWrite(OUT_PIN, LEFT_COMMAND);
  
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
  // send signal to NodeMCU
  digitalWrite(OUT_PIN, STOP_COMMAND);
  
  leftMotor1.run(5); rightMotor1.run(5);
  leftMotor2.run(5); rightMotor2.run(5);
}

long microsecondsToInches(long microseconds) {
   return microseconds / 74 / 2;
}

#define TRIG_PIN 10
#define ECHO_PIN 9

#define in1 7
#define in1PWM 3
#define in2 8
#define in2PWM 11

#define maxSpeed 150
#define turnSpeed 125
#define servoDelay 200
#define distanceToStop 300

#include <Arduino.h>
#include <Servo.h>
#include <hcsr04.h>
#include <timer.cpp>
#include <GyverMotor.h>

HCSR04 hcsr(TRIG_PIN, ECHO_PIN);
Servo servo = Servo();
Timer distTimer(100);
GMotor lMotor(DRIVER2WIRE, in1, in1PWM, HIGH);
GMotor rMotor(DRIVER2WIRE, in2, in2PWM, HIGH);

enum
{
  angle_0,
  angle_30,
  angle_60,
  angle_90,
  angle_120,
  angle_150,
  angle_180
};
int angles[7] = {180, 150, 120, 90, 60, 30, 0};
int distances[7] = {10000, 10000, 10000, 10000, 10000, 10000, 10000};

void stop()
{
  lMotor.setSpeed(0);
  rMotor.setSpeed(0);
}

void goForward()
{
  lMotor.setSpeed(maxSpeed);
  rMotor.setSpeed(maxSpeed);
}

void checkAhead()
{
  static int currentAngle = angle_60;
  static bool leftToRight = true;

  servo.write(angles[currentAngle]);
  delay(servoDelay);
  distances[currentAngle] = hcsr.distanceInMillimeters();

  if (leftToRight)
  {
    currentAngle++;
  }
  else
  {
    currentAngle--;
  }

  if (currentAngle == angle_120 && leftToRight)
  {
    leftToRight = false;
  }
  if (currentAngle == angle_60 && !leftToRight)
  {
    leftToRight = true;
  }
}

void findNewDirection()
{

  //int currentDistance;
  bool goLeft = false;

  // find out where to go
  servo.write(angles[angle_180]);
  delay(servoDelay * 3);
  distances[angle_180] = hcsr.distanceInMillimeters();

  servo.write(angles[angle_0]);
  delay(servoDelay * 3);
  distances[angle_0] = hcsr.distanceInMillimeters();

  goLeft = distances[angle_0] > distances[angle_180];

  // turn while controlling clearance ahead
  int angleCorrection = (goLeft) ? -10 : 10;
  servo.write(angles[angle_90] + angleCorrection);
  delay(servoDelay);

  if (goLeft)
  {
    lMotor.setSpeed(-turnSpeed);
    rMotor.setSpeed(turnSpeed);
  }
  else
  {
    lMotor.setSpeed(turnSpeed);
    rMotor.setSpeed(-turnSpeed);
  }

  delay(150);
  stop();

  // currentDistance = hcsr.distanceInMillimeters();
  // while (currentDistance < distanceToStop)
  // {
  //   delay(50);
  //   currentDistance = hcsr.distanceInMillimeters();
  // }
  // stop();

  for (int i = angle_60; i <= angle_120; i++)
  {
    servo.write(angles[i]);
    delay(servoDelay);
    distances[i] = hcsr.distanceInMillimeters();
  }
}

void movementLogic()
{
  checkAhead();

  if (distances[angle_60] < distanceToStop ||
      distances[angle_90] < distanceToStop ||
      distances[angle_120] < distanceToStop)
  {
    stop();
    findNewDirection();
  }
  else
  {
    goForward();
  }
}

void setup()
{

  // Пины D3 и D11 - 4 кГц
  TCCR2B = 0b00000010; // x8
  TCCR2A = 0b00000001; // phase correct

  servo.attach(4);
  //servo.write(angles[angle_90]); // center position

  lMotor.setMode(AUTO);
  lMotor.setDirection(REVERSE);
  rMotor.setMode(AUTO);
  rMotor.setDirection(REVERSE);
}

void loop()
{
  movementLogic();
}
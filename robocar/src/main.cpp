#include <Arduino.h>
#include <Servo.h>
#include <hcsr04.h>
#include <timer.cpp>

#define TRIG_PIN 13
#define ECHO_PIN 12

HCSR04 hcsr(TRIG_PIN, ECHO_PIN, 20, 4000);
Timer distTimer(50);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  if (distTimer.itsTime())
  {
    Serial.println(hcsr.distanceInMillimeters());
  }
}
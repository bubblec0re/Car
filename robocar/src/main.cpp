#include <Arduino.h>
#include <Servo.h>
#include <hcsr04.h>
#include <timer.cpp>
#include <RadioLib.h>

#define TRIG_PIN 3
#define ECHO_PIN 2
#define cs_pin 5
#define irq_pin 7
#define ce_pin 8

HCSR04 hcsr(TRIG_PIN, ECHO_PIN, 20, 4000);
nRF24 nrf = new Module(cs_pin, irq_pin, ce_pin);
Timer distTimer(50);
Timer transmitTimer(200);
int distance = 0;

void setup()
{
  Serial.begin(9600);

  Serial.print(F("[nRF24] Initializing ... "));
  int state = nrf.begin();
  if (state == ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
    {
    };
  }

  byte addr[] = {0x01, 0x23, 0x45, 0x67, 0x89};
  Serial.print(F("[nRF24] Setting transmit pipe ... "));
  state = nrf.setTransmitPipe(addr);
  if (state == ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
    {
    };
  }
}

void loop()
{
  if (distTimer.itsTime())
  {
    distance = hcsr.distanceInMillimeters();
    Serial.println(distance);
  }

  if (transmitTimer.itsTime())
  {
    Serial.print(F("[nRF24] Transmitting packet ... "));
    char distArray[2];
    distArray[0] = distance & 0x00ff;
    distArray[1] = (distance & 0xff00) >> 8;
    int state = nrf.transmit(distArray);
    if (state == ERR_NONE)
    {
      // the packet was successfully transmitted
      Serial.println(F("success!"));
    }
    else if (state == ERR_PACKET_TOO_LONG)
    {
      // the supplied packet was longer than 32 bytes
      Serial.println(F("too long!"));
    }
    else if (state == ERR_ACK_NOT_RECEIVED)
    {
      // acknowledge from destination module
      // was not received within 15 retries
      Serial.println(F("ACK not received!"));
    }
    else if (state == ERR_TX_TIMEOUT)
    {
      // timed out while transmitting
      Serial.println(F("timeout!"));
    }
    else
    {
      // some other error occurred
      Serial.print(F("failed, code "));
      Serial.println(state);
    }
  }
}
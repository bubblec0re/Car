#include <RadioLib.h>
#define cs_pin 5
#define irq_pin 7
#define ce_pin 8

nRF24 nrf = new Module(cs_pin, irq_pin, ce_pin);

void setup()
{
  Serial.begin(9600);

  // initialize nRF24
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
      ;
  }

  Serial.print(F("[nRF24] Setting address for receive pipe 0 ... "));
  byte addr[] = {0x01, 0x23, 0x45, 0x67, 0x89};
  state = nrf.setReceivePipe(0, addr);
  if (state == ERR_NONE)
  {
    Serial.println(F("success!"));
  }
  else
  {
    Serial.print(F("failed, code "));
    Serial.println(state);
    while (true)
      ;
  }
}

void loop()
{
  byte byteArr[2];
  int state = nrf.receive(byteArr, 2);

  if (state == ERR_NONE)
  {
    int dist = 0;
    if (byteArr != (byte)0x00)
    {
      dist |= byteArr[0];
      dist <<= 8;
    }
    dist |= byteArr[1];

    Serial.print(F("[nRF24] Data:\t\t"));
    Serial.print(byteArr[0]);
    Serial.print('\t');
    Serial.println(byteArr[1]);
    Serial.println(dist);
  }
  else if (state == ERR_RX_TIMEOUT)
  {
    Serial.println("timeout");
  }
  else
  {
    // some other error occurred
    Serial.print(F("failed, code "));
    Serial.println(state);
  }

}

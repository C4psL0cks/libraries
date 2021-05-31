#include <HardwareSerial.h>

#define RXPIN  16
#define TXPIN  17

HardwareSerial mySerial(2);

void setup()
{
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("ESP32"));
  gsm.begin(&mySerial, 9600, RXPIN, TXPIN);
  gsm.Event_debug = debug;
  Serial.println(F("UC20"));
}

void loop()
{
  while (gsm.available()) {
    Serial.write(gsm.read());
  }
  while (Serial.available()) {
    gsm.write(Serial.read());
  }
}

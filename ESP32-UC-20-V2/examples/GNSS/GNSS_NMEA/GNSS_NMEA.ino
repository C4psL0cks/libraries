#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "gnss.h"

#define RXPIN  16
#define TXPIN  17

GNSS gps;

HardwareSerial mySerial(2);

void debug(String data)
{
  Serial.println(data);
}
void data_out(char data)
{
  Serial.write(data);
}
void setup() 
{


  Serial.begin(9600);
  Serial.println();
  Serial.println(F("ESP32"));
  gsm.begin(&mySerial, 9600, RXPIN, TXPIN);
  gsm.Event_debug = debug;
  Serial.println(F("UC20"));
  gsm.PowerOn();
  while (gsm.WaitReady()) {}
  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());
  Serial.println(F("Disconnect net"));

  gps.Start();
  Serial.println(F("Enable NMEA"));
  gps.EnableNMEA();
}
void loop() 
{
  Serial.println(gps.GetNMEA("GGA"));
  delay(500);  
}

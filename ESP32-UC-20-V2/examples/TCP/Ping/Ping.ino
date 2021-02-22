#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "internet.h"
#include "tcp.h"

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define RXPIN  16
#define TXPIN  17

INTERNET net;
TCP tcp;

HardwareSerial mySerial(2);

void debug(String data)
{
  Serial.println(data);
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
  net.DisConnect();
  Serial.println(F("Set APN and Password"));
  net.Configure(APN, USER, PASS);
  Serial.println(F("Connect net"));
  net.Connect();
  Serial.println(F("Show My IP"));
  Serial.println(net.GetIP());
  tcp.Ping(1,"www.google.co.th");  
}
void loop() 
{
   if (gsm.available())
  {
    Serial.write(gsm.read());
  } 
  if (Serial.available())
  {
    char c = Serial.read();
    gsm.write(c);
    
  }
}


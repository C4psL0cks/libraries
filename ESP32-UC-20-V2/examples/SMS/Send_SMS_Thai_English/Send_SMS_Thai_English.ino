#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "call.h"
#include "sms.h"

#define RXPIN  16
#define TXPIN  17

CALL call;
SMS sms;
String phone_number = "0xxxxxxxxx";

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
  Serial.println("Send SMS");
  sms.DefaultSetting();
  sms.Start(phone_number);  
  sms.Sendln("hello");
  sms.Sendln("สวัสดี");
  sms.Sendln("วันนี้ท้องฟ้าแจ่มใส่");
  sms.Sendln("อากาศก็ดี้...ดี");
  sms.Stop();
  Serial.println("END"); 
 
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

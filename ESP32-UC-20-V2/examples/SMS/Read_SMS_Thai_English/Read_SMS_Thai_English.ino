#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "call.h"
#include "sms.h"

#define RXPIN  16
#define TXPIN  17

CALL call;
SMS sms;

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
 
}

void loop() 
{
 
  if(gsm.event_input())
  {
    if(gsm.event_type == EVENT_RING)
    {
      Serial.println("Ring");
    }
    if(gsm.event_type == EVENT_SMS)
    {
      Serial.println("NEW SMS");
      Serial.print("index = ");
      int index = sms.IndexNewSMS();
      Serial.println(index);
      String str_sms =sms.ReadSMS(index);
      Serial.println(sms.ConvertStrUnicodeToTIS620(str_sms));
      sms.DeleteSMS(index);
    }
  }  
}

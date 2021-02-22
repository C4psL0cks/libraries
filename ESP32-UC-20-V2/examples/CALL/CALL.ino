#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "call.h"

#define RXPIN  16
#define TXPIN  17

CALL call;

HardwareSerial mySerial(2);

String phone_number = "09xxxxxxxx";

void debug(String data){
  Serial.println(data);
}

void setup() {
  
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
  if(call.WaitRing())
  {
    Serial.print("Ring");
    Serial.println(call.CurrentCallsMe());
  }
  if (Serial.available())
  {
    char c = Serial.read();
    switch(c)
    {
      case 'c':
        call.Call(phone_number);
      break;
      case 'h':
        call.HangUp();  
      break;
      case 'a':
         call.Answer();  
      break;
    } 
  }
}

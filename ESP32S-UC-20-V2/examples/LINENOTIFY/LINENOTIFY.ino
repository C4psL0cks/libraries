#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "internet.h"
#include "LineNotify.h"

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define Button 2
#define Authen  "Your Token" 

#define RXPIN  16
#define TXPIN  17

INTERNET net;
LINE_NOTIFY line;

HardwareSerial mySerial(2);

void debug(String data)
{
  Serial.println(data);
}

void setup() 
{  
  pinMode(Button,INPUT);
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

  line.begin(Authen);
  sendLine("Hello My name is UC20");
}

void loop() 
{
   if(!digitalRead(Button))
   {
     sendLine("Push Button");
   }

}
void sendLine(String msg)
{
  int ret = line.connect();
  if(ret==1)
  { 
      Serial.println("Connected");
      line.sendMessage(msg);
  }
  else
     Serial.println("Not Connect");
  line.close();
}

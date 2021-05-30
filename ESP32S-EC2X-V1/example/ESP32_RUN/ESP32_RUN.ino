#include <HardwareSerial.h>
#include "ESP32_EC2X.h"
#include "internet.h"
#include "File.h"
#include "http.h"

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define RXPIN  27
#define TXPIN  26

INTERNET net;
UC_FILE file;
HTTP http;

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
  Serial.begin(115200);
  gsm.begin(&mySerial, 115200, RXPIN, TXPIN);
  gsm.Event_debug = debug;

  gsm.SetPowerKeyPin();
  Serial.println();
  Serial.println(F("Safe-box"));
  gsm.PowerOn();
  while (gsm.WaitReady()) {}

  delay(8000);
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
  delay(500);
  
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
#include <HardwareSerial.h>
#include "TEE_UC20.h"
#include "internet.h"
#include "http.h"
#include "File.h"
#include "ftp.h"
#include "call.h"
#include "sms.h"

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

#define RXPIN  16
#define TXPIN  17

INTERNET net;
UC_FILE file;
FTP ftp;
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

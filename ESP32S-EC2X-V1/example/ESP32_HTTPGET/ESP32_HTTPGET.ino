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

int water_sensor_1 = 0;
int water_sensor_2 = 0;
int motion_sensor = 0;
int car_sensor = 0;
float temperatureC = 0;
String mac_address = "";
String latitude = "-";
String longitude = "-";

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

  Serial.println(F("Start HTTP"));
  http.begin(1);
  Serial.println(F("Send HTTP GET"));
  http.url("http://103.233.193.144:5001/get?macaddress_uaa=" + mac_address
  + "&latitude=" + latitude
  + "&longitude=" + longitude
  + "&water_sensor_1=" + water_sensor_1
  + "&water_sensor_2=" + water_sensor_2
  + "&motion_sensor=" + motion_sensor
  + "&car_sensor=" + car_sensor
  + "&temperatureC=" + temperatureC);
  Serial.println(http.get());
  Serial.println();
  Serial.println(F("Clear data in RAM"));
  file.Delete(RAM, "*");
  Serial.println(F("Save HTTP Response To RAM"));
  http.SaveResponseToMemory(RAM, "web.hml");
  Serial.println(F("Read data in RAM"));
  read_file(RAM, "web.hml");
  Serial.println(F("Disconnect net"));
  net.DisConnect();
}

void read_file(String pattern, String file_name)
{
  file.DataOutput =  data_out;
  file.ReadFile(pattern, file_name);
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

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

String getValue(String data, char separator, int index)
{
  int found = 0;
  int strIndex[] = {0, -1};
  int maxIndex = data.length()-1;

  for(int i=0; i<=maxIndex && found<=index; i++){
    if(data.charAt(i)==separator || i==maxIndex){
        found++;
        strIndex[0] = strIndex[1]+1;
        strIndex[1] = (i == maxIndex) ? i+1 : i;
    }
  }

  return found>index ? data.substring(strIndex[0], strIndex[1]) : "";
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
  Serial.print(F("GPS Start"));
  delay(500);
}

void loop() 
{
  String GPS_DATA = gps.GetPosition();
  Serial.println(GPS_DATA);
  String latitude = getValue(GPS_DATA,',',1 );
  Serial.println("latitude : " + latitude);
  String longitude = getValue(GPS_DATA,',',2 );
  Serial.println("longitude : " + longitude);
  delay(500);   
}

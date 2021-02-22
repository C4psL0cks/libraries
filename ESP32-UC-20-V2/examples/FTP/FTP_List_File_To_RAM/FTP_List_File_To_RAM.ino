#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "internet.h"
#include "File.h"
#include "ftp.h"

//SIM TRUE  internet
#define APN "internet"
#define USER "true"
#define PASS "true"

//FTP Server
#define USERNAME  "Your username"
#define PASSWORD  "Your passwore"
#define HOST      "Your host name"
#define PORT      21
#define PATH      "/"

#define RXPIN  16
#define TXPIN  17

INTERNET net;
UC_FILE file;
FTP ftp;


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

  
  ftp.begin(1);
   Serial.println(F("Set FTP Username and Password"));
  ftp.SetUsernamePassword(USERNAME,PASSWORD);
  Serial.println(F("Set File Type = Binary"));
  ftp.SetFileType(BINARY);
  Serial.println(F("Set FTP = Passive Mode"));
  ftp.SetTransMode(PASSIVE);
  Serial.println(F("Set Timeout"));
  ftp.SetTimeout(90);
  Serial.print(F("Login Server"));
  Serial.println(ftp.LoginServer(HOST,PORT));
  Serial.println(F("Set Path"));
  Serial.println(ftp.SetPath(PATH));
 
  Serial.println(F("Clear data in RAM"));
  file.Delete(RAM,"*");
  Serial.println(F("Save List To RAM"));
  int ret=ftp.ListToMemory("*",RAM,"list.txt");
  Serial.println(ret);
  Serial.println(F("Read data in RAM"));
  read_file(RAM,"list.txt");
  Serial.println(ftp.Logout());
  Serial.println(F("Disconnect net"));
  net.DisConnect();
}
void read_file(String pattern,String file_name)
{
  file.DataOutput =  data_out;
  file.ReadFile(pattern,file_name);
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

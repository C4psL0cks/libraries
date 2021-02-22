#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "File.h"

#define RXPIN  16
#define TXPIN  17

UC_FILE file;

HardwareSerial mySerial(2);

String File_name = "test123.txt";

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
  gsm.PowerOn();
  while (gsm.WaitReady()) {}
  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());
  delay(3000);
  
  file.begin();
  get_space(UFS);
  file.Delete(UFS,File_name);
  //file.Delete(UFS,"*"); //Delete All File
  Serial.println(F("Write Data..."));
  write_file(File_name,F("Hello\r\nTest Write File To UFS"));
  Serial.println(F("Read Data..."));
  read_file(UFS,File_name);
  Serial.println();
  Serial.println("List File...");
  list_file();
  Serial.println();
}

void list_file()
{
  file.ListOutput = debug;
  file.List(UFS);
  Serial.println();
}
void get_space(String pattern)
{
  long space_size = file.GetSpace(pattern);
  Serial.print("Get All Space = ");
  Serial.println(space_size);
  Serial.print("Get Free Space = ");
  space_size = file.GetFreeSpace(pattern);
  Serial.println(space_size);
}

void read_file(String pattern,String file_name)
{
  file.DataOutput =  data_out;
  file.ReadFile(pattern,file_name);
}


void write_file(String file_name,String data)
{
  int handle = file.Open(UFS,file_name);
  if(handle!=-1)
  {
    if(file.BeginWrite(handle,data.length()))
    {
      file.Print(data);
      file.WaitFinish();
    }
  }
  file.Close(handle);
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

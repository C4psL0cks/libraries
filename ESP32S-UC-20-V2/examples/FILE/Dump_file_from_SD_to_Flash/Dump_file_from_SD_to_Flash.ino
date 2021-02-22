#include <HardwareSerial.h>
#include "ESP32S_UC20.h"
#include "File.h"
#include <SPI.h>
#include <SD.h>
#define SD_CS 5
#define RXPIN  16
#define TXPIN  17

INTERNET net;
UC_FILE file;
File myFile;

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
  gsm.PowerOn();
  while (gsm.WaitReady()) {}
  Serial.print(F("GetOperator --> "));
  Serial.println(gsm.GetOperator());
  Serial.print(F("SignalQuality --> "));
  Serial.println(gsm.SignalQuality());
  delay(3000);

  Serial.println(F("Ready"));
  if (!SD.begin(SD_CS)) 
  {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  file.Delete(UFS,"*"); 
  copy_file_from_SD_to_Flash("pic.jpg");
  file.ListOutput=debug;
  file.List(UFS);
  
}

void list_file()
{
  file.List(UFS);
  while(file.available())
  {
    Serial.println(file.ReadLine());
  }
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
void read_file(String file_name)
{
  int handle = file.Open(UFS,file_name);
  if(handle!=-1)
  {
    file.SeekAtStart(handle);
    int buf_size=100;
    char buf[buf_size];
    Serial.println();
    int size_ = file.Read(handle,buf_size,buf);
    while(size_!=-1)
    {
      for(int i=0;i<size_;i++)
      {
        Serial.write(buf[i]);
      }
      size_ = file.Read(handle,buf_size,buf);
    }
   }
   file.Close(handle);
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
void copy_file_from_SD_to_Flash(String filename)
{
  char fn[10] ;
  filename.toCharArray(fn,filename.length()+1);
  Serial.println(fn);
  myFile = SD.open(fn);
  Serial.print("\r\n File Size =");
  Serial.println(myFile.size());
  int handle;
  long copy_size=0;
  if (myFile) 
  {
     handle = file.Open(UFS,filename);
     if(handle!=-1)
     {
         file.BeginWrite(handle,myFile.size());
     }
      while (myFile.available()) 
     {
        char c = myFile.read();
        //Serial.print(c,HEX);
        Serial.println(copy_size);
        copy_size++;
        file.Write(c);
     }
     file.WaitFinish();
  }
  file.Close(handle);
  myFile.close();
  Serial.println("\r\n End Copy");
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

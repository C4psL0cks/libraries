/**
 * SHT1x Driver 3 PIN for long wire
 *
 * Read temperature and humidity values from an SHT1x-series (SHT10,
 * SHT11, SHT15) sensor.
 * 
 * With Transistor Drive to 3 pin -- AC-3BSHT15 at http://www.smicrothai.com/smicro_a.php
 * Active LOW 1 PIN SCK and 2 Pin Data Receive and Data Transmitter
 *
 *   Drive --- Arduino
 * - DATR  to  PIN D10
 * - DATT  to  PIN D11
 * - SCK   to  PIN D11
 *
 *
 * Copyright 2018 Prakit Junkhum <prakit340@hotmail.com>
 * www.prakitblog.com
 */

#include <TCS_SHT1x3P.h>

// Specify data and clock connections and instantiate SHT1x object
#define dataRPin  10
#define dataTPin  11
#define clockPin  12

TCS_SHT1x3P sht1x(dataRPin, dataTPin, clockPin);

void setup()
{
   Serial.begin(9600); // Open serial connection to report values to host
   Serial.println("Starting up");
}

void loop()
{
  float temp_c;
  float temp_f;
  float humidity;

  // Read values from the sensor
  temp_c = sht1x.readTemperatureC();
  temp_f = sht1x.readTemperatureF();
  humidity = sht1x.readHumidity();

  // Print the values to the serial port
  Serial.print("Temperature: ");
  Serial.print(temp_c, DEC);
  Serial.print("C / ");
  Serial.print(temp_f, DEC);
  Serial.print("F. Humidity: ");
  Serial.print(humidity);
  Serial.println("%");

  delay(2000);
}
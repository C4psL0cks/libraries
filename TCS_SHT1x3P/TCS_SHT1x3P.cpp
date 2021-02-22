/**
 * TCS_SHT1x3P Library
 *
 * Copyright 2009 Jonathan Oxer <jon@oxer.com.au> / <www.practicalarduino.com>
 * Based on previous work by:
 *    Maurice Ribble: <www.glacialwanderer.com/hobbyrobotics/?p=5>
 *    Wayne ?: <ragingreality.blogspot.com/2008/01/ardunio-and-sht15.html>
 *
 * Manages communication with SHT1x series (SHT10, SHT11, SHT15)
 * temperature / humidity sensors from Sensirion (www.sensirion.com).
 */

/*
 * Modify SHT1x to TCS_SHT1x3P Driver 3 PIN Library for long wire
 * With Transistor Drive to 3 pin -- AC-3BSHT15 at http://www.smicrothai.com/smicro_a.php
 * Convert to active LOW 1 PIN SCK and 2 Pin Data Receive and Data Transmitter
 * Copyright 2018 Prakit Junkhum <prakit340@hotmail.com> / <www.prakitblog.com>
 */


#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

#include "TCS_SHT1x3P.h"

TCS_SHT1x3P::TCS_SHT1x3P(int dataRPin, int dataTPin, int clockPin)
{
  _dataRPin = dataRPin;
  _dataTPin = dataTPin;
  _clockPin = clockPin;
}


/* ================  Public methods ================ */

/**
 * Reads the current temperature in degrees Celsius
 */
float TCS_SHT1x3P::readTemperatureC()
{
  int _val;                // Raw value returned from sensor
  float _temperature;      // Temperature derived from raw value

  // Conversion coefficients from SHT15 datasheet
  const float D1 = -40.0;  // for 14 Bit @ 5V
  const float D2 =   0.01; // for 14 Bit DEGC

  // Fetch raw value
  _val = readTemperatureRaw();

  // Convert raw value to degrees Celsius
  _temperature = (_val * D2) + D1;

  return (_temperature);
}

/**
 * Reads the current temperature in degrees Fahrenheit
 */
float TCS_SHT1x3P::readTemperatureF()
{
  int _val;                 // Raw value returned from sensor
  float _temperature;       // Temperature derived from raw value

  // Conversion coefficients from SHT15 datasheet
  const float D1 = -40.0;   // for 14 Bit @ 5V
  const float D2 =   0.018; // for 14 Bit DEGF

  // Fetch raw value
  _val = readTemperatureRaw();

  // Convert raw value to degrees Fahrenheit
  _temperature = (_val * D2) + D1;

  return (_temperature);
}

/**
 * Reads current temperature-corrected relative humidity
 */
float TCS_SHT1x3P::readHumidity()
{
  int _val;                    // Raw humidity value returned from sensor
  float _linearHumidity;       // Humidity with linear correction applied
  float _correctedHumidity;    // Temperature-corrected humidity
  float _temperature;          // Raw temperature value

  // Conversion coefficients from SHT15 datasheet
  const float C1 = -4.0;       // for 12 Bit
  const float C2 =  0.0405;    // for 12 Bit
  const float C3 = -0.0000028; // for 12 Bit
  const float T1 =  0.01;      // for 14 Bit @ 5V
  const float T2 =  0.00008;   // for 14 Bit @ 5V

  // Command to send to the SHT1x to request humidity
  int _gHumidCmd = 0b00000101;

  // Fetch the value from the sensor
  sendCommandSHT(_gHumidCmd, _dataRPin, _dataTPin, _clockPin);
  waitForResultSHT(_dataRPin, _dataTPin);
  _val = getData16SHT(_dataRPin, _dataTPin, _clockPin);
  skipCrcSHT(_dataTPin, _clockPin);

  // Apply linear conversion to raw value
  _linearHumidity = C1 + C2 * _val + C3 * _val * _val;

  // Get current temperature for humidity correction
  _temperature = readTemperatureC();

  // Correct humidity value for current temperature
  _correctedHumidity = (_temperature - 25.0 ) * (T1 + T2 * _val) + _linearHumidity;

  return (_correctedHumidity);
}


/* ================  Private methods ================ */

/**
 * Reads the current raw temperature value
 */
float TCS_SHT1x3P::readTemperatureRaw()
{
  int _val;

  // Command to send to the SHT1x to request Temperature
  int _gTempCmd  = 0b00000011;

  sendCommandSHT(_gTempCmd, _dataRPin, _dataTPin, _clockPin);
  waitForResultSHT(_dataRPin, _dataTPin);
  _val = getData16SHT(_dataRPin, _dataTPin, _clockPin);
  skipCrcSHT(_dataTPin, _clockPin);

  return (_val);
}

/**
 */
int TCS_SHT1x3P::shiftIn(int _dataRPin, int _clockPin, int _numBits)
{
  int ret = 0;
  int i;
  
  for (i=0; i<_numBits; ++i)
  {
     digitalWrite(_clockPin, LOW);
     delay(10);  // I don't know why I need this, but without it I don't get my 8 lsb of temp
     ret = ret*2 + digitalRead(_dataRPin);
     digitalWrite(_clockPin, HIGH);
  }

  return(ret);
}

/**
 */
void TCS_SHT1x3P::sendCommandSHT(int _command, int _dataRPin, int _dataTPin, int _clockPin)
{
  int ack;
  
  pinMode(_dataRPin, INPUT);
  pinMode(_dataTPin, OUTPUT);   digitalWrite(_dataTPin, HIGH);
  pinMode(_clockPin, OUTPUT);   digitalWrite(_clockPin, HIGH);

  // Transmission Start
  digitalWrite(_dataTPin, LOW);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_dataTPin, HIGH);
  digitalWrite(_clockPin, HIGH);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_dataTPin, LOW);
  digitalWrite(_clockPin, HIGH);

  // The command (3 msb are address and must be 000, and last 5 bits are command)
  uint8_t i;
    for(i = 0; i < 8; i++) 
    {
        digitalWrite(_dataTPin, !!!(_command & (1 << (7 - i))));
        digitalWrite(_clockPin, LOW);
        digitalWrite(_clockPin, HIGH);
    }

  // Verify we get the correct ack
  digitalWrite(_clockPin, LOW);

  digitalWrite(_dataTPin, LOW);

  ack = digitalRead(_dataRPin);
  if (ack != LOW) {
    //Serial.println("Ack Error 0");
  }
  digitalWrite(_clockPin, HIGH);
  ack = digitalRead(_dataRPin);
  if (ack != HIGH) {
    //Serial.println("Ack Error 1");
  }
}

/**
 */
void TCS_SHT1x3P::waitForResultSHT(int _dataRPin, int _dataTPin)
{
  int i;
  int ack;

  pinMode(_dataRPin, INPUT);
  pinMode(_dataTPin, OUTPUT);  digitalWrite(_dataTPin, HIGH);
  digitalWrite(_dataTPin, LOW);

  for(i= 0; i < 100; ++i)
  {
    delay(10);
    ack = digitalRead(_dataRPin);

    if (ack == LOW) {
      break;
    }
  }

  if (ack == HIGH) {
    //Serial.println("Ack Error 2"); // Can't do serial stuff here, need another way of reporting errors
  }
}

/**
 */
int TCS_SHT1x3P::getData16SHT(int _dataRPin, int _dataTPin, int _clockPin)
{
  int val;

  pinMode(_dataRPin, INPUT);
  pinMode(_clockPin, OUTPUT);   digitalWrite(_clockPin, HIGH);
  pinMode(_dataTPin, OUTPUT);   digitalWrite(_dataTPin, HIGH);

  // Get the most significant bits (MSB)
  digitalWrite(_dataTPin, LOW);
  val = shiftIn(_dataRPin, _clockPin, 8);
  val *= 256;

  // Send the required ack
  digitalWrite(_dataTPin, LOW);
  digitalWrite(_dataTPin, HIGH);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_clockPin, HIGH);

  // Get the least significant bits (LSB)
  digitalWrite(_dataTPin, LOW);
  val |= shiftIn(_dataRPin, _clockPin, 8);

  return val;
}

/**
 */
void TCS_SHT1x3P::skipCrcSHT(int _dataTPin, int _clockPin)
{
  pinMode(_dataTPin, OUTPUT);  digitalWrite(_dataTPin, HIGH);
  pinMode(_clockPin, OUTPUT);  digitalWrite(_clockPin, HIGH);

  // Skip acknowledge to end trans (no CRC)
  digitalWrite(_dataTPin, LOW);
  digitalWrite(_clockPin, LOW);
  digitalWrite(_clockPin, HIGH);
}

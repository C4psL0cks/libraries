/**
 * SHTTCS_SHT1x3P1x Library
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


#ifndef TCS_SHT1x3P_h
#define TCS_SHT1x3P_h

#if (ARDUINO >= 100)
#include <Arduino.h>
#else
#include <WProgram.h>
#endif

class TCS_SHT1x3P
{
  public:
    TCS_SHT1x3P(int dataRPin, int dataTPin, int clockPin);
    float readHumidity();
    float readTemperatureC();
    float readTemperatureF();
  private:
    int _dataRPin;
    int _dataTPin;
    int _clockPin;
    int _numBits;
    float readTemperatureRaw();
    int shiftIn(int _dataRPin, int _clockPin, int _numBits);
    void sendCommandSHT(int _command, int _dataRPin, int _dataTPin, int _clockPin);
    void waitForResultSHT(int _dataRPin, int _dataTPin);
    int getData16SHT(int _dataRPin, int _dataTPin, int _clockPin);
    void skipCrcSHT(int _dataTPin, int _clockPin);
};

#endif

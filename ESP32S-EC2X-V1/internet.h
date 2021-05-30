#ifndef INTERNET_h
#define INTERNET_h

#include "ESP32_EC2X.h"

class INTERNET
{
public:
	INTERNET();
	bool Configure(String apn, String user, String password);
	bool Connect();
	bool DisConnect();
	String GetIP();
};

#endif
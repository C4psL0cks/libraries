#ifndef HTTP_h
#define HTTP_h

#include "ESP32_EC2X.h"
#include "ssl.h"
#define CONTEX 1

class HTTP
{
public:
	HTTP();
	bool begin(unsigned char context_ID);
	int get();
	int post(String data);
	int post();
	void ReadData();
	bool SaveResponseToMemory(String pattern, String Filename);
	bool url(String url);
};

#endif

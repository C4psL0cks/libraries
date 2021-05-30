#include "ESP32_UC2X.h"
#define START_PIN 16

UC2X gsm;
unsigned long previousMillis_timeout = 0; 


void event_null(String data){}
UC2X::UC2X()
{
	Event_debug =  event_null;
}

void UC2X:: begin(HardwareSerial *serial,long baud,int rx,int tx)
{
	serial->begin(baud, SERIAL_8N1, rx, tx); 
	_Serial = serial;
}

void UC2X:: debug (String data)
{
	(*Event_debug)(data);
}

void UC2X::SetPowerKeyPin()
{
	pinMode(START_PIN, OUTPUT);
	digitalWrite(START_PIN, LOW);
	delay(200);
	digitalWrite(START_PIN, HIGH);
	delay(200);
}

bool UC2X::PowerOn()
{
	while(1)
	{
		if(_Serial->available())
		{
			String req = _Serial->readStringUntil('\n');	
			// debug(req);
			if(req.indexOf(F("RDY")) != -1)
			{
				start_time_out();
				debug(F("Power ON"));

				return(true);
			}			
		}
	}
}

bool UC2X::WaitReady()
{
	_Serial->println(F("AT"));
	String req = _Serial->readStringUntil('\n');	
	// debug(req);
	if(req.indexOf(F("OK")) != -1)
	{
		debug(F("\r\nUC2X Ready..."));
		debug(F("\r\nClose Echo"));
		_Serial->println(F("ATE0"));
		wait_ok(1000);
		
		return(false);
	}
	else
	{
		return(true);
	}
	return(true);
}

String UC2X::GetOperator()
{
	_Serial->println(F("AT+COPS?"));
	while(!_Serial->available())
	{}
	start_time_out();
	while(1)
	{
		String req = _Serial->readStringUntil('\n');	 
		// debug("\r\n"+req);
		if(req.indexOf(F("+COPS:")) != -1)
		{
			return(req.substring(req.indexOf(F("\""))));
		}
		if(time_out(3000))
		{
			return(F(""));
		}
		
	}
	return(F(""));
}
unsigned char UC2X::SignalQuality()
{
	unsigned char ret = 101;
	_Serial->println(F("AT+CSQ"));

	while(!_Serial->available())
	{}
	start_time_out();
	while(1)
	{
		String req = _Serial->readStringUntil('\n');
		// debug(req);
		if(req.indexOf(F("+CSQ:")) != -1)
		{
			return(req.substring(req.indexOf(F(" "))+1,req.indexOf(F(","))).toInt());
		}
		if(time_out(3000))
		{
			return(ret);
		}
	}
	return(ret);
}
void UC2X::start_time_out()
{
	previousMillis_timeout = millis();
}
bool UC2X::time_out(long timeout_interval)
{
	unsigned long currentMillis = millis();
	if(currentMillis - previousMillis_timeout >= timeout_interval) 
	{
		previousMillis_timeout = currentMillis;
		return(true);
	}
	return(false);
}
bool UC2X:: wait_ok(long time)
{
	return wait_ok_(time,true);
}
bool UC2X:: wait_ok_ndb(long time)
{
	return wait_ok_(time,false);
}
	
bool UC2X:: wait_ok_(long time,bool ack)
{
	unsigned long previousMillis = millis(); 
	while(1)
	{
		String req = _Serial->readStringUntil('\n');
		if(req.indexOf(F("OK")) != -1)
		{
			if(ack)
			debug(F("OK"));
			return(1);
		}
		if(req.indexOf(F("ERROR")) != -1)
		{
			if(ack)
			debug(F("Error"));
			return(0);
		}
		//debug(req);	
		unsigned long currentMillis = millis();
		if(currentMillis - previousMillis >= time) 
		{
			previousMillis = currentMillis;
			if(ack)
			debug(F("Error"));
			return(0);
		}			

	}
	
}

unsigned char UC2X:: event_input()
{
	while(_Serial->available())
	{
		String req = _Serial->readStringUntil('\n');	
	   
	    if(req.indexOf(F("RING")) != -1)
		{
			event_type = EVENT_RING;
			return(EVENT_RING);//EVENT_RING
		}
		else if(req.indexOf(F("+CMTI: \"ME\"")) != -1)
		{
			event_type = EVENT_SMS;
			char index = req.indexOf(F(","));
			index_new_SMS =  req.substring(index+1).toInt();
			
			return(EVENT_SMS);//EVENT_SMS
		}
	}
	event_type = EVENT_NULL;
	return(EVENT_NULL);//EVENT_NULL
}


/* Code Serial */
String UC2X:: readStringUntil(char data)
{
	 return _Serial->readStringUntil(data);
}
size_t UC2X:: print(String data)
{
     return _Serial->print(data);
}
size_t UC2X:: println(String data)
{
     return _Serial->println(data);
}

void UC2X:: print(unsigned char data,int type)
{
	_Serial->print(data,type);
}


void UC2X:: print(int data,int type)
{
	_Serial->print(data,type);
}
void UC2X:: print(unsigned int data,int type)
{
	_Serial->print(data,type);
}
void UC2X:: print(long data,int type)
{
	_Serial->print(data,type);
}

size_t UC2X:: print(String data,int type)
{
   int i=0;
   while(data[i])
   {
		_Serial->print(data[i],type);
		i++;   
   }
   return(i-1);
   
   // Serial.print(data,type);
	// return _Serial->print(data,type);
}
size_t UC2X:: println(String data,int type)
{
	int i=0;
	while(data[i])
	{
		_Serial->print(data[i],type);
		i++;   
	}
	_Serial->println("");
    return(i+1);


	//return _Serial->println(data,type);
}

int UC2X:: peek()
{
     return _Serial->peek();
}
size_t UC2X:: write(uint8_t byte)
{
     return _Serial->write(byte);
}
int UC2X:: read()
{
     return _Serial->read();
}
int UC2X:: available()
{
     return _Serial->available();
}
void UC2X:: flush()
{
     _Serial->flush();
}

void UC2X :: my_flush()
{
	while(gsm.available())
	{
		gsm.read();
	}
}

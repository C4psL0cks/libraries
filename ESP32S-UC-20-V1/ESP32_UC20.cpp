#include "ESP32_UC20.h"
#define START_PIN 16

UC20 gsm;
unsigned long previousMillis_timeout = 0; 


void event_null(String data){}
UC20::UC20()
{
	Event_debug =  event_null;
}

void UC20:: begin(HardwareSerial *serial,long baud,int rx,int tx)
{
	serial->begin(baud, SERIAL_8N1, rx, tx); //3,1 rx-tx
	_Serial = serial;
}

void UC20:: debug (String data)
{
	(*Event_debug)(data);
}

void UC20::SetPowerKeyPin()
{
	pinMode(START_PIN, OUTPUT);
	digitalWrite(START_PIN, LOW);
	delay(500);
	digitalWrite(START_PIN, HIGH);
	delay(500);
}

bool UC20::PowerOn()
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

bool UC20::WaitReady()
{
	_Serial->println(F("AT"));
	String req = _Serial->readStringUntil('\n');	
	// debug(req);
	if(req.indexOf(F("OK")) != -1)
	{
		debug(F("\r\nUC20 Ready..."));
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

String UC20::GetOperator()
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
unsigned char UC20::SignalQuality()
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
void UC20::start_time_out()
{
	previousMillis_timeout = millis();
}
bool UC20::time_out(long timeout_interval)
{
	unsigned long currentMillis = millis();
	if(currentMillis - previousMillis_timeout >= timeout_interval) 
	{
		previousMillis_timeout = currentMillis;
		return(true);
	}
	return(false);
}
bool UC20:: wait_ok(long time)
{
	return wait_ok_(time,true);
}
bool UC20:: wait_ok_ndb(long time)
{
	return wait_ok_(time,false);
}
	
bool UC20:: wait_ok_(long time,bool ack)
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

unsigned char UC20:: event_input()
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
String UC20:: readStringUntil(char data)
{
	 return _Serial->readStringUntil(data);
}
size_t UC20:: print(String data)
{
     return _Serial->print(data);
}
size_t UC20:: println(String data)
{
     return _Serial->println(data);
}

void UC20:: print(unsigned char data,int type)
{
	_Serial->print(data,type);
}


void UC20:: print(int data,int type)
{
	_Serial->print(data,type);
}
void UC20:: print(unsigned int data,int type)
{
	_Serial->print(data,type);
}
void UC20:: print(long data,int type)
{
	_Serial->print(data,type);
}

size_t UC20:: print(String data,int type)
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
size_t UC20:: println(String data,int type)
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

int UC20:: peek()
{
     return _Serial->peek();
}
size_t UC20:: write(uint8_t byte)
{
     return _Serial->write(byte);
}
int UC20:: read()
{
     return _Serial->read();
}
int UC20:: available()
{
     return _Serial->available();
}
void UC20:: flush()
{
     _Serial->flush();
}

void UC20 :: my_flush()
{
	while(gsm.available())
	{
		gsm.read();
	}
}

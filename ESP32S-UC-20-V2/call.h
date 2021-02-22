#ifndef UC20_CALL
#define UC20_CALL
#include "ESP32S_UC20.h"

class CALL 
{
public:
	CALL();
	unsigned char Call(String call_number);
	bool HangUp();
	bool Answer();
	bool DisconnectExisting();
	String CurrentCallsMe();
	bool WaitRing();
};
#endif
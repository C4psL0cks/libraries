#ifndef EC2X_CALL
#define EC2X_CALL
#include "ESP32_EC2X.h"

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
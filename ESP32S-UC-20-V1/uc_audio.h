#ifndef UCAUDIO_h
#define UCAUDIO

#include "ESP32_UC20.h"
#define UPLINK false
#define DOWNLINK true

class UCxAUDIO
{
public:
	UCxAUDIO();
	bool StartRecord(bool ctrl, String pattern, String fn, unsigned char format, bool dlink);
	bool StartRecords(bool ctrl, String fn, unsigned char format, bool dlink);
	bool StartWAVRecord(String pattern, String fn, bool dlink); // StartWAVRecord("*","run",false);
	bool StartWAVRecords(String fn, bool dlink);				// StartWAVRecords("run",false);
	bool StopRecord();											// StopRecord();
	bool PlayWAV(bool state, String pattern, String fn, bool repeat, bool ulmute, bool dlmute);
	bool PlayWAV(String pattern, String fn, bool ulmute, bool dlmute);
	bool StopWAV();
	void taskAudio();
	bool StatusPlay();
	bool PlayMP3(String pattern, String fn, bool repeat, unsigned char volumn);
	bool StopMP3();
	bool TextToSpeech(String text);

private:
	bool readinput();
};

#endif
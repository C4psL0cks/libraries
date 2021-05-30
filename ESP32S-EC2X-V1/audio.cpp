#include "audio.h"

bool flag_status_play = false;

ECXAUDIO::ECXAUDIO()
{
}
bool ECXAUDIO::StartRecord(bool ctrl, String pattern, String fn, unsigned char format, bool dlink)
{
	//AT+QAUDRD=1,"A.amr",3
	String cmd = "AT+QAUDRD=";
	if (ctrl)
	{
		cmd += "1,\"";
		cmd += pattern + ":" + fn + "\"";
		if ((format == 3) || (format == 13) || (format == 14) || (format == 5))
		{
			cmd += "," + String(format);
		}
		else
		{
			Serial.println(cmd);
			gsm.println(cmd);
			return (readinput());
		}
		cmd += "," + String(dlink);
		gsm.println(cmd);
		return (readinput());
	}
	else
	{
		cmd += "0";
		gsm.println(cmd);
		return (readinput());
	}
}
bool ECXAUDIO::StartRecords(bool ctrl, String fn, unsigned char format, bool dlink)
{
	//AT+QAUDRD=1,"A.amr",3
	String cmd = "AT+QAUDRD=";
	if (ctrl)
	{
		cmd += "1,\"";
		cmd += "" + fn + "\"";
		if ((format == 3) || (format == 13) || (format == 14) || (format == 5))
		{
			cmd += "," + String(format);
		}
		else
		{
			Serial.println(cmd);
			gsm.println(cmd);
			return (readinput());
		}
		cmd += "," + String(dlink);
		gsm.println(cmd);
		return (readinput());
	}
	else
	{
		cmd += "0";
		gsm.println(cmd);
		return (readinput());
	}
}
bool ECXAUDIO::readinput()
{
	const long interval = 3000;
	unsigned long previousMillis = millis();
	unsigned long currentMillis;
	currentMillis = millis();
	while (1)
	{
		if (gsm.available())
		{
			String req = gsm.readStringUntil('\n');
			//gsm.debug(req);
			if (req.indexOf(F("OK")) != -1)
			{
				gsm.debug(req);
				return (true);
			}
			else
			{
				gsm.debug(req);
				return (false);
			}
		}
		if (currentMillis - previousMillis >= interval)
		{
			gsm.debug(F("Response Timeout"));
			return (false);
			previousMillis = currentMillis;
		}
	}
}

bool ECXAUDIO::StartWAVRecords(String fn, bool dlink)
{
	return (StartRecords(true, fn, 13, dlink));
}
bool ECXAUDIO::StopRecord()
{
	return (StartRecord(false, "", "", 0, false));
}
bool ECXAUDIO::PlayWAV(bool state, String pattern, String fn, bool repeat, bool ulmute, bool dlmute)
{
	//AT+QPSND=1,“A.wav”,0
	flag_status_play = true;
	String cmd = "AT+QPSND=";
	if (state)
	{
		cmd += String(state) + ",\"" + pattern + ":" + fn + ".wav" + "\"," + String(repeat) + "," + String(ulmute) + "," + String(dlmute);
	}
	else
		cmd += String(state);
	gsm.println(cmd);
	//Serial.println(cmd);
	return (readinput());
}

bool ECXAUDIO::PlayWAV(String pattern, String fn, bool ulmute, bool dlmute)
{
	return (PlayWAV(true, pattern, fn, false, ulmute, dlmute));
}

void ECXAUDIO::taskAudio()
{
	if (gsm.available())
	{
		String req = gsm.readStringUntil('\n');
		if (req.indexOf(F("QPSND: 0")) != -1)
		{
			gsm.debug(F("End Sound"));
			flag_status_play = false;
		}
		if (req.indexOf(F("QAUDPLAY: 0")) != -1)
		{
			gsm.debug(F("End Sound"));
			flag_status_play = false;
		}
		if (req.indexOf(F("QTTS: 0")) != -1)
		{
			gsm.debug(F("End Sound"));
			flag_status_play = false;
		}
	}
}
bool ECXAUDIO::StopWAV()
{
	return (PlayWAV(false, "", "", false, false, false));
}

bool ECXAUDIO::StatusPlay()
{
	return (flag_status_play);
}

bool ECXAUDIO::PlayMP3(String pattern, String fn, bool repeat, unsigned char volumn)
{
	flag_status_play = true;
	String cmd = "AT+QAUDPLAY=\"" + pattern + ":" + fn + "\"," + String(repeat) + "," + String(volumn);
	gsm.println(cmd);
	return (readinput());
}
bool ECXAUDIO::StopMP3()
{
	String cmd = "AT+QAUDSTOP";
	gsm.println(cmd);
	return (readinput());
}
bool ECXAUDIO::TextToSpeech(String text)
{
	String cmd = "AT+QTTS=2,\"" + text + "\"";
	gsm.println(cmd);
	return (readinput());
}

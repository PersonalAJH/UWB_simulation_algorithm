#pragma once

#include "serialport.h"

#define RETURN_SUCCESS 1
#define RETURN_FAIL 0


class CSerialComm
{
public:
	CSerialComm();
	~CSerialComm();

	CSerialPort	serial;
	int		connect(const char* _portNum);
	int		sendCommand(char pos);
	int		sendCommand(char* data);
	void		disconnect();
	int		readCommand(unsigned char* data);
	int		readCommand(char* data);
	int		readCommand3f(char* data1, char* data2, char* data3);
};

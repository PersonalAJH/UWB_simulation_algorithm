#include "pch.h"
#include "SerialComm.h"

CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}


int CSerialComm::connect(const char* portNum)
{
	//CString str_portNum= portNum;
	CString str_portNum;
	str_portNum.Format(_T("%S"), portNum);
	if (!serial.OpenPort(str_portNum)) //포트를 오픈하고 오픈에 실패하였으면 fail을 반환한다.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_9600, 8, FALSE, NOPARITY, ONESTOPBIT); //포트 기본값을 설정한다.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout값 설정

	return RETURN_SUCCESS;
}


int CSerialComm::sendCommand(char pos) //데이터를 전송하는 함수
{
	if (serial.WriteByte(pos)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}

int	CSerialComm::sendCommand(char* data)
{
	if (serial.WriteData(data)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}

int	CSerialComm::readCommand(unsigned char* data)
{
	int i = 0;
	while (1)
	{
		BYTE dt;
		if (serial.ReadByte(dt)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		{
			data[i] = dt;
			i++;
			//printf("%d\n", i);
			if (dt == 't')
				break;
		}
	}
	return true;
}

int	CSerialComm::readCommand(char* data)
{
	bool res=false;
	int i = 0;
	while (1)
	{
		BYTE dt;
		if (serial.ReadByte(dt)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		{
			data[i] = dt;
			i++;
			//printf("%d\n", i);
			if (dt == '\n')
			{
				res = true;
				break;
			}
		}
	}
	return res;
}


int	CSerialComm::readCommand3f(char* data1,char* data2, char* data3)
{
	int i = 0;
	int j = 0;
	int k = 0;
	BYTE dt;

	while (1)
	{

		if (serial.ReadByte(dt)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		{
			data1[i] = dt;
			i++;
			//printf("%d\n", i);
			if (dt == 't')
				break;
		}
	}

	while (1)
	{

		if (serial.ReadByte(dt)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		{
			data2[j] = dt;
			j++;
			//printf("%d\n", i);
			if (dt == 't')
				break;
		}
	}

	while (1)
	{

		if (serial.ReadByte(dt)) //WriteByte()를 통해 데이터 전송에 성공하면 SUCCESS, 실패하면 FAIL을 반환한다.
		{
			data3[k] = dt;
			k++;
			//printf("%d\n", i);
			if (dt == 't')
				break;
		}
	}
	return true;
}

void CSerialComm::disconnect() //포트를 다 쓰고 난뒤 닫는 함수
{
	serial.ClosePort();
}
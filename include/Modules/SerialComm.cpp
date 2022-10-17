#include "pch.h"
#include "SerialComm.h"

CSerialComm::CSerialComm() {}
CSerialComm::~CSerialComm() {}


int CSerialComm::connect(const char* portNum)
{
	//CString str_portNum= portNum;
	CString str_portNum;
	str_portNum.Format(_T("%S"), portNum);
	if (!serial.OpenPort(str_portNum)) //��Ʈ�� �����ϰ� ���¿� �����Ͽ����� fail�� ��ȯ�Ѵ�.
		return RETURN_FAIL;

	serial.ConfigurePort(CBR_9600, 8, FALSE, NOPARITY, ONESTOPBIT); //��Ʈ �⺻���� �����Ѵ�.
	serial.SetCommunicationTimeouts(0, 0, 0, 0, 0); //Timeout�� ����

	return RETURN_SUCCESS;
}


int CSerialComm::sendCommand(char pos) //�����͸� �����ϴ� �Լ�
{
	if (serial.WriteByte(pos)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
		return RETURN_SUCCESS;
	else
		return RETURN_FAIL;
}

int	CSerialComm::sendCommand(char* data)
{
	if (serial.WriteData(data)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
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
		if (serial.ReadByte(dt)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
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
		if (serial.ReadByte(dt)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
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

		if (serial.ReadByte(dt)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
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

		if (serial.ReadByte(dt)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
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

		if (serial.ReadByte(dt)) //WriteByte()�� ���� ������ ���ۿ� �����ϸ� SUCCESS, �����ϸ� FAIL�� ��ȯ�Ѵ�.
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

void CSerialComm::disconnect() //��Ʈ�� �� ���� ���� �ݴ� �Լ�
{
	serial.ClosePort();
}
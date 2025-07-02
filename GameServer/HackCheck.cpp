#include "stdafx.h"
#include "HackCheck.h"
#include "ServerInfo.h"

BYTE EncDecKey1;
BYTE EncDecKey2;
BYTE MHPEncDecKey1;
BYTE MHPEncDecKey2;

void DecryptData(BYTE* lpMsg,int size) // OK
{
	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		MHPDecryptData(lpMsg,size);
	}

	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^EncDecKey1)-(EncDecKey2*EncDecKey1);
	}
}

void EncryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+(EncDecKey2*EncDecKey1))^EncDecKey1;
	}

	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		MHPEncryptData(lpMsg,size);
	}
}

void MHPDecryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]^MHPEncDecKey1)-(MHPEncDecKey2*MHPEncDecKey1);
	}
}

void MHPEncryptData(BYTE* lpMsg,int size) // OK
{
	for(int n=0;n < size;n++)
	{
		lpMsg[n] = (lpMsg[n]+(MHPEncDecKey2*MHPEncDecKey1))^MHPEncDecKey1;
	}
}

void InitHackCheck() // OK
{
	WORD EncDecKey = 0;

	wsprintf(CustomerName, "MuOnline");

	for(int n=0;n < sizeof(CustomerName);n++)
	{
		EncDecKey += (BYTE)(CustomerName[n]^gServerInfo.m_ServerSerial[(n%sizeof(gServerInfo.m_ServerSerial))]);
		EncDecKey ^= (BYTE)(CustomerName[n]-gServerInfo.m_ServerSerial[(n%sizeof(gServerInfo.m_ServerSerial))]);
	}

	EncDecKey1 = (BYTE)0xB0;
	EncDecKey2 = (BYTE)0xF8;

	EncDecKey1 += LOBYTE(EncDecKey);
	EncDecKey2 += HIBYTE(EncDecKey);

	MHPEncDecKey1 = gServerInfo.m_ServerEncDecKey1;
	MHPEncDecKey2 = gServerInfo.m_ServerEncDecKey2;

	if(MHPEncDecKey1 != 0 || MHPEncDecKey2 != 0)
	{
		WORD MHPEncDecKey = 0;

		for(int n=0;n < sizeof(CustomerName);n++)
		{
			MHPEncDecKey += CustomerName[n];
		}

		MHPEncDecKey1 += LOBYTE(MHPEncDecKey);
		MHPEncDecKey2 += HIBYTE(MHPEncDecKey);
	}
}

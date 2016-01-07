
// ES_Network.cpp: implementation of the ES_Network class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Netprobe1.h"
#include "ES_Network.h"


//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
//int const ES_Network::MAX_NAME=100;

///////////////////////////////////////////
//		Constructor
///////////////////////////////////////////
ES_Network::ES_Network()
{
	WSADATA wsaData;
	WORD wVersionRequested = MAKEWORD(2, 0);
	int err = WSAStartup(wVersionRequested, &wsaData);
	if (err) {
		AfxMessageBox("Error: cannot find a useable winsock.dll\r\nProgram exit");
		exit(1);
	}
	if (wVersionRequested != wsaData.wVersion) {
		AfxMessageBox("Error: cannot find a useable winsock.dll\r\nProgram exit");
		WSACleanup();
		exit(1);
	}
	myhostent = NULL;
	hSocket = NULL;
}

///////////////////////////////////////////
//		Destructor
///////////////////////////////////////////
ES_Network::~ES_Network()
{
	WSACleanup();
}


///////////////////////////////////////////
//	Get localhost name
///////////////////////////////////////////
int ES_Network::GetLocalHostName(char* hostname)
{
	return gethostname(hostname, MAX_NAME);
}

///////////////////////////////////////////
//		get host address by name
///////////////////////////////////////////
char* ES_Network::GetHostAddrByName(char* hostname)
{
	if((inet_addr(hostname)) == INADDR_NONE)
	{
		myhostent = gethostbyname(hostname);
		if (myhostent)
			return inet_ntoa(*((struct in_addr*)myhostent->h_addr_list[0]));
		else
			return NULL;
	}
	else
		return hostname;
}

///////////////////////////////////////////
//		get hostname by address
///////////////////////////////////////////
char* ES_Network::GetHostNameByAddr(const char* addr) 
{
	if ((inet_addr(addr)) != INADDR_NONE)
	{
		unsigned long input = inet_addr(addr);
		myhostent=gethostbyaddr((char *)(&input), sizeof(input), AF_INET);
	}

	if (myhostent)
		return myhostent->h_name;
	else
		return NULL;
}

///////////////////////////////////////////
//		get all host addresses
///////////////////////////////////////////
char* ES_Network::GetNthHostAddr(int i)
{
	if (myhostent)
		return ((myhostent->h_addr_list[i]) ? inet_ntoa(*((struct in_addr*)myhostent->h_addr_list[i])) : NULL);
	else
		return NULL;
}

///////////////////////////////////////////
//		get all host names
///////////////////////////////////////////
char* ES_Network::GetNthHostName(int i)
{
	return ((myhostent) ? myhostent->h_aliases[i] : NULL);
}

///////////////////////////////////////////
//		check whether the host is valid
///////////////////////////////////////////	
int ES_Network::ValidateHost(char* hostname)
{	
	return ((GetHostAddrByName(hostname)) ? 1 : 0);
}

///////////////////////////////////////////////////////////////////
//	 Send: virtual function, which can be inherited by subclass
///////////////////////////////////////////////////////////////////
int ES_Network::Send(const char* pdata, int datalen)
{
	return 0;
}

////////////////////////////////////////////////////////////////////
//	Receive: virtual function, which can be inherited by subclass
////////////////////////////////////////////////////////////////////
int ES_Network::Recv(int datalen)
{
	return 0;
}

///////////////////////////////////////////
//		get socket handle
///////////////////////////////////////////
SOCKET ES_Network::GetHandle()
{
	return hSocket;
}

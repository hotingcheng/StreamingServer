// ES_TCPClient.cpp: implementation of the ES_TCPClient class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Netprobe1.h"
#include "ES_TCPClient.h"

///////////////////////////////////////////
//	Consturctor/ Destructor
///////////////////////////////////////////
ES_TCPClient::ES_TCPClient()
{
}

ES_TCPClient::~ES_TCPClient()
{

}

///////////////////////////////////////////
//	TCP client connect
///////////////////////////////////////////
int ES_TCPClient::Connect(int port, char* hostname)
{
	hSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (hSocket == INVALID_SOCKET)
	{
//		AfxMessageBox("Error in creating socket for TCP,terminated.");
		return -1;
	}	
	struct sockaddr_in addr;
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(GetHostAddrByName(hostname));
	
	if (connect(hSocket, (struct sockaddr *) &addr, sizeof(addr)) < 0)
	{
//		char temp[20];
//		sprintf(temp, "%d", port);
//		AfxMessageBox("Unable to connect peer ["+(CString) hostname+":"+(CString)temp+"], terminated.");
//		return -1;
	}
	return 0;
}


///////////////////////////////////////////
//	TCP client disconnect
///////////////////////////////////////////
void ES_TCPClient::Disconnect()
{
	Shutdown(hSocket);
}

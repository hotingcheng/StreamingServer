// ES_TCP.cpp: implementation of the ES_TCP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Netprobe1.h"
#include "ES_TCP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ES_TCP::ES_TCP()
{
}

ES_TCP::~ES_TCP()
{
}

///////////////////////////////////////////
//		TCP Send (virtual)
///////////////////////////////////////////
int ES_TCP::Send(const char *pdata,int datalen)
{
	int lastsend = 0, senddata = 0;
	const char* p = pdata;

	do 
	{	
		p += lastsend*sizeof(char);
		lastsend = send(hSocket, p, datalen-senddata, 0);
		if (lastsend == SOCKET_ERROR)
			return -1;

		senddata+=lastsend;
	} while (senddata < datalen);

	return 0;
}

///////////////////////////////////////////
//		TCP receive (virtual)
///////////////////////////////////////////
int ES_TCP::Recv(int datalen,char *buffer)
{
	int datarecv;

	datarecv = recv(hSocket, buffer, datalen, 0);

	if (datarecv == 0 || WSAGetLastError() == 10054)
		return 0;
	else if (datarecv == SOCKET_ERROR)
		return -1;
	else
		return datarecv;

	return 0;
}

///////////////////////////////////////////
//		TCP shut down (virtual)
///////////////////////////////////////////
int ES_TCP::Shutdown(SOCKET sock)
{
	if (!sock) return 0;

	if (shutdown(sock, 2) == SOCKET_ERROR)
	{
		if (WSAGetLastError() != 10054 && WSAGetLastError() != 10057)
			AfxMessageBox("Error in shuting down the socket for TCP, terminated.");
	}
	if (closesocket(sock) == SOCKET_ERROR)
	{
		AfxMessageBox("Error in closing the socket for TCP, terminated.");
		return -1;
	}
	return 0;
}

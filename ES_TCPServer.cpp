// ES_TCPServer.cpp: implementation of the ES_TCPServer class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Netprobe1.h"
#include "ES_TCPServer.h"
#include "ES_Connection.h"

///////////////////////////////////////////
//		TCP server constructor
///////////////////////////////////////////
ES_TCPServer::ES_TCPServer()
{
	buffer = NULL;
	cSocket = NULL;
	packetrecv = 0;
	lNumRecv = 0;
	currseqno = 0;
	connected = false;
}

///////////////////////////////////////////
//		TCP server destructor
///////////////////////////////////////////
ES_TCPServer::~ES_TCPServer()
{
	Shutdown(cSocket);
	Shutdown(hSocket);
}

///////////////////////////////////////////
//		TCP Server : Close File
///////////////////////////////////////////
void ES_TCPServer::CloseFile()
{
	f1->Close();
	delete f1;
}

///////////////////////////////////////////
//		TCP Receive
///////////////////////////////////////////
int ES_TCPServer::Recv(int datalen, bool file)
{
	int received=0;
	
	if (!buffer)
	{
		buffer = new char[datalen];
		memset(buffer, 0, datalen);
	}

	received = ES_TCP::Recv((datalen-lNumRecv), buffer+lNumRecv*sizeof(char));

	if (lNumRecv == 0 && received > 8)
	{
		if (received == datalen)
		{
			char* p = buffer;
			p += sizeof(unsigned long);
			if (datalen != *((int *)p))		// if size mismatches, return error value
				return -2;		// -2 , defined for "Size Mismatch

			if (*((int *)p) == 0)	
				packetrecv=1;
		}		
	}

	if ((file)&& (received>0))
	{
		if (lNumRecv==0)
			f1->Write( buffer+sizeof(unsigned long)+sizeof(int), received-sizeof(unsigned long)-sizeof(int) );			
		else
			f1->Write( buffer+lNumRecv*sizeof(char), received );			
	}
	
	if (received > 0)
		lNumRecv += received;
	else
		return -1;
	
	// if one TCP transaction receives, update number of "packet" (transaction) received
	if (!(lNumRecv < datalen))
	{
		currseqno = *((unsigned long *)buffer);
		packetrecv++;
		lNumRecv = 0;		
		delete [] buffer;
		buffer = NULL;	
	}

	return received;
}

///////////////////////////////////////////
//		Get packet received
///////////////////////////////////////////
long ES_TCPServer::GetPacketRecv()
{
	return packetrecv;
}

///////////////////////////////////////////
//		Calcuate the packet loss rate
///////////////////////////////////////////
double ES_TCPServer::GetLossRate()
{
	return (double) (100 - (100*packetrecv)/((float)(currseqno+1)));
}

///////////////////////////////////////////
//		TCP accept the connection
///////////////////////////////////////////
bool ES_TCPServer::Accept()
{
	struct sockaddr_in newaddr;
	int newaddrlen = sizeof(struct sockaddr_in);
	
	if (!connected)
		hSocket = accept(cSocket, (struct sockaddr *) &newaddr, &newaddrlen);
	
	if (hSocket == INVALID_SOCKET)
	{
//		AfxMessageBox("Error in accept for TCP,terminated.");
		hSocket = NULL;
		return false;
	}
	connected = true;
	return true;
}

///////////////////////////////////////////
//		get socket handle
///////////////////////////////////////////
SOCKET ES_TCPServer::GetHandle()
{
	return cSocket;
}

///////////////////////////////////////////
//		get the numbet of packet loss
///////////////////////////////////////////
long ES_TCPServer::GetLossPacket()
{
	return (currseqno-packetrecv+1);
}

////////////////////////////////////////////////////////
//		TCP startup: create socket, bind and listen
////////////////////////////////////////////////////////
bool ES_TCPServer::Startup(int port, char *IPaddr, bool file, LPCTSTR filename)
{
//	if (file)
//		f1 = new CFile(filename, CFile::modeCreate | CFile::modeWrite);

	cSocket = socket(PF_INET, SOCK_STREAM, 0);
	if (cSocket == INVALID_SOCKET)
	{
		AfxMessageBox("Error in creating socket for TCP,terminated.");
		cSocket = NULL;
		return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = inet_addr(GetHostAddrByName(IPaddr));
	
	if (bind(cSocket, (struct sockaddr *) &addr, sizeof(addr)) == SOCKET_ERROR)
	{
		char temp[20];
		sprintf(temp, "%d", port);
		AfxMessageBox("Unable to bind ["+(CString)IPaddr+":"+(CString)temp+"] for TCP.");
		
		return false;
	}

	if (listen(cSocket, 1) == SOCKET_ERROR)
	{
		AfxMessageBox("Error in listen for TCP,terminated.");
		return false;
	}
	return true;
}

///////////////////////////////////////////
//		get total packet received
///////////////////////////////////////////
long ES_TCPServer::GetTotalPacket()
{
	return (currseqno + 1);
}

bool ES_TCPServer::IOCPStartUp(int port, char *IPaddr)
{
	cSocket = WSASocket(PF_INET, 
						SOCK_STREAM, 
						IPPROTO_TCP, 
						(LPWSAPROTOCOL_INFO) 0, 
						(GROUP) 0, 
						WSA_FLAG_OVERLAPPED);
	
	if (cSocket == INVALID_SOCKET)
	{
		AfxMessageBox("Error in creating socket for TCP,terminated.");
		cSocket = NULL;
		return false;
	}

	struct sockaddr_in addr;
	addr.sin_family = AF_INET; 
	addr.sin_port = htons(port);
	addr.sin_addr.s_addr = INADDR_ANY;
	
	int err;
	err = bind(cSocket, (struct sockaddr *) &addr, sizeof(addr));
	
	if (err == SOCKET_ERROR)
	{
		AfxMessageBox("Fail in binding, terminated");	
		return 0;
	}

		
	if (listen(cSocket, 1) == SOCKET_ERROR)
	{
		AfxMessageBox("Error in listen for TCP,terminated.");

		return 0;
	}

	return true;
}

bool ES_TCPServer::IOCPAccept(ES_Connection **C, int n)
{
	struct sockaddr_in newaddr;
	int newaddrlen = sizeof(struct sockaddr_in);

	hSocket = WSASocket(PF_INET, 
						SOCK_STREAM, 
						IPPROTO_TCP, 
						(LPWSAPROTOCOL_INFO) 0, 
						(GROUP) 0, 
						WSA_FLAG_OVERLAPPED);

	hSocket = accept(cSocket, (struct sockaddr *) &newaddr, &newaddrlen);
	
	if (hSocket == INVALID_SOCKET)
	{
		AfxMessageBox("Error in accept for TCP,terminated.");
		hSocket = NULL;
		return 0;
	} 

	*C = new ES_Connection(hSocket, newaddr, n);

	return true;
}

SOCKET ES_TCPServer::get_hSocket()
{
	return hSocket;

}

int ES_TCPServer::ServerSend(const char *pdata, int datalen, SOCKET child)
{
	int lastsend = 0, senddata = 0;
	const char* p = pdata;

	do 
	{	
		p += lastsend*sizeof(char);
		lastsend = send(child, p, datalen-senddata, 0);
		if (lastsend == SOCKET_ERROR)
			return -1;

		senddata+=lastsend;
	} while (senddata < datalen);

	return 0;
}

void ES_TCPServer::ServedClient(SOCKET c)
{
	hSocket = c;
}

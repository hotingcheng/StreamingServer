// ES_Connection.cpp: implementation of the ES_Connection class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Netprobe1.h"
#include "ES_Connection.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ES_Connection::ES_Connection()
{

}


ES_Connection::ES_Connection(SOCKET hSocket, struct sockaddr_in newaddr, int n)
{
	client =  WSASocket(PF_INET, 
						SOCK_STREAM, 
						IPPROTO_TCP, 
						(LPWSAPROTOCOL_INFO) 0, 
						(GROUP) 0, 
						WSA_FLAG_OVERLAPPED);


	ConnectStatus = true;
	client = hSocket;
	addr = newaddr;
	num = n;
	sendSeqNo = 0;
	Bsent = 0;
	f3 = NULL;
}

ES_Connection::~ES_Connection()
{
	if (f3 != NULL)
		delete f3;
}

bool ES_Connection::connected()
{
	return ConnectStatus;
}

SOCKET ES_Connection::getClientSocket()
{
	return client;
}

int ES_Connection::getClientPort()
{
	return addr.sin_port;
}


char * ES_Connection::getClientIP()
{
	return inet_ntoa(addr.sin_addr);
}

void ES_Connection::ConnEst()
{
	ConnectStatus = true;
}

void ES_Connection::ConnDis()
{
	ConnectStatus = false;
}

int ES_Connection::getClientNum()
{
	return num;
}

CString ES_Connection::getfile()
{
	return mediafile;
}

HANDLE ES_Connection::getState()
{
	return State;
}

void ES_Connection::ChangeState(HANDLE h)
{
	State = h;
}

unsigned int ES_Connection::getSeqNo()
{
	return sendSeqNo;
}

void ES_Connection::UpdateSeqNo(unsigned int sn)
{
	sendSeqNo = sn;
}

void ES_Connection::Initfp(CString f)
{
	f3 = new CFile((LPCSTR)f, CFile::modeRead |CFile::shareDenyNone);
}

CFile * ES_Connection::getfp()
{
	return f3;
}

void ES_Connection::setrecvfile(CString path)
{
	mediafile = path; 
}


void ES_Connection::setProtocol(int p)
{
	iProtocol = p;
}

int ES_Connection::getProtocol()
{
	return iProtocol;
}
	

struct sockaddr_in ES_Connection::getClientaddr()
{
	return addr;
}

ES_Connection::ES_Connection(struct sockaddr_in newaddr, int n)
{
	client = WSASocket(PF_INET, 
					SOCK_DGRAM, 
					IPPROTO_UDP, 
					(LPWSAPROTOCOL_INFO) 0, 
					(GROUP) 0, 
					WSA_FLAG_OVERLAPPED);

	ConnectStatus = true;
	addr = newaddr;
	num = n;
	sendSeqNo = 0;
	Bsent = 0;
	f3 = NULL;

	//= UDPEvent[n] = CreateEvent(NULL,false,false,NULL);
	UDP = n;
	SRate = 0;
}


int ES_Connection::getID()
{
	return ID;
}


void ES_Connection::setID(int p)
{
	ID = p;
}

void ES_Connection::CloseConnection()
{
	closesocket(client);
}

double ES_Connection::bytessent()
{
	return Bsent;
}

void ES_Connection::UpdateBsent(double l)
{
	Bsent += l;
}

int ES_Connection::getUDP()
{
	return UDP;
}

void ES_Connection::setSRate(int R)
{
	SRate = R;
}

int ES_Connection::getSRate()
{
	return SRate;
}

void ES_Connection::setFS(int f)
{
	if (f == 1)
		FileOrStream = true;
	else
		FileOrStream = false;
}

bool ES_Connection::getFS()
{
	return FileOrStream; //	0 -- save file ; 1 -- streaming
}


// IOCP.cpp: implementation of the IOCP class.
//
//////////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include "Netprobe1.h"
#include "IOCP.h"

IOCP::IOCP()
{
/*	hConn = CreateEvent(NULL,false, false, NULL);
	hTCP = CreateEvent(NULL,false, false, NULL);
	hUDP = CreateEvent(NULL,false, false, NULL);
	hSend = CreateEvent(NULL,false, false, NULL);
	hReceive = CreateEvent(NULL,false, false, NULL);
	hFin = CreateEvent(NULL,false, false, NULL);
	hFile = CreateEvent(NULL,false, false, NULL);
	hWait = CreateEvent(NULL,false, false, NULL);
*/
}

IOCP::IOCP(int n)
{
	m_NumOfThread = n;
	hTCP = CreateEvent(NULL,false, false, NULL);
	hUDP = CreateEvent(NULL,false, false, NULL);
	hSend = CreateEvent(NULL,false, false, NULL);
	hConn = CreateEvent(NULL,false, false, NULL);
	hReceive = CreateEvent(NULL,false, false, NULL);
	hFile = CreateEvent(NULL,false, false, NULL);
	hFin = CreateEvent(NULL,false, false, NULL);
	hWait = CreateEvent(NULL,false, false, NULL);
}

IOCP::~IOCP(){}

int IOCP::getNumOfThread()
{
	return m_NumOfThread;
}

void IOCP::StartUp(int NumThreads)
{/*
	hCompletePort2 = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,
											   NULL,
											   0,
											   NumThreads);

*/
	hCompletePort = ::CreateIoCompletionPort(INVALID_HANDLE_VALUE,
											   NULL,
											   0,
											   NumThreads);
}

HANDLE IOCP::getTCPHandle()
{
	return hCompletePort;
}


HANDLE IOCP::getUDPHandle()
{
	return hCompletePort2;
}
HANDLE IOCP::gethConn()
{
	return hConn;
}

HANDLE IOCP::gethTCP()
{
	return hTCP;
}

HANDLE IOCP::gethUDP()
{
	return hUDP;
}

HANDLE IOCP::getSend()
{
	return hSend;

}

HANDLE IOCP::gethRecv()
{
	return hReceive;
}


HANDLE IOCP::gethFin()
{
	return hFin;
}

HANDLE IOCP::gethFile()
{
	return hFile;
}

HANDLE IOCP::gethWait()
{
	return hWait;
}

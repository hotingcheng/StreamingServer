
// IOCP.h: interface for the ES_Network class.
//
//////////////////////////////////////////////////////////////////////
#ifndef IOCP_H
#define IOCP_H

#include "ES_Object.h"

#include "IOCP.h"

class IOCP : public ES_Object  
{
	private:
		int m_NumOfThread;
		HANDLE hCompletePort2;
		HANDLE hCompletePort;
		HANDLE hConn;
		HANDLE hTCP;
		HANDLE hUDP;
		HANDLE hSend;
		HANDLE hReceive;
		HANDLE hFin;
		HANDLE hFile;
		HANDLE hWait;
	
	protected:
		HANDLE UDPEvent[100];

	public:
		IOCP();
		HANDLE gethWait();
		HANDLE gethFile();
		HANDLE gethFin();
		HANDLE gethRecv();
		HANDLE getSend();
		HANDLE gethUDP();
		HANDLE gethTCP();
		HANDLE gethConn();
		IOCP(int);
		virtual ~IOCP();
		int getNumOfThread();
		HANDLE getTCPHandle();
		HANDLE getUDPHandle();
		void StartUp(int);
};

#endif

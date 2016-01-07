#ifndef ES_TCPServer_H
#define ES_TCPServer_H

#include "ES_Connection.h"
#include "ES_TCP.h"

class ES_TCPServer : public ES_TCP  
{
	private:
		long lastrecv;
		CFile *f1;
		long currseqno;
		long lNumRecv;
		char * buffer;
		long packetrecv;
		SOCKET cSocket;

	public:
		void ServedClient(SOCKET);
		int ServerSend(const char *,int , SOCKET);
		SOCKET get_hSocket();
		bool IOCPAccept(ES_Connection ** ,int);
		bool IOCPStartUp(int port, char *IPaddr);
		ES_TCPServer();
		virtual ~ES_TCPServer();
		long GetTotalPacket();
		bool Startup(int port, char* IPaddr, bool, LPCTSTR);
		long GetLossPacket();
		SOCKET GetHandle();
		bool Accept(void);
		double GetLossRate(void);
		bool connected;
		long GetPacketRecv(void);
		int Recv(int datalen, bool);
		void CloseFile();
};

#endif
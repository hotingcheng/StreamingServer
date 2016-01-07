#ifndef ES_UDP_H
#define ES_UDP_H

#include "ES_Network.h"

class ES_UDP : public ES_Network  
{
	private:
		CFile *f1;
		struct sockaddr_in dest_addr;
		long packetrecv;
		unsigned long currseqno;

	public:
		bool IOCPStartUp(int port, char* IPaddr);
		ES_UDP();
		virtual ~ES_UDP();	
		long GetTotalPacket();
		bool Startup(bool server, int port, char* IPaddr,bool,LPCTSTR);
		long GetLossPacket();
		SOCKET GetHandle();
		double GetLossRate(void);
		long GetPacketRecv(void);
		int Send(char* pData, int datalen);	
		int Recv(int datalen, bool);			
		void setPacketNo();
		void setSeqNo();
		void CloseFile();
};

#endif
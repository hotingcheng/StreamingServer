
// ES_Network.h: interface for the ES_Network class.
//
//////////////////////////////////////////////////////////////////////
#ifndef ES_Network_H
#define ES_Network_H

#include "ES_Object.h"

class ES_Network : public ES_Object  
{
	private:
//		struct hostent *myhostent;

	protected:
		SOCKET hSocket;
		struct hostent *myhostent;
	public:
	//	SOCKET hSocket;	
		ES_Network();
		virtual SOCKET GetHandle();		
		virtual ~ES_Network();
		int GetLocalHostName(char* hostname);
		char* GetHostAddrByName(char* hostname);
		char* GetHostNameByAddr(const char* addr);
		char* GetNthHostAddr(int i);
		char* GetNthHostName(int i);
		int ValidateHost(char* hostname);
	//	static const int MAX_NAME;
		virtual int Send(const char* pdata, int datalen);
		virtual int Recv(int datalen);
		enum{MAX_NAME=128}; 
};

#endif

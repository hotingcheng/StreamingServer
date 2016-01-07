#ifndef ES_TCP_H
#define ES_TCP_H

#include "ES_Network.h"

class ES_TCP : public ES_Network  
{
	public:		
		ES_TCP();		
		virtual ~ES_TCP();
		virtual int Send(const char *pdata, int datalen);
		virtual int Recv(int datalen, char* buffer);
		virtual int Shutdown(SOCKET sock);
};

#endif 
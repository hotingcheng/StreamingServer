#ifndef ES_TCPClient_H
#define ES_TCPClient_H

#include "ES_TCP.h"

class ES_TCPClient : public ES_TCP  
{
public:
	ES_TCPClient();
	virtual ~ES_TCPClient();
	int Connect(int port, char* hostname);
	void Disconnect(void);
};

#endif 

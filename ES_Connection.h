// ES_Connection.h: interface for the ES_Connection class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ES_CONNECTION_H__2E4569BB_CCFA_4D52_8E68_6E5D200CBABA__INCLUDED_)
#define AFX_ES_CONNECTION_H__2E4569BB_CCFA_4D52_8E68_6E5D200CBABA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "IOCP.h"

class ES_Connection : public IOCP
{
private:
	int SRate;	// 0 -- normal rate; 1 -- slower rate ; 2 -- faster rate
	int UDP;
	double Bsent;
	int ID;
	int iProtocol;	
	CFile * f3;
	unsigned int sendSeqNo;
	HANDLE State;
	CString mediafile;
	struct sockaddr_in addr;
	bool ConnectStatus;
	SOCKET client;
	int num;
	bool FileOrStream;


public:	
	bool getFS();
	void setFS(int);
	int getSRate();
	void setSRate(int);
	int getUDP();
	void UpdateBsent(double);
	double bytessent();
	void CloseConnection();
	void setID(int);
	int getID();
	ES_Connection(struct sockaddr_in newaddr,int);
	struct sockaddr_in getClientaddr();
	void setProtocol(int p);
	int getProtocol();
	void setrecvfile(CString);
	CFile * getfp();
	void Initfp (CString);
	void UpdateSeqNo(unsigned int);
	unsigned int getSeqNo();
	void ChangeState(HANDLE);
	HANDLE getState();
	CString getfile();
	int getClientNum();
	void ConnDis();
	void ConnEst();
	char * getClientIP();
	int getClientPort();
	SOCKET getClientSocket();


	bool connected();
	ES_Connection();
	ES_Connection(SOCKET, struct sockaddr_in, int);
	virtual ~ES_Connection();

};

#endif // !defined(AFX_ES_CONNECTION_H__2E4569BB_CCFA_4D52_8E68_6E5D200CBABA__INCLUDED_)

// ES_UDP.cpp: implementation of the ES_UDP class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "Netprobe1.h"
#include "ES_UDP.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
ES_UDP::ES_UDP()
{
	packetrecv = 0;
	currseqno = 0;
}

ES_UDP::~ES_UDP()
{
	if (!hSocket)
		return;
	if (shutdown(hSocket, 2) == SOCKET_ERROR)
		AfxMessageBox("Error in shuting down the socket for UDP, treminated.");	
	if (closesocket(hSocket) == SOCKET_ERROR)
		AfxMessageBox("Error in closing the socket for UDP, treminated.");		
}
 
///////////////////////////////////////////////////////////////////////////////////
//		UDP startup: create socket , for receiver(server), also bind the socket
///////////////////////////////////////////////////////////////////////////////////
bool ES_UDP::Startup(bool server, int port, char* IPaddr, bool file, LPCTSTR filename)
{
	if (file)
		f1 = new CFile(filename, CFile::modeCreate | CFile::modeWrite);

	hSocket = socket(PF_INET, SOCK_DGRAM, 0); 
	if (hSocket == INVALID_SOCKET)
	{
		AfxMessageBox("Error in creating socket for UDP,terminated.");
		closesocket(hSocket);
		return false;
	}

	dest_addr.sin_family = AF_INET; 
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(GetHostAddrByName(IPaddr));

	if (server)
	{
		if (bind(hSocket, (struct sockaddr *) &dest_addr, sizeof(dest_addr)) == SOCKET_ERROR)
		{
			char temp[20];
			sprintf(temp, "%d", port);
			AfxMessageBox("Unable to bind ["+(CString)IPaddr+":"+(CString) temp +"] for UDP.");
			return false;
		}
	}
	return true;
}

////////////////////////////////////////////////////////
//		UDP Send
////////////////////////////////////////////////////////
int ES_UDP::Send(char *pData, int datalen)
{
	int datasend;
	datasend = sendto(hSocket, pData, datalen, 0, (struct sockaddr *)&dest_addr, sizeof(struct sockaddr_in));
	if (datasend == SOCKET_ERROR)
		return -1;
	return 0;
}

////////////////////////////////////////////////////////
//		UDP Receive
////////////////////////////////////////////////////////
int ES_UDP::Recv(int datalen, bool file)
{
	int datarecv;
	char* buffer = new char[datalen];
	int addrlen = sizeof(struct sockaddr_in);
	
	memset(buffer, 0, datalen);
	datarecv = recvfrom(hSocket, buffer, datalen, 0, (struct sockaddr *)&dest_addr, &addrlen);
	if (datarecv < 0)
		return -1;
	
	char* p = buffer;
	p += sizeof(unsigned long);		// for the header, which is a sequence number
	if (datalen != *((int *)p))
		return -2;

	if (*((unsigned long *)buffer) < currseqno)
		packetrecv =0;

	currseqno = max( *((unsigned long *)buffer) , currseqno);
	
//	if (currseqno == 0)	
//		packetrecv=1;
//	else
		packetrecv++;

	if ((file)&& (datarecv>0))
	{
		if (datalen == *((int *)p))
			f1->Write( buffer+sizeof(unsigned long)+sizeof(int), datarecv-sizeof(unsigned long)-sizeof(int) );			
		else
			f1->Write( buffer, datarecv );			
	}

	delete [] buffer;

	return datarecv;
}

////////////////////////////////////////////////////////
//		Get packet received
////////////////////////////////////////////////////////
long ES_UDP::GetPacketRecv()
{
	return packetrecv;
}

////////////////////////////////////////////////////////
//		get packet loss rate
////////////////////////////////////////////////////////
double ES_UDP::GetLossRate()
{
	if ((double) (100 - (100*packetrecv)/((float)(currseqno+1)))<0)
		return 0;
	else
		return (double) (100 - (100*packetrecv)/((float)(currseqno+1)));
}

////////////////////////////////////////////////////////
//		get socket handle
////////////////////////////////////////////////////////
SOCKET ES_UDP::GetHandle()
{
	return hSocket;
}

////////////////////////////////////////////////////////
//		get number of packet loss 
////////////////////////////////////////////////////////
long ES_UDP::GetLossPacket()
{
	if ((currseqno-packetrecv+1) <= 0)
		return 0;
	else
		return (currseqno-packetrecv+1);
}

////////////////////////////////////////////////////////
//			get total packet received
////////////////////////////////////////////////////////
long ES_UDP::GetTotalPacket()
{	
	if ((currseqno+1) <= 0) 
		return 0;
	else 
		return (currseqno + 1);
}

void ES_UDP::setPacketNo()
{
	packetrecv=1;
}

void ES_UDP::setSeqNo()
{
	currseqno=0;
}

///////////////////////////////////////////
//		UDP Server : Close File
///////////////////////////////////////////
void ES_UDP::CloseFile()
{
	f1->Close();
	delete f1;
}

bool ES_UDP::IOCPStartUp(int port, char* IPaddr)
{
	hSocket = WSASocket(PF_INET, 
						SOCK_DGRAM, 
						IPPROTO_UDP, 
						(LPWSAPROTOCOL_INFO) 0, 
						(GROUP) 0, 
						WSA_FLAG_OVERLAPPED);

	if (hSocket == INVALID_SOCKET)
	{
		AfxMessageBox("Error in creating socket for UDP,terminated.");
		closesocket(hSocket);
		return false;
	}

	dest_addr.sin_family = AF_INET; 
	dest_addr.sin_port = htons(port);
	dest_addr.sin_addr.s_addr = inet_addr(GetHostAddrByName(IPaddr));
//	dest_addr.sin_addr.s_addr = INADDR_ANY;

	int err = bind(hSocket, (struct sockaddr *) &dest_addr, sizeof(dest_addr));
	if (err == SOCKET_ERROR)
	{
		AfxMessageBox("Fail in binding, terminated");
		return 0;
	}

	return true;


}

#ifndef __Socket_H__
#define __Socket_H__

typedef struct _SOCK_DATA_
{
	char Buffer[4096];
} SOCK_DATA;

typedef struct
{
	SOCK_DATA Data;
	SOCKET Server;
	SOCKET Client;
	SOCKET Socket;
	int Protocol;
	BOOL Connected;
	BOOL Listening;
	TIMEVAL	*Timeval;
} SOCK_INFO;

BOOL InitializeWinsock();
BOOL CreateSockClient(SOCK_INFO *Sock, char *Server, char *Port);
BOOL CreateSockServer(SOCK_INFO *Sock, char *Port);
void DestroySock(SOCK_INFO *Sock);
BOOL SendSockData(SOCK_INFO *Sock, char *Format, ...);
BOOL ReceiveSockData(SOCK_INFO *Sock);
BOOL SetSocketBlocking(SOCKET Socket, BOOL blocking);

#endif __Socket_H__
#ifndef __IRC_H__
#define __IRC_H__

#include "Sockets.h"

typedef struct
{
	SOCK_INFO Sock;
	char *Nickname, *Username, *Realname;
	char *Channel;
} IRC_INFO;

BOOL ConnectToIRC(IRC_INFO *IRC, char *Server, char *Port);
BOOL RegisterIRC(IRC_INFO *IRC);
BOOL ParseIRC(IRC_INFO *IRC);
BOOL ParseCommand(IRC_INFO *IRC, char *Channel, char *Data);
BOOL JoinChannel(IRC_INFO *IRC, char *Channel);
BOOL PartChannel(IRC_INFO *IRC, char *Channel);

#endif
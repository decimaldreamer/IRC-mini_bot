#include <WinSock2.h>
#include <ws2tcpip.h>
#include <Windows.h>
#include <stdio.h>

#include "IRC.h"

BOOL ConnectToIRC(IRC_INFO *IRC, char *Server, char *Port)
{
	if (!InitializeWinsock())
		return FALSE;

	if (!CreateSockClient(&IRC->Sock, Server, Port))
		return FALSE;

#ifdef _DEBUG
	printf("Connected to IRC server: %s:%s\n", Server, Port);
#endif

	if (!RegisterIRC(IRC))
		return FALSE;

	// receive data until connection is ended
	do 
	{
		if (!ReceiveSockData(&IRC->Sock))
			break;

		if (IRC->Sock.HasCommand && !ParseIRC(IRC))
			break;
		
	} while (&IRC->Sock.Connected);

	// clean up socket
	DestroySock(&IRC->Sock);

	return TRUE;
}

BOOL RegisterIRC(IRC_INFO *IRC)
{
	if (IRC->Sock.Connected == FALSE)
		return FALSE;

	// Setup initial bot data
	if (SendSockData(&IRC->Sock, "NICK %s\r\n USER %s 0 * %s\r\n", IRC->Nickname, IRC->Username, IRC->Realname) == FALSE)
		return FALSE;

	return TRUE;
}

BOOL ParseIRC(IRC_INFO *IRC)
{
	int status;
	char *Ptr, Temp[512], Channel[32], Body[256];

	if (IRC->Sock.Connected == FALSE)
		return FALSE;

	// IRC server sends a PING with a unique code every X seconds, and must be replied to, or the connection will be closed
 	status = sscanf(IRC->Sock.Data.Buffer, "PING :%s", Temp);
 	if (status > 0)
 	{
 		if (!SendSockData(&IRC->Sock, "PONG :%s\r\n", Temp))
 			return FALSE;
 	}
	
	// scan for RPL_WELCOME (001) message, if found we are ready for user interaction
	if (strstr(IRC->Sock.Data.Buffer, "001"))
		SendSockData(&IRC->Sock, "JOIN %s\r\n", IRC->Channel);
	
	// scan for PRIVMSG, if found it possibly contains a command, so parse it
	if ((Ptr = strstr(IRC->Sock.Data.Buffer, "PRIVMSG")))
	{
		// probably not the best way to parse, but it works
		if (sscanf(Ptr, "PRIVMSG %31s :%255[^\n]", Channel, Body) > 0)
		{
			// if the first character of the PRIVMSG contains a !, it's a command
			if (strstr(Ptr, ":!"))
			{
				if (!ParseCommand(IRC, Channel, Body))
					return FALSE;
			}
		}
	}

	return TRUE;
}

BOOL ParseCommand(IRC_INFO *IRC, char *Channel, char *Data)
{
	char *Command[16];
	char *Token;
	int i = 0;

	// split up command parameters into a array by using space as a delimiter
	Token = strtok(Data, " ");
	while (Token)
	{
		Command[i++] = Token;
		Token = strtok(NULL, " ");
	}

#ifdef _DEBUG
	printf("Received command: %s\n", Command[0]);
#endif

	// Command[0] is the command it's self, and every element after it is a parameters
	 
	if (!strcmp(Command[0], "!join") && Command[1] != NULL)
		SendSockData(&IRC->Sock, "JOIN %s\r\n", Command[1]);
	else if (!strcmp(Command[0], "!part") && Command[1] != NULL)
		SendSockData(&IRC->Sock, "PART %s\r\n", Command[1]);
	else if (!strcmp(Command[0], "!msgbox") && Command[1] != NULL && Command[2] != NULL)
		MessageBoxA(NULL, Command[2], Command[1], MB_OK);
	else
		SendSockData(&IRC->Sock, "PRIVMSG %s :Invalid command!\r\n", Channel);

	return TRUE;
}

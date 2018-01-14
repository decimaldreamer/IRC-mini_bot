#include <Windows.h>
#include <stdio.h>

#include "IRC.h"

#define IRC_SERVER "irc.undernix.net"
#define IRC_PORT "6667"

void EntryPoint()
{
	IRC_INFO IRC;

	IRC.Nickname = "Alprazolam";
	IRC.Username = "Xanax";
	IRC.Realname = "Alprazolam Xanax";
	IRC.Channel = "#dev";

	if (!ConnectToIRC(&IRC, IRC_SERVER, IRC_PORT))
	{
#ifdef _DEBUG
		printf("[ERROR] ConnectToIRC: failed to connect to %s:%s\n", IRC_SERVER, IRC_PORT);
#endif
	}
	
	return;
}
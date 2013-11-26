#include <iostream>
#include <string>
#include <cstdlib>

#include "../Sockets/Network.h"
#include "GameServer.h"

int main(int nargs, const char *args[])
{
	InitializeNetwork();

	GameServer GS;
	GS.startServer("wozzy");

	int nPlayers = GS.getNumberOfPlayers();
	while(true)
	{
		if(nPlayers != GS.getNumberOfPlayers())
		{
			nPlayers = GS.getNumberOfPlayers();
			cout << "nPlayers = " << nPlayers << endl;
		}
	}

	ShutdownNetwork();

	return 0;
}

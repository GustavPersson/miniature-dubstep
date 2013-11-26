#ifndef _GAMESERVER_H
#define _GAMESERVER_H

#include <iostream>
#include <string>
#include "ServerGroup.h"
#include "ConnectionManager.h"
#include "IdManager.h"

namespace network
{

class ClientMessageHandler;

class GameServer
{
	friend ClientMessageHandler;

private:
	int mMaxPlayers;
	string mServerName;
	ConnectionManager mConnectionManager;
	ServerGroup *mServerGroup;
	IdManager mIdManager;

	// Helper functions
	void getPeerAddress(SOCKET soc, char **addr, int *port=NULL);

public:

	GameServer();

	void startServer(string servername,
					 int maxPlayers=4,
					 string servergroupIP = "234.5.6.7",
					 int servergroupPort = 99999);


	int getNumberOfPlayers()const;
	int getMaxPlayers()const;

	void addPlayer(SOCKET soc, string name, string ip, int port);
	void removePlayer(int id);

	// Shuts down the server and send the participant list to all clients.
	void startGame();
};
}

#endif

#include <iostream>
#include <sstream>
#include "Protocol.h"
#include "ClientMessageHandler.h"

#include "GameServer.h"

using namespace network;

GameServer::GameServer()
{

}
void GameServer::getPeerAddress(SOCKET soc, char **addr, int *port)
{
	sockaddr *socAddr = new sockaddr();
	int nLengthAddr = 16;
	getpeername(soc, socAddr, &nLengthAddr);
	*addr = inet_ntoa(((sockaddr_in*)socAddr)->sin_addr);
	if(port)
		*port = ntohs(((sockaddr_in*)socAddr)->sin_port);
}

void GameServer::startServer(string servername,
							 int maxPlayers,
							 string servergroupIP,
							 int servergroupPort)
{
	// Initiate server.

	// Set server name.
	this->mServerName = servername;

	// Set maximum number of players.
	this->mMaxPlayers = maxPlayers;

	// set message handler to our connection manager.
	this->mConnectionManager.addMessageHandler(new ClientMessageHandler(this));

	// setup and join the servergroup.
	this->mServerGroup = new ServerGroup(servergroupIP.c_str(), servergroupPort);
	this->mServerGroup->setGreetingMessage(servername); // when greeting a client we present our name.
	this->mServerGroup->setConnectionManager(&this->mConnectionManager);
	this->mServerGroup->join();
}
int GameServer::getNumberOfPlayers()const
{
	return this->mIdManager.size();
}
int GameServer::getMaxPlayers()const
{
	return this->mMaxPlayers;
}
void GameServer::addPlayer(SOCKET soc, string name, string ip, int port)
{
	int id = this->mIdManager.size();
	this->mIdManager.registerID(soc, id, name, ip, port);
}
void GameServer::removePlayer(int id)
{
	SOCKET *player = NULL;
	player = this->mIdManager.getSocketByID(id);
	if(player)
		this->mIdManager.removeID(*player);
}

void GameServer::startGame()
{
	// Drop from the servergroup.
	this->mServerGroup->drop();

	// Sending Publish list to all participating players.
	ostringstream publishList(ios_base::in | ios_base::out);
	int len;
	int type;

	publishList << this->mIdManager;
	len = publishList.str().length();
	type = MessageType_PublishList;

	this->mConnectionManager.sendToAll(len, type, publishList.str().c_str());
}

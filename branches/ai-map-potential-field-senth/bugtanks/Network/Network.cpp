#include "../Utilities/Macros.h"
#include "Network.h"

#include <iostream>
using namespace std;

using namespace network;

Network::Network(int port)
{
	pthread_mutex_init(&mMutex, NULL);
	pthread_cond_init(&mCondVar, NULL);

	this->mListeningPort = port;

	mPeer = NULL;
	mServerHandler = NULL;
	mClientHandler = NULL;
}
Network::~Network()
{
	SAFE_DELETE(mPeer);
	pthread_mutex_destroy(&mMutex);
}

void Network::setProfile(std::string nickname)
{
	mNickName = nickname;
	mPeer = myNew Peer(mNickName, mListeningPort);
}

Peer* Network::getPeer()
{
	Peer* result;
	pthread_mutex_lock(&mMutex);
	result = mPeer;
	pthread_mutex_unlock(&mMutex);
	return result;
}

void Network::startServer(std::string ServerName, int MaxPlayers)
{
	mServerName = ServerName;
	mServerLookup.registerServer(ServerName, (u_short)mListeningPort);
	mServerHandler = myNew MsgH_GameServer(mServerName, mPeer->getPeerManager(), MaxPlayers);
	mPeer->getPeerManager()->addMessageHandler(mServerHandler);
}

void Network::stopServer()
{
	mServerLookup.unregisterServer(mServerName);
	mPeer->getPeerManager()->removeMessageHandler(mServerHandler);
}
	
void Network::join(std::string addr, int port)
{
	if(mPeer != NULL)
	{
		mClientHandler = myNew MsgH_GameClient(mPeer);
		mPeer->getServerManager()->addMessageHandler(mClientHandler);
		mPeer->joinNetwork(addr, port);
	}
}

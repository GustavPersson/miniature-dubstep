#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "../Utilities/Macros.h"
#include "Protocol.h"
#include "Peer.h"
#include "../Logic/PlayerInfo.h"

using namespace std;
using namespace network;

Peer::Peer(std::string nickName, int port)
{
	pthread_mutex_init(&mMutex, NULL);

	mIDManager = IdManager::getInstance();

	mPeerManager = NULL;
	mServerManager = NULL;

	mPeerManager = myNew ConnectionManager();
	mServerManager = myNew ConnectionManager();

	mMsgH_PlayerID = myNew MsgH_PlayerID();
	mNickName = nickName;
	mID = -1;
	mPort = port;
	mPeerManager->addMessageHandler(mMsgH_PlayerID);
	mConnector.setConnectionManager(mPeerManager);
	mConnector.startAccept((u_short)port);
}
Peer::~Peer()
{
	mIDManager->clear();
	mConnector.stopAccept();
	SAFE_DELETE(mPeerManager);
	SAFE_DELETE(mServerManager);

	pthread_mutex_destroy(&mMutex);
}

IdManager* Peer::getIdManager()
{
	IdManager* result;
	pthread_mutex_lock(&mMutex);
	result = mIDManager;
	pthread_mutex_unlock(&mMutex);
	return result;
}

ConnectionManager* Peer::getPeerManager()
{
	ConnectionManager* result;
	
	pthread_mutex_lock(&mMutex);
	result = mPeerManager;
	pthread_mutex_unlock(&mMutex);
	
	return result;
}
ConnectionManager* Peer::getServerManager()
{
	ConnectionManager * result;
	
	pthread_mutex_lock(&mMutex);
	result = mServerManager;
	pthread_mutex_unlock(&mMutex);

	return result;
}
void Peer::joinNetwork(std::string serverAddr, int port)
{
	// Compile join message.
	ostringstream joinMessage(ios_base::in | ios_base::out);
	joinMessage << mNickName << " " << mPort;

	Connector connector;
	SOCKET soc;
	if(connector.connectTo(serverAddr.c_str(), (u_short)port, &soc))
	{
		mServerManager->addConnection(soc);
		mServerManager->sendTo(soc, joinMessage.str().length(),
			MessageType_JoinRequest, joinMessage.str().c_str());
	}
}
void Peer::updateNetwork(const char *publishList, unsigned int size)
{
	mPeerList.clear();

	// Parse publish list.
	istringstream plist (string(publishList, size), istringstream::in);

	char line[256];
	while(plist.getline(line, 256))
	{
		if(line[0] != '#' && line[0] != '\n') // Ignore comment lines, lines that starts with newline.
		{
			int id, port;
			string name, ip;

			istringstream iline (string(line), istringstream::in);
			iline >> id >> name >> ip >> port;

			mPeerList[id].id = id;
			mPeerList[id].name = name;
			mPeerList[id].ip = ip;
			mPeerList[id].port = port;
		}
	}
	
	vector<IDstructure> players = mIDManager->getIds();
	vector<IDstructure>::iterator player = players.begin();
	map<int, PeerInfo_t>::iterator peer = mPeerList.begin();

	for(player = players.begin(); player != players.end(); player++)
	{
		bool found=false;
		for(peer = mPeerList.begin(); peer != mPeerList.end(); peer++)
		{
			if(player->ip.compare(peer->second.ip)==0 && player->port == peer->second.port)
			{
				found = true;
				break;
			}
			else
				found = false;
		}
		if(found)
		{
			// updating name and id.
			(*player).id = (*peer).second.id;
			(*player).name = (*peer).second.name;

			// remove peer from mPeerList since there is no need to compare it to any other players.
			this->mPeerList.erase(peer);
		}
		else
		{
			// remove player from the IdManager.
			this->mIDManager->removeID((*player).socket);
		}
	}

	// Compile identification message.
	ostringstream idMessage(ios_base::in | ios_base::out);
	idMessage << mID << " " << mNickName << " " << mPort;

	// Connect to all new Peers that has an ID number higher than myself
	for(peer = mPeerList.begin(); peer != mPeerList.end(); peer++)
	{
		if((*peer).second.id > this->mID)
		{
			SOCKET soc;
			if(mConnector.connectTo(peer->second.ip.c_str(), (u_short)peer->second.port, &soc))
			{
				mPeerManager->sendTo(soc, idMessage.str().length(), MessageType_PlayerID, idMessage.str().c_str());
				mIDManager->registerID(soc, peer->second.id, peer->second.name, peer->second.ip, peer->second.port);
			}
		}
	}

	/*

	// Remove all peers, from the IdManager, that isn't in the list anymore.
	vector<IDstructure> players = mIDManager->getIds();
	for(int i=0; i< mIDManager->size(); i++)
	{
		// remove id if player not found in the mPeerList.
		if(mPeerList.find(players.at(i).id) == mPeerList.end())
		{
			mIDManager->removeID(players.at(i).socket);
		}
	}

	// Compile identification message.
	ostringstream idMessage(ios_base::in | ios_base::out);
	idMessage << mID << " " << mNickName << " " << mPort;

	// Connect to new peers that comes after you in the list and add them to the mIdManager.
	bool pastMyID=false;
	map<int, PeerInfo_t>::iterator p;
	for(p = mPeerList.begin(); p != this->mPeerList.end(); p++)
	{
		if(pastMyID) // We are past our own id.
		{
			if(mIDManager->getSocketByID((*p).first)==NULL) // Peer was not registered.
			{
				SOCKET soc;
				if(mConnector.connectTo(p->second.ip.c_str(), (u_short)p->second.port, &soc))
				{
					mPeerManager->sendTo(soc, idMessage.str().length(), MessageType_PlayerID, idMessage.str().c_str());
					mIDManager->registerID(soc, p->second.id, p->second.name, p->second.ip, p->second.port);
				}
			}
		}

		if((*p).first == this->mID)
			pastMyID=true;
	}
	*/
}

void Peer::closeConnector()
{
	mConnector.stopAccept();
}

void Peer::sendToAll(int len, int type, const char *message)
{
	// compile message
	char *msg = myNew char[len+sizeof(msgHeader_t)];
	msgHeader_t *header = (msgHeader_t*)&msg[0];
	header->len = len;
	header->type = type;
	
	for(int i=0; i<len; i++)
		msg[sizeof(msgHeader_t)+i]=message[i];

	// Lock Mutex for socket.
	pthread_mutex_lock(&mMutex);
	
	// Send message to all peers registered in the idmanager.
	vector<IDstructure> ids = mIDManager->getIds();
	vector<IDstructure>::iterator it;
	for(it = ids.begin(); it != ids.end(); it++)
	{
		if ((*it).id != mID)
		{
			send((*it).socket, msg, len+sizeof(msgHeader_t), 0);
		}
	}
	
	// Unlock Mutex for socket.
	pthread_mutex_unlock(&mMutex);

	// Release allocated memory.
	SAFE_DELETE_ARRAY(msg);
}

void Peer::sendTo(int id, int len, int type, const char *message)
{
	pthread_mutex_lock(&mMutex);

	SOCKET *soc = NULL;
	soc = this->mIDManager->getSocketByID(id);
	if(soc)
	{
		this->mPeerManager->sendTo(*soc, len, type, message);
	}

	pthread_mutex_unlock(&mMutex);
}

void Peer::sendTo(std::string name, int len, int type, const char *message)
{
	pthread_mutex_lock(&mMutex);

	SOCKET *soc = NULL;
	soc = this->mIDManager->getSocketByName(name);
	if(soc)
	{
		this->mPeerManager->sendTo(*soc, len, type, message);
	}

	pthread_mutex_unlock(&mMutex);
}
void Peer::setID(int id)
{
	this->mID = id;
}
int Peer::getID()const
{
	return this->mID;
}
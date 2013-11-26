#include <iostream>
#include <string>
#include <vector>
#include <sstream>

#include "Protocol.h"
#include "MsgH_PlayerID.h"
#include "Peer.h"

using namespace std;
using namespace network;

Peer::Peer(string name, int port)
{
	this->mName=name;
	this->mID=-1;
	this->mPort=port;
	this->mPeerManager.addMessageHandler(new MsgH_PlayerID(&this->mIdManager));
	this->mConnector.setConnectionManager(&this->mPeerManager);
	this->mConnector.startAccept(port);
}
IdManager* Peer::getIdManager()
{
	return &this->mIdManager;
}
ConnectionManager* Peer::getPeerManager()
{
	return &this->mPeerManager;
}
ConnectionManager* Peer::getServerManager()
{
	return &this->mServerManager;
}
void Peer::joinNetwork(std::string serverAddr, int port)
{
	// Compile join message.
	ostringstream joinMessage(ios_base::in | ios_base::out);
	joinMessage << this->mName << " " << this->mPort;

	Connector connector;
	SOCKET soc;
	if(connector.connectTo(serverAddr.c_str(), port, &soc))
	{
		this->mServerManager.addConnection(soc);
		this->mServerManager.sendTo(soc, joinMessage.str().length(),
			MessageType_JoinRequest, joinMessage.str().c_str());
	}
}
void Peer::updateNetwork(const char *publishList, unsigned int size)
{
	mPeerList.clear();
	this->mID = -1;

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

			if(this->mName.compare(name)==0)
			{
				this->mID = id; // Get my own id from the list.
			}
		}
	}

	// Remove all peers, from the IdManager, that isn't in the list anymore.
	vector<IDstructure> players = this->mIdManager.getIds();
	for(int i=0; i<this->mIdManager.size(); i++)
	{
		// remove id if player not found in the mPeerList.
		if(this->mPeerList.find(players.at(i).id) == mPeerList.end())
		{
			this->mIdManager.removeID(players.at(i).socket);
		}
	}

	// Compile identification message.
	ostringstream idMessage(ios_base::in | ios_base::out);
	idMessage << this->mID << " " << this->mName << " " << this->mPort;

	// Connect to new peers that comes after you in the list and add them to the mIdManager.
	bool pastMyID=false;
	map<int, PeerInfo_t>::iterator p;
	for(p = this->mPeerList.begin(); p != this->mPeerList.end(); p++)
	{
		if(pastMyID) // We are past our own id.
		{
			if(this->mIdManager.getSocketByID((*p).first)==NULL) // Peer was not registered.
			{
				SOCKET soc;
				if(this->mConnector.connectTo(p->second.ip.c_str(), p->second.port, &soc))
				{
					this->mPeerManager.sendTo(soc, idMessage.str().length(), MessageType_PlayerID, idMessage.str().c_str());
					this->mIdManager.registerID(soc, p->second.id, p->second.name, p->second.ip, p->second.port);
				}
			}
		}

		if((*p).first == this->mID)
			pastMyID=true;
	}
}
void Peer::sendToAll(int len, int type, const char *message)
{
	this->mPeerManager.sendToAll(len, type, message);
}
void Peer::sendTo(int id, int len, int type, const char *message)
{
	SOCKET *soc = NULL;
	soc = this->mIdManager.getSocketByID(id);
	if(soc)
		this->mPeerManager.sendTo(*soc, len, type, message);
}
void Peer::sendTo(string name, int len, int type, const char *message)
{
	SOCKET *soc = NULL;
	soc = this->mIdManager.getSocketByName(name);
	if(soc)
		this->mPeerManager.sendTo(*soc, len, type, message);
}
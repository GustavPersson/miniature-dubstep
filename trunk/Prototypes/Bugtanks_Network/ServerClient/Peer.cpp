#include <iostream>
#include <string>
#include <vector>
#include <sstream>
using namespace std;

#include "Peer.h"

Peer::Peer(string name, int port)
{
	this->mName=name;
	this->mID=-1;
	this->mConnectionManager.addMessageHandler(new PeerMessageHandler(this));
	this->mConnector.setConnectionManager(&this->mConnectionManager);
	this->mConnector.startAccept(port);
}
void Peer::joinNetwork(const char *publishList, unsigned int size)
{
	// Parse publish list.
	istringstream plist (string(publishList, size), istringstream::in);

	char line[256];
	while(plist.getline(line, 256))
	{
		if(line[0] != '#' && line[0] != '\n') // Ignore comment lines, lines that starts with newline.
		{
			SOCKET soc;
			int id, port;
			string name, ip;

			istringstream iline (string(line), istringstream::in);
			iline >> id >> name >> ip >> port;

			if(this->mName.compare(name)==0)
			{
				this->mID = id; // Get my own id from the list.
			}
			else if(this->mID != -1) // Only connect to those that comes after you in the list.
			{
				if(this->mConnector.connectTo(ip.c_str(), port, &soc))
				{
					this->mIdManager.registerID(soc, id, name, ip, port);
				}
			}
		}
	}

	// FORALL Players in the list, identify yourself to them.
	ostringstream idMessage(ios_base::in | ios_base::out);
	idMessage << this->mID << " " << this->mName;

	vector<IDstructure> playes = this->mIdManager.getIds();
	for(int i=0; i<this->mIdManager.size(); i++)
	{
		mConnectionManager.sendTo(playes.at(i).socket, idMessage.str().length(), MessageType_PlayerID, idMessage.str().c_str());
	}
}
void Peer::sendToAll(int len, int type, const char *message)
{
	this->mConnectionManager.sendToAll(len, type, message);
}
void Peer::sendTo(int id, int len, int type, const char *message)
{
	SOCKET *soc = NULL;
	soc = this->mIdManager.getSocketByID(id);
	if(soc)
		this->mConnectionManager.sendTo(*soc, len, type, message);
}
void Peer::sendTo(string name, int len, int type, const char *message)
{
	SOCKET *soc = NULL;
	soc = this->mIdManager.getSocketByName(name);
	if(soc)
		this->mConnectionManager.sendTo(*soc, len, type, message);
}
void Peer::addMessageHandler(MessageHandler *msgH)
{
	this->mConnectionManager.addMessageHandler(msgH);
}
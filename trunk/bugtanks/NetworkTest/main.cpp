#include <iostream>
#include <string>
#include <map>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "../Network/Peer.h"
#include "../Network/IDHandler.h"
#include "../Utilities/Macros.h"

using namespace std;

class MsgHandler
{
private:

public:
	MsgHandler(){}
	~MsgHandler(){}
	void handle(PeerConnection *peer, int type, char *message, int len)
	{
		char *name;
		int port;
		peer->getPeerAddress(&name, &port);
		switch(type)
		{
		case 4: // ID request
			
			break;
		case 5: // Text message
			string msg(message);
			msg = msg.substr(0, len);
			cout << name << ":" << port << " says: " << msg << endl;
			break;
		}
	}
};

class Interface : public Peer
{
private:
	MsgHandler *mMsgHandler;

public:
	Interface(int port): Peer(port){ mMsgHandler = new MsgHandler(); }
	~Interface()
	{
		delete mMsgHandler;
	}

	void handleConnection(Socket peerSoc)
	{
		Peer::handleConnection(peerSoc);
	}
	void SendToAll(string message, int len)
	{
		list<PeerConnection*>::iterator it;
		for(it = peerList.begin(); it != peerList.end(); it++)
		{
			(*it)->sendMessage(len, 5, message.c_str());
		}
	}

	virtual void handleMessage(PeerConnection *peer, int type, char *message, int len)
	{
		Peer::handleMessage(peer, type, message, len);
		mMsgHandler->handle(peer, type, message, len);
	}
};

void Chat(Interface *Iface)
{
	cout << "--------------------------------------------------" << endl
		 << " Chat" << endl
		 << "--------------------------------------------------" << endl;
	bool done=false;
	string message;
	while(!done)
	{
		cout << "Say something: ";
		cin >> message;

	
		if(message.compare("quit")==0)
			done = true;

		Iface->SendToAll(message, message.length());
	}
}

int main(int nArgs, char* args[])
{
	string name;
	int port = 68000;
	bool host = true;
	InitializeSockets();

	Interface *peer;

	for(int i=1; i<nArgs; i++)
	{
		string arg(args[i]);
		size_t found;

		found = arg.rfind("host=");
		if(found!=string::npos)
		{
			cout << "host!!" << endl;
			name = arg.substr(found+string("host=").length()).c_str();
			host=false;
		}
	}

	//--------------------------------------------------
	//	Host
	//--------------------------------------------------
	if(host)
	{
		peer = new Interface(port);
	}
	//--------------------------------------------------
	//	Client
	//--------------------------------------------------
	else
	{
		peer = new Interface(0);
		peer->join(name.c_str(), port);
	}

	int nLoops = 3;
	while(nLoops)
	{
		
		system("cls"); // clear screen!

		// present publish list
		cout << "--------------------------------------------------" << endl
			 << " Connection list" << endl
			 << "--------------------------------------------------" << endl;
			 if(peer->peerList.empty())
				 cout << "list is empty!" << endl << endl;
			 else
			 {
				 char *addr;
				 int port;
				 for(list<PeerConnection*>::iterator it = peer->peerList.begin(); it != peer->peerList.end(); it++)
				 {
					 (*it)->getPeerAddress(&addr, &port);
					 cout << addr << ":" << port << endl;
				 }
			 }

		system("pause");
		nLoops--;
	};

	Chat(peer);

	delete peer;
	ShutdownSockets();

	return 0;
}

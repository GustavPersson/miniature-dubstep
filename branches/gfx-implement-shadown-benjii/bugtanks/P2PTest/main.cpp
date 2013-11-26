#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "../Network/Network.h"

using namespace std;
using namespace network;

void joinNetwork(Peer *peer)
{
	int length;
  char * buffer;

  ifstream is;
  is.open ("publishList.txt", ios::binary );

  if(!is.is_open())
  {
	  cout << "unable to open publishList.txt" << endl;
  }

  // get length of file:
  is.seekg (0, ios::end);
  length = is.tellg();
  is.seekg (0, ios::beg);

  // allocate memory:
  buffer = new char [length];

  // read data as a block:
  is.read (buffer,length);
  is.close();

  peer->updateNetwork(buffer, length);

  peer->updateNetwork(buffer, length);

  peer->updateNetwork(buffer, length);
  //delete[] buffer;
}
void writeMessages(Peer *peer)
{
	char text[256];
	cout << "write something: ";
	cin.getline(text, 256);

	peer->sendToAll(strlen(text), MessageType_TextMessage, text);
}
int main(int nargs, const char *args[])
{
	InitializeNetwork();

	bool host=false;

	if(nargs<3)
	{
		cout << "insuficent arguments" << endl;
		cout << "usage: ServerClient.exe name port" << endl;
		return 0;
	}

	string name;
	int port;

	name = args[1];
	port = atoi(args[2]);

	Peer peer(name, port);
	
	//MessageHandlers
	MessageHandler *msgServer, *msgClient;

	// Setup peer to handle messages from game server.
	msgClient = new MsgH_GameClient(&peer);
	peer.getServerManager()->addMessageHandler(msgClient);


	char cval;
	cout << "Host or Join (h/j): ";
	cin >> cval;
	if(cval=='h') host=true;

	//setup host
	if(host)
	{
		string serverName;
		cout << "Enter server name: ";
		cin >> serverName;

		// Setup peer to handle messages from clients.
		msgServer = new MsgH_GameServer(serverName, peer.getPeerManager());
		peer.getPeerManager()->addMessageHandler(msgServer);
		
		// Join server.
		peer.joinNetwork("localhost", port);
		
	}
	else
	{
		string serveraddr;
		int serverPort;

		cout << "Enter host address (addr port): ";
		cin >> serveraddr;
		cin >> serverPort;
		
		// Join server.
		peer.joinNetwork(serveraddr, serverPort);
	}

	// register a text message handler to peer.
	MsgH_TextMessage msgh_text(peer.getIdManager());
	peer.getPeerManager()->addMessageHandler(&msgh_text);

	//system("pause");

	//joinNetwork(&peer);
	
	while(true)
	{
		writeMessages(&peer);
	}

	ShutdownNetwork();

	system("pause");
	return 0;
}

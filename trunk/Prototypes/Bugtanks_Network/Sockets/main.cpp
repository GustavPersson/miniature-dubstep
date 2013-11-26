#include <iostream>
#include <cstdlib>
#include "Network.h"

using namespace std;


bool startGame=false;

class ClientMessageHandler : public MessageHandler
{
public:

	ClientMessageHandler():MessageHandler()
	{

	}
	void handleMessage(SOCKET src, int len, int type, const char *message)
	{
		cout << "len = " << len << endl;
		cout << "type = " << type << endl;
		cout << message << endl;
	}
};

void doGameServer(string serverName, int port)
{
	ConnectionManager CM;
	MessageHandler *clientMsgH = new ClientMessageHandler();

	CM.addMessageHandler(clientMsgH);

	ServerGroup group("234.5.6.7", port);
	group.setConnectionManager(&CM);
	group.setGreetingMessage(serverName);
	group.join();
	
	int nConnections = CM.getNumberOfConnections();
	while(!startGame)
	{
		if(nConnections != CM.getNumberOfConnections())
		{
			nConnections = CM.getNumberOfConnections();
			cout << "number of connections = " << nConnections << endl;
		}
	}
	group.drop();


	// send participant list to all players.
}

ConnectionManager* GameServerLookup()
{
	/*
	int port = 9876;
	ConnectionManager *CM = new ConnectionManager();
	Connector connector(CM);
	connector.startAccept(port);

	ServerGroup group("234.5.6.7", port);
	group.multicast((char*)&port, sizeof(port));
	*/
	return NULL;
}
void doClient()
{

}

int main(int nargs, const char *args[])
{
	InitializeNetwork();
	
	SOCKET soc1 = socket(AF_INET, 0, IPPROTO_TCP);
	SOCKET soc2 = socket(AF_INET, 0, IPPROTO_TCP);
	SOCKET soc3 = socket(AF_INET, 0, IPPROTO_TCP);
	SOCKET soc4 = socket(AF_INET, 0, IPPROTO_TCP);

	IdManager idm;
	idm.registerID(soc1, 4, "Olle", "192.168.0.1", 87473);
	idm.registerID(soc2, 3, "Adrian", "192.168.0.2", 56564);
	idm.registerID(soc3, 1, "Kebab", "192.168.0.3", 35438);
	idm.registerID(soc4, 2, "Nicklas", "192.168.0.4", 12345);

	cout << idm << "--------------------------------------------" << endl;
	idm.setIdsToIndexValue();
	
	cout << idm << "--------------------------------------------" << endl;

	idm.removeID(*idm.getSocketByName("Kebab"));

	cout << idm << "--------------------------------------------" << endl;
	/*
	bool host = false;
	
	for(int i=0; i<nargs; i++){
		if(string(args[i]).compare("host")==0)
		{
			cout << "I'm a host" << endl;
			host = true;
		}
	}

	ConnectionManager CM;
	Connector connector(&CM);
	//connector.startAccept(9876);


	if(host)
	{
		connector.startAccept(9876);
		int nConnections = CM.getNumberOfConnections();
		while(true)
		{
			if(nConnections != CM.getNumberOfConnections())
			{
				nConnections = CM.getNumberOfConnections();
				cout << "number of connections = " << nConnections << endl;
			}
		}
	}
	else
	{
		if(connector.connectTo("localhost", 9876))
		{
			cout << "Connection successful established!" << endl;
		}
		while(true)
		{

		}
	}

	*/


	/*
	ServerGroup group("234.5.6.7", 89542);
	group.setConnectionManager(&CM);
	group.join();
	
	int port=172837;
	while(true)
	{
		group.multicast((char*)&port, sizeof(port));
	};

	group.drop();
	*/


	ShutdownNetwork();
	system("pause");
}

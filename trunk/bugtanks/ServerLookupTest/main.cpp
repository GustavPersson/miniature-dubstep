#include <iostream>
#include <sstream>
#include <string>
#include <cstdlib>

#include "../Network/Network.h"

using namespace std;
using namespace network;

int main(int nargs, const char *args[])
{
	InitializeNetwork();

	bool host=false;
	char cval;
	cout << "Host or Join (h/j): ";
	cin >> cval;
	if(cval=='h') host=true;

	ServerLookup SL;
	
	if(host)
	{
		string serverName;
		int serverPort;
		cout << "Enter server name (name port): ";
		cin >> serverName >> serverPort;
		

		SL.registerServer(serverName, serverPort);
		while(true)
		{

		}
	}
	else
	{
		//SL.startLookup(6668);
		SL.sendLookupMessage();
	
		int tick=0;
		while(true)
		{
			if(tick==999999999)
			{
				tick=0;
				SL.sendLookupMessage();

				system("cls");
				cout << "Active Servers:" << endl;

				vector<ServerInfo> servers = SL.getServerList();

				for(unsigned int i=0; i < servers.size(); i++)
				{
					cout << servers.at(i).name << " " << servers.at(i).addr << " " << servers.at(i).port << endl;
				}

			}
			tick++;
		}

	}
// 	SL.stopLookup();

	ShutdownNetwork();

	system("pause");
	return 0;
}

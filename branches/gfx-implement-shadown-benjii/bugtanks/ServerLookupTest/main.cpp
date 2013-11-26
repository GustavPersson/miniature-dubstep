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

	if(host)
	{
		string serverName;
		int serverPort;
		cout << "Enter server name (name port): ";
		cin >> serverName >> serverPort;

		// Compile Greeting Message
		ostringstream servInfo(ios_base::in | ios_base::out);
		servInfo << serverName << " " << serverPort;

		ServerGroup SG("234.5.6.7", 9633);
		SG.setGreetingMessage(servInfo.str());
		SG.join();
		if(!SG.isMember()) cout << "Could not join server group!" << endl;
		else
		{
			while(true)
			{

			}
		}
	}
	else
	{

		ServerLookup SL;

		SL.startLookup(9998);
		SL.sendLookupMessage();

		system("pause");

		vector<ServerInfo> servers = SL.getServerList();
		cout << "number of servers found: " << servers.size();

		SL.stopLookup();

		for(int i=0; i<servers.size(); i++)
		{
			cout << servers.at(i).name << " " << servers.at(i).addr << " " << servers.at(i).port << endl;
		}
	}

	ShutdownNetwork();

	system("pause");
	return 0;
}

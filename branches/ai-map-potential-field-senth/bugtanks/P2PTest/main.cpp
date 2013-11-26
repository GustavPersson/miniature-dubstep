#include "../Utilities/Macros.h"
#include "TestMsgHandler.h"
#include <iostream>
#include "windows.h"
#include <fstream>
#include <string.h>
#include <sstream>
#include <cstdlib>


#include <strstream>

#include "../Network/Network.h"



using namespace network;
using namespace std;


int main(int nargs, const char *args[])
{
	setVerbosityLevel(LEVEL_LOWEST, OUTPUT_CONSOLE);

	int hostPort=4567, port;
	string playername, hostaddr("localhost");
	int host=0;

	if(nargs < 4)
	{
		cout << "this program does not take " << nargs << " arguments" << endl;
		cout << "Correct parameters are folloing: <<PlayerName>> <<port>> <<host(= 1 or 0)>>" << endl;
		system("pause");
		return 0;
	}
	if(nargs==5)
	{
		hostaddr = args[4];
	}

	playername = string(args[1]);
	port = atoi(args[2]);
	host = atoi(args[3]);

	cout << "Playername = " << playername << "  port = " << port << "  host = " << host << endl;

	if(InitializeNetwork()==false)
	{
		cout << "Unable to initialize network" << endl;
		ShutdownNetwork();
		return 0;
	}

	Network *mNetwork=NULL;
	if(host==1)
	{
		mNetwork = new Network(hostPort);
		mNetwork->setProfile(playername);

		string servername = playername;
		servername.append("'s Server");
		mNetwork->startServer(servername, 4);
	}
	else
	{
		mNetwork = new Network(port);
		mNetwork->setProfile(playername);
	}

	
	mNetwork->join(hostaddr, hostPort);

	TestMsgHandler *mEventMsgHandler = myNew TestMsgHandler();
	mNetwork->getPeer()->getPeerManager()->addMessageHandler(mEventMsgHandler);
	
	if(host==1)
	{
		int nIDs=0;
		bool running = true;
		while(running)
		{
			if(IdManager::getInstance()->size()!=nIDs)
			{
				cout << "----Registered ID's------" << endl << IdManager::getInstance();
				nIDs = IdManager::getInstance()->size();
			}
			if (IdManager::getInstance()->size() > 2)
			{
				cout << "Stopping server!" << endl;
				mNetwork->stopServer();
				
				for (int i = 0; i < 100; i++)
				{	
					std::ostrstream out;
					
					out << i << " ";
					out << "\r\n";

					mNetwork->getPeer()->sendToAll(out.pcount(), static_cast<int>(MessageType_Test), out.rdbuf()->str());
				}
				running = false;
			}
		}
	}
	system("pause");

	delete mNetwork;
	ShutdownNetwork();
	

	system("pause");
	return 0;
}

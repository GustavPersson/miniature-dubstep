#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>

#include "Peer.h"


#include "../Sockets/Network.h"


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

  peer->joinNetwork(buffer, length);
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

	cout << " name = " << name << ", port = " << port << endl;

	Peer peer(name, port);

	system("pause");

	joinNetwork(&peer);
	
	while(true)
	{
		writeMessages(&peer);
	}


	ShutdownNetwork();

	system("pause");
	return 0;
}

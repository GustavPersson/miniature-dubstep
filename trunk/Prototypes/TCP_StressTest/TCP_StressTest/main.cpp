#include <iostream>
#include <string>
#include <cstdlib>
#include <pthread.h>
#include <time.h>
#include "Socket.h"

using namespace std;

int nMessages = 36000;
int len = 20;
char buffer[20];
string message("STRESSS!");

clock_t g_startTime, g_endTime;

void startTimer()
{
	g_startTime = clock();
}
void endTimer()
{
	g_endTime = clock();
	g_endTime = g_endTime - g_startTime;
}
void printTime()
{
	float time;
	time = (float)g_endTime/(float)CLOCKS_PER_SEC;
	cout << "Timer: " << time << " sec"<< endl;
}

pthread_t thread1, thread2;

void *sending_func( void *ptr );
void *receiving_func( void *ptr );


int main(int nArgs, char* args[])
{
	int port = 20000;
	string name = "localhost";
	bool Server = true;

	for(int i=1; i<nArgs; i++)
	{
		string arg(args[i]);
		size_t found;
		found = arg.rfind("nMessages=");
		if(found!=string::npos)
		{
			nMessages = atoi(arg.substr(found+string("nMessages=").length()).c_str());
			cout << "nMessages = " << nMessages << endl;
		}

		found = arg.rfind("port=");
		if(found!=string::npos)
		{
			port = atoi(arg.substr(found+string("port=").length()).c_str());
			cout << "port = " << port << endl;
		}

		found = arg.rfind("host=");
		if(found!=string::npos)
		{
			name = arg.substr(found+string("host=").length()).c_str();
			cout << "host = " << port << endl;
		}

		found = arg.rfind("-Client");
		if(found!=string::npos)
		{
			Server=false;
		}
	}

	InitializeSockets();

	if(Server)	// Server
	{
		Socket listener, client;
		listener.ListenTo(port);
		if(listener.WaitToAccept(client)!=NETWORK_ERROR)
		{
			cout << "Client has connected!" << endl;
			startTimer();
			pthread_create( &thread1, NULL, sending_func, (void*) &client);
			pthread_create( &thread2, NULL, receiving_func, (void*) &client);
			/*
			int brecv;
			int data;
			do{
				brecv = client.Receive((char*)&data, sizeof(int));
				cout << "Receiveing: \"" << data << "\" - bytes reveived=" << brecv << endl;
			}while(brecv > 0);
			*/
		}

	}
	else	// Client
	{
		Socket serverSock;
		char *host = new char[name.length()];
		memcpy(host, name.c_str(), name.length()+1);
		cout << host << endl;
		if(serverSock.ConnectTo(host, port)==NETWORK_OK)
		{
			startTimer();
			pthread_create( &thread1, NULL, sending_func, (void*) &serverSock);
			pthread_create( &thread2, NULL, receiving_func, (void*) &serverSock);
			/*
			int bsent;
			for(int i=0; i<nMessages; i++)
			{
				bsent = serverSock.Send((char*)&i, sizeof(int));
				cout << "Sending: \"" << i << "\" - bytes sent=" << bsent << endl;
			}
			*/
		}
	}

	pthread_join( thread1, NULL);
    pthread_join( thread2, NULL);
	endTimer();
	printTime();

	system("pause");
	ShutdownSockets();
	return 0;
}

void *sending_func( void *ptr )
{
	cout << "sending initiated" << endl;
	Socket *socket = (Socket*)ptr;
	for(int i=0; i<nMessages; i++)
	{
		socket->Send((char*)&i, sizeof(int));
	}
	int exit = -1;
	socket->Send((char*)&exit, 4);

	cout << "sending terminated" << endl;
	return NULL;
}
void *receiving_func( void *ptr )
{
	cout << "receiving initiated" << endl;
	Socket *socket = (Socket*)ptr;

	int brecv;
	int data;
	do{
		brecv = socket->Receive((char*)&data, sizeof(int));
		//cout << "Receiveing: \"" << data << "\" - bytes reveived=" << brecv << endl;
		if(data == -1){
			//cout << "Exit receiver func" << endl;
			break;
		}
	}while(brecv > 0);

	/*
	while(socket->Receive(buffer, len)==NETWORK_OK)
	{
		//cout << "---------------------" << endl;
		cout << buffer << endl;
		//Check for exit command
		if(string(buffer).compare("exit\0")==0)
		{
			cout << "exit found!" << endl;
			cout << "receiving terminated" << endl;
			return NULL;
		}
	}
	*/
	cout << "receiving terminated" << endl;
	return NULL;
}

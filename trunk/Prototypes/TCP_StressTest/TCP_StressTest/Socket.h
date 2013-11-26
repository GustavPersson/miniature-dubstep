#ifndef _SOCKET_H_
#define _SOCKET_H_

#include <iostream>
using namespace std;

#define NETWORK_ERROR -1
#define NETWORK_OK     0

#define PLATFORM_WINDOWS  1
#define PLATFORM_MAC      2
#define PLATFORM_UNIX     3

#if defined(_WIN32)
#define PLATFORM PLATFORM_WINDOWS
#elif defined(__APPLE__)
#define PLATFORM PLATFORM_MAC
#else
#define PLATFORM PLATFORM_UNIX
#endif


#if PLATFORM == PLATFORM_WINDOWS

#include <winsock.h>

#elif PLATFORM == PLATFORM_MAC || PLATFORM == PLATFORM_UNIX

#include <sys/socket.h>
#include <netinet/in.h>
#include <fcntl.h>

#endif


inline bool InitializeSockets()
{
	#if PLATFORM == PLATFORM_WINDOWS
    WSADATA WsaData;
    return WSAStartup( MAKEWORD(1,1), &WsaData ) == NO_ERROR;
    #else
    return true;
    #endif
}

inline void ShutdownSockets()
{
	#if PLATFORM == PLATFORM_WINDOWS
    WSACleanup();
    #endif
}

class Socket
{
private:
	SOCKET mSocket;
public:
	Socket();

	int ConnectTo(char *hostName, int port); // Connect to host by name.
	
	int Send(const char *buff, int len, int flags=0);
	int Receive(char *buff, int len, int flags=0);

	int ListenTo(int port);	// Setup the port to listen for incoming connections.
	int WaitToAccept(Socket &sock);	// Wait until a connection appears and accepts it.

	void Close(); //Close the socket.
};


#endif
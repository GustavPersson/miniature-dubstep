#include "NetworkUtils.h"

//using namespace network;

void network::getPeerAddress(SOCKET soc, char **addr, int *port)
{
	sockaddr *socAddr = new sockaddr();
	int nLengthAddr = 16;
	getpeername(soc, socAddr, &nLengthAddr);
	*addr = inet_ntoa(((sockaddr_in*)socAddr)->sin_addr);
	if(port)
		*port = ntohs(((sockaddr_in*)socAddr)->sin_port);
}
#include "../Utilities/Macros.h"
#include "NetworkUtils.h"

std::string network::getPeerAddress(SOCKET soc, int *port)
{
	std::string address;

	sockaddr *socAddr = myNew sockaddr;
	int nLengthAddr = 16;
	getpeername(soc, socAddr, &nLengthAddr);
	address = inet_ntoa(((sockaddr_in*)socAddr)->sin_addr);
	if(port)
		*port = ntohs(((sockaddr_in*)socAddr)->sin_port);

	SAFE_DELETE(socAddr);
	return address;
}
#ifndef _CLIENTMESSAGEHANDLER_H_
#define _CLIENTMESSAGEHANDLER_H_

#include "../Utilities/Macros.h"

#include <winsock.h>
#include <strstream>
#include "GameServer.h"
#include "MessageHandler.h"
#include "../Logic/IEventManager.h"

namespace network
{

class GameServer;

class ClientMessageHandler : public MessageHandler
{
private:
	GameServer *mServer;
public:
	ClientMessageHandler(GameServer *server);
	void handleMessage(SOCKET src, int len, int type, const char *message);
	void createEvent(std::istrstream &in);
};
}
#endif

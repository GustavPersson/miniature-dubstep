#ifndef __KEY_HANDLER_H__
#define __KEY_HANDLER_H__

#include <stack>
#include <windows.h>

typedef int KeyType;

const int MAX_PLAYERS = 4;
const KeyType INVALID_KEY = -1;

class KeyHandler
{
public:
	~KeyHandler();
	static KeyHandler* getInstance(int client);
	KeyType allocateKey(int player);
	void freeKey(KeyType freeKey);

private:
	KeyHandler(int client);

	static KeyHandler* mpsInstances[MAX_PLAYERS];

	int mClient;
	std::stack<KeyType>	mFreeKeys[MAX_PLAYERS];
};

#endif
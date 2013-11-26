#include <iostream>
using namespace std;


#include "ConnectionManager.h"


void *ConnectionManager::socketFunc(void* args)
{
	connection_t *connection = (connection_t*)args;
	int len = 256;
	char buff[256];

	int brecv;
	do{
		/* If no error occurs, recv returns the number of bytes received.
		*  If the connection has been gracefully closed, the return value is zero.
		*/
		brecv = recv(connection->soc, buff, len, 0);

		if(brecv<=0) // If connection is lost we jump out of this loop and remove the connection.
			break;
	
		// iterate trough all the messages in the buffer and dispatch them.
		connection->manager->handleMessages(connection->soc, buff, brecv);

	// Continue to loop while connection is fine.
	}while(brecv > 0);
	
	connection->manager->removeConnection(connection->soc);

	pthread_exit(NULL);
	return NULL;
}
void ConnectionManager::handleMessages(SOCKET src, char *messagebuffer, unsigned int len)
{
	if(len == 0)
		return;

	msgHeader_t *header = (msgHeader_t*)&messagebuffer[0];
	unsigned int headerSize = sizeof(msgHeader_t);
	unsigned int totalMessageLenght = header->len+headerSize;

	if(totalMessageLenght<=len)
	{
		list<MessageHandler*>::iterator msgH;
		for(msgH = mMessageHandlers.begin(); msgH != mMessageHandlers.end(); msgH++)
			(*msgH)->handleMessage(src, header->len, header->type, &messagebuffer[headerSize]);

		handleMessages(src, &messagebuffer[len], len-totalMessageLenght);
	}
	else
	{
		// The length of the message exeeds the lengh of the package. This is an error.
	}
}
ConnectionManager::ConnectionManager()
{

}

void ConnectionManager::addConnection(SOCKET soc)
{
	connection_t *connection = new connection_t;
	connection->soc = soc;
	connection->manager = this;
	pthread_create(&connection->socketListener, NULL, socketFunc, connection);
	mConnections.push_back(connection);
}
void ConnectionManager::removeConnection(SOCKET soc)
{
	list<connection_t*>::iterator it;
	for(it = mConnections.begin(); it != mConnections.end(); it++)
	{
		if((*it)->soc == soc)
		{
			it = mConnections.erase(it);
			break;
		}
	}
}
void ConnectionManager::addMessageHandler(MessageHandler *msgH)
{
	mMessageHandlers.push_back(msgH);
}
void ConnectionManager::removeMessageHandler(MessageHandler *msgH)
{
	mMessageHandlers.remove(msgH);
}
void ConnectionManager::sendToAll(int len, int type, const char* message)
{
	// compile message
	char *msg = new char[len+sizeof(msgHeader_t)];
	msgHeader_t *header = (msgHeader_t*)&msg[0];
	header->len = len;
	header->type = type;
	
	for(int i=0; i<len; i++)
		msg[sizeof(msgHeader_t)+i]=message[i];
	
	// For all sockets, send the message.
	list<connection_t*>::iterator it;
	for(it = mConnections.begin(); it != mConnections.end(); it++)
	{
		send((*it)->soc, msg, len+sizeof(msgHeader_t), 0);
	}
}
void ConnectionManager::sendTo(SOCKET dest, int len, int type, const char* message)
{
	// compile message
	char *msg = new char[len+sizeof(msgHeader_t)];
	msgHeader_t *header = (msgHeader_t*)&msg[0];
	header->len = len;
	header->type = type;
	
	for(int i=0; i<len; i++)
		msg[sizeof(msgHeader_t)+i]=message[i];

	// Send message
	send(dest, msg, len+sizeof(msgHeader_t), 0);
}
int ConnectionManager::getNumberOfConnections()const
{
	return mConnections.size();
}
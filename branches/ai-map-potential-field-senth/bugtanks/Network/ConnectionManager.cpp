#include <iostream>

#include "../Utilities/Macros.h"
#include "../Logic/IEventManager.h"
#include "../Logic/Events.h"
#include "IdManager.h"
#include "Protocol.h"
#include "ConnectionManager.h"

using namespace std;
using namespace network;

void *ConnectionManager::socketFunc(void* args)
{
	pthread_setcancelstate (PTHREAD_CANCEL_ENABLE, NULL);
	pthread_setcanceltype (PTHREAD_CANCEL_ASYNCHRONOUS, NULL);

	connection_t *connection = (connection_t*)args;
	holdingBuffer_t holdingBuffer;
	holdingBuffer.bytesHeld=0;

	int len = 512;
	char buff[512];
	int bytes_read;

	int brecv;
	do{


		bytes_read=0;

		// Copy any data remaining from previous call into buff.
		if(holdingBuffer.bytesHeld > 0)
		{
			memcpy(buff, holdingBuffer.buffer, holdingBuffer.bytesHeld);
			bytes_read = holdingBuffer.bytesHeld;
			holdingBuffer.bytesHeld = 0;
		}

		/* If no error occurs, recv returns the number of bytes received.
		*  If the connection has been gracefully closed, the return value is zero.
		*/
		brecv = recv(connection->soc, buff+bytes_read, len-bytes_read, 0);
		bytes_read += brecv;

		if(brecv<=0) // If connection is lost we jump out of this loop and remove the connection.
		{
			if(brecv!=0)
			{
				int error = WSAGetLastError();

				switch(error)
				{
				case WSAECONNRESET: // Connection was forcibly closed by the remote host.
					DEBUG_MESSAGE(LEVEL_HIGH, "Connection Error(" << error << "): Connection was forcibly closed by the remote host.");
					break;
				case WSAECONNABORTED: // Software caused connection abort.
					DEBUG_MESSAGE(LEVEL_HIGH, "Connection Error(" << error << "): Software caused connection abort.");
					break;

				default:
					DEBUG_MESSAGE(LEVEL_LOWEST, "[ConnectionManager::socketFunc]: Conection was succesfully terminated!");
				}
			}
			break;
		}
	
		// iterate trough all the messages in the buffer and dispatch them.
		connection->manager->handleMessages(connection->soc, buff, bytes_read, &holdingBuffer);

	// Continue to loop while connection is fine.
	}while(brecv > 0);

	//WSAECONNRESET
	
	// Send dropout message to all message handlers.
	list<MessageHandler*>::iterator msgH;
		for(msgH = connection->manager->mMessageHandlers.begin(); msgH != connection->manager->mMessageHandlers.end(); msgH++)
			(*msgH)->handleMessage(connection->soc, 0, MessageType_Dropout, NULL);

	//closesocket(connection->soc);
	pthread_exit(NULL);
	return NULL;
}
void ConnectionManager::handleMessages(SOCKET src, char *messagebuffer, unsigned int len, holdingBuffer_t *holdingBuffer)
{
	if(len == 0)
		return;

	unsigned int headerSize = sizeof(msgHeader_t);
	if(headerSize>len)
	{
		holdingBuffer->bytesHeld = len;
		memcpy(holdingBuffer->buffer, &messagebuffer[0], holdingBuffer->bytesHeld);
		return;
	}
	
	msgHeader_t *header = (msgHeader_t*)&messagebuffer[0];
	unsigned int totalMessageLenght = header->len+headerSize;

	if(totalMessageLenght>len)
	{
		holdingBuffer->bytesHeld = len;
		memcpy(holdingBuffer->buffer, &messagebuffer[0], holdingBuffer->bytesHeld);
		return;
	}
	else
	{
		list<MessageHandler*>::iterator msgH;
		for(msgH = mMessageHandlers.begin(); msgH != mMessageHandlers.end(); msgH++)
			(*msgH)->handleMessage(src, header->len, header->type, &messagebuffer[headerSize]);

		handleMessages(src, &messagebuffer[totalMessageLenght], len-totalMessageLenght, holdingBuffer);
	}
}
ConnectionManager::ConnectionManager()
{
	// Set thread detached attribute.
	pthread_attr_init(&mListenerAttribute);
	pthread_attr_setdetachstate(&mListenerAttribute, PTHREAD_CREATE_JOINABLE);

	// Init mutex.
	pthread_mutex_init(&mMutex, NULL);
}

ConnectionManager::~ConnectionManager()
{
	pthread_attr_destroy(&mListenerAttribute);

	if(this->mConnections.empty() == false)
	{
		this->closeAll();
	}

	// Delete all registered message handlers.
	//pthread_mutex_lock(&mMutex);
	list<MessageHandler*>::iterator msgH;
	for(msgH = mMessageHandlers.begin(); msgH != mMessageHandlers.end(); msgH++)
	{
		SAFE_DELETE((*msgH)); // Call the destructor of the message handler
	}
	mMessageHandlers.clear();
	//pthread_mutex_unlock(&mMutex);

	pthread_mutex_destroy(&mMutex);
}

void ConnectionManager::addConnection(SOCKET soc)
{
	pthread_mutex_lock(&mMutex);

	connection_t *connection = myNew connection_t;
	connection->soc = soc;
	connection->manager = this;
	pthread_create(&connection->socketListener, &mListenerAttribute, socketFunc, connection);
	mConnections.push_back(connection);

	pthread_mutex_unlock(&mMutex);
}
void ConnectionManager::removeConnection(SOCKET soc)
{
	list<connection_t*>::iterator connection;
	for(connection = mConnections.begin(); connection != mConnections.end(); connection++)
	{
		if((*connection)->soc == soc)
		{
			if(closesocket((*connection)->soc)!=0)
			{
				DEBUG_MESSAGE(LEVEL_LOWEST, "[ConnectionManager::removeConnection] error(" << WSAGetLastError() << "): failed to close socket");
			}
			pthread_join((*connection)->socketListener, NULL);
			(*connection)->manager=NULL;
			SAFE_DELETE((*connection));
			this->mConnections.erase(connection);
			break;
		}
	}
}
void ConnectionManager::addMessageHandler(MessageHandler *msgH)
{
	pthread_mutex_lock(&mMutex);
	mMessageHandlers.push_back(msgH);
	pthread_mutex_unlock(&mMutex);
}
void ConnectionManager::removeMessageHandler(MessageHandler *msgH)
{
	pthread_mutex_lock(&mMutex);
	mMessageHandlers.remove(msgH); // Remove from list.
	SAFE_DELETE(msgH); // Delete object.
	pthread_mutex_unlock(&mMutex);
}
void ConnectionManager::sendToAll(int len, int type, const char* message)
{
	// compile message
	char *msg = myNew char[len+sizeof(msgHeader_t)];
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
	char *msg = myNew char[len+sizeof(msgHeader_t)];
	msgHeader_t *header = (msgHeader_t*)&msg[0];
	header->len = len;
	header->type = type;
	
	for(int i=0; i<len; i++)
		msg[sizeof(msgHeader_t)+i]=message[i];

	// Send message
	send(dest, msg, len+sizeof(msgHeader_t), 0);

	// Release allocated memory.
	SAFE_DELETE_ARRAY(msg);
}
int ConnectionManager::getNumberOfConnections()
{
	int result;

	pthread_mutex_lock(&mMutex);

	result = mConnections.size();

	pthread_mutex_unlock(&mMutex);

	return result;
}
void ConnectionManager::closeAll()
{
	list<connection_t*>::iterator connection;
	for(connection = mConnections.begin(); connection != mConnections.end(); connection++)
	{
		if(closesocket((*connection)->soc)!=0)
		{
			DEBUG_MESSAGE(LEVEL_LOWEST, "[ConnectionManager::closeAll] error(" << WSAGetLastError() << "): failed to close socket");
		}
	}
	for(connection = mConnections.begin(); connection != mConnections.end(); connection++)
	{
		pthread_join((*connection)->socketListener, NULL);
		(*connection)->manager=NULL;
		SAFE_DELETE((*connection));
	}
	mConnections.clear();
}
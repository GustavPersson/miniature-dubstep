#include "IdManager.h"

using namespace std;
using namespace network;

IdManager::IdManager()
{

}
void IdManager::registerID(SOCKET soc, int id, string name, string ip, int port)
{
	IDstructure container;
	container.socket=soc;
	container.id=id;
	container.name=name;
	container.ip=ip;
	container.port=port;
	ids.push_back(container);
}
void IdManager::removeID(SOCKET soc)
{
	vector<IDstructure>::iterator it;
	for(it=ids.begin(); it!=ids.end(); it++)
	{
		if((*it).socket == soc)
		{
			ids.erase(it);
			break;
		}
	}
}

SOCKET* IdManager::getSocketByID(int id)
{
	SOCKET *soc = NULL;
	vector<IDstructure>::iterator it;
	for(it=ids.begin(); it!=ids.end(); it++)
	{
		if((*it).id == id)
		{
			soc = &(*it).socket;
			break;
		}
	}
	return soc;
}
SOCKET* IdManager::getSocketByName(string name)
{
	SOCKET *soc = NULL;
	vector<IDstructure>::iterator it;
	for(it=ids.begin(); it!=ids.end(); it++)
	{
		if((*it).name.compare(name)==0)
		{
			soc = &(*it).socket;
			break;
		}
	}
	return soc;
}
int* IdManager::getIdBySocket(SOCKET soc)
{
	int *ID = NULL;
	vector<IDstructure>::iterator it;
	for(it=ids.begin(); it!=ids.end(); it++)
	{
		if((*it).socket == soc)
		{
			ID = &(*it).id;
			break;
		}
	}
	return ID;
}
string* IdManager::getNameBySocket(SOCKET soc)
{
	string *name = NULL;
	vector<IDstructure>::iterator it;
	for(it=ids.begin(); it!=ids.end(); it++)
	{
		if((*it).socket == soc)
		{
			name = &(*it).name;
			break;
		}
	}
	return name;
}
void IdManager::setIdsToIndexValue()
{
	int index=0;
	vector<IDstructure>::iterator it;
	for(it=ids.begin(); it!=ids.end(); it++)
	{
		(*it).id = index;
		index++;
	}
}
void IdManager::clear()
{
	ids.clear();
}
vector<IDstructure> IdManager::getIds()const
{
	return ids;
}
int IdManager::size()const
{
	return ids.size();
}

ostream& operator << (ostream &out, const IdManager &idManager)
{
	vector<IDstructure> ids = idManager.getIds();
	vector<IDstructure>::iterator it;
	for(it=ids.begin(); it!=ids.end(); it++)
	{
		out << (*it).id << " " << (*it).name << " " << (*it).ip << " " << (*it).port << endl;
	}
	return out;
}
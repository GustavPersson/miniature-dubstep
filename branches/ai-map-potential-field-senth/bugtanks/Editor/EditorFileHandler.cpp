/**
 * @file
 * @author Victor Lundberg <lundberg.victor@gmail.com>
 * @version 1.0
 * Copyright (©) A-Team.
 *
 * @section LICENSE
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of
 * the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
 * General Public License for more details at
 * http://www.gnu.org/copyleft/gpl.html
 */

#include "EditorFileHandler.h"

using namespace editor;
using namespace gfx;
using namespace logic;
using std::vector;
using std::fstream;
using std::ios;
using std::ios_base;
using std::string;
using std::map;

EditorFileHandler::EditorFileHandler() : mMapFile()
{

}

EditorFileHandler::EditorFileHandler(const EditorFileHandler &handler) : mMapFile()
{

}

EditorFileHandler::~EditorFileHandler()
{
	closeMapFile();
}

const EditorFileHandler &EditorFileHandler::operator=(const EditorFileHandler &handler)
{
	if(this != &handler)
	{

	}
	return *this;
}

bool EditorFileHandler::saveMap(const char *mapName, bool overwrite)
{
	if(!openMapFileSave(mapName, ios_base::out | ios_base::binary, overwrite))
	{
		ERROR_MESSAGE("Failed to open map file for saving. File: " << mapName)
		return false;
	}

	unsigned int location = 0;


	MapDataWorldPlane planeData;
	vector<D3DXVECTOR4> planeWeights;
	
	if(!EditorMapHandler::packWorldPlane(planeData, planeWeights))
	{
		return false;
	}

	//Write plane data.
	write((const char *)&planeData, sizeof(MapDataWorldPlane), location);

	//Write plane weights.
	for(vector<D3DXVECTOR4>::iterator weightIter = planeWeights.begin();
		weightIter != planeWeights.end(); ++weightIter)
	{
		write((const char *)&(*weightIter), sizeof(D3DXVECTOR4), location);
	}

	//Get the ActorHandler instance.
	ActorHandler *pActorHandler = ActorHandler::getInstance();

	MapDataActor actorData;
	map<KeyType, Actor *> actorMap = pActorHandler->getActorMap();
	map<KeyType, Actor *>::iterator actorIter = actorMap.begin();

	//Write the number of actors.
	unsigned int nActors = actorMap.size();
	write((const char *)&nActors, sizeof(unsigned int), location);

	//Write all actors to file.
	for(; actorIter != actorMap.end(); ++actorIter)
	{
		if(EditorMapHandler::packActor((*actorIter).second, actorData))
		{
			write((const char *)&actorData, sizeof(MapDataActor), location);
		}
	}

	closeMapFile();

	return true;
}

bool EditorFileHandler::loadMap(const char *mapName)
{
	if(!openMapFileLoad(mapName, ios_base::in | ios_base::binary))
	{
		ERROR_MESSAGE("Failed to open map file for loading. File: " << mapName)
		return false;
	}

	unsigned int location = 0;
	unsigned int n = 0;

	//Read world plane data.
	MapDataWorldPlane planeData;
	vector<D3DXVECTOR4> planeWeights;
	D3DXVECTOR4 weight;

	read((char *)&planeData, sizeof(MapDataWorldPlane), location);

	unsigned int nTiles = planeData.mTileX * planeData.mTileZ;
	unsigned int nTileVertices = planeData.mTileWidth * planeData.mTileHeight;
	unsigned int nWeights = nTiles * nTileVertices;
	for(n = 0; n < nWeights; ++n)
	{
		read((char *)&weight, sizeof(D3DXVECTOR4), location);
		planeWeights.push_back(weight);
	}

	//Update world plane.
	EditorMapHandler::updateWorldPlane(planeData, planeWeights);

	unsigned int nActors = 0;

	//Read the number of actors.
	read((char *)&nActors, sizeof(unsigned int), location);

	//Read all actors and send creation events.
	MapDataActor actorData;
	for(n = 0; n < nActors; ++n)
	{
		read((char *)&actorData, sizeof(MapDataActor), location);
		EditorMapHandler::createActorFromData(actorData);
	}

	closeMapFile();

	return true;
}

bool EditorFileHandler::read(char *pData, unsigned int dataSize, unsigned int &location)
{
	if(mMapFile.is_open())
	{
		mMapFile.seekg(location);
		mMapFile.read(pData, dataSize);
		location += dataSize;
		return true;
	}
	return false;
}

bool EditorFileHandler::write(const char *pData, unsigned int dataSize, unsigned int &location)
{
	if(mMapFile.is_open())
	{
		mMapFile.seekp(location);
		mMapFile.write(pData, dataSize);
		location += dataSize;
		return true;
	}
	return false;
}

bool EditorFileHandler::openMapFileSave(const char *mapName, std::ios_base::open_mode mode, bool overwrite)
{
	if(mMapFile.is_open())
	{
		mMapFile.close();
	}

	string mapPath = "../../Map/";
	mapPath += mapName;
	mapPath += ".btm";

	if(overwrite)
	{
		mMapFile.open(mapPath.c_str(), mode | ios_base::trunc);
	}
	else
	{
		mMapFile.open(mapPath.c_str(), ios_base::in);
		if(!mMapFile.is_open())
		{
			mMapFile.clear();
			mMapFile.open(mapPath.c_str(), mode | ios_base::trunc);
		}
		else
		{
			mMapFile.close();
		}
	}

	if(mMapFile.is_open())
	{
		return true;
	}
	return false;
}

bool EditorFileHandler::openMapFileLoad(const char *mapName, std::ios_base::open_mode mode)
{
	if(mMapFile.is_open())
	{
		mMapFile.close();
	}

	string mapPath = "../../Map/";
	mapPath += mapName;
	mapPath += ".btm";

	mMapFile.open(mapPath.c_str(), mode);

	if(mMapFile.is_open())
	{
		return true;
	}
	return false;
}

void EditorFileHandler::closeMapFile()
{
	if(mMapFile.is_open())
	{
		mMapFile.close();
	}
}
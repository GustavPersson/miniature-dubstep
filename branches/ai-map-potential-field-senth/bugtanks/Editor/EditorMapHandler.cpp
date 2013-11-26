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

#include "EditorMapHandler.h"

using namespace editor;
using namespace gfx;
using namespace logic;

EditorMapHandler::EditorMapHandler()
{

}

EditorMapHandler::EditorMapHandler(const EditorMapHandler &handler)
{

}

EditorMapHandler::~EditorMapHandler()
{

}

const EditorMapHandler &EditorMapHandler::operator=(const EditorMapHandler &handler)
{
	if(this != &handler)
	{

	}
	return *this;
}

void EditorMapHandler::updateWorldPlane(const MapDataWorldPlane &data, const std::vector<D3DXVECTOR4> &weights)
{
	WorldPlane *pPlane = DX10Module::getInstance()->getWorldPlane();
	if(!pPlane)
	{
		ERROR_MESSAGE("World plane not found. Attempts to skip...")
		return;
	}

	pPlane->setTileX(data.mTileX);
	pPlane->setTileZ(data.mTileZ);
	pPlane->setTileSizeX(data.mTileSizeX);
	pPlane->setTileSizeZ(data.mTileSizeZ);

	//Remove current tiles.
	pPlane->removeTiles();

	//Force plane update.
	pPlane->update();

	if(pPlane)
	{
		pPlane->setWeights(weights);
	}
}

void EditorMapHandler::createActorFromData(const MapDataActor &data)
{
	utilities::Position actorPos(data.mPosition.x, data.mPosition.y, data.mPosition.z);
	utilities::Direction actorDir(data.mDirection.x, data.mDirection.y, data.mDirection.z);
	utilities::Vec3Float actorScale(data.mScale.x, data.mScale.y, data.mScale.z); 
	switch(data.mType)
	{
		case Actor::ActorType_StaticEnvironment:
			safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment((StaticEnvironment::StaticEnvironmentTypes)data.mSubType, actorPos,
				actorDir, actorScale)));
			break;

		case Actor::ActorType_Enemy:
			safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy((Enemy::EnemyTypes)data.mSubType, actorPos, actorDir)));
			break;

		default:
			break;
	}
}

bool EditorMapHandler::packWorldPlane(MapDataWorldPlane &data, std::vector<D3DXVECTOR4> &weights)
{
	//Get the world plane.
	WorldPlane *pPlane = DX10Module::getInstance()->getWorldPlane();
	if(!pPlane)
	{
		ERROR_MESSAGE("World plane not found. Maps cannot be saved without a plane.")
		return false;
	}

	data.mPosition = pPlane->getPosition();
	data.mTileX = pPlane->getTileX();
	data.mTileZ = pPlane->getTileZ();
	data.mTileSizeX = pPlane->getTileSizeX();
	data.mTileSizeZ = pPlane->getTileSizeZ();

	InstanceShapePlane **pTiles = pPlane->getTiles();
	if(!pTiles)
	{
		ERROR_MESSAGE("No tiles found in world plane.")
		return false;
	}
	data.mTileWidth = pTiles[0]->getWidth();
	data.mTileHeight = pTiles[0]->getHeight();
	pPlane->getWeights(weights);
	return true;
}

bool EditorMapHandler::packActor(logic::Actor *pActor, MapDataActor &data)
{
	if(!pActor)
	{
		ERROR_MESSAGE("NULL pointer passed to pack method.")
		return false;
	}
	if(!pActor->getTransformObject())
	{
		ERROR_MESSAGE("NULL pointer retrieved from Actor TransformObject.")
		return false;
	}

	utilities::Position actorPos = pActor->getPosition();
	utilities::Direction actorDir = pActor->getDirection();
	data.mType = pActor->getActorType();
	data.mSubType = pActor->getActorSubType();
	data.mPosition = actorPos.convertToD3DXVECTOR3();
	data.mDirection = actorDir.convertToD3DXVECTOR3();
	data.mScale = pActor->getTransformObject()->getScale();

	return true;
}
/**
 * @file
 * @author Jonathan Udd <jonis.x@gmail.com>
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
 * 
 * @section DESCRIPTION
 *
 *	This class will be a layer between the InputHandler and and the logic. 
 *	There will be different states with different keybinds i.e. in the menu 
 *	and the actual game. This class will send events to the eventhandler 
 *	depending on which state and what keys are pressed.
 */


#include "ModelInfo.h"
#include "ActorHandler.h"

using namespace logic;

ModelInfo* ModelInfo::mpsInstance = NULL;

ModelInfo::ModelInfo()
{
	mpDXModule = NULL;
	mNrOfModels = 0;

	mpDXModule = gfx::DX10Module::getInstance();
}

ModelInfo::~ModelInfo()
{
	mpsInstance = NULL;
}

ModelInfo* ModelInfo::getInstance()
{
	if(mpsInstance == NULL)
	{
		mpsInstance = myNew ModelInfo();
	}
	return mpsInstance;
}

void ModelInfo::bindMeshToActorType(int actorType, int actorSubType, const std::string& modelName,
	const char *geometryFilePath, const char *materialFilePath, const char *weightFilePath)
{
	if(mLoadedModels.find(modelName) == mLoadedModels.end())
	{
		if(actorType == Actor::ActorType_StaticEnvironment)
		{
			mpDXModule->loadMeshInstance(geometryFilePath, materialFilePath, mNrOfModels, modelName.c_str(), 5000, false);
		}
		else
		{
			if(strcmp(weightFilePath, "") == 0)
			{
				mpDXModule->loadMeshInstance(geometryFilePath, materialFilePath, mNrOfModels, modelName.c_str(), 5000, true);
			}
			else
			{
				mpDXModule->loadAnimationMeshInstance(geometryFilePath,
					materialFilePath,
					weightFilePath,
					mNrOfModels, modelName.c_str(), 5000, true);
			}
		}
		DEBUG_MESSAGE(LEVEL_LOW, modelName << " Loaded.");
		mLoadedModels[modelName] = true;
		mNrOfModels++;
	}

	mModelNames[actorType][actorSubType] = modelName;

	switch(actorType)
	{
	// Bind modelname to driver
	case Actor::ActorType_Driver:
		if(actorSubType != 0)
		{
			ERROR_MESSAGE("Wrong drivertype provided in bindMeshToActorType() in ModelInfo, drivermodel may be loaded incorrect.");
		}
		break;
	// Bind modelname to enemies
	case Actor::ActorType_Enemy:
		if(actorSubType <= Enemy::EnemyType_Invalid && actorSubType >= Enemy::EnemyType_Lim)
		{
			ERROR_MESSAGE("Wrong enemytype provided in bindMeshToActorType() in ModelInfo, enemymodels may be loaded incorrect.");
		}
		break;
	// Bind modelname to munitions
	case Actor::ActorType_Munition:
		if(actorSubType <= Munitions::MunitionType_Invalid && actorSubType >= Munitions::MunitionType_Lim)
		{
			ERROR_MESSAGE("Wrong munitiontype provided in bindMeshToActorType() in ModelInfo, munitionmodels may be loaded incorrect.");
		}
		break;
	// Bind modelname to pedestrian
	case Actor::ActorType_Pedestrian:
		if(actorSubType != 0)
		{
			ERROR_MESSAGE("Wrong pedestriantype provided in bindMeshToActorType() in ModelInfo, pedestrianmodel may be loaded incorrect.");
		}
		break;
	// Bind modelname to StaticEnvironment
	case Actor::ActorType_StaticEnvironment:
		if(actorSubType <= StaticEnvironment::StaticEnvironmentType_Invalid && actorSubType >= StaticEnvironment::StaticEnvironmentType_Lim)
		{
			ERROR_MESSAGE("Wrong StaticEnvironmenttype provided in bindMeshToActorType() in ModelInfo, StaticEnvironmentmodel may be loaded incorrect.");
		}
		break;
	// Bind modelname to tank
	case Actor::ActorType_Tank:
		if(actorSubType <= Tank::TankType_Invalid && actorSubType >= Tank::TankType_Lim)
		{
			ERROR_MESSAGE("Wrong tanktype provided in bindMeshToActorType() in ModelInfo, tanktypemodel may be loaded incorrect.");
		}
		break;
	default:
		ERROR_MESSAGE("Wrong actortype provided in bindMeshToActorType() in ModelInfo, actortype may be loaded incorrect.");
	}
}

const char* ModelInfo::getModelName(int actorType, int actorSubType) const
 {
	const char* returnString = NULL;

	std::map<int, std::map<int, std::string> >::const_iterator it;
	it = mModelNames.find(actorType);
	if(it != mModelNames.end())
	{
		std::map<int, std::string>::const_iterator subIt;
		subIt = it->second.find(actorSubType);
		if(subIt != it->second.end())
		{
			returnString = subIt->second.c_str();
		}
	}
	assert(returnString != NULL);
	return returnString;
}
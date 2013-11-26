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
 */

#include "StaticEnvironment.h"
#include "../AI/AiFactory.h"
#include <climits>

using namespace logic;

StaticEnvironment::StaticEnvironment(int networkId, StaticEnvironmentTypes type, utilities::Position pos, utilities::Direction dir, utilities::Vec3Float scale) : StaticActor(pos, dir, networkId)
{
	mActorType = Actor::ActorType_StaticEnvironment;
	mType = type;
	mSpeed = 0.0f;
	mScale = scale;
	if(StaticEnvironmentType_CityMall == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("CityMall");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_CornerStore == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("CornerStore");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_GreenOffice == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("GreenOffice");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_LBuidling == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("LBuilding");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_Skyscraper == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("Skyscraper");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_StandAloneBuilding == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("StandAloneBuilding");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_Taipei == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("Taipei");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_TallGlassBuilding == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("TallGlassBuilding");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_TurningTorso == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("Turningtorso");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_WindowBuilding == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("WindowBuilding");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	else if(StaticEnvironmentType_WTFBuilding == type)
	{
		mpTransformObject = mpsDXModule->createMeshInstanced(ModelInfo::getInstance()->getModelName(mActorType, mType));
		mpTransformObject->setID(mId.convertToGfxId());
		mpTransformObject->setName("WTFBuilding");
		mpTransformObject->setPosition(D3DXVECTOR3(mPosition.x, mPosition.y, mPosition.z));
		dynamic_cast<gfx::InstanceMesh*>(mpTransformObject)->setScale(scale.convertToD3DXVECTOR3());
		mpTransformObject->setActor(this);
		float angle = mDirection.getZXAngleClockwise();
		mpTransformObject->setRotationY(angle);
	}
	mpTransformObject->setScale(scale.convertToD3DXVECTOR3());

	// HACK - Use a size of 3x3 for every static environment for now
	mSize.x = 3;
	mSize.y = 3;

// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
	// Add the enemy to the AiFactory
	ai::AiFactory* pAiFactory = ai::AiFactory::getInstance();
	if (pAiFactory != NULL)
	{
		pAiFactory->addBuilding(this);
	}
#endif
// ------------------------------
// AI Section END
// ------------------------------
}
StaticEnvironment::~StaticEnvironment()
{
// ------------------------------
// AI Section BEGIN
// ------------------------------
#ifdef USE_AI
	// Remove the enemy from the factory
	ai::AiFactory* pAiFactory = ai::AiFactory::getInstance();
	if (pAiFactory != NULL)
	{
		pAiFactory->removeBuilding(getKey());
	}
#endif
// ------------------------------
// AI Section END
// ------------------------------
}

void StaticEnvironment::update(float deltaTime)
{
	//Note: implement a "require update" check.
	gfx::OBB *pBV = (gfx::OBB *)getTransformObject()->getBoundingVolume();
	if(pBV)
	{
		mSize.x = (int)(pBV->getSizeX() * utilities::WORLD_IN_MAP_COORDS);
		mSize.y = (int)(pBV->getSizeZ() * utilities::WORLD_IN_MAP_COORDS);
	}
}

std::istream& operator>>(std::istream &in, logic::StaticEnvironment::StaticEnvironmentTypes& staticEnvironmentType)
{
	int intStaticEnvironmentType;
	in >> intStaticEnvironmentType;
	staticEnvironmentType = static_cast<logic::StaticEnvironment::StaticEnvironmentTypes>(intStaticEnvironmentType);
	return in;
}

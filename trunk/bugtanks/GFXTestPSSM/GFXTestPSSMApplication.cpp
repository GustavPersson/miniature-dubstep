/**
* @file
* @author Benjamin Golba <benjii.g@gmail.com>
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

#include "GFXTestPSSMApplication.h"
#include "../Logic/Actor.h"
#include "../Logic/Tank.h"

using namespace logic;

GFXTestPSSMApplication::GFXTestPSSMApplication()
{
	// TODO - Constructor
}

GFXTestPSSMApplication::~GFXTestPSSMApplication()
{
	// TODO - Destructor
}

void GFXTestPSSMApplication::createInstance()
{
	assert(mpsInstance == NULL);
	mpsInstance = myNew GFXTestPSSMApplication();
}

void GFXTestPSSMApplication::gameLoop(float deltaTime)
{
	Application::gameLoop(deltaTime);
}

void GFXTestPSSMApplication::loadMeshes()
{
	Application::loadMeshes();
	/*mpDxModule->loadObjectShapeInstance(gfx::ShapeTypes::CUBE, 1, "Cube", 5000);
	DEBUG_MESSAGE(LEVEL_LOW, "Cube Loaded");
	mpDxModule->loadObjectShapeInstance(gfx::ShapeTypes::CUBE, 2, "TriggerObject", 500);
	mpDxModule->getObjectInstanceBase("TriggerObject")->setFlagShadow(false);

	mpModelInfo->bindMeshToActorType(Actor::ActorType_Tank,	Tank::TankType_CloseCombat,	"CloseCombatTank",	"../../ModelFiles/TankAir/NewTankAirGeometry.btb",	"../../ModelFiles/TankAir/NewTankAirMaterial.btb", "");*/
}

void GFXTestPSSMApplication::createObjects()
{
	//Application::createObjects();
	int colorCounter;
	gfx::TransformObject *tempObj;

	for(int i = 0; i < 10; i++)
	{

		colorCounter = i;
		for(int j = 0; j < 10; j++)
		{
			int rnd = rand()%10;
			
			if(rnd == 0)
			{
			#ifdef USE_NETWORK
				if (mpGameOptions->mIsServer)
				{
					safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateStaticEnvironment(StaticEnvironment::StaticEnvironmentType_StandAloneBuilding, utilities::Position(-30.0f * j, 0.0f, -30.0f * i),utilities::Direction(1.0f,0.0f,0.0f), utilities::Vec3Float(5.0f,5.0f,5.0f))));
				}
			#endif
			}
			else
			{
				tempObj = mpDxModule->createObjectInstanced(1);
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j, 0.0f,-30.0f * i));
				tempObj->setScale(D3DXVECTOR3(15.0f, 0.1f, 15.0f));
				tempObj->setID(KEY_GROUND.convertToGfxId());

				int color = colorCounter % 4;
				if(color == 0)
					tempObj->setColor(D3DXCOLOR(1.0f,1.0f,1.0f,1.0f));
				else if(color == 1)
					tempObj->setColor(D3DXCOLOR(1.0f,0.0f,0.0f,1.0f));
				else if(color == 2)
					tempObj->setColor(D3DXCOLOR(1.0f,1.0f,0.0f,1.0f));
				else if(color == 3)
					tempObj->setColor(D3DXCOLOR(0.0f,1.0f,1.0f,1.0f));
				colorCounter++;
			}
		}
	}

	safeThreadSafeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateTank(logic::Tank::TankType_CloseCombat, utilities::Position(50.0f, 3.0f, 0.0f + (40.0f * mpPlayerInfo->getNetworkId())),utilities::Direction(1.0f,0.0f,0.0f), Actor::PlayerMovingInfo())));

}

void GFXTestPSSMApplication::setCommandWindow()
{
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd,2000,100,1500,600,TRUE);
}
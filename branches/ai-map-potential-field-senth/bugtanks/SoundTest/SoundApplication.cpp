/**
* @file
* @author Viktor Svensson <arkangel88@gmail.com>
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

#include "SoundApplication.h"
#include "../Logic/IEventManager.h"
#include "../Logic/Events.h"
#include "../Logic/StaticEnvironment.h"

using namespace sound;

SoundApplication::SoundApplication()
{
	// TODO - Constructor
}

SoundApplication::~SoundApplication()
{
	// TODO - Destructor
}

void SoundApplication::createInstance()
{
	assert(mpsInstance == NULL);

	mpsInstance = myNew SoundApplication();
}

void SoundApplication::gameLoop(float deltaTime)
{
	Application::gameLoop(deltaTime);
}

void SoundApplication::loadMeshes()
{
	Application::loadMeshes();

	/*mpDxModule->loadObjectShapeInstance(gfx::ShapeTypes::CUBE, 1, "Cube", 5000);
	DEBUG_MESSAGE(LEVEL_LOW, "Cube Loaded");
	mpDxModule->loadObjectShapeInstance(gfx::ShapeTypes::PLANE, 2, "Plane", 100);
	DEBUG_MESSAGE(LEVEL_LOW, "Plane Loaded");


	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Tank,		logic::Tank::TankType_CloseCombat,		"CloseCombatTank",	"../../ModelFiles/TankAir/NewTankAirGeometry.txt", "../../ModelFiles/TankAir/NewTankAirMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Tank,		logic::Tank::TankType_LongRange,		"LongRangeTank",	"../../ModelFiles/TankGround/NewTankGroundGeometry.txt", "../../ModelFiles/TankGround/NewTankGroundMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Driver,	0,										"CloseCombatTank",	"../../ModelFiles/TankAir/NewTankAirGeometry.txt",	"../../ModelFiles/TankAir/NewTankAirMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Enemy,		logic::Enemy::EnemyType_Ant,			"Ant", "../../ModelFiles/Ant/NewAntGeometry.txt", "../../ModelFiles/Ant/NewAntMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Enemy,		logic::Enemy::EnemyType_BlackWidow,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.txt", "../../ModelFiles/Ant/NewAntMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Enemy,		logic::Enemy::EnemyType_FireAnt,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.txt", "../../ModelFiles/Ant/NewAntMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Enemy,		logic::Enemy::EnemyType_Jumper,			"Ant", "../../ModelFiles/Ant/NewAntGeometry.txt", "../../ModelFiles/Ant/NewAntMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Enemy,		logic::Enemy::EnemyType_KamikazeAnt,	"Ant", "../../ModelFiles/Ant/NewAntGeometry.txt", "../../ModelFiles/Ant/NewAntMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Enemy,		logic::Enemy::EnemyType_PlasmaBeetle,	"Ant", "../../ModelFiles/Ant/NewAntGeometry.txt", "../../ModelFiles/Ant/NewAntMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Enemy,		logic::Enemy::EnemyType_Scarab,			"Ant", "../../ModelFiles/Ant/NewAntGeometry.txt", "../../ModelFiles/Ant/NewAntMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Enemy,		logic::Enemy::EnemyType_Tarantula,		"Ant", "../../ModelFiles/Ant/NewAntGeometry.txt", "../../ModelFiles/Ant/NewAntMaterial.txt");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Munition,	logic::Munitions::MunitionType_Acid,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.txt", "");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Munition,	logic::Munitions::MunitionType_Beam,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.txt", "");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Munition,	logic::Munitions::MunitionType_Bullet,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.txt", "");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Munition,	logic::Munitions::MunitionType_Fire,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.txt", "");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Munition,	logic::Munitions::MunitionType_Flame,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.txt", "");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Munition,	logic::Munitions::MunitionType_Plasma,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.txt", "");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Munition,	logic::Munitions::MunitionType_Pulse,	"Bullet", "../../ModelFiles/TankShell/NewTankShellGeometry.txt", "");
	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_Pedestrian, 0,										"CloseCombatTank", "../../ModelFiles/TankAir/NewTankAirGeometry.txt", "../../ModelFiles/TankAir/NewTankAirMaterial.txt");

	mpModelInfo->bindMeshToActorType(logic::Actor::ActorType_StaticEnvironment,	logic::StaticEnvironment::StaticEnvironmentType_StandAloneBuilding,
		"StandAloneBuilding",	"../../ModelFiles/Buildings/StandAloneBuilding/StandAloneBuildingGeometry.txt", "../../ModelFiles/Buildings/StandAloneBuilding/StandAloneBuildingMaterial.txt");
*/
}

void SoundApplication::createObjects()
{
	Application::createObjects();

	/*gfx::TransformObject *tempObj = NULL;

	tempObj = mpDxModule->createObjectInstanced("Plane");
	tempObj->setName("WorldPlane");
	tempObj->setPosition(D3DXVECTOR3(0.0f, 0.0f, 0.0f));
	tempObj->setScale(D3DXVECTOR3(40.0f, 1.0f, 40.0f));
	tempObj->setID(logic::KEY_GROUND.convertToGfxId());
	tempObj->setColor(D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f));

	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_FireAnt, utilities::Position(-50.0f, 3.0f, 40.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_Ant, utilities::Position(-90.0f, 3.0f, 40.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	//safeQueueEvent(EventDataPtr(myNew EvtData_To_ActorCreateEnemy(Enemy::EnemyType_Tarantula, utilities::Position(-90.0f, 0.0f, 20.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateTank(logic::Tank::TankType_CloseCombat, utilities::Position(80.0f, 0.0f, 20.0f), utilities::Direction(1.0f, 0.0f, 0.0f), logic::Actor::PlayerMovingInfo())));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateStaticEnvironment(logic::StaticEnvironment::StaticEnvironmentType_CityMall, utilities::Position(-200.0f, 0.0f, 30.0f), utilities::Direction(1.0f, 0.0f, 0.0f), utilities::Vec3Float(1.0f,1.0f,1.0f))));

	for(int i = 0; i < 5; i++)
	{
		logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_FireAnt, utilities::Position(100.0f, 3.0f, i*20.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	}*/

	/*srand(static_cast<unsigned int>(time(0)));
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
					logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateStaticEnvironment(logic::StaticEnvironment::StaticEnvironmentType_StandAloneBuilding, utilities::Position(-30.0f * j, 0.0f, -30.0f * i),utilities::Direction(1.0f,0.0f,0.0f), utilities::Vec3Float(5.0f,5.0f,5.0f))));
				}
			#endif
			}
			else
			{
				tempObj = mpDxModule->createObjectInstanced(1);
				tempObj->setPosition(D3DXVECTOR3(-30.0f * j, 0.0f,-30.0f * i));
				tempObj->setScale(D3DXVECTOR3(15.0f, 0.1f, 15.0f));
				tempObj->setID(logic::KEY_GROUND.convertToGfxId());

				int color = colorCounter % 4;
				if(color == 0)
					tempObj->setColor(D3DXCOLOR(0.0f,0.0f,0.0f,1.0f));
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
	
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateTank(static_cast<logic::Tank::TankTypes>(mpGameOptions->mTankType), utilities::Position(50.0f, 3.0f, 0.0f + (40.0f * mpPlayerInfo->getNetworkId())),utilities::Direction(1.0f,0.0f,0.0f), logic::Actor::PlayerMovingInfo())));

	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_FireAnt, utilities::Position(-50.0f, 3.0f, 0.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_Ant, utilities::Position(70.0f, 3.0f, 40.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_KamikazeAnt, utilities::Position(20, 3.0f, 100.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_Tarantula, utilities::Position(45.0f, 3.0f, -70.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_BlackWidow, utilities::Position(100.0f, 3.0f, 0.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_Jumper, utilities::Position(65.0f, 3.0f, 23.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_Scarab, utilities::Position(44.0f, 3.0f, 22.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	logic::safeQueueEvent(logic::EventDataPtr(myNew logic::EvtData_To_ActorCreateEnemy(logic::Enemy::EnemyType_PlasmaBeetle, utilities::Position(77.0f, 3.0f, 88.0f), utilities::Direction(1.0f, 0.0f, 0.0f))));
	*/
}

void SoundApplication::setCommandWindow()
{
	HWND hWnd = GetConsoleWindow();
	MoveWindow(hWnd,2000,100,800,600,TRUE);
}
void SoundApplication::registerEvents()
{
	Application::registerEvents();


}